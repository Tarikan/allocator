//
// Created by Tarikan on 09.03.2021.
//

#define NUM 5

#include "allocator.h"
#include "string.h"

int main() {

    char *test_arr[NUM];

    for (int i = 0; i < NUM; i++) {
        test_arr[i] = mem_alloc(128);
        strcpy(test_arr[i], "Hello, World\n");
    }

    for (int i = 0; i < NUM; i++) {
        if (i == 6)
            continue;
        mem_free(test_arr[i]);
    }

    //int *test3 = mem_alloc(8096);

    mem_dump();

    //mem_free(test);
    return 0;
}
