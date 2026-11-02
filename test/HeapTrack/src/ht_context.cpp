#include "ht_internal.h"

#include <pthread.h>

/* ---------------- String interner ---------------- */

#define HT_STR_BYTES (256 * 1024)
#define HT_STR_SLOTS 8192

static char *g_str_buf;
static uint32_t g_str_used = 1; /* offset 0 is reserved as "no id" */
static uint32_t *g_str_slots;
static pthread_mutex_t g_str_lock = PTHREAD_MUTEX_INITIALIZER;

#define HT_SITE_SLOTS 4096
static HtSite *g_sites;
static uint32_t g_site_count = 1;
static uint32_t *g_site_slots;
static pthread_mutex_t g_site_lock = PTHREAD_MUTEX_INITIALIZER;

void ht_intern_init(void)
{
    g_str_buf = (char *)ht_map(HT_STR_BYTES);
    g_str_slots = (uint32_t *)ht_map(HT_STR_SLOTS * sizeof(uint32_t));
    g_sites = (HtSite *)ht_map(HT_SITE_SLOTS * sizeof(HtSite));
    g_site_slots = (uint32_t *)ht_map(HT_SITE_SLOTS * sizeof(uint32_t));
}

const char *ht_str(uint32_t id)
{
    if (!id || !g_str_buf)
        return "";
    return g_str_buf + id;
}

uint32_t ht_intern(const char *s)
{
    if (!s || !g_str_buf)
        return 0;
    size_t len = strlen(s);
    uint32_t h = ht_hash_bytes(s, len);
    uint32_t mask = HT_STR_SLOTS - 1;
    uint32_t i = h & mask;
    uint32_t probes;

    for (probes = 0; probes < HT_STR_SLOTS; probes++, i = (i + 1) & mask)
    {
        uint32_t v = ht_load(&g_str_slots[i]);
        if (!v)
            break;
        if (strcmp(g_str_buf + v, s) == 0)
            return v;
    }

    pthread_mutex_lock(&g_str_lock);
    i = h & mask;
    for (probes = 0; probes < HT_STR_SLOTS; probes++, i = (i + 1) & mask)
    {
        uint32_t v = g_str_slots[i];
        if (!v)
        {
            if (g_str_used + len + 1 > HT_STR_BYTES)
            {
                pthread_mutex_unlock(&g_str_lock);
                return 0;
            }
            v = g_str_used;
            memcpy(g_str_buf + v, s, len + 1);
            g_str_used += (uint32_t)len + 1;
            ht_store(&g_str_slots[i], v);
            pthread_mutex_unlock(&g_str_lock);
            return v;
        }
        if (strcmp(g_str_buf + v, s) == 0)
        {
            pthread_mutex_unlock(&g_str_lock);
            return v;
        }
    }
    pthread_mutex_unlock(&g_str_lock);
    return 0;
}

uint32_t ht_site_intern(const char *var, const char *file, int line)
{
    if (!g_sites)
        return 0;
    uint32_t var_id = ht_intern(var);
    uint32_t file_id = ht_intern(file);
    uint32_t h = ht_hash_u32(var_id ^ (file_id * 2654435761u) ^ (uint32_t)line);
    uint32_t mask = HT_SITE_SLOTS - 1;
    uint32_t i = h & mask;
    uint32_t probes;

    for (probes = 0; probes < HT_SITE_SLOTS; probes++, i = (i + 1) & mask)
    {
        uint32_t v = ht_load(&g_site_slots[i]);
        if (!v)
            break;
        if (g_sites[v].var_name_id == var_id && g_sites[v].file_id == file_id && g_sites[v].line == line)
            return v;
    }

    pthread_mutex_lock(&g_site_lock);
    i = h & mask;
    for (probes = 0; probes < HT_SITE_SLOTS; probes++, i = (i + 1) & mask)
    {
        uint32_t v = g_site_slots[i];
        if (!v)
        {
            if (g_site_count >= HT_SITE_SLOTS)
            {
                pthread_mutex_unlock(&g_site_lock);
                return 0;
            }
            v = g_site_count++;
            g_sites[v].var_name_id = var_id;
            g_sites[v].file_id = file_id;
            g_sites[v].line = line;
            ht_store(&g_site_slots[i], v);
            pthread_mutex_unlock(&g_site_lock);
            return v;
        }
        if (g_sites[v].var_name_id == var_id && g_sites[v].file_id == file_id && g_sites[v].line == line)
        {
            pthread_mutex_unlock(&g_site_lock);
            return v;
        }
    }
    pthread_mutex_unlock(&g_site_lock);
    return 0;
}

const HtSite *ht_site(uint32_t id)
{
    if (!id || id >= g_site_count)
        return NULL;
    return &g_sites[id];
}

/* ---------------- Context trie ---------------- */

#define HT_CHILD_SLOTS 8192

static HeapTrackNode *g_nodes;
static uint32_t g_nodes_used = 1; /* node id 0 means "no node" */
static uint32_t *g_child_slots;
static pthread_mutex_t g_node_lock = PTHREAD_MUTEX_INITIALIZER;
static uint32_t g_global_node;

static HT_TLS uint32_t t_scope[HT_SCOPE_DEPTH];
static HT_TLS int t_depth;
static pthread_key_t g_exit_key;
static int g_exit_key_ok;

static void thread_exit_cleanup(void *);

void ht_ctx_init(void)
{
    g_nodes = (HeapTrackNode *)ht_map(HT_MAX_NODES * sizeof(HeapTrackNode));
    g_child_slots = (uint32_t *)ht_map(HT_CHILD_SLOTS * sizeof(uint32_t));
    g_exit_key_ok = (pthread_key_create(&g_exit_key, thread_exit_cleanup) == 0);
}

