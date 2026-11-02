/* cross-thread free, async context adopt, concurrent alloc/free stress */
#include "heaptrack.hpp"
#include "ht_test_sink.h"

#include <cstdlib>
#include <pthread.h>

#define STRESS_THREADS 4
#define STRESS_ITERS 20000

static void *g_queued;
static unsigned int g_captured_ctx;

static void *consumer_main(void *)
{
    HEAPTRACK_SCOPE_CPP("Worker::consume");
    free(g_queued); /* allocated on the main thread */
    return NULL;
}

static void *adopted_main(void *)
{
    heaptrack::AdoptContext adopt(g_captured_ctx);
    void *p = malloc(2048); /* attributed to the submitting thread's node */
    ht_test_touch(p, 2048);
    free(p);
    void *leak = malloc(1024);
    ht_test_touch(leak, 1024);
    return NULL;
}

static void *stress_main(void *)
{
    HEAPTRACK_SCOPE_CPP("Stress::worker");
    for (int i = 0; i < STRESS_ITERS; i++)
    {
        void *p = malloc(64 + (i & 63));
        ht_test_touch(p, 64);
        free(p);
    }
    return NULL;
}

int main()
{
    ht_test_use_stdout_sink();

    pthread_t th;

    {
        HEAPTRACK_SCOPE_CPP("Player::initialize");
        g_queued = malloc(4096);
        ht_test_touch(g_queued, 4096);
        g_captured_ctx = heaptrack_ctx_capture();

        pthread_create(&th, NULL, consumer_main, NULL);
        pthread_join(th, NULL);

        pthread_create(&th, NULL, adopted_main, NULL);
        pthread_join(th, NULL);
    }

    {
        HEAPTRACK_SCOPE_CPP("Stress");
        pthread_t ts[STRESS_THREADS];
        for (int i = 0; i < STRESS_THREADS; i++)
            pthread_create(&ts[i], NULL, stress_main, NULL);
        for (int i = 0; i < STRESS_THREADS; i++)
            pthread_join(ts[i], NULL);
    }

    heaptrack_report();
    return 0;
}
