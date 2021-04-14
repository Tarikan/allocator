#ifndef LAB1_TEST_H
#define LAB1_TEST_H
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "allocator.h"

#define ARRAY_SIZE 500

struct OperationResult {
    void *addr;
    size_t size;
    unsigned int checksum;
};

void test(size_t max_size, int N);

#endif //LAB1_TEST_H
