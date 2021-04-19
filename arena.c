#include "arena.h"
#include "driver.h"
#include "Utils/align_utils.h"

struct Arena *create_arena(size_t size, struct Tree *tree) {
    struct Arena *ptr;
    if (size == 0) {
        size = 1;
    }

    size_t aligned_size = align(size + ARENA_SIZE + HEADER_SIZE, get_page_size());

    if (size > aligned_size) {
        return NULL;
    }

    size = aligned_size;

    if (size > DEFAULT_ARENA_PAGES * get_page_size()) {
        ptr = kernel_alloc(size);
    } else {
        ptr = kernel_alloc(DEFAULT_ARENA_PAGES * get_page_size());
    }

    if (!ptr) {
        return NULL;
    }

    ptr->size = size - ARENA_SIZE;

    create_header(ptr->size - HEADER_SIZE, NULL, get_first_header(ptr), tree);

    return ptr;
}

void delete_arena(struct Arena *arena) {
    kernel_free(arena);
}

struct Header *get_first_header(struct Arena *arena) {
    //return (struct Header *) (arena + 1);
    return (struct Header*)((char *)arena + ARENA_SIZE);
}

struct Arena *get_arena_from_header(struct Header *header) {
    while (get_prev(header)) {
        header = get_prev(header);
    }

    return (struct Arena *) ((char *) header - ARENA_SIZE);
}
