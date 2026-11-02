/* alloc_basic + nested_api + lifetime: attribution, roll-up, outliving scope */
#include "heaptrack.hpp"
#include "ht_test_sink.h"

#include <cstdio>
#include <cstdlib>

static void *g_outlives_scope;

static void parse_manifest()
{
    HEAPTRACK_SCOPE_CPP("parse_manifest");
    void *tmp = malloc(4096);
    ht_test_touch(tmp, 4096);
    free(tmp);

    /* Still live when the scope ends -> OUTLIVED_SCOPE at report time. */
    g_outlives_scope = malloc(65536);
    ht_test_touch(g_outlives_scope, 65536);
}

static void initialize_stream()
{
    /* No scope of its own: attributed to the innermost active node. */
    void *p = calloc(16, 64);
    ht_test_touch(p, 1024);
    free(p);
}

static void player_initialize()
{
    HEAPTRACK_SCOPE_CPP("Player::initialize");
    void *buf = malloc(1024);
    ht_test_touch(buf, 1024);
    parse_manifest();
    initialize_stream();
    free(buf);
}

int main()
{
    ht_test_use_stdout_sink();

    void *before = malloc(32); /* outside any scope -> untracked */
    ht_test_touch(before, 32);
    free(before);

    player_initialize();
    player_initialize();

    printf("[test_basic] scopes done, reporting\n");
    heaptrack_report();

    free(g_outlives_scope);
    printf("[test_basic] freed late allocation, reporting again\n");
    heaptrack_report();
    return 0;
}
