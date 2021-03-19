//
// Created by Tarikan on 09.03.2021.
//

#include "../driver.h"
#include "memoryapi.h"
#include "../arena.h"
#include <windows.h>

static int page_size;

void *kernel_alloc(size_t size) {


    return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void kernel_free(void *ptr) {
    VirtualFree(ptr, 0, MEM_RELEASE);
}

int get_page_size() {
    if (!page_size) {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        page_size = si.dwPageSize;
    }

    return page_size;
}