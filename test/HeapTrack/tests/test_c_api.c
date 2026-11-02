/* pure C usage: cleanup-attribute scope macro, early return, HEAPTRACK_ASSIGN */
#include "heaptrack.h"
#include "ht_test_sink.h"

#include <stdlib.h>
#include <string.h>

static void *g_cache;

static int parse_manifest(int fail)
{
    HEAPTRACK_SCOPE("parse_manifest");

    char *tmp = malloc(2048);
    ht_test_touch(tmp, 2048);
    if (fail)
    {
        free(tmp);
        return -1; /* early return: scope still closed by the cleanup attribute */
    }

    HEAPTRACK_ASSIGN(g_cache, malloc(32768));
    ht_test_touch(g_cache, 32768);
    HEAPTRACK_ASSIGN(g_cache, malloc(16384)); /* previous block lost */
    ht_test_touch(g_cache, 16384);

    free(tmp);
    return 0;
}

static void player_tune(void)
{
    HEAPTRACK_SCOPE("player_tune");
    char *s = strdup("allocated inside libc");
    ht_test_touch(s, 8);
    parse_manifest(1);
    parse_manifest(0);
    free(s);
}

int main(void)
{
    ht_test_use_stdout_sink();

    heaptrack_global_begin("test_c_api");
    player_tune();
    heaptrack_global_end();
    heaptrack_report();
    return 0;
}
