#ifndef HT_TEST_SINK_H
#define HT_TEST_SINK_H

#include "heaptrack.h"
#include <string.h>
#include <unistd.h>

static void ht_test_sink_write(void *ctx, const char *data, size_t len)
{
    (void)ctx;
    ssize_t r = write(1, data, len);
    (void)r;
}

static void ht_test_use_stdout_sink(void)
{
    HeaptrackSink sink;
    sink.write = ht_test_sink_write;
    sink.ctx = 0;
    heaptrack_set_sink(&sink);
}

/* Make the block escape, otherwise the compiler deletes the whole
 * malloc/free pair at -O2 and there is nothing left to track. */
static void *volatile ht_test_escape;

static void ht_test_touch(void *p, size_t len)
{
    if (!p)
        return;
    memset(p, 0x5A, len < 64 ? len : 64);
    ht_test_escape = p;
}

#endif
