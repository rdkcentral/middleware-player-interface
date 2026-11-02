#include "ht_internal.h"

#include <pthread.h>
#include <signal.h>
#include <unistd.h>

static HeapAllocRecord *g_snap;
static uint32_t *g_order;
static uint32_t g_snap_cap;
static pthread_mutex_t g_report_lock = PTHREAD_MUTEX_INITIALIZER;
static int g_pipe[2] = {-1, -1};

static uint64_t g_rollup[HT_MAX_NODES];
static uint32_t g_start[HT_MAX_NODES + 1];
static uint32_t g_cursor[HT_MAX_NODES + 1];
static uint32_t g_findings[HT_MAX_NODES][HT_FIND_COUNT];

static void write_node_path(HtWriter *w, uint32_t id)
{
    if (!id)
    {
        ht_w(w, "<untracked>");
        return;
    }
    HeapTrackNode *n = ht_node(id);
    if (!n)
    {
        ht_w(w, "<node %u>", (uint64_t)id);
        return;
    }
    if (n->parent_id)
    {
        write_node_path(w, n->parent_id);
        ht_w(w, " > ");
    }
    ht_w(w, "%s", ht_str(n->api_name_id));
}

static void write_site(HtWriter *w, uint32_t site_id)
{
    const HtSite *s = ht_site(site_id);
    if (!s)
    {
        ht_w(w, "-");
        return;
    }
    ht_w(w, "%s:%d", ht_str(s->file_id), s->line);
}

static void write_finding(HtWriter *w, const HeapAllocRecord *rec, HtFinding f)
{
    const HtSite *site = ht_site(rec->site_id);
    ht_w(w, "    [%s] var=%s size=%u site=", ht_finding_name(f),
         site ? ht_str(site->var_name_id) : "<unnamed>", (uint64_t)rec->size);
    write_site(w, rec->site_id);
    ht_w(w, " addr=%x", rec->addr);

    if (rec->rebind_site_id)
    {
        ht_w(w, " (re-assigned at ");
        write_site(w, rec->rebind_site_id);
        ht_w(w, ", old value lost from the variable)");
    }
    if (rec->owner_node != rec->origin_node)
    {
        ht_w(w, " owner=");
        write_node_path(w, rec->owner_node);
        ht_w(w, " (handoffs: %u)", (uint64_t)rec->handoff_count);
    }
    ht_w(w, "\n");

    int depth = 0;
    const uintptr_t *pcs = ht_depot_get(rec->stack_id, &depth);
    for (int i = 0; i < depth; i++)
    {
        char sym[512];
        ht_sym_resolve(pcs[i], sym, sizeof(sym));
        ht_w(w, "        #%u %s\n", (uint64_t)i, sym);
    }
}

