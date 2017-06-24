#include "stdlib.h"
#include "stdio.h"
#include "stddef.h"

void* e_malloc(size_t size)
{
    void* res = malloc(size);
    if(res == NULL) {
        printf("Can't allocate enough memory: %u bytes", &size);
        exit(1);
    }
    return res;
}

void* e_realloc(void* ptr, size_t size)
{
    void* res = realloc(ptr, size);
    if(res == NULL) {
        printf("Can't reallocate block %p to %u bytes", &size);
        exit(1);
    }
    return res;
}

void* e_calloc(size_t num, size_t size)
{
    void* res = calloc(num, size);
    if(res == NULL) {
        size *= num;
        printf("Can't allocate enough memory: %u bytes", &size);
        exit(1);
    }
    return res;
}