//
// Created by Tarikan on 08.03.2021.
//

#ifndef LAB1_HEADER_H
#define LAB1_HEADER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "tree.h"

struct Header {
    size_t size;
    struct Header *prev;
    struct Header *next;
    bool free;
};

void *get_body_ptr(struct Header *header);

struct Header *get_header_from_body(void *body);

size_t get_size(struct Header *header);

struct Header *get_prev(struct Header *header);

struct Header *get_next(struct Header *header);

bool get_status(struct Header *header);

void merge_right(struct Header *header);

void merge_left(struct Header *header);

void create_header(size_t size, struct Header *prev, void *ptr);

void split_header(struct Header *header, size_t new_size);

void mark_free(struct Header *header);

void mark_reserved(struct Header *header);

struct Header *get_next_addr(struct Header *header);

void change_size(struct Header *header, size_t new_size);

void change_next(struct Header *header, struct Header *next);

void change_prev(struct Header *header, struct Header *prev);



#endif //LAB1_HEADER_H