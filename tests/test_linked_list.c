#include <assert.h>
#include <linked_list.h>
#include <stdio.h>
#include <stdlib.h>

void handle_data(void *data)
{
    int *d = (int *) data;
    printf("data = %d\n", *d);
}

void free_data(void *data)
{
    if (data) {
        free(data);
    }
}

int equal_data(const void *lsh, const void *rsh)
{
    int *l = (int *) lsh;
    int *r = (int *) rsh;
    if (*l == *r) {
        return 0;
    }
    return 1;
}

int main()
{
    LinkedList *list = create_linked_list(handle_data, free_data, equal_data);
    int *arr[5] = {0};
    for (int i = 0; i < 5; ++i) {
        arr[i] = malloc(sizeof(int));
        *arr[i] = i + 1;
    }
    list_push_back(list, arr[0]);
    list_push_back(list, arr[1]);
    list_push_back(list, arr[2]);
    list_push_front(list, arr[3]);
    list_push_front(list, arr[4]);

    assert(list_size(list) == 5);
    ListNode *node = list_find(list, arr[3]);
    assert(*(int *) node->data == *arr[3]);
    traversal_linked_list(list);
    list_move_node_to_front(list, node);
    printf("======================\n");
    traversal_linked_list(list);
    destroy_linked_list(&list);
    return 0;
}