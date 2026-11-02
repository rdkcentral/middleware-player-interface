/* realloc protocol: realloc(NULL,n), realloc(p,0), grow, shrink, untracked p */
#include "heaptrack.hpp"
#include "ht_test_sink.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

int main()
{
    ht_test_use_stdout_sink();

    void *outside = malloc(64); /* allocated with no active scope */

    {
        HEAPTRACK_SCOPE_CPP("realloc_api");

        void *a = realloc(NULL, 128); /* behaves as malloc */
        memset(a, 0xAA, 128);

        a = realloc(a, 4096); /* grow, probably moves */
        a = realloc(a, 64);   /* shrink */

        void *b = realloc(outside, 256); /* untracked p -> registered as new */

        void *c = malloc(512);
        c = realloc(c, 0); /* glibc frees and returns NULL */
        if (c != NULL)
            printf("[test_realloc] unexpected non-NULL from realloc(p,0)\n");

        free(a);
        free(b);

        /* left live on purpose */
        void *leak = realloc(NULL, 777);
        (void)leak;
    }

    printf("[test_realloc] done, reporting\n");
    heaptrack_report();
    return 0;
}
