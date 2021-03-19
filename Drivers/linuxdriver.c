//
// Created by Tarikan on 09.03.2021.
//

#include "../driver.h"
#include <sys/mman.h>
#include "../arena.h"
#include <unistd.h>

static int page_size;

void *kernel_alloc(size_t size) {
    if (!page_size)
        page_size = sysconf(_SC_PAGESIZE);


    return mmap(NULL, size,
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1, 0);
}

void kernel_free(void *ptr) {
    munmap(ptr, ((struct Arena *) ptr)->size);
}

int get_page_size() {
    if (!page_size)
        page_size = getpagesize();
    return page_size;
}