#ifndef LIB_H
#define LIB_H

#define e_free(x) free(x)

void* e_malloc(size_t size);
void* e_calloc(size_t num, size_t size);
void* e_realloc(void* ptr, size_t size);

#endif