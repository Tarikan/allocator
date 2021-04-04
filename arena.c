//
// Created by Tarikan on 09.03.2021.
//

#include "arena.h"
#include "driver.h"
#include "Utils/align_utils.h"

struct Arena *create_arena(size_t size, struct Tree *tree) {
    if (size == 0) {
        size = 1;
    }

    size = align(size + ARENA_SIZE, get_page_size());

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

/*
struct Header *add_header(struct Arena *arena, size_t size) {
    if (!arena->started) {
        arena->started = true;
        struct Header *header = get_first_header(arena);
        change_prev(header, NULL);
        change_next(header, NULL);
        change_size(header, size);
        //arena->free_space -= align(sizeof (struct Arena), ALIGNMENT) - sizeof (struct Arena);
        //arena->free_space -= size + align(sizeof(struct Header), ALIGNMENT);
        mark_free(header, NULL);
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

    mark_free(header, NULL);
    change_size(header, size);
}
*/

struct Header *search_first_suiting_block(struct Arena *arena, size_t size) {
    //if (!arena->started)
    //    return NULL;

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