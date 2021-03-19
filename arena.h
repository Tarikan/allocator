//
// Created by Tarikan on 08.03.2021.
//

#ifndef LAB1_ARENA_H
#define LAB1_ARENA_H

#include <stdbool.h>
#include <stddef.h>
#include "header.h"

struct Arena {
    bool started;
    size_t size;
    struct Arena *next;
    struct Arena *prev;
    size_t free_space;
};

struct Arena *create_arena(size_t size, struct Arena *prev);

void delete_arena(struct Arena *arena);

struct Header *get_first_header(struct Arena *arena);

struct Header *get_last_header(struct Arena *arena);

struct Header *add_header(struct Arena *arena, size_t size);

struct Header *search_first_suiting_block(struct Arena *arena, size_t size);

struct Arena *get_arena_from_header(struct Header *header);

#endif //LAB1_ARENA_H
