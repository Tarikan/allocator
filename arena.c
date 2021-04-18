#include "arena.h"
#include "driver.h"
#include "Utils/align_utils.h"

struct Arena *create_arena(size_t size, struct Tree *tree) {
    if (size == 0) {
        size = 1;
    }

    int aligned_size = align(size + ARENA_SIZE, get_page_size());

    if (size > aligned_size) {
        return NULL;
    }

    size = aligned_size;

    struct Arena *ptr = kernel_alloc(size);

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

struct Header *get_last_header(struct Arena *arena) {
    struct Header *header = get_first_header(arena);
    while (get_next(header))
        header = get_next(header);
    return header;
}
