//
// Created by Tarikan on 08.03.2021.
//

#ifndef LAB1_DRIVER_H
#define LAB1_DRIVER_H

#include <stddef.h>

void *kernel_alloc(size_t size);

void kernel_free(void *ptr);

int get_page_size();

#endif //LAB1_DRIVER_H
