#include "header.h"
#include "Utils/align_utils.h"
#include "macros.h"

#pragma region ptr_functions

void *get_body_ptr(struct Header *header) {
    //return header + 1;
    return (char *) header + HEADER_SIZE;
}

struct Header *get_header_from_body(void *body) {
    //return (struct Header *) body - 1;
    return (struct Header *) ((char *) body - HEADER_SIZE);
}

struct Header *get_next(struct Header *header) {
    return header->next;
}

void set_next(struct Header *header, struct Header *next) {
    header->next = next;
}

void set_prev(struct Header *header, struct Header *prev) {
    header->prev = prev;
}

struct Header *get_prev(struct Header *header) {
    return header->prev;
}

struct Header *get_next_addr(struct Header *header) {
    //return (struct Header *) ((char *) header + sizeof(struct Header) + header->size);
    return (struct Header *) ((char *) get_body_ptr(header) + get_size(header));
}

#pragma endregion ptr_functions

#pragma region size_functions

size_t get_size(struct Header *header) {
    return header->size;
}

void set_size(struct Header *header, size_t new_size) {
    header->size = new_size;
}

#pragma endregion size_functions

#pragma region merge_functions

struct Header *merge_right(struct Header *header, struct Tree *tree) {
    if (get_next(header) && get_status(get_next(header))) {
        if (get_size(header) >= NODE_SIZE) {
            remove_item(tree, (struct Node *) get_body_ptr(header));
        }
        if (get_size(get_next(header)) >= NODE_SIZE) {
            remove_item(tree, (struct Node *) get_body_ptr(get_next(header)));
        }
        //header->size += header->next->size + align(sizeof(struct Header), ALIGNMENT);
        set_size(header, get_size(header) +
                         get_size(get_next(header)) +
                         align(sizeof(struct Header), ALIGNMENT));
        //header->next = header->next->next;
        set_next(header, get_next(get_next(header)));

        if (get_next(header)) {
            set_prev(get_next(header), header);
        }
        if (get_prev(header)) {
            set_next(get_prev(header), header);
        }

        if (get_size(header) >= NODE_SIZE) {
            insert_item(tree, init_node(get_body_ptr(header), get_size(header)));
        }
    }

    return header;
}

struct Header *merge_left(struct Header *header, struct Tree *tree) {
    if (get_prev(header) && get_status(get_prev(header))) {
        header = merge_right(get_prev(header), tree);
    }

    return header;
}

#pragma endregion merge_functions

#pragma region status_functions

void mark_free(struct Header *header, struct Tree *tree) {
    if (get_size(header) >= NODE_SIZE) {
        insert_item(tree, init_node(get_body_ptr(header), get_size(header)));
    }
    header->free = true;
}

void mark_reserved(struct Header *header, struct Tree *tree) {
    if (get_size(header) >= NODE_SIZE) {
        remove_item(tree, get_body_ptr(header));
    }
    header->free = false;
}

bool get_status(struct Header *header) {
    return header->free;
}

#pragma endregion status_functions

#pragma region creation_functions

void create_header(size_t size, struct Header *prev, void *ptr, struct Tree *tree) {
    set_prev((struct Header *) ptr, prev);
    set_size((struct Header *) ptr, size);
    mark_free((struct Header *) ptr, tree);
}

void split_header(struct Header *header, size_t new_size, struct Tree *tree) {
    new_size = align(new_size, ALIGNMENT);
    if (get_size(header) <= new_size) {
        return;
    }

    if (get_size(header) - new_size <= HEADER_SIZE) {
        return;
    }

    if (get_status(header) &&
        get_size(header) >= NODE_SIZE) {
        remove_item(tree, get_body_ptr(header));
    }

    size_t old_size = get_size(header);

    set_size(header, new_size);

    if (get_status(header)) { mark_free(header, tree); }

    //if (new_size >= NODE_SIZE) {
    //    insert_item(tree, init_node(get_body_ptr(header), new_size));
    //}

    struct Header *new_header =
            get_next_addr(header);

    set_size(new_header, old_size - new_size - HEADER_SIZE);
    set_prev(new_header, header);
    set_next(new_header, get_next(header));
    if (get_next(new_header)) {
        set_prev(get_next(new_header), new_header);
    }
    set_next(header, new_header);

    //if (get_size(new_header) >= NODE_SIZE) {
    //    insert_item(tree, init_node(get_body_ptr(new_header), get_size(new_header)));
    //}

    mark_free(new_header, tree);
}

#pragma endregion creation_functions