HeapTrackNode *ht_node(uint32_t id)
{
    if (!id || id >= ht_load(&g_nodes_used))
        return NULL;
    return &g_nodes[id];
}

uint32_t ht_node_count(void) { return ht_load(&g_nodes_used); }

static uint32_t node_find_or_create(uint32_t parent, uint32_t name_id)
{
    uint32_t h = ht_hash_u32((parent * 2654435761u) ^ name_id);
    uint32_t mask = HT_CHILD_SLOTS - 1;
    uint32_t i = h & mask;
    uint32_t probes;

    if (!g_nodes || !g_child_slots)
        return 0;

    for (probes = 0; probes < HT_CHILD_SLOTS; probes++, i = (i + 1) & mask)
    {
        uint32_t v = ht_load(&g_child_slots[i]);
        if (!v)
            break;
        if (g_nodes[v].parent_id == parent && g_nodes[v].api_name_id == name_id)
            return v;
    }

    pthread_mutex_lock(&g_node_lock);
    i = h & mask;
    for (probes = 0; probes < HT_CHILD_SLOTS; probes++, i = (i + 1) & mask)
    {
        uint32_t v = g_child_slots[i];
        if (!v)
        {
            if (g_nodes_used >= HT_MAX_NODES)
            {
                pthread_mutex_unlock(&g_node_lock);
                ht_add64(&g_node_overflows, 1);
                return parent; /* attribute new paths to the parent */
            }
            v = g_nodes_used;
            g_nodes[v].node_id = v;
            g_nodes[v].parent_id = parent;
            g_nodes[v].api_name_id = name_id;
            ht_store(&g_nodes_used, v + 1);
            ht_store(&g_child_slots[i], v);
            pthread_mutex_unlock(&g_node_lock);
            return v;
        }
        if (g_nodes[v].parent_id == parent && g_nodes[v].api_name_id == name_id)
        {
            pthread_mutex_unlock(&g_node_lock);
            return v;
        }
    }
    pthread_mutex_unlock(&g_node_lock);
    ht_add64(&g_node_overflows, 1);
    return parent;
}

uint32_t ht_ctx_top(void)
{
    return t_depth > 0 ? t_scope[t_depth - 1] : 0;
}

uint32_t ht_ctx_current(void)
{
    uint32_t top = ht_ctx_top();
    if (top)
        return top;
    return ht_load(&g_global_node);
}

void ht_ctx_push_node(uint32_t node)
{
    if (t_depth >= HT_SCOPE_DEPTH)
    {
        ht_add64(&g_pairing_errors, 1);
        return;
    }
    t_scope[t_depth++] = node;
    HeapTrackNode *n = ht_node(node);
    if (n)
        ht_inc(&n->active_invocations);
    if (g_exit_key_ok && t_depth == 1)
        pthread_setspecific(g_exit_key, (void *)1);
}

void ht_ctx_pop_node(void)
{
    if (t_depth <= 0)
        return;
    HeapTrackNode *n = ht_node(t_scope[--t_depth]);
    if (n)
        ht_dec(&n->active_invocations);
}

uint32_t ht_ctx_begin(const char *api_name)
{
    uint32_t name_id = ht_intern(api_name);
    uint32_t parent = ht_ctx_current();
    uint32_t node = node_find_or_create(parent, name_id);
    HeapTrackNode *n = ht_node(node);
    ht_store(&g_tracking_enabled, 1u);
    if (n)
        ht_add64(&n->stats.calls, 1);
    ht_ctx_push_node(node);
    return node;
}

uint32_t ht_ctx_named_node(const char *name)
{
    return node_find_or_create(0, ht_intern(name));
}

void ht_ctx_end(uint32_t node)
{
    int i;
    if (t_depth <= 0)
    {
        ht_add64(&g_pairing_errors, 1);
        return;
    }
    if (t_scope[t_depth - 1] == node)
    {
        ht_ctx_pop_node();
        return;
    }
    /* Mismatched end: pop down to the matching node so one missed end cannot
     * corrupt the stack permanently (design section 3.1). */
    ht_add64(&g_pairing_errors, 1);
    for (i = t_depth - 1; i >= 0; i--)
    {
        if (t_scope[i] == node)
        {
            while (t_depth > i)
                ht_ctx_pop_node();
            return;
        }
    }
    ht_ctx_pop_node();
}

void ht_ctx_global_begin(const char *component_name)
{
    uint32_t node = node_find_or_create(0, ht_intern(component_name));
    HeapTrackNode *n = ht_node(node);
    if (n)
    {
        ht_inc(&n->active_invocations);
        ht_add64(&n->stats.calls, 1);
    }
    ht_store(&g_global_node, node);
    ht_store(&g_tracking_enabled, 1u);
}

void ht_ctx_global_end(void)
{
    uint32_t node = ht_load(&g_global_node);
    HeapTrackNode *n = ht_node(node);
    ht_store(&g_global_node, 0u);
    if (n)
        ht_dec(&n->active_invocations);
}

static void thread_exit_cleanup(void *)
{
    while (t_depth > 0)
        ht_ctx_pop_node();
}

void ht_ctx_lock_all(void)
{
    pthread_mutex_lock(&g_node_lock);
    pthread_mutex_lock(&g_str_lock);
    pthread_mutex_lock(&g_site_lock);
}

void ht_ctx_unlock_all(void)
{
    pthread_mutex_unlock(&g_site_lock);
    pthread_mutex_unlock(&g_str_lock);
    pthread_mutex_unlock(&g_node_lock);
}
