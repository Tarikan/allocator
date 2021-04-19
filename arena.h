#ifndef LAB1_ARENA_H
#define LAB1_ARENA_H

#include <stdbool.h>
#include <stddef.h>
#include "header.h"
#include "macros.h"

struct Arena {
    size_t size;
};

struct Arena *create_arena(size_t size, struct Tree *tree);

void delete_arena(struct Arena *arena);

struct Header *get_first_header(struct Arena *arena);

struct Arena *get_arena_from_header(struct Header *header);

#endif //LAB1_ARENA_H
