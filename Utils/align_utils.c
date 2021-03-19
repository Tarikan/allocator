//
// Created by Tarikan on 19.03.2021.
//

#include "align_utils.h"

size_t align(size_t size, size_t alignment) {
    size_t rem = size % alignment;
    size_t res = size;
    if (rem != 0) {
        res += alignment - rem;
        if(res < size)
            return 0;
    }
    return res;
}
