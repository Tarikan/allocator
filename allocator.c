#include <string.h>
#include <stdio.h>
#include "allocator.h"
#include "arena.h"
#include "driver.h"
#include <stddef.h>
#include "Utils/align_utils.h"


static struct Tree tree_head = {.root = NULL};

void *mem_alloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    size_t aligned_size = align(size, ALIGNMENT);

    if (size > aligned_size) {
        return NULL;
    }

    size = aligned_size;

    if (size < NODE_SIZE) {
        size = NODE_SIZE;
    }

    struct Arena *arena;
    struct Header *header;

    struct Node *node = search_suitable(&tree_head, size);

    //printf("Trying to alloc %ld B\n", size);

    if (node) {
        header = HEADER_FROM_NODE(node);
    } else {
        arena = create_arena(size + HEADER_SIZE, &tree_head);
        if (!arena) {
            return NULL;
        }
        header = get_first_header(arena);
    }

    if (get_size(header) == size) {
        mark_reserved(header, &tree_head);
        return get_body_ptr(header);
    }

    if (get_size(header) > size) {
        split_header(header, size, &tree_head);

        mark_reserved(header, &tree_head);
        return get_body_ptr(header);
    }
}

void mem_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    struct Header *header = get_header_from_body(ptr);
    struct Arena *arena;

    mark_free(header, &tree_head);

    header = merge_right(header, &tree_head);

    header = merge_left(header, &tree_head);

    if (!get_next(header) &&
        !get_prev(header)) {
        mark_reserved(header, &tree_head);
        arena = get_arena_from_header(header);
        //printf("Delete arena with size %ld\n", arena->size);
        delete_arena(arena);
    }
}

void *mem_realloc(void *ptr, size_t new_size) {
    if (new_size == 0) {
        //mem_free(ptr);
        return NULL;
    }

    size_t new_size_aligned = align(new_size, ALIGNMENT);

    if (new_size > new_size_aligned) {
        return NULL;
    }

    new_size = new_size_aligned;

    if (!ptr) {
        return mem_alloc(new_size);
    }

    struct Header *old_header = get_header_from_body(ptr);

    // if new_size < old_size -> split block
    if (get_size(old_header) > new_size) {
        split_header(old_header, new_size, &tree_head);
        merge_right(get_next(old_header), &tree_head);
        return ptr;
    }

    // if next block is free and has big enough size to merge
    if (get_next(old_header) &&
        get_status(get_next(old_header)) &&
        get_size(get_next(old_header)) + HEADER_SIZE + get_size(old_header) >= new_size) {
        merge_right(old_header, &tree_head);
        return get_body_ptr(old_header);
    }

    ptr = mem_alloc(new_size);
    if (!ptr) {
        return NULL;
    }

    struct Header *new_header = get_header_from_body(ptr);
    memcpy(get_body_ptr(new_header),
           get_body_ptr(old_header),
           get_size(old_header) > new_size ? new_size : get_size(old_header));

    mem_free(get_body_ptr(old_header));

    return get_body_ptr(new_header);
}

/// deprecated
void mem_dump() {
    printf("Page size: %d\n", MAX_SIZE);
    print_tree(&tree_head);
}
