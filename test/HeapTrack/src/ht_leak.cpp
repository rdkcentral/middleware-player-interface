#include "ht_internal.h"

static bool is_overwritten(const HeapAllocRecord *r, const HeapTrackNode *)
{
    return (r->flags & HT_F_OVERWRITTEN) != 0;
}

static bool is_in_scope(const HeapAllocRecord *, const HeapTrackNode *n)
{
    return n && ht_load(&n->active_invocations) > 0;
}

static bool is_outlived(const HeapAllocRecord *, const HeapTrackNode *)
{
    return true;
}

struct Classifier
{
    HtFinding finding;
    bool (*test)(const HeapAllocRecord *, const HeapTrackNode *);
};

/* Checked in order, first match wins. */
static const Classifier g_classifiers[] = {
    {HT_FIND_OVERWRITTEN, is_overwritten},
    {HT_FIND_IN_SCOPE, is_in_scope},
    {HT_FIND_OUTLIVED_SCOPE, is_outlived},
};

HtFinding ht_classify(const HeapAllocRecord *rec, const HeapTrackNode *node)
{
    for (unsigned i = 0; i < sizeof(g_classifiers) / sizeof(g_classifiers[0]); i++)
    {
        if (g_classifiers[i].test(rec, node))
            return g_classifiers[i].finding;
    }
    return HT_FIND_IN_SCOPE;
}

const char *ht_finding_name(HtFinding f)
{
    switch (f)
    {
    case HT_FIND_IN_SCOPE:
        return "IN_SCOPE";
    case HT_FIND_OUTLIVED_SCOPE:
        return "OUTLIVED_SCOPE";
    case HT_FIND_OVERWRITTEN:
        return "OVERWRITTEN";
    default:
        return "UNKNOWN";
    }
}
