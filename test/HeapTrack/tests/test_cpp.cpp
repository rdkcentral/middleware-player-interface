/* new/delete kind hints, mismatch detection, overwrite leak, ownership handoff */
#include "heaptrack.hpp"
#include "ht_test_sink.h"

#include <cstdio>
#include <cstdlib>

struct Sample
{
    char payload[128];
};

static void *g_handed_off;

static void new_delete_cases()
{
    HEAPTRACK_SCOPE_CPP("Cpp::newDelete");

    Sample *s = new Sample();
    delete s; /* matched */

    char *arr = new char[256];
    delete[] arr; /* matched */

    char *bad = new char[64];
    delete bad; /* mismatch: new[] freed with delete */

    void *m = malloc(32);
    operator delete(m); /* mismatch: malloc freed with delete */

    Sample *live = new Sample();
    (void)live; /* leaked on purpose */
}

static void overwrite_case()
{
    HEAPTRACK_SCOPE_CPP("Cpp::overwrite");

    char *buffer = NULL;
    HEAPTRACK_ASSIGN(buffer, (char *)malloc(4096));
    HEAPTRACK_ASSIGN(buffer, (char *)malloc(8192)); /* old block lost -> OVERWRITTEN */
    free(buffer);
}

static void handoff_case()
{
    HEAPTRACK_SCOPE_CPP("Player::initialize");

    char *owned = NULL;
    HEAPTRACK_ASSIGN(owned, (char *)malloc(4096));
    HEAPTRACK_HANDOFF(owned, "Worker::run");
    g_handed_off = owned;
}

static void worker_frees()
{
    HEAPTRACK_SCOPE_CPP("Worker::run");
    free(g_handed_off); /* cross-context free, shows in the freed-by summary */
}

int main()
{
    ht_test_use_stdout_sink();

    new_delete_cases();
    overwrite_case();
    handoff_case();
    worker_frees();

    printf("[test_cpp] done, reporting\n");
    heaptrack_report();
    return 0;
}
