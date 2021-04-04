//
// Created by Tarikan on 01.04.2021.
//

#ifndef LAB1_MACROS_H
#define LAB1_MACROS_H

#define ALIGNMENT _Alignof(max_align_t)
#define MAX_SIZE get_page_size()
#define ARENA_SIZE align(sizeof(struct Arena), ALIGNMENT)
#define NODE_SIZE align(sizeof(struct Node), ALIGNMENT)
#define HEADER_SIZE align(sizeof(struct Header), ALIGNMENT)
#define HEADER_FROM_NODE(node) (struct Header *)((char *)node - HEADER_SIZE)
#define NODE_FROM_HEADER(header) (struct Node *)(get_body_ptr(header))

#endif //LAB1_MACROS_H
