#pragma once

struct list_node {
    void *data;
    struct list_node *prev;
    struct list_node *next;
};

typedef void (*func)(struct list_node *node);

struct list_node *create_list_node(void *data);

struct list_node *create_linked_list();

struct list_node *list_push_back(struct list_node *root, void *data);

struct list_node *list_push_front(struct list_node *root, void *data);

struct list_node *list_move_to_front(struct list_node *root, struct list_node *node);

void destroy_linked_list(struct list_node *root);

void traversal_linked_list(struct list_node *root, func f);