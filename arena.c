//
// Created by Tarikan on 09.03.2021.
//

#include "arena.h"
#include "driver.h"
#include "Utils/align_utils.h"

#define ALIGNMENT _Alignof(max_align_t)

struct Arena *create_arena(size_t size, struct Arena *prev) {
    struct Arena *ptr = kernel_alloc(size);

    if (!ptr) {
        return NULL;
    }

    ptr->size = size - sizeof(struct Arena);
    ptr->free_space = size - align(sizeof (struct Arena), ALIGNMENT);
    ptr->started = false;
    ptr->prev = prev;
    //prev->next = ptr;

    return ptr;
}

void delete_arena(struct Arena *arena) {
    if (arena->next) {
        if (arena->prev) {
            arena->prev->next = arena->next;
            arena->next->prev = arena->prev;
        } else {
            arena->next->prev = NULL;
        }
    } else if (arena->prev) {
        arena->prev->next = NULL;
    }
    kernel_free(arena);
}


struct Header *get_first_header(struct Arena *arena) {
    //return (struct Header *) (arena + 1);
    return (struct Header*)((char *)arena + align(sizeof (struct Arena), ALIGNMENT));
}

struct Arena *get_arena_from_header(struct Header *header) {
    while (get_prev(header)) {
        header = get_prev(header);
    }

    return (struct Arena *) ((char *) header - align(sizeof(struct Arena), ALIGNMENT));
}

struct Header *get_last_header(struct Arena *arena) {
    struct Header *header = get_first_header(arena);
    while (get_next(header))
        header = get_next(header);
    return header;
}

struct Header *add_header(struct Arena *arena, size_t size) {
    if (!arena->started) {
        arena->started = true;
        struct Header *header = get_first_header(arena);
        change_prev(header, NULL);
        change_next(header, NULL);
        change_size(header, size);
        //arena->free_space -= align(sizeof (struct Arena), ALIGNMENT) - sizeof (struct Arena);
        arena->free_space -= size + align(sizeof(struct Header), ALIGNMENT);
        mark_free(header);
        return header;
    }
    struct Header *last_header = get_last_header(arena);
    struct Header *header;
    if (arena->started) {
        header = get_next_addr(last_header);
        change_prev(header, last_header);
        change_next(last_header, header);
    } else {
        header = last_header;
        arena->started = true;
    }

    arena->free_space -= size + sizeof(struct Header);

    mark_free(header);
    change_size(header, size);
}

struct Header *search_first_suiting_block(struct Arena *arena, size_t size) {
    if (!arena->started)
        return NULL;

    struct Header *header = get_first_header(arena);

    while (header) {
        if (get_status(header) && get_size(header) >= size)
            break;

        if (get_next(header))
            header = get_next(header);
        else
            break;
    }

    if (get_status(header) && get_size(header) >= size)
        return header;
    else
        return NULL;
}