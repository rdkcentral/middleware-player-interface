#include "ht_internal.h"

#include <pthread.h>

struct Shard
{
    pthread_mutex_t lock;
    HeapAllocRecord *tab;
    uint32_t mask;
    uint32_t count;
};

static Shard g_shards[HT_SHARDS];

static inline uint32_t shard_of(uintptr_t addr)
{
    return (uint32_t)((addr >> 4) & (HT_SHARDS - 1));
}

void ht_reg_init(void)
{
    uint32_t total = ht_pow2_ceil(g_ht_cfg.max_records * 2u);
    uint32_t per = total / HT_SHARDS;
    if (per < 64)
        per = 64;
    for (uint32_t i = 0; i < HT_SHARDS; i++)
    {
        pthread_mutex_init(&g_shards[i].lock, NULL);
        g_shards[i].tab = (HeapAllocRecord *)ht_map((size_t)per * sizeof(HeapAllocRecord));
        g_shards[i].mask = per - 1;
    }
}

static uint32_t find_slot(Shard *s, uintptr_t addr)
{
    if (!s->tab)
        return UINT32_MAX;
    uint32_t i = ht_hash_ptr(addr) & s->mask;
    for (uint32_t p = 0; p <= s->mask; p++, i = (i + 1) & s->mask)
    {
        if (s->tab[i].addr == addr)
            return i;
        if (s->tab[i].addr == 0)
            break;
    }
    return UINT32_MAX;
}

/* Backward-shift deletion keeps the probe chains intact without tombstones. */
static void erase_slot(Shard *s, uint32_t i)
{
    uint32_t j = i;
    for (;;)
    {
        s->tab[i].addr = 0;
        uint32_t k;
        do
        {
            j = (j + 1) & s->mask;
            if (s->tab[j].addr == 0)
            {
                s->count--;
                return;
            }
            k = ht_hash_ptr(s->tab[j].addr) & s->mask;
        } while ((i <= j) ? (i < k && k <= j) : (i < k || k <= j));
        s->tab[i] = s->tab[j];
        i = j;
    }
}

bool ht_reg_insert(const HeapAllocRecord *rec)
{
    Shard *s = &g_shards[shard_of(rec->addr)];
    pthread_mutex_lock(&s->lock);
    if (!s->tab || s->count >= (s->mask + 1) - (s->mask + 1) / 8)
    {
        pthread_mutex_unlock(&s->lock);
        ht_add64(&g_registry_full_drops, 1);
        return false;
    }
    uint32_t i = ht_hash_ptr(rec->addr) & s->mask;
    while (s->tab[i].addr != 0 && s->tab[i].addr != rec->addr)
        i = (i + 1) & s->mask;
    if (s->tab[i].addr == 0)
        s->count++;
    s->tab[i] = *rec;
    pthread_mutex_unlock(&s->lock);
    ht_inc(&g_live_tracked);
    return true;
}

bool ht_reg_take(uintptr_t addr, HeapAllocRecord *out)
{
    Shard *s = &g_shards[shard_of(addr)];
    pthread_mutex_lock(&s->lock);
    uint32_t i = find_slot(s, addr);
    if (i == UINT32_MAX)
    {
        pthread_mutex_unlock(&s->lock);
        return false;
    }
    *out = s->tab[i];
    erase_slot(s, i);
    pthread_mutex_unlock(&s->lock);
    ht_dec(&g_live_tracked);
    return true;
}

HeapAllocRecord *ht_reg_find_locked(uintptr_t addr, uint32_t *shard)
{
    uint32_t sid = shard_of(addr);
    Shard *s = &g_shards[sid];
    pthread_mutex_lock(&s->lock);
    uint32_t i = find_slot(s, addr);
    if (i == UINT32_MAX)
    {
        pthread_mutex_unlock(&s->lock);
        return NULL;
    }
    *shard = sid;
    return &s->tab[i];
}

void ht_reg_unlock(uint32_t shard)
{
    pthread_mutex_unlock(&g_shards[shard].lock);
}

uint32_t ht_reg_snapshot(HeapAllocRecord *out, uint32_t max)
{
    uint32_t n = 0;
    for (uint32_t si = 0; si < HT_SHARDS && n < max; si++)
    {
        Shard *s = &g_shards[si];
        if (!s->tab)
            continue;
        pthread_mutex_lock(&s->lock);
        for (uint32_t i = 0; i <= s->mask && n < max; i++)
        {
            if (s->tab[i].addr)
                out[n++] = s->tab[i];
        }
        pthread_mutex_unlock(&s->lock);
    }
    return n;
}

void ht_reg_lock_all(void)
{
    for (uint32_t i = 0; i < HT_SHARDS; i++)
        pthread_mutex_lock(&g_shards[i].lock);
}

void ht_reg_unlock_all(void)
{
    for (uint32_t i = HT_SHARDS; i > 0; i--)
        pthread_mutex_unlock(&g_shards[i - 1].lock);
}

/* ---------------- Freed-by table ---------------- */

#define HT_FREEDBY_SLOTS 2048

struct FreedBy
{
    uint32_t origin;
    uint32_t freeing;
    uint64_t count;
    uint64_t bytes;
    uint8_t used;
};

static FreedBy g_freedby[HT_FREEDBY_SLOTS];
static pthread_mutex_t g_freedby_lock = PTHREAD_MUTEX_INITIALIZER;

void ht_freedby_note(uint32_t origin, uint32_t freeing, uint32_t bytes)
{
    uint32_t i = ht_hash_u32((origin * 2654435761u) ^ freeing) & (HT_FREEDBY_SLOTS - 1);
    pthread_mutex_lock(&g_freedby_lock);
    for (uint32_t p = 0; p < HT_FREEDBY_SLOTS; p++, i = (i + 1) & (HT_FREEDBY_SLOTS - 1))
    {
        FreedBy *e = &g_freedby[i];
        if (!e->used)
        {
            e->used = 1;
            e->origin = origin;
            e->freeing = freeing;
        }
        if (e->origin == origin && e->freeing == freeing)
        {
            e->count++;
            e->bytes += bytes;
            break;
        }
    }
    pthread_mutex_unlock(&g_freedby_lock);
}

uint32_t ht_freedby_count(void) { return HT_FREEDBY_SLOTS; }

bool ht_freedby_get(uint32_t index, uint32_t *origin, uint32_t *freeing,
                    uint64_t *count, uint64_t *bytes)
{
    FreedBy *e = &g_freedby[index];
    if (!e->used)
        return false;
    *origin = e->origin;
    *freeing = e->freeing;
    *count = e->count;
    *bytes = e->bytes;
    return true;
}
