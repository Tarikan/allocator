#ifndef LAB1_HEADER_H
#define LAB1_HEADER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "rbtree/rbtree.h"
#include "macros.h"

struct Header {
    size_t size;
    struct Header *prev;
    //struct Header *next;
    bool free : 1;
    bool has_next : 1;
};

void *get_body_ptr(struct Header *header);

struct Header *get_header_from_body(void *body);

size_t get_size(struct Header *header);

struct Header *get_prev(struct Header *header);

struct Header *get_next(struct Header *header);

bool get_status(struct Header *header);

struct Header *merge_right(struct Header *header, struct Tree* tree);

struct Header *merge_left(struct Header *header, struct Tree* tree);

void create_header(size_t size, struct Header *prev, void *ptr, struct Tree *tree);

void split_header(struct Header *header, size_t new_size, struct Tree* tree);

void mark_free(struct Header *header, struct Tree *tree);

void mark_reserved(struct Header *header, struct Tree *tree);

struct Header *get_next_addr(struct Header *header);

void set_size(struct Header *header, size_t new_size);

void set_next(struct Header *header, struct Header *next);

void set_prev(struct Header *header, struct Header *prev);

#endif //LAB1_HEADER_H