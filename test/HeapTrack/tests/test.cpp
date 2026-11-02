/*
 * Full heaptrack demo: a mini media player with known allocation sizes.
 * Prints what is expected, then the tracker's own report, so the two can
 * be compared line by line.
 */
#include "heaptrack.hpp"
#include "ht_test_sink.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>

/* Keep the frames out of main so the captured call tree is meaningful. */
#define DEMO_API __attribute__((noinline))

struct Sample
{
    char payload[128];
};

static void *g_handed_to_worker; /* 32 KB, allocated in initialize, freed by worker */
static char *g_config;           /* re-assigned without free -> OVERWRITTEN */

/* ---- Player::initialize > Manifest::parse ---------------------------- */
static DEMO_API void manifest_parse()
{
    HEAPTRACK_SCOPE_CPP("Manifest::parse");

    char *text = strdup("#EXTM3U ... manifest body allocated inside libc strdup");
    ht_test_touch(text, 8);
    free(text);

    void *chunks[10];
    for (int i = 0; i < 10; i++)
    {
        chunks[i] = malloc(1024);
        ht_test_touch(chunks[i], 1024);
    }
    for (int i = 0; i < 9; i++)
        free(chunks[i]);
    /* chunks[9] (1 KB) leaks on purpose */
}

/* ---- Player::initialize ---------------------------------------------- */
static DEMO_API void player_initialize()
{
    HEAPTRACK_SCOPE_CPP("Player::initialize");

    void *leaked = malloc(64 * 1024); /* never freed */
    ht_test_touch(leaked, 64 * 1024);

    for (int i = 0; i < 4; i++)
    {
        void *scratch = malloc(2048);
        ht_test_touch(scratch, 2048);
        free(scratch);
    }

    g_handed_to_worker = malloc(32 * 1024);
    ht_test_touch(g_handed_to_worker, 32 * 1024);
    HEAPTRACK_HANDOFF(g_handed_to_worker, "Worker::run");

    manifest_parse();
}

/* ---- Player::tune > Decoder::allocBuffers ---------------------------- */
static DEMO_API void decoder_alloc_buffers()
{
    HEAPTRACK_SCOPE_CPP("Decoder::allocBuffers");

    void *buffers[5];
    for (int i = 0; i < 5; i++)
    {
        buffers[i] = malloc(256 * 1024);
        ht_test_touch(buffers[i], 256 * 1024);
    }
    for (int i = 0; i < 4; i++)
        free(buffers[i]);
    /* buffers[4] (256 KB) leaks on purpose */
}

/* ---- Player::tune > Subtitle::init ----------------------------------- */
static DEMO_API void subtitle_init()
{
    HEAPTRACK_SCOPE_CPP("Subtitle::init");

    Sample *s = new Sample();
    ht_test_touch(s, sizeof(Sample));
    delete s; /* matched */

    char *cues = new char[512];
    ht_test_touch(cues, 512);
    delete cues; /* new[] freed with delete -> mismatched free */
}

static DEMO_API void player_tune()
{
    HEAPTRACK_SCOPE_CPP("Player::tune");
    decoder_alloc_buffers();
    subtitle_init();
}

/* ---- Config::load: pointer overwritten without free ------------------ */
static DEMO_API void config_load()
{
    HEAPTRACK_SCOPE_CPP("Config::load");

    HEAPTRACK_ASSIGN(g_config, (char *)malloc(4096));
    ht_test_touch(g_config, 4096);
    HEAPTRACK_ASSIGN(g_config, (char *)malloc(1024)); /* 4 KB lost here */
    ht_test_touch(g_config, 1024);
}

/* ---- Worker thread: frees what initialize handed over ---------------- */
static DEMO_API void *worker_main(void *)
{
    HEAPTRACK_SCOPE_CPP("Worker::run");
    free(g_handed_to_worker);
    return NULL;
}

/* Harness only: the expected numbers, so the report below can be compared
 * against them. A real component prints nothing; the tracker does it all. */
static void print_expectations()
{
    printf("\n===== HARNESS: what this workload does (expected) =====\n");
    printf("Player::initialize                  : 64 KB leak, 4x2 KB freed, 32 KB handed to Worker::run\n");
    printf("Player::initialize > Manifest::parse: strdup freed, 10x1 KB alloc, 9 freed -> 1 KB leak\n");
    printf("Player::tune > Decoder::allocBuffers: 5x256 KB alloc, 4 freed -> 256 KB leak\n");
    printf("Player::tune > Subtitle::init       : new/delete matched, new[]/delete -> 1 mismatched free\n");
    printf("Config::load                        : 4 KB OVERWRITTEN (lost from var 'g_config'), 1 KB live\n");
    printf("Worker::run                         : frees the 32 KB -> cross-context free in freed-by table\n");
    printf("Expected total suspected leak       : 64 + 1 + 256 + 4 + 1 = 326 KB\n");
    printf("=======================================================\n");
    fflush(stdout);
}

int main()
{
    ht_test_use_stdout_sink();
    print_expectations();

    player_initialize();
    player_tune();
    config_load();

    pthread_t th;
    pthread_create(&th, NULL, worker_main, NULL);
    pthread_join(th, NULL);

    heaptrack_report();
    return 0;
}
