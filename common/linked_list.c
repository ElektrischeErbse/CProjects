#include "linked_list.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

ListNode *create_list_node(void *data)
{
    ListNode *node = malloc(sizeof(ListNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = node;
    node->prev = node;
    return node;
}

LinkedList *create_linked_list(HandleData handle_data, FreeData free_data, EqualData equal_data)
{
    LinkedList *linked_list = malloc(sizeof(LinkedList));
    if (linked_list == NULL) {
        return linked_list;
    }
    linked_list->handle_data = handle_data;
    linked_list->free_data = free_data;
    linked_list->equal_data = equal_data;
    linked_list->size = 0;
    ListNode *node = create_list_node(NULL);
    assert(node != NULL);
    linked_list->root = node;
    return linked_list;
}

int list_push_back(LinkedList *linked_list, void *data)
{
    if (linked_list == NULL) {
        return -1;
    }
    ListNode *node = create_list_node(data);
    if (node == NULL) {
        return -1;
    }
    ListNode *root = linked_list->root;
    assert(root != NULL);
    ListNode *tail = root->prev;
    tail->next = node;
    node->prev = tail;
    root->prev = node;
    node->next = root;
    linked_list->size += 1;
    return 0;
}

int list_push_front(LinkedList *linked_list, void *data)
{
    if (linked_list == NULL) {
        return -1;
    }
    ListNode *node = create_list_node(data);
    if (node == NULL) {
        return -1;
    }
    ListNode *root = linked_list->root;
    assert(root != NULL);
    ListNode *header = root->next;
    header->prev = node;
    node->next = header;
    root->next = node;
    node->prev = root;
    linked_list->size += 1;
    return 0;
}

int list_move_node_to_front(LinkedList *linked_list, ListNode *node)
{
    if (linked_list == NULL || node == NULL) {
        return -1;
    }
    ListNode *root = linked_list->root;
    assert(root != NULL);
    ListNode *left = node->prev;
    ListNode *right = node->next;
    left->next = right;
    right->prev = left;
    ListNode *header = root->next;
    header->prev = node;
    node->next = header;
    root->next = node;
    node->prev = root;
    return 0;
}

ListNode *list_find(LinkedList *linked_list, void *data)
{
    if (linked_list == NULL) {
        return NULL;
    }
    ListNode *root = linked_list->root;
    assert(root != NULL);
    ListNode *p = root->next;
    while (p != root && p) {
        void *lsh = p->data;
        void *rsh = data;
        EqualData equal_data = linked_list->equal_data;
        if (equal_data && equal_data(lsh, rsh) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

void destroy_linked_list(LinkedList **linked_list)
{
    if (linked_list == NULL || *linked_list == NULL) {
        return;
    }
    LinkedList *list = *linked_list;
    ListNode *root = list->root;
    if (root == NULL) {
        return;
    }
    ListNode *p = root->next;
    while (p != root && p) {
        if (list->free_data) {
            list->free_data(p->data);
        }
        ListNode *next = p->next;
        free(p);
        p = next;
    }
    free(root);
    free(list);
    *linked_list = NULL;
}

void traversal_linked_list(LinkedList *linked_list)
{
    if (linked_list == NULL) {
        return;
    }
    ListNode *root = linked_list->root;
    assert(root != NULL);
    ListNode *p = root->next;
    while (p != root && p) {
        if (linked_list->handle_data) {
            linked_list->handle_data(p->data);
        }
        p = p->next;
    }
}

void traversal_linked_list_ex(LinkedList *linked_list, Handle handle, void *arg)
{
    if (linked_list == NULL || handle == NULL) {
        return;
    }
    ListNode *root = linked_list->root;
    assert(root != NULL);
    ListNode *p = root->next;
    while (p != root && p) {
        handle(p->data, arg);
        p = p->next;
    }
}

size_t list_size(LinkedList *linked_list)
{
    return linked_list ? linked_list->size : 0;
}

ListNode *list_front(LinkedList *linked_list)
{
    if (linked_list == NULL) {
        return NULL;
    }
    ListNode *root = linked_list->root;
    assert(root != NULL);
    return root->next;
}

ListNode *list_back(LinkedList *linked_list)
{
    if (linked_list == NULL) {
        return NULL;
    }
    ListNode *root = linked_list->root;
    assert(root != NULL);
    return root->prev;
}

int list_pop_back(LinkedList *linked_list)
{
    if (linked_list == NULL) {
        return -1;
    }
    ListNode *root = linked_list->root;
    assert(root != NULL);
    ListNode *tail = root->prev;
    ListNode *left = tail->prev;
    ListNode *right = tail->next;
    left->next = right;
    right->prev = left;
    if (linked_list->free_data) {
        linked_list->free_data(tail->data);
    }
    free(tail);
    linked_list->size -= 1;
    return 0;
}