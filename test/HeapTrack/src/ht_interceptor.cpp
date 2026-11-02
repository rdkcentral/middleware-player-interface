#include "ht_internal.h"

#include <new>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/syscall.h>
#include <errno.h>

#ifndef __THROW
#define __THROW
#endif

HT_TLS int t_ht_guard;
HT_TLS uint8_t t_ht_alloc_kind = HT_KIND_NONE;
HT_TLS uint8_t t_ht_free_kind = HT_KIND_NONE;
static HT_TLS uint32_t t_tid;

static inline uint32_t ht_tid(void)
{
    if (!t_tid)
        t_tid = (uint32_t)syscall(SYS_gettid);
    return t_tid;
}

static inline uint64_t ht_now_ns(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
}

static inline uint8_t take_alloc_kind(uint8_t dflt)
{
    uint8_t k = t_ht_alloc_kind;
    t_ht_alloc_kind = HT_KIND_NONE;
    return (k == HT_KIND_NONE) ? dflt : k;
}

void ht_on_alloc(void *ptr, size_t size, uint8_t default_kind)
{
    uint8_t kind = take_alloc_kind(default_kind);
    if (HT_LIKELY(!ht_load(&g_tracking_enabled)) || ht_guarded())
        return;

    uint32_t node = ht_ctx_current();
    if (!node)
        return;

    HtGuard guard;
    HeapAllocRecord rec;
    memset(&rec, 0, sizeof(rec));
    rec.addr = (uintptr_t)ptr;
    rec.size = (uint32_t)size;
    rec.origin_node = node;
    rec.owner_node = node;
    rec.kind = kind;
    rec.tid = ht_tid();
    rec.alloc_ts_ns = ht_now_ns();

    if (g_ht_cfg.depth && size >= g_ht_cfg.min_stack_size)
    {
        uintptr_t pcs[HT_MAX_DEPTH];
        int n = ht_unwind(pcs, (int)g_ht_cfg.depth);
        rec.stack_id = ht_depot_put(pcs, n);
    }

    if (!ht_reg_insert(&rec))
        return;

    HeapTrackNode *n = ht_node(node);
    if (n)
    {
        ht_add64(&n->stats.allocs, 1);
        ht_add64(&n->stats.bytes_alloc, size);
        ht_add64(&n->stats.live, 1);
        ht_add64(&n->stats.cur_bytes, size);
        ht_max64(&n->stats.peak_bytes, ht_get64(&n->stats.cur_bytes));
    }
}

void ht_on_free(void *ptr)
{
    uint8_t free_kind = t_ht_free_kind;
    t_ht_free_kind = HT_KIND_NONE;

    if (HT_LIKELY(!ht_load(&g_live_tracked)) || ht_guarded())
        return;

    HeapAllocRecord rec;
    if (!ht_reg_take((uintptr_t)ptr, &rec))
        return;

    HtGuard guard;
    HeapTrackNode *origin = ht_node(rec.origin_node);
    if (origin)
    {
        ht_add64(&origin->stats.frees, 1);
        ht_add64(&origin->stats.bytes_freed, rec.size);
        ht_sub64(&origin->stats.cur_bytes, rec.size);
        ht_sub64(&origin->stats.live, 1);

        uint8_t expect = (rec.kind == HT_KIND_NEW || rec.kind == HT_KIND_NEW_ARRAY) ? rec.kind : HT_KIND_NONE;
        if (free_kind != expect)
            ht_inc(&origin->stats.mismatched_frees);
    }

    uint32_t freeing = ht_ctx_current();
    if (freeing != rec.origin_node)
    {
        ht_freedby_note(rec.origin_node, freeing, rec.size);
        if (origin)
            ht_add64(&origin->stats.cross_frees, 1);
    }
}

/* ---------------- Interposed allocator symbols ---------------- */

/* A failed allocation must still drop the pending new/delete kind hint. */
static inline void *ht_alloc_result(void *p, size_t size, uint8_t kind)
{
    if (p)
        ht_on_alloc(p, size, kind);
    else
        t_ht_alloc_kind = HT_KIND_NONE;
    return p;
}

extern "C" {

HT_EXPORT void *malloc(size_t size) __THROW
{
    return ht_alloc_result(ht_real_malloc(size), size, HT_KIND_MALLOC);
}

HT_EXPORT void *calloc(size_t n, size_t size) __THROW
{
    return ht_alloc_result(ht_real_calloc(n, size), n * size, HT_KIND_CALLOC);
}

HT_EXPORT void free(void *ptr) __THROW
{
    if (!ptr || ht_boot_owns(ptr))
    {
        t_ht_free_kind = HT_KIND_NONE;
        return;
    }
    ht_on_free(ptr);
    ht_real_free(ptr);
}

HT_EXPORT void *realloc(void *ptr, size_t size) __THROW
{
    if (!ptr)
        return malloc(size);
    if (size == 0)
    {
        free(ptr);
        return NULL;
    }
    if (ht_boot_owns(ptr))
        return ht_real_realloc(ptr, size);

    /* Take the record out first so no other thread can insert the old
     * address while realloc may be freeing it (design section 5.5). */
    HeapAllocRecord rec;
    bool tracked = ht_load(&g_live_tracked) && !ht_guarded() && ht_reg_take((uintptr_t)ptr, &rec);

    void *np = ht_real_realloc(ptr, size);

    if (!tracked)
        return ht_alloc_result(np, size, HT_KIND_REALLOC);
    if (!np)
    {
        ht_reg_insert(&rec); /* old block still valid */
        return NULL;
    }

    HtGuard guard;
    HeapTrackNode *origin = ht_node(rec.origin_node);
    if (origin)
    {
        if (size > rec.size)
        {
            ht_add64(&origin->stats.bytes_alloc, size - rec.size);
            ht_add64(&origin->stats.cur_bytes, size - rec.size);
            ht_max64(&origin->stats.peak_bytes, ht_get64(&origin->stats.cur_bytes));
        }
        else
        {
            ht_add64(&origin->stats.bytes_freed, rec.size - size);
            ht_sub64(&origin->stats.cur_bytes, rec.size - size);
        }
    }
    rec.addr = (uintptr_t)np;
    rec.size = (uint32_t)size;
    ht_reg_insert(&rec);
    return np;
}

HT_EXPORT void *memalign(size_t align, size_t size) __THROW
{
    return ht_alloc_result(ht_real_memalign(align, size), size, HT_KIND_ALIGNED);
}

HT_EXPORT int posix_memalign(void **out, size_t align, size_t size) __THROW
{
    if (align < sizeof(void *) || (align & (align - 1)))
        return EINVAL;
    void *p = ht_alloc_result(ht_real_memalign(align, size), size, HT_KIND_ALIGNED);
    if (!p)
        return ENOMEM;
    *out = p;
    return 0;
}

HT_EXPORT void *aligned_alloc(size_t align, size_t size) __THROW
{
    return memalign(align, size);
}

HT_EXPORT void *valloc(size_t size) __THROW
{
    return memalign((size_t)sysconf(_SC_PAGESIZE), size);
}

HT_EXPORT void *pvalloc(size_t size) __THROW
{
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    return memalign(page, (size + page - 1) & ~(page - 1));
}

HT_EXPORT size_t malloc_usable_size(void *ptr) __THROW
{
    return ptr ? ht_real_usable_size(ptr) : 0;
}

} /* extern "C" */

/* ---------------- new / delete: kind hints only (design section 5.2) -------- */

static void *ht_new_impl(size_t size, uint8_t kind)
{
    for (;;)
    {
        t_ht_alloc_kind = kind;
        void *p = malloc(size ? size : 1);
        if (p)
            return p;
        std::new_handler h = std::get_new_handler();
        if (!h)
            return NULL;
        h();
    }
}

HT_EXPORT void *operator new(size_t size)
{
    void *p = ht_new_impl(size, HT_KIND_NEW);
    if (!p)
        throw std::bad_alloc();
    return p;
}

HT_EXPORT void *operator new[](size_t size)
{
    void *p = ht_new_impl(size, HT_KIND_NEW_ARRAY);
    if (!p)
        throw std::bad_alloc();
    return p;
}

HT_EXPORT void *operator new(size_t size, const std::nothrow_t &) noexcept
{
    return ht_new_impl(size, HT_KIND_NEW);
}

HT_EXPORT void *operator new[](size_t size, const std::nothrow_t &) noexcept
{
    return ht_new_impl(size, HT_KIND_NEW_ARRAY);
}

HT_EXPORT void operator delete(void *p) noexcept
{
    t_ht_free_kind = HT_KIND_NEW;
    free(p);
}

HT_EXPORT void operator delete[](void *p) noexcept
{
    t_ht_free_kind = HT_KIND_NEW_ARRAY;
    free(p);
}

HT_EXPORT void operator delete(void *p, size_t) noexcept { operator delete(p); }
HT_EXPORT void operator delete[](void *p, size_t) noexcept { operator delete[](p); }
HT_EXPORT void operator delete(void *p, const std::nothrow_t &) noexcept { operator delete(p); }
HT_EXPORT void operator delete[](void *p, const std::nothrow_t &) noexcept { operator delete[](p); }

#if defined(__cpp_aligned_new)
HT_EXPORT void *operator new(size_t size, std::align_val_t al)
{
    t_ht_alloc_kind = HT_KIND_NEW;
    void *p = memalign((size_t)al, size);
    if (!p)
        throw std::bad_alloc();
    return p;
}

HT_EXPORT void *operator new[](size_t size, std::align_val_t al)
{
    t_ht_alloc_kind = HT_KIND_NEW_ARRAY;
    void *p = memalign((size_t)al, size);
    if (!p)
        throw std::bad_alloc();
    return p;
}

HT_EXPORT void operator delete(void *p, std::align_val_t) noexcept { operator delete(p); }
HT_EXPORT void operator delete[](void *p, std::align_val_t) noexcept { operator delete[](p); }
HT_EXPORT void operator delete(void *p, size_t, std::align_val_t) noexcept { operator delete(p); }
HT_EXPORT void operator delete[](void *p, size_t, std::align_val_t) noexcept { operator delete[](p); }
#endif
