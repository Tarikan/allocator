//
// Created by Tarikan on 09.03.2021.
//

#include "header.h"

#pragma region ptr_functions

void *get_body_ptr(struct Header *header) {
    return header + 1;
}

struct Header *get_header_from_body(void *body) {
    return (struct Header *) body - 1;
}

struct Header *get_next(struct Header *header) {
    return header->next;
    //return (struct Header*)((char *)get_body_ptr(header) + get_size(header));
}

void change_next(struct Header *header, struct Header *next) {
    header->next = next;
}

void change_prev(struct Header *header, struct Header *prev) {
    header->prev = prev;
}

struct Header *get_prev(struct Header *header) {
    return header->prev;
}

struct Header *get_next_addr(struct Header *header) {
    return (struct Header *) ((char *) header + sizeof(struct Header) + header->size);
}

#pragma endregion ptr_functions

#pragma region size_functions

size_t get_size(struct Header *header) {
    return header->size;
}

void change_size(struct Header *header, size_t new_size) {
    header->size = new_size;
}

#pragma endregion size_functions

#pragma region merge_functions

void merge_right(struct Header *header) {
    if (header->next && header->next->free) {
        header->size += header->next->size + sizeof (struct Header);
        header->next = header->next->next;
        struct Header *next_h = header->next;

        if (next_h)
            next_h->prev = header;
    }
}

void merge_left(struct Header *header) {
    if (get_prev(header) && get_status(get_prev(header))) {
        header = get_prev(header);
        header->size += header->next->size + sizeof (struct Header);
        header->next = header->next->next;
        if (get_next(header))
            get_next(header)->prev = header;
    }
}

#pragma endregion merge_functions

#pragma region status_functions

void mark_free(struct Header *header) {
    header->free = true;
}

void mark_reserved(struct Header *header) {
    header->free = false;
}

bool get_status(struct Header *header) {
    return header->free;
}

#pragma endregion status_functions

#pragma region creation_functions

void create_header(size_t size, struct Header *prev, void *ptr) {
    ((struct Header *) ptr)->size = size;
    //((struct Header*)ptr)->prev = NULL;
    change_prev((struct Header *) ptr, NULL);
    mark_free((struct Header *) ptr);
}

void split_header(struct Header *header, size_t new_size) {
    if (header->size <= new_size)
        return;

    size_t old_size = header->size;

    header->size = new_size;

    struct Header *new_header =
            (struct Header *)((char *)get_body_ptr(header) + new_size);

    new_header->size = old_size - new_size - sizeof (struct Header);
    new_header->prev = header;
    if (header->next) {
        new_header->next = header->next;
        header->next->prev = new_header;
    }
    new_header->prev = header;
    header->next = new_header;
    mark_free(new_header);
}

#pragma endregion creation_functions
