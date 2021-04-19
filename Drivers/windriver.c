#include "../driver.h"
#include "memoryapi.h"
#include <windows.h>


static DWORD page_size;

void *kernel_alloc(size_t size) {

    return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void kernel_free(void *ptr) {
    WINBOOL result = VirtualFree(ptr, 0, MEM_RELEASE);
    if (!result) {
        printf("VirtualAlloc exited with error, aborting");
        abort();
    }
}

size_t get_page_size() {
    if (!page_size) {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        page_size = si.dwPageSize;
    }

    return page_size;
}