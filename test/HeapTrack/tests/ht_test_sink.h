#ifndef HT_TEST_SINK_H
#define HT_TEST_SINK_H

#include "heaptrack.h"
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

#endif
