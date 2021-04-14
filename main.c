#include "test.h"
#define N 100000

int main() {
    test(32, N);
    test(512, N);
    test(RAND_MAX, N);
    return 0;
}