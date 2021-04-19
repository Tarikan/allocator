#ifndef LAB1_DRIVER_H
#define LAB1_DRIVER_H

#include <stddef.h>

void *kernel_alloc(size_t size);

void kernel_free(void *ptr);

size_t get_page_size();

#endif //LAB1_DRIVER_H
