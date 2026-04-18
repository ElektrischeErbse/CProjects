#include "linked_list.h"
#include <stddef.h>
#include <stdlib.h>

struct list_node *create_list_node(void *data)
{
    struct list_node *node = (struct list_node *) malloc(sizeof(struct list_node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = node;
    node->prev = node;
    return node;
}

struct list_node *create_linked_list()
{
    return create_list_node(NULL);
}

struct list_node *list_push_back(struct list_node *root, void *data)
{
    if (root == NULL) {
        return NULL;
    }
    struct list_node *node = create_list_node(data);
    if (node == NULL) {
        return root;
    }
    struct list_node *tail = root->prev;
    tail->next = node;
    node->prev = tail;
    node->next = root;
    root->prev = node;
    return root;
}

struct list_node *list_push_front(struct list_node *root, void *data)
{
    if (root == NULL) {
        return NULL;
    }
    struct list_node *node = create_list_node(data);
    if (node == NULL) {
        return root;
    }
    struct list_node *head = root->next;
    root->next = node;
    node->prev = root;
    node->next = head;
    head->prev = node;
    return root;
}

struct list_node *list_move_to_front(struct list_node *root, struct list_node *node)
{
    if (root == NULL || node == NULL) {
        return root;
    }
    struct list_node *left = node->prev;
    struct list_node *right = node->next;
    struct list_node *head = root->next;
    left->next = right;
    right->prev = left;
    head->prev = node;
    node->next = head;
    root->next = node;
    node->prev = root;
    return root;
}

void destroy_linked_list(struct list_node *root)
{
    if (root == NULL) {
        return;
    }
    struct list_node *p = root->next;
    while (p != root) {
        if (p == NULL) {
            break;
        }
        struct list_node *next_node = p->next;
        free(p);
        p = next_node;
    }
    if (p) {
        free(p);
    }
    root = NULL;
}

void traversal_linked_list(struct list_node *root, func f)
{
    if (root == NULL) {
        return;
    }
    struct list_node *p = root->next;
    while (p != root) {
        if (p == NULL) {
            break;
        }
        if (f) {
            f(p);
        }
        p = p->next;
    }
}