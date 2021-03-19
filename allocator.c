//
// Created by Tarikan on 08.03.2021.
//

#include <string.h>
#include <stdio.h>
#include "allocator.h"
#include "arena.h"
#include "driver.h"
#include <stddef.h>

#define ALIGNMENT _Alignof(max_align_t)
#define MAX_SIZE get_page_size()

static struct Arena *first_arena;

void *mem_alloc(size_t size) {
    if (!first_arena)
        first_arena = create_arena(MAX_SIZE, NULL);

    size_t rem = size % ALIGNMENT;
    if (rem != 0)
        size += ALIGNMENT - rem;

    //size += HEADER_SIZE;

    struct Arena *arena = first_arena;
    struct Header *header;

    while (arena) {
        header = search_first_suiting_block(arena, size);

        if (header)
            break;

        arena = arena->next;
    }

    if (header && get_size(header) == size) {
        mark_reserved(header);
        return get_body_ptr(header);
    }

    if (header && header->size > size) {
        split_header(header, size);
        mark_reserved(header);
        return get_body_ptr(header);
    }

    arena = first_arena;

    while (arena) {
        if (arena->free_space >= size)
            break;
        if (arena->next) {
            arena = arena->next;
        } else {
            if (size + sizeof(struct Header) > MAX_SIZE) {
                size_t rem1 = size + sizeof(struct Header) % MAX_SIZE;
                size_t size_to_alloc = size + sizeof(struct Header);
                if (rem1 != 0)
                    size_to_alloc += MAX_SIZE - rem;
                arena->next = create_arena(size_to_alloc, arena);
            } else {
                arena->next = create_arena(MAX_SIZE, arena);
            }
        }
    }

    header = add_header(arena, size);
    mark_reserved(header);
    return get_body_ptr(header);
}

void mem_free(void *ptr) {
    if (ptr == NULL)
        return;

    struct Header *header = (get_header_from_body(ptr));
    struct Arena *arena = get_arena_from_header(header);

    mark_free(header);

    merge_right(header);

    merge_left(header);

    header = get_first_header(arena);

    if (!get_next(header) && get_status(header)) {
        if (first_arena == arena)
            first_arena = NULL;
        delete_arena(arena);
    }
}

void *mem_realloc(void *ptr, size_t new_size) {
    if (!ptr)
        return mem_alloc(new_size);

    struct Header *old_header = (struct Header *) ptr - 1;

    if (get_size(old_header) >= new_size) {
        split_header(ptr, new_size);
        return ptr;
    }

    if (!get_next(old_header)) {
        change_size(old_header, new_size);
        return ptr;
    }

    struct Header *new_header = mem_alloc(new_size);
    memcpy(get_body_ptr(new_header), get_body_ptr(old_header), get_size(old_header));

    mem_free(old_header);

    return get_body_ptr(new_header);
}

void mem_dump() {
    //char* addr = (char*)get_first_header(first_arena);
    printf("Page size: %d\n", MAX_SIZE);
    char *addr;
    int arena_num = 0;

    size_t total = 0;
    int total_count = 0;

    int total_total = 0;
    int total_total_count = 0;

    struct Arena *arena = first_arena;

    while (arena) {

        total = 0;
        total_count = 0;


        addr = (char *) get_first_header(arena);

        printf("Arena №%d\nUsed space: %ld/%ld\n", ++arena_num, arena->size - arena->free_space, arena->size);

        printf("%15s%15s%8s%15s\n", "ADDR", "FREE", "SIZE", "NEXT_ADDR");
        while (addr != NULL) {
            printf((((struct Header *) addr)->next != NULL ? "%15p%15s%8ld%15p\n" : "%15p%15s%8ld%15s\n"),
                   addr, ((struct Header *) addr)->free ? "true" : "false", ((struct Header *) addr)->size,
                   (((struct Header *) addr)->next != NULL ? (char *) (((struct Header *) addr)->next)
                                                           : "END OF MEMORY"));
            total = total + ((struct Header *) addr)->size;
            total_total = total_total + ((struct Header *) addr)->size;
            addr = (char *) ((struct Header *) addr)->next;
            total_count++;
            total_total_count++;
        }
        printf("Results:\n");
        printf("Total blocks allocated/used: %d\nMemory used by headers: %ld B\nMemory reserved for usage: %ld B\n",
               total_count,
               total_count * sizeof(struct Header), total);

        arena = arena->next;
    }

    if (arena_num > 1) {
        printf("\nTotal arenas allocated: %d\nTotal memory used by headers: %ld B\nTotal memory reserved for usage: %d B\n",
               arena_num, total_total_count * sizeof(struct Header), total_total);
    }

    if (!arena_num) {
        printf("Total arenas allocated: 0");
    }
}
