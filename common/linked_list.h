#pragma once

#include <stddef.h>

typedef struct ListNode {
    void *data;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

typedef void (*HandleData)(void *data);
typedef void (*FreeData)(void *data);
typedef int (*EqualData)(const void *lsh, const void *rsh);
typedef void (*Handle)(void *data, void *arg);

typedef struct LinkedList {
    ListNode *root;
    size_t size;
    HandleData handle_data;
    FreeData free_data;
    EqualData equal_data;
} LinkedList;

LinkedList *create_linked_list(HandleData handle_data, FreeData free_data, EqualData equal_data);

int list_push_back(LinkedList *linked_list, void *data);

int list_push_front(LinkedList *linked_list, void *data);

int list_move_node_to_front(LinkedList *linked_list, ListNode *node);

ListNode *list_find(LinkedList *linked_list, void *data);

void destroy_linked_list(LinkedList **linked_list);

void traversal_linked_list(LinkedList *linked_list);

void traversal_linked_list_ex(LinkedList *linked_list, Handle handle, void *arg);

size_t list_size(LinkedList *linked_list);

ListNode *list_front(LinkedList *linked_list);

ListNode *list_back(LinkedList *linked_list);

int list_pop_back(LinkedList *linked_list);