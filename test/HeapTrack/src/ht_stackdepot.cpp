#include "ht_internal.h"

#include <pthread.h>

#define HT_DEPOT_SLOTS 16384
#define HT_DEPOT_ENTRIES 16384
#define HT_DEPOT_FRAMES (256 * 1024)

struct DepotEntry
{
    uint32_t hash;
    uint32_t offset;
    uint32_t depth;
};

static uint32_t *g_slots;
static DepotEntry *g_entries;
static uintptr_t *g_frames;
static uint32_t g_entry_used = 1; /* id 0 means "no stack" */
static uint32_t g_frame_used;
static pthread_mutex_t g_depot_lock = PTHREAD_MUTEX_INITIALIZER;

void ht_depot_init(void)
{
    g_slots = (uint32_t *)ht_map(HT_DEPOT_SLOTS * sizeof(uint32_t));
    g_entries = (DepotEntry *)ht_map(HT_DEPOT_ENTRIES * sizeof(DepotEntry));
    g_frames = (uintptr_t *)ht_map(HT_DEPOT_FRAMES * sizeof(uintptr_t));
}

static bool same_stack(const DepotEntry *e, const uintptr_t *pcs, int n)
{
    return e->depth == (uint32_t)n &&
           memcmp(&g_frames[e->offset], pcs, (size_t)n * sizeof(uintptr_t)) == 0;
}

uint32_t ht_depot_put(const uintptr_t *pcs, int n)
{
    if (!g_slots || n <= 0)
        return 0;
    uint32_t h = ht_hash_bytes(pcs, (size_t)n * sizeof(uintptr_t));
    uint32_t mask = HT_DEPOT_SLOTS - 1;
    uint32_t i = h & mask;

    for (uint32_t p = 0; p < HT_DEPOT_SLOTS; p++, i = (i + 1) & mask)
    {
        uint32_t v = ht_load(&g_slots[i]);
        if (!v)
            break;
        if (g_entries[v].hash == h && same_stack(&g_entries[v], pcs, n))
            return v;
    }

    pthread_mutex_lock(&g_depot_lock);
    i = h & mask;
    for (uint32_t p = 0; p < HT_DEPOT_SLOTS; p++, i = (i + 1) & mask)
    {
        uint32_t v = g_slots[i];
        if (!v)
        {
            if (g_entry_used >= HT_DEPOT_ENTRIES || g_frame_used + (uint32_t)n > HT_DEPOT_FRAMES)
            {
                pthread_mutex_unlock(&g_depot_lock);
                ht_add64(&g_depot_full_drops, 1);
                return 0;
            }
            v = g_entry_used++;
            g_entries[v].hash = h;
            g_entries[v].offset = g_frame_used;
            g_entries[v].depth = (uint32_t)n;
            memcpy(&g_frames[g_frame_used], pcs, (size_t)n * sizeof(uintptr_t));
            g_frame_used += (uint32_t)n;
            ht_store(&g_slots[i], v);
            pthread_mutex_unlock(&g_depot_lock);
            return v;
        }
        if (g_entries[v].hash == h && same_stack(&g_entries[v], pcs, n))
        {
            pthread_mutex_unlock(&g_depot_lock);
            return v;
        }
    }
    pthread_mutex_unlock(&g_depot_lock);
    ht_add64(&g_depot_full_drops, 1);
    return 0;
}

const uintptr_t *ht_depot_get(uint32_t id, int *n)
{
    if (!id || id >= g_entry_used)
    {
        *n = 0;
        return NULL;
    }
    *n = (int)g_entries[id].depth;
    return &g_frames[g_entries[id].offset];
}

void ht_depot_lock_all(void) { pthread_mutex_lock(&g_depot_lock); }
void ht_depot_unlock_all(void) { pthread_mutex_unlock(&g_depot_lock); }
