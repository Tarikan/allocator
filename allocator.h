//
// Created by Tarikan on 08.03.2021.
//

#ifndef LAB1_ALLOCATOR_H
#define LAB1_ALLOCATOR_H

#include <stddef.h>

void *mem_alloc(size_t size);

void mem_free(void *ptr);

void *mem_realloc(void *ptr, size_t new_size);

void mem_dump();

#endif //LAB1_ALLOCATOR_H


