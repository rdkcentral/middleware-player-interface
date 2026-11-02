#ifndef HT_INTERNAL_H
#define HT_INTERNAL_H

#include "ht_platform.h"
#include "ht_bootstrap.h"
#include "heaptrack.h"

/* ---------------- Common types ---------------- */

enum HtKind
{
    HT_KIND_MALLOC = 0,
    HT_KIND_CALLOC,
    HT_KIND_REALLOC,
    HT_KIND_ALIGNED,
    HT_KIND_NEW,
    HT_KIND_NEW_ARRAY,
    HT_KIND_NOTED,
    HT_KIND_NONE = 0xFF
};

enum HtRecFlags
{
    HT_F_OVERWRITTEN = 1,
    HT_F_HANDED_OFF = 2
};

struct HeapAllocRecord
{
    uintptr_t addr;
    uint32_t size;
    uint32_t origin_node;
    uint32_t owner_node;
    uint32_t stack_id;
    uint32_t site_id;
    uint32_t rebind_site_id;
    uint64_t alloc_ts_ns;
    uint32_t tid;
    uint8_t kind;
    uint8_t flags;
    uint16_t handoff_count;
};

struct HeapTrackStats
{
    uint64_t calls;
    uint64_t allocs;
    uint64_t frees;
    uint64_t bytes_alloc;
    uint64_t bytes_freed;
    uint64_t cur_bytes;
    uint64_t peak_bytes;
    uint64_t live;
    uint64_t cross_frees;
    uint32_t mismatched_frees;
};

struct HeapTrackNode
{
    uint32_t node_id;
    uint32_t parent_id;
    uint32_t api_name_id;
    int active_invocations;
    HeapTrackStats stats;
};

struct HtSite
{
    uint32_t var_name_id;
    uint32_t file_id;
    int32_t line;
};

struct HtConfig
{
    uint32_t max_records;
    uint32_t depth;
    uint32_t min_stack_size;
    uint32_t max_findings;
    int report_at_exit;
    int use_signal;
    const char *sink_path;
};

#define HT_MAX_DEPTH 32
#define HT_SCOPE_DEPTH 32
#define HT_MAX_NODES 4096
#define HT_SHARDS 64

extern HtConfig g_ht_cfg;
extern uint32_t g_tracking_enabled;
extern uint32_t g_live_tracked;
extern uint64_t g_registry_full_drops;
extern uint64_t g_node_overflows;
extern uint64_t g_depot_full_drops;
extern uint64_t g_pairing_errors;

void ht_init(void);

/* ---------------- Recursion guard (section 13) ---------------- */

extern HT_TLS int t_ht_guard;

struct HtGuard
{
    HtGuard() { t_ht_guard++; }
    ~HtGuard() { t_ht_guard--; }
};

static inline bool ht_guarded() { return t_ht_guard != 0; }

/* ---------------- String / site interning ---------------- */

void ht_intern_init(void);
uint32_t ht_intern(const char *s);
const char *ht_str(uint32_t id);
uint32_t ht_site_intern(const char *var, const char *file, int line);
const HtSite *ht_site(uint32_t id);

/* ---------------- Context manager ---------------- */

void ht_ctx_init(void);
uint32_t ht_ctx_begin(const char *api_name);
void ht_ctx_end(uint32_t node);
uint32_t ht_ctx_current(void);
uint32_t ht_ctx_top(void);
uint32_t ht_ctx_named_node(const char *name);
void ht_ctx_push_node(uint32_t node);
void ht_ctx_pop_node(void);
void ht_ctx_global_begin(const char *component_name);
void ht_ctx_global_end(void);
HeapTrackNode *ht_node(uint32_t id);
uint32_t ht_node_count(void);
void ht_ctx_lock_all(void);
void ht_ctx_unlock_all(void);

/* ---------------- Registry ---------------- */

void ht_reg_init(void);
bool ht_reg_insert(const HeapAllocRecord *rec);
bool ht_reg_take(uintptr_t addr, HeapAllocRecord *out);
HeapAllocRecord *ht_reg_find_locked(uintptr_t addr, uint32_t *shard);
void ht_reg_unlock(uint32_t shard);
uint32_t ht_reg_snapshot(HeapAllocRecord *out, uint32_t max);
void ht_reg_lock_all(void);
void ht_reg_unlock_all(void);

void ht_freedby_note(uint32_t origin, uint32_t freeing, uint32_t bytes);
uint32_t ht_freedby_count(void);
bool ht_freedby_get(uint32_t index, uint32_t *origin, uint32_t *freeing,
                    uint64_t *count, uint64_t *bytes);

/* ---------------- Interceptor hooks ---------------- */

extern HT_TLS uint8_t t_ht_alloc_kind;
extern HT_TLS uint8_t t_ht_free_kind;

void ht_on_alloc(void *ptr, size_t size, uint8_t default_kind);
void ht_on_free(void *ptr);

/* ---------------- Unwinder / stack depot ---------------- */

void ht_unwind_warmup(void);
int ht_unwind(uintptr_t *pcs, int max);

void ht_depot_init(void);
uint32_t ht_depot_put(const uintptr_t *pcs, int n);
const uintptr_t *ht_depot_get(uint32_t id, int *n);
void ht_depot_lock_all(void);
void ht_depot_unlock_all(void);

/* ---------------- ELF symbol reader ---------------- */

struct HtElf
{
    void *map;
    size_t size;
    const void *syms;
    uint32_t nsyms;
    uint32_t symentsz;
    const char *strs;
    size_t strsz;
};

bool ht_elf_open(HtElf *e, const char *path);
void ht_elf_close(HtElf *e);
bool ht_elf_lookup(const HtElf *e, uintptr_t vaddr, const char **name, uintptr_t *off);

/* ---------------- Symbolizer ---------------- */

void ht_sym_begin(void);
void ht_sym_end(void);
/* writes "name+0xoff" or "module+0xoff" into buf */
void ht_sym_resolve(uintptr_t pc, char *buf, size_t buflen);

/* ---------------- Leak classification ---------------- */

enum HtFinding
{
    HT_FIND_IN_SCOPE = 0,
    HT_FIND_OUTLIVED_SCOPE,
    HT_FIND_OVERWRITTEN,
    HT_FIND_COUNT
};

HtFinding ht_classify(const HeapAllocRecord *rec, const HeapTrackNode *node);
const char *ht_finding_name(HtFinding f);

/* ---------------- Sink / writer ---------------- */

struct HtWriter
{
    char buf[4096];
    size_t len;
};

void ht_sink_set(const HeaptrackSink *sink);
void ht_sink_open(void);
void ht_sink_close(void);
void ht_w_flush(HtWriter *w);
void ht_w_raw(HtWriter *w, const char *s, size_t n);
/* %s const char*, %d int, %u uint64_t, %x uintptr_t, %b byte count */
void ht_w(HtWriter *w, const char *fmt, ...);

/* ---------------- Report ---------------- */

void ht_report_init(void);
void ht_report_run(void);

#endif /* HT_INTERNAL_H */
