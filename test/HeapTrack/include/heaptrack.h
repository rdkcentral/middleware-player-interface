#ifndef HEAPTRACK_H
#define HEAPTRACK_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------- Scope tracking ---------------- */

void heaptrack_begin(const char *api_name);
void heaptrack_end(void);

typedef struct
{
    unsigned int node;
} heaptrack_scope_t;

heaptrack_scope_t heaptrack_scope_enter(const char *api_name);
void heaptrack_scope_exit(heaptrack_scope_t *s);

#define HT_CAT2(a, b) a##b
#define HT_CAT(a, b) HT_CAT2(a, b)

/* Early-return safe scope for GCC/Clang (C) */
#define HEAPTRACK_SCOPE(name)                                   \
    heaptrack_scope_t HT_CAT(_ht_scope_, __LINE__)              \
        __attribute__((cleanup(heaptrack_scope_exit))) =        \
            heaptrack_scope_enter(name)

/* ---------------- Component-wide mode ---------------- */

void heaptrack_global_begin(const char *component_name);
void heaptrack_global_end(void);

/* ---------------- Variable binding / ownership ---------------- */

void heaptrack_bind(const void *old_ptr, const void *new_ptr,
                    const char *var_name, const char *file, int line);
void heaptrack_handoff(const void *ptr, const char *new_owner_api);

/* var must be free of side effects, it is evaluated more than once */
#define HEAPTRACK_ASSIGN(var, expr)                                  \
    do                                                               \
    {                                                                \
        __typeof__(var) _ht_new = (expr);                            \
        heaptrack_bind((const void *)(var), (const void *)_ht_new,   \
                       #var, __FILE__, __LINE__);                    \
        (var) = _ht_new;                                             \
    } while (0)

#define HEAPTRACK_HANDOFF(ptr, owner) heaptrack_handoff((const void *)(ptr), (owner))

/* ---------------- Asynchronous context propagation ---------------- */

unsigned int heaptrack_ctx_capture(void);
void heaptrack_ctx_attach(unsigned int ctx);
void heaptrack_ctx_detach(void);

/* ---------------- Custom allocators ---------------- */

void heaptrack_note_alloc(const void *ptr, size_t size);
void heaptrack_note_free(const void *ptr);

/* ---------------- Reporting ---------------- */

typedef struct HeaptrackSink
{
    void (*write)(void *ctx, const char *data, size_t len);
    void *ctx;
} HeaptrackSink;

void heaptrack_set_sink(const HeaptrackSink *sink);
void heaptrack_report(void);

#ifdef __cplusplus
}
#endif

#endif /* HEAPTRACK_H */
