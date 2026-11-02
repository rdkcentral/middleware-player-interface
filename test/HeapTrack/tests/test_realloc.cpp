/* realloc protocol: realloc(NULL,n), realloc(p,0), grow, shrink, untracked p */
#include "heaptrack.hpp"
#include "ht_test_sink.h"

#include <cstdlib>

int main()
{
    ht_test_use_stdout_sink();

    void *outside = malloc(64); /* allocated with no active scope */
    ht_test_touch(outside, 64);

    {
        HEAPTRACK_SCOPE_CPP("realloc_api");

        void *a = realloc(NULL, 128); /* behaves as malloc */
        ht_test_touch(a, 128);

        a = realloc(a, 4096); /* grow, probably moves */
        ht_test_touch(a, 4096);

        a = realloc(a, 64); /* shrink */
        ht_test_touch(a, 64);

        void *b = realloc(outside, 256); /* untracked p -> registered as new */
        ht_test_touch(b, 256);

        void *c = malloc(512);
        ht_test_touch(c, 512);
        c = realloc(c, 0); /* glibc frees and returns NULL */
        ht_test_touch(c, 1);

        free(a);
        free(b);

        void *leak = realloc(NULL, 777); /* left live on purpose */
        ht_test_touch(leak, 777);
    }

    heaptrack_report();
    return 0;
}
