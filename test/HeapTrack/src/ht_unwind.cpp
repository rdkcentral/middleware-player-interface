#include "ht_internal.h"

#include <unwind.h>
#include <link.h>

static uintptr_t g_self_lo;
static uintptr_t g_self_hi;

struct UnwindState
{
    uintptr_t *pcs;
    int max;
    int n;
};

static int self_range_cb(struct dl_phdr_info *info, size_t, void *arg)
{
    uintptr_t probe = *(uintptr_t *)arg;
    for (int i = 0; i < info->dlpi_phnum; i++)
    {
        const ElfW(Phdr) *ph = &info->dlpi_phdr[i];
        if (ph->p_type != PT_LOAD)
            continue;
        uintptr_t lo = (uintptr_t)info->dlpi_addr + ph->p_vaddr;
        uintptr_t hi = lo + ph->p_memsz;
        if (probe >= lo && probe < hi)
        {
            g_self_lo = lo;
            g_self_hi = hi;
            return 1;
        }
    }
    return 0;
}

static _Unwind_Reason_Code unwind_cb(struct _Unwind_Context *ctx, void *arg)
{
    UnwindState *s = (UnwindState *)arg;
    uintptr_t ip = (uintptr_t)_Unwind_GetIP(ctx);
    if (!ip)
        return _URC_END_OF_STACK;
    /* Drop the tracker's own leading frames. */
    if (s->n == 0 && ip >= g_self_lo && ip < g_self_hi)
        return _URC_NO_REASON;
    s->pcs[s->n++] = ip;
    return (s->n >= s->max) ? _URC_END_OF_STACK : _URC_NO_REASON;
}

int ht_unwind(uintptr_t *pcs, int max)
{
    UnwindState s = {pcs, max, 0};
    _Unwind_Backtrace(unwind_cb, &s);
    return s.n;
}

void ht_unwind_warmup(void)
{
    uintptr_t probe = (uintptr_t)&ht_unwind;
    dl_iterate_phdr(self_range_cb, &probe);

    /* The unwinder allocates on first use: do it under the recursion guard. */
    HtGuard g;
    uintptr_t pcs[4];
    ht_unwind(pcs, 4);
}
