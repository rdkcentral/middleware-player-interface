#include "ht_internal.h"

#include <pthread.h>
#include <stdlib.h>

HtConfig g_ht_cfg = {262144, 24, 0, 20, 1, 0, NULL};
uint32_t g_tracking_enabled;
uint32_t g_live_tracked;
uint64_t g_registry_full_drops;
uint64_t g_node_overflows;
uint64_t g_depot_full_drops;
uint64_t g_pairing_errors;

extern "C" {
unsigned char g_ht_c64_locks[64];
}

static uint32_t env_u32(const char *name, uint32_t dflt)
{
    const char *v = getenv(name);
    if (!v || !*v)
        return dflt;
    uint32_t r = 0;
    while (*v >= '0' && *v <= '9')
        r = r * 10u + (uint32_t)(*v++ - '0');
    return r;
}

static void atfork_prepare(void)
{
    ht_ctx_lock_all();
    ht_depot_lock_all();
    ht_reg_lock_all();
}

static void atfork_release(void)
{
    ht_reg_unlock_all();
    ht_depot_unlock_all();
    ht_ctx_unlock_all();
}

static void init_once(void)
{
    HtGuard guard;

    g_ht_cfg.max_records = env_u32("HEAPTRACK_MAX_RECORDS", g_ht_cfg.max_records);
    g_ht_cfg.depth = env_u32("HEAPTRACK_DEPTH", g_ht_cfg.depth);
    if (g_ht_cfg.depth > HT_MAX_DEPTH)
        g_ht_cfg.depth = HT_MAX_DEPTH;
    g_ht_cfg.min_stack_size = env_u32("HEAPTRACK_MIN_SIZE", g_ht_cfg.min_stack_size);
    g_ht_cfg.max_findings = env_u32("HEAPTRACK_MAX_FINDINGS", g_ht_cfg.max_findings);
    g_ht_cfg.report_at_exit = (int)env_u32("HEAPTRACK_REPORT_AT_EXIT", 1);
    g_ht_cfg.use_signal = (int)env_u32("HEAPTRACK_SIGNAL", 0);
    g_ht_cfg.sink_path = getenv("HEAPTRACK_SINK");

    ht_intern_init();
    ht_ctx_init();
    ht_reg_init();
    ht_depot_init();
    ht_unwind_warmup();

    pthread_atfork(atfork_prepare, atfork_release, atfork_release);
    ht_report_init();
}

void ht_init(void)
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once(&once, init_once);
}

__attribute__((constructor)) static void ht_ctor(void)
{
    ht_init();
}

__attribute__((destructor)) static void ht_dtor(void)
{
    if (g_ht_cfg.report_at_exit && ht_load(&g_tracking_enabled))
        ht_report_run();
}

extern "C" {

HT_EXPORT void heaptrack_begin(const char *api_name)
{
    ht_init();
    HtGuard guard;
    ht_ctx_begin(api_name);
}

HT_EXPORT void heaptrack_end(void)
{
    HtGuard guard;
    ht_ctx_end(ht_ctx_top());
}

HT_EXPORT heaptrack_scope_t heaptrack_scope_enter(const char *api_name)
{
    ht_init();
    HtGuard guard;
    heaptrack_scope_t s;
    s.node = ht_ctx_begin(api_name);
    return s;
}

HT_EXPORT void heaptrack_scope_exit(heaptrack_scope_t *s)
{
    HtGuard guard;
    ht_ctx_end(s->node);
}

HT_EXPORT void heaptrack_global_begin(const char *component_name)
{
    ht_init();
    HtGuard guard;
    ht_ctx_global_begin(component_name);
}

HT_EXPORT void heaptrack_global_end(void)
{
    HtGuard guard;
    ht_ctx_global_end();
}

HT_EXPORT void heaptrack_bind(const void *old_ptr, const void *new_ptr,
                              const char *var_name, const char *file, int line)
{
    if (!ht_load(&g_tracking_enabled))
        return;

    HtGuard guard;
    uint32_t site = ht_site_intern(var_name, file, line);
    uint32_t shard;

    if (new_ptr)
    {
        HeapAllocRecord *rec = ht_reg_find_locked((uintptr_t)new_ptr, &shard);
        if (rec)
        {
            rec->site_id = site;
            ht_reg_unlock(shard);
        }
    }
    if (old_ptr && old_ptr != new_ptr)
    {
        HeapAllocRecord *rec = ht_reg_find_locked((uintptr_t)old_ptr, &shard);
        if (rec)
        {
            rec->flags |= HT_F_OVERWRITTEN;
            rec->rebind_site_id = site;
            ht_reg_unlock(shard);
        }
    }
}

HT_EXPORT void heaptrack_handoff(const void *ptr, const char *new_owner_api)
{
    if (!ht_load(&g_tracking_enabled) || !ptr)
        return;

    HtGuard guard;
    uint32_t owner = ht_ctx_named_node(new_owner_api);
    uint32_t shard;
    HeapAllocRecord *rec = ht_reg_find_locked((uintptr_t)ptr, &shard);
    if (!rec)
        return;
    rec->owner_node = owner;
    rec->handoff_count++;
    rec->flags |= HT_F_HANDED_OFF;
    rec->flags &= (uint8_t)~HT_F_OVERWRITTEN;
    ht_reg_unlock(shard);
}

HT_EXPORT unsigned int heaptrack_ctx_capture(void)
{
    return ht_ctx_current();
}

HT_EXPORT void heaptrack_ctx_attach(unsigned int ctx)
{
    ht_init();
    HtGuard guard;
    ht_ctx_push_node(ctx);
}

HT_EXPORT void heaptrack_ctx_detach(void)
{
    HtGuard guard;
    ht_ctx_pop_node();
}

HT_EXPORT void heaptrack_note_alloc(const void *ptr, size_t size)
{
    if (ptr)
        ht_on_alloc((void *)ptr, size, HT_KIND_NOTED);
}

HT_EXPORT void heaptrack_note_free(const void *ptr)
{
    if (ptr)
        ht_on_free((void *)ptr);
}

HT_EXPORT void heaptrack_set_sink(const HeaptrackSink *sink)
{
    ht_init();
    ht_sink_set(sink);
}

HT_EXPORT void heaptrack_report(void)
{
    ht_init();
    ht_report_run();
}

} /* extern "C" */
