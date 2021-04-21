#include "test.h"
#define min(a, b) a < b ? a : b

void random_addr(void *ptr, size_t size) {
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
        // include position of each bit
        sum = (sum << 2) ^ (sum >> 5) ^ *((char *) ptr + i);
    return sum;
}

void test(size_t max_size, int N) {
    unsigned int seed = time(NULL);
    srand(seed);
    struct OperationResult results[ARRAY_SIZE] = {{.checksum = 0, .addr = NULL, .size = 0}};
    printf("Starting test with max size %ld, \nseed %u:\n", max_size, seed);

    for (unsigned int i = 0;
         i < N;
         i++) {
        // 0 - ALLOC
        // 1 - REALLOC
        // 2 - FREE
        unsigned short action = rand() % 3;
        size_t size = rand() % max_size;
        unsigned int rand_index = rand() % ARRAY_SIZE;
        struct OperationResult result;
        void *ptr;
        if (i % N / 100 == 0) {
            printf("%u%%\r", i);
            fflush (stdout);
        }

        switch (action) {
            case 0: // ALLOC
                ptr = mem_alloc(size);
                if (ptr) {
                    random_addr(ptr, size);
                    result = (struct OperationResult) {
                            .addr = ptr,
                            .size = size,
                            .checksum = get_checksum(ptr, size),
                    };
                    assert(get_checksum(result.addr, result.size) == result.checksum && "bad checksum");
                    mem_free(results[rand_index].addr);
                    results[rand_index] = result;
                }
                break;
            case 1:
                result = results[rand_index];
                if (result.addr) { // Якщо був алоцирований
                    assert(get_checksum(result.addr, result.size) == result.checksum && "bad checksum");
                }
                unsigned int controll = get_checksum(result.addr, min(size, result.size));
                void *ptr1 = mem_realloc(result.addr, size);
                if (ptr1) {
                    assert(get_checksum(ptr1, min(size, result.size)) == controll && "bad checksum");
                    random_addr(ptr1, size);
                    results[rand_index] = (struct OperationResult) {
                            .addr = ptr1,
                            .size = size,
                            .checksum = get_checksum(ptr1, size),
                    };
                }
                break;
            case 2:
                result = results[rand_index];
                if (result.addr) { // Якщо був алоцирований
                    assert(get_checksum(result.addr, result.size) == result.checksum && "bad checksum");
                }
                mem_free(result.addr);
                results[rand_index] = (struct OperationResult) {
                        .addr = NULL,
                        .size = 0,
                        .checksum = 0,
                };

                break;
            default:
                break;
        }
    }

    for (
            unsigned int i = 0;
            i < ARRAY_SIZE;
            i++) {
        assert(get_checksum(results[i].addr, results[i].size) == results[i].checksum && "bad checksum");
        mem_free(results[i].addr);
    }

    printf("Test with maximum size %zu finished successfully\n", max_size);
}