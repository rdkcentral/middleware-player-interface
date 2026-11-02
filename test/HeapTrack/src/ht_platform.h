#ifndef HT_PLATFORM_H
#define HT_PLATFORM_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sys/mman.h>

#define HT_TLS __thread __attribute__((tls_model("initial-exec")))
#define HT_EXPORT __attribute__((visibility("default")))
#define HT_LIKELY(x) __builtin_expect(!!(x), 1)
#define HT_UNLIKELY(x) __builtin_expect(!!(x), 0)

#define ht_load(p) __atomic_load_n((p), __ATOMIC_ACQUIRE)
#define ht_store(p, v) __atomic_store_n((p), (v), __ATOMIC_RELEASE)
#define ht_inc(p) __atomic_fetch_add((p), 1, __ATOMIC_RELAXED)
#define ht_dec(p) __atomic_fetch_sub((p), 1, __ATOMIC_RELAXED)
#define ht_cas(p, e, d) __atomic_compare_exchange_n((p), (e), (d), 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)

#ifdef __cplusplus
extern "C" {
#endif

static inline void *ht_map(size_t bytes)
{
    void *p = mmap(NULL, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return (p == MAP_FAILED) ? NULL : p;
}

static inline uint32_t ht_hash_bytes(const void *data, size_t len)
{
    const unsigned char *p = (const unsigned char *)data;
    uint32_t h = 2166136261u;
    size_t i;
    for (i = 0; i < len; i++)
    {
        h ^= p[i];
        h *= 16777619u;
    }
    return h;
}

static inline uint32_t ht_hash_u32(uint32_t v)
{
    v ^= v >> 16;
    v *= 0x7feb352du;
    v ^= v >> 15;
    v *= 0x846ca68bu;
    v ^= v >> 16;
    return v;
}

static inline uint32_t ht_hash_ptr(uintptr_t a)
{
    return ht_hash_u32((uint32_t)(a >> 4) ^ (uint32_t)(((uint64_t)a) >> 32));
}

static inline uint32_t ht_pow2_ceil(uint32_t v)
{
    uint32_t r = 1;
    while (r < v)
        r <<= 1;
    return r;
}

/*
 * 64-bit counters. Atomic where the target has lock-free 64-bit operations,
 * otherwise a tiny spinlock bank (design section 8.4). The branch is a
 * compile-time constant, so only one variant survives.
 */
extern unsigned char g_ht_c64_locks[64];

static inline unsigned ht_c64_slot(const void *p)
{
    return (unsigned)(((uintptr_t)p >> 3) & 63u);
}

static inline void ht_c64_lock(const void *p)
{
    unsigned s = ht_c64_slot(p);
    while (__atomic_test_and_set(&g_ht_c64_locks[s], __ATOMIC_ACQUIRE))
    {
    }
}

static inline void ht_c64_unlock(const void *p)
{
    __atomic_clear(&g_ht_c64_locks[ht_c64_slot(p)], __ATOMIC_RELEASE);
}

static inline void ht_add64(uint64_t *p, uint64_t d)
{
    if (__atomic_always_lock_free(8, 0))
    {
        __atomic_fetch_add(p, d, __ATOMIC_RELAXED);
        return;
    }
    ht_c64_lock(p);
    *p += d;
    ht_c64_unlock(p);
}

static inline void ht_sub64(uint64_t *p, uint64_t d)
{
    if (__atomic_always_lock_free(8, 0))
    {
        __atomic_fetch_sub(p, d, __ATOMIC_RELAXED);
        return;
    }
    ht_c64_lock(p);
    *p -= d;
    ht_c64_unlock(p);
}

static inline uint64_t ht_get64(const uint64_t *p)
{
    uint64_t v;
    if (__atomic_always_lock_free(8, 0))
        return __atomic_load_n(p, __ATOMIC_RELAXED);
    ht_c64_lock(p);
    v = *p;
    ht_c64_unlock(p);
    return v;
}

static inline void ht_max64(uint64_t *p, uint64_t v)
{
    if (__atomic_always_lock_free(8, 0))
    {
        uint64_t cur = __atomic_load_n(p, __ATOMIC_RELAXED);
        while (v > cur && !__atomic_compare_exchange_n(p, &cur, v, 1, __ATOMIC_RELAXED, __ATOMIC_RELAXED))
        {
        }
        return;
    }
    ht_c64_lock(p);
    if (v > *p)
        *p = v;
    ht_c64_unlock(p);
}

#ifdef __cplusplus
}
#endif

#endif /* HT_PLATFORM_H */
