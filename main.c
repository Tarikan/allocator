/*
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
        mem_free(test_arr[i]);
    }

    int* test2 = mem_alloc(222);

    int* test4 = mem_alloc(241);

    mem_realloc(test4, 500);

    int *test3 = mem_alloc(8096);

    mem_dump();

    //mem_free(test);
    return 0;
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "allocator.h"

#define N 100000
#define MAX_ARRAY_SIZE 500

#define min(a, b) min_size_t(a, b)

static size_t min_size_t(size_t a, size_t b) {
    return a < b ? a : b;
}

struct Result {
    void *curr;
    size_t curr_size;
    unsigned int checksum;
};

void randomize_place(void *ptr, size_t size) {
    for (size_t i = 0;
         i < size;
         i++) {
        *((char *) ptr + i) = (char) rand();
    }
}

unsigned int get_checksum(void *ptr, size_t size) {
    unsigned int sum = 0;
    for (size_t i = 0;
         i < size;
         i++)
        sum = (sum << 2) ^ (sum >> 5) ^ *((char *) ptr + i);
    return sum;
}

void auto_test(size_t max_size) {
    unsigned int seed = time(NULL);
    srand(seed);
    struct Result results[MAX_ARRAY_SIZE] = {{.checksum = 0, .curr = NULL, .curr_size = 0}};
    printf("TEST START\nseed: %u\n", seed);

    for (unsigned int i = 0;
         i < N;
         i++) {
        // 0 - ALLOC
        // 1 - REALLOC
        // 2 - FREE
        // if (i % (N / 100) == 0)
        //     printf("%u\n", i / (N / 100));
        unsigned short action = rand() % 3;
        size_t size = rand() % max_size;
        unsigned int rand_index = rand() % MAX_ARRAY_SIZE;
        struct Result result;
        void *ptr;
        if (i % N / 100 == 0) {
            printf("%ld\n", i);
        }
        //printf("%ld\n", i);
        switch (action) {
            case 0: // ALLOC
                ptr = mem_alloc(size);
                if (ptr) {
                    randomize_place(ptr, size);
                    result = (struct Result) {
                            .curr = ptr,
                            .curr_size = size,
                            .checksum = get_checksum(ptr, size),
                    };
                    mem_free(results[rand_index].curr);
                    results[rand_index] = result;
                }
                break;
            case 1:
                result = results[rand_index];
                if (result.curr) { // Якщо був алоцирований
                    if (get_checksum(result.curr, result.curr_size) != result.checksum) {
                        printf("Current: %ld\nExprected: %ld", get_checksum(result.curr, result.curr_size), result.checksum);
                    }
                    assert(get_checksum(result.curr, result.curr_size) == result.checksum && "bad checksum");
                }
                unsigned int controll = get_checksum(result.curr, min(size, result.curr_size));
                void *ptr1 = mem_realloc(result.curr, size);
                if (ptr1) {
                    if (get_checksum(ptr1, min(size, result.curr_size)) != controll) {
                        printf("Current: %ld\nExprected: %ld\nNew size: %ld\nOld Size: %ld\n",
                               get_checksum(ptr1, min(size, result.curr_size)),
                               controll,
                               size,
                               result.curr_size);
                    }
                    assert(get_checksum(ptr1, min(size, result.curr_size)) == controll && "bad checksum");
                    randomize_place(ptr1, size);
                    results[rand_index] = (struct Result) {
                            .curr = ptr1,
                            .curr_size = size,
                            .checksum = get_checksum(ptr1, size),
                    };
                }
                break;
            case 2:
                result = results[rand_index];
                if (result.curr) { // Якщо був алоцирований
                    if (get_checksum(result.curr, result.curr_size) != result.checksum) {
                        printf("Current: %ld\nExprected: %ld", get_checksum(result.curr, result.curr_size), result.checksum);
                    }
                    assert(get_checksum(result.curr, result.curr_size) == result.checksum && "bad checksum");
                }
                mem_free(result.curr);
                results[rand_index] = (struct Result) {
                        .curr = NULL,
                        .curr_size = 0,
                        .checksum = 0,
                };

                break;
            default:
                break;
        }
    }

// clean all alloc blocks

    for (
            unsigned int i = 0;
            i < MAX_ARRAY_SIZE;
            i++) {
        assert(get_checksum(results[i].curr, results[i].curr_size) == results[i].checksum && "bad checksum");
        mem_free(results[i]
                         .curr);
    }

    printf("TEST END\n");
}

int main() {
    // test with very small allocation max_size
    printf("test with very small allocation max_size\n");
    auto_test(16);
    // test with small allocation max_size
    printf("test with small allocation max_size\n");
    auto_test(512);
    // test with big allocation max_size
    printf("test with big allocation max_size\n");
    auto_test(RAND_MAX);
    return 0;
}