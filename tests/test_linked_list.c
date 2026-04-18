#include <linked_list.h>
#include <stdio.h>

void callback(struct list_node *node)
{
    int *data = (int *) node->data;
    printf("data = %d\n", *data);
}

int main()
{
    struct list_node *root = create_linked_list();
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i = 0; i < 5; ++i) {
        root = list_push_back(root, &arr[i]);
    }
    printf("list size: %zu\n", list_size(root));
    for (int i = 5; i < 10; ++i) {
        root = list_push_front(root, &arr[i]);
    }
    printf("list size: %zu\n", list_size(root));
    traversal_linked_list(root, &callback);
    root = list_move_to_front(root, root->prev);
    printf("================================\n");
    traversal_linked_list(root, callback);
    root = destroy_linked_list(root);
    printf("list size: %zu\n", list_size(root));
    return 0;
}