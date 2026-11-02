#include "ht_bootstrap.h"
#include "ht_platform.h"

#include <dlfcn.h>
#include <unistd.h>

#define HT_BOOT_ARENA_SIZE (16 * 1024)

typedef struct
{
    size_t size;
} BootHeader;

static char g_boot_arena[HT_BOOT_ARENA_SIZE] __attribute__((aligned(16)));
static size_t g_boot_used;

static __attribute__((unused)) void *boot_alloc(size_t size)
{
    size_t need = (sizeof(BootHeader) + size + 15u) & ~(size_t)15u;
    size_t off = __atomic_fetch_add(&g_boot_used, need, __ATOMIC_RELAXED);
    BootHeader *h;
    if (off + need > HT_BOOT_ARENA_SIZE)
        return NULL;
    h = (BootHeader *)(g_boot_arena + off);
    h->size = size;
    return (void *)(h + 1);
}

int ht_boot_owns(const void *ptr)
{
    const char *p = (const char *)ptr;
    return p >= g_boot_arena && p < g_boot_arena + HT_BOOT_ARENA_SIZE;
}

size_t ht_boot_size(const void *ptr)
{
    return ((const BootHeader *)ptr)[-1].size;
}

#if defined(__GLIBC__)

extern void *__libc_malloc(size_t);
extern void *__libc_calloc(size_t, size_t);
extern void *__libc_realloc(void *, size_t);
extern void *__libc_memalign(size_t, size_t);
extern void __libc_free(void *);

void *ht_real_malloc(size_t size) { return __libc_malloc(size); }
void *ht_real_calloc(size_t n, size_t size) { return __libc_calloc(n, size); }
void *ht_real_realloc(void *ptr, size_t size) { return __libc_realloc(ptr, size); }
void *ht_real_memalign(size_t align, size_t size) { return __libc_memalign(align, size); }
void ht_real_free(void *ptr) { __libc_free(ptr); }

#else /* dlsym fallback for non-glibc libc */

static void *(*s_malloc)(size_t);
static void *(*s_calloc)(size_t, size_t);
static void *(*s_realloc)(void *, size_t);
static void *(*s_memalign)(size_t, size_t);
static void (*s_free)(void *);
static int s_resolving;

static void resolve_real(void)
{
    if (s_malloc || s_resolving)
        return;
    s_resolving = 1; /* dlsym may allocate: served from the bootstrap arena */
    s_calloc = (void *(*)(size_t, size_t))dlsym(RTLD_NEXT, "calloc");
    s_realloc = (void *(*)(void *, size_t))dlsym(RTLD_NEXT, "realloc");
    s_memalign = (void *(*)(size_t, size_t))dlsym(RTLD_NEXT, "memalign");
    s_free = (void (*)(void *))dlsym(RTLD_NEXT, "free");
    __atomic_store_n(&s_malloc, (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc"), __ATOMIC_RELEASE);
    s_resolving = 0;
}

void *ht_real_malloc(size_t size)
{
    if (!__atomic_load_n(&s_malloc, __ATOMIC_ACQUIRE))
    {
        if (s_resolving)
            return boot_alloc(size);
        resolve_real();
    }
    return s_malloc(size);
}

void *ht_real_calloc(size_t n, size_t size)
{
    if (!__atomic_load_n(&s_malloc, __ATOMIC_ACQUIRE))
    {
        if (s_resolving)
        {
            void *p = boot_alloc(n * size);
            if (p)
                memset(p, 0, n * size);
            return p;
        }
        resolve_real();
    }
    return s_calloc(n, size);
}

void *ht_real_realloc(void *ptr, size_t size)
{
    if (ht_boot_owns(ptr))
    {
        void *p = ht_real_malloc(size);
        size_t old = ht_boot_size(ptr);
        if (p)
            memcpy(p, ptr, old < size ? old : size);
        return p;
    }
    if (!__atomic_load_n(&s_malloc, __ATOMIC_ACQUIRE))
        resolve_real();
    return s_realloc(ptr, size);
}

void *ht_real_memalign(size_t align, size_t size)
{
    if (!__atomic_load_n(&s_malloc, __ATOMIC_ACQUIRE))
        resolve_real();
    return s_memalign(align, size);
}

void ht_real_free(void *ptr)
{
    if (ht_boot_owns(ptr))
        return;
    if (!__atomic_load_n(&s_malloc, __ATOMIC_ACQUIRE))
        resolve_real();
    s_free(ptr);
}

#endif

size_t ht_real_usable_size(void *ptr)
{
    static size_t (*s_usable)(void *);
    if (ht_boot_owns(ptr))
        return ht_boot_size(ptr);
    if (!s_usable)
        s_usable = (size_t(*)(void *))dlsym(RTLD_NEXT, "malloc_usable_size");
    return s_usable ? s_usable(ptr) : 0;
}