void ht_report_run(void)
{
    HtGuard guard;
    pthread_mutex_lock(&g_report_lock);

    if (!g_snap)
    {
        g_snap_cap = g_ht_cfg.max_records;
        g_snap = (HeapAllocRecord *)ht_map((size_t)g_snap_cap * sizeof(HeapAllocRecord));
        g_order = (uint32_t *)ht_map((size_t)g_snap_cap * sizeof(uint32_t));
    }
    if (!g_snap || !g_order)
    {
        pthread_mutex_unlock(&g_report_lock);
        return;
    }

    uint32_t live = ht_reg_snapshot(g_snap, g_snap_cap);
    uint32_t nodes = ht_node_count();

    /* Group live records by origin node (counting sort) and classify. */
    memset(g_start, 0, sizeof(g_start));
    memset(g_findings, 0, sizeof(g_findings));
    for (uint32_t i = 0; i < live; i++)
    {
        uint32_t id = g_snap[i].origin_node;
        if (id >= HT_MAX_NODES)
            id = 0;
        g_start[id]++;
    }
    uint32_t acc = 0;
    for (uint32_t i = 0; i < HT_MAX_NODES; i++)
    {
        uint32_t c = g_start[i];
        g_start[i] = acc;
        g_cursor[i] = acc;
        acc += c;
    }
    g_start[HT_MAX_NODES] = acc;
    for (uint32_t i = 0; i < live; i++)
    {
        uint32_t id = g_snap[i].origin_node;
        if (id >= HT_MAX_NODES)
            id = 0;
        g_order[g_cursor[id]++] = i;
        g_findings[id][ht_classify(&g_snap[i], ht_node(id))]++;
    }

    /* Roll up current bytes into every ancestor. */
    memset(g_rollup, 0, sizeof(g_rollup));
    for (uint32_t i = 1; i < nodes; i++)
    {
        uint64_t cur = ht_get64(&ht_node(i)->stats.cur_bytes);
        uint32_t p = i;
        for (int hop = 0; p && hop < HT_SCOPE_DEPTH; hop++)
        {
            g_rollup[p] += cur;
            p = ht_node(p)->parent_id;
        }
    }

    ht_sink_open();
    ht_sym_begin();

    HtWriter w;
    w.len = 0;
    ht_w(&w, "\n========== HEAPTRACK REPORT (pid %u) ==========\n\n", (uint64_t)getpid());

    for (uint32_t id = 1; id < nodes; id++)
    {
        HeapTrackNode *n = ht_node(id);
        HeapTrackStats *s = &n->stats;
        if (!ht_get64(&s->calls) && !ht_get64(&s->allocs))
            continue;

        ht_w(&w, "API: ");
        write_node_path(&w, id);
        ht_w(&w, "   (calls: %u)\n", ht_get64(&s->calls));
        ht_w(&w, "  Total allocations     : %u\n", ht_get64(&s->allocs));
        ht_w(&w, "  Total deallocations   : %u\n", ht_get64(&s->frees));
        ht_w(&w, "  Total allocated       : %b\n", ht_get64(&s->bytes_alloc));
        ht_w(&w, "  Total freed           : %b\n", ht_get64(&s->bytes_freed));
        ht_w(&w, "  Current heap usage    : %b     (rolled up incl. children: %b)\n",
             ht_get64(&s->cur_bytes), g_rollup[id]);
        ht_w(&w, "  Peak heap usage       : %b\n", ht_get64(&s->peak_bytes));
        ht_w(&w, "  Live allocations      : %u\n", ht_get64(&s->live));
        ht_w(&w, "  Mismatched frees      : %u\n", (uint64_t)ht_load(&s->mismatched_frees));
        ht_w(&w, "  Cross-context frees   : %u\n", ht_get64(&s->cross_frees));
        ht_w(&w, "  Findings              : IN_SCOPE=%u OUTLIVED_SCOPE=%u OVERWRITTEN=%u\n",
             (uint64_t)g_findings[id][HT_FIND_IN_SCOPE],
             (uint64_t)g_findings[id][HT_FIND_OUTLIVED_SCOPE],
             (uint64_t)g_findings[id][HT_FIND_OVERWRITTEN]);

        uint32_t shown = 0;
        bool header = false;
        for (uint32_t k = g_start[id]; k < g_cursor[id] && shown < g_ht_cfg.max_findings; k++)
        {
            const HeapAllocRecord *rec = &g_snap[g_order[k]];
            HtFinding f = ht_classify(rec, n);
            if (f == HT_FIND_IN_SCOPE)
                continue;
            if (!header)
            {
                ht_w(&w, "\n  Findings\n");
                header = true;
            }
            write_finding(&w, rec, f);
            shown++;
        }
        ht_w(&w, "\n");
    }

    bool fb_header = false;
    for (uint32_t i = 0; i < ht_freedby_count(); i++)
    {
        uint32_t origin, freeing;
        uint64_t count, bytes;
        if (!ht_freedby_get(i, &origin, &freeing, &count, &bytes))
            continue;
        if (!fb_header)
        {
            ht_w(&w, "Freed-by summary\n");
            fb_header = true;
        }
        ht_w(&w, "  ");
        write_node_path(&w, origin);
        ht_w(&w, " -> ");
        write_node_path(&w, freeing);
        ht_w(&w, "   %u allocs   %b\n", count, bytes);
    }
    if (fb_header)
        ht_w(&w, "\n");

    ht_w(&w, "Live tracked allocations   : %u\n", (uint64_t)live);
    ht_w(&w, "Dropped records (registry full): %u\n", ht_get64(&g_registry_full_drops));
    ht_w(&w, "Node table overflows       : %u\n", ht_get64(&g_node_overflows));
    ht_w(&w, "Stack depot drops          : %u\n", ht_get64(&g_depot_full_drops));
    ht_w(&w, "Scope pairing errors       : %u\n", ht_get64(&g_pairing_errors));
    ht_w(&w, "==================================================\n");

    ht_w_flush(&w);
    ht_sym_end();
    ht_sink_close();
    pthread_mutex_unlock(&g_report_lock);
}

/* ---------------- Signal-driven reporter thread (section 11.3) ---------------- */

static void *reporter_main(void *)
{
    t_ht_guard++; /* the reporter never registers its own allocations */
    char c;
    while (read(g_pipe[0], &c, 1) == 1)
        ht_report_run();
    return NULL;
}

static void report_signal(int)
{
    char c = 1;
    ssize_t r = write(g_pipe[1], &c, 1);
    (void)r;
}

void ht_report_init(void)
{
    if (!g_ht_cfg.use_signal)
        return;
    if (pipe(g_pipe) != 0)
        return;

    pthread_t tid;
    if (pthread_create(&tid, NULL, reporter_main, NULL) != 0)
        return;
    pthread_detach(tid);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = report_signal;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR2, &sa, NULL);
}
