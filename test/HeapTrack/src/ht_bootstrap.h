#ifndef HT_BOOTSTRAP_H
#define HT_BOOTSTRAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void *ht_real_malloc(size_t size);
void *ht_real_calloc(size_t n, size_t size);
void *ht_real_realloc(void *ptr, size_t size);
void *ht_real_memalign(size_t align, size_t size);
void ht_real_free(void *ptr);
size_t ht_real_usable_size(void *ptr);

/* Allocations served from the static bootstrap arena before the real
 * allocator is resolved. They must never be passed to the real free(). */
int ht_boot_owns(const void *ptr);
size_t ht_boot_size(const void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* HT_BOOTSTRAP_H */
