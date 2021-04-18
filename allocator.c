#include <string.h>
#include <stdio.h>
#include "allocator.h"
#include "arena.h"
#include "driver.h"
#include <stddef.h>
#include "Utils/align_utils.h"
#include "rbtree/rbtree.h"
#include "macros.h"


static struct Tree tree_head = {.root = NULL};

void *mem_alloc(size_t size) {

    int aligned_size = align(size, ALIGNMENT);

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

    if (header && get_size(header) == size) {
        mark_reserved(header, &tree_head);
        return get_body_ptr(header);
    }

    if (header && get_size(header) > size) {
        size_t test = get_size(header);
        split_header(header, size, &tree_head);

        mark_reserved(header, &tree_head);
        return get_body_ptr(header);
    }

    mark_reserved(header, &tree_head);
    return get_body_ptr(header);
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
    int new_size_aligned = align(new_size, ALIGNMENT);

    if (new_size > new_size_aligned) {
        return NULL;
    }

    new_size = new_size_aligned;

    if (!ptr) {
        return mem_alloc(new_size);
    }

    struct Header *old_header = get_header_from_body(ptr);

    if (new_size == 0) {
        //mem_free(ptr);
        return NULL;
    }

    if (get_size(old_header) > new_size) {
        split_header(old_header, new_size, &tree_head);
        return ptr;
    }

    struct Header *new_header = get_header_from_body(mem_alloc(new_size));
    memcpy(get_body_ptr(new_header),
           get_body_ptr(old_header),
           get_size(old_header) > new_size ? new_size : get_size(old_header));

    mem_free(get_body_ptr(old_header));

    return get_body_ptr(new_header);
}

/// deprecated
void mem_dump() {
    //char* addr = (char*)get_first_header(first_arena);
    printf("Page size: %d\n", MAX_SIZE);
    char *addr;
    int arena_num = 0;

    size_t total = 0;
    int total_count = 0;

    int total_total = 0;
    int total_total_count = 0;

    struct Arena *arena = NULL;

    while (arena) {

        total = 0;
        total_count = 0;


        addr = (char *) get_first_header(arena);

        printf("Arena №%d\nUsed space: %ld/%ld\n", ++arena_num, -1, arena->size);

        printf("%15s%15s%8s%15s\n", "ADDR", "FREE", "SIZE", "NEXT_ADDR");
        while (addr != NULL) {
            printf((get_next(((struct Header *) addr)) != NULL ? "%15p%15s%8ld%15p\n" : "%15p%15s%8ld%15s\n"),
                   addr, get_status(((struct Header *) addr)) ? "true" : "false", get_size(((struct Header *) addr)),
                   (get_next(((struct Header *) addr)) != NULL ? (char *) (get_next(((struct Header *) addr)))
                                                               : "END OF MEMORY"));
            total = total + get_size(((struct Header *) addr));
            total_total += get_size(((struct Header *) addr));
            addr = (char *) get_next(((struct Header *) addr));
            total_count++;
            total_total_count++;
        }
        printf("Results:\n");
        printf("Total blocks allocated/used: %d\nMemory used by headers: %ld B\nMemory reserved for usage: %ld B\n",
               total_count,
               total_count * sizeof(struct Header), total);
    }

    if (arena_num > 1) {
        printf("\nTotal arenas allocated: %d\nTotal memory used by headers: %ld B\nTotal memory reserved for usage: %d B\n",
               arena_num, total_total_count * sizeof(struct Header), total_total);
    }

    printf("\n");
    print_tree(&tree_head);
}
