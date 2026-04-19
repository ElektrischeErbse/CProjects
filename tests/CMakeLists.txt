add_executable(test_string_view test_string_view.c)
target_link_libraries(test_string_view PRIVATE string_view)

add_executable(test_linked_list test_linked_list.c)
target_link_libraries(test_linked_list PRIVATE linked_list)

add_executable(test_hash_table test_hash_table.c)
target_link_libraries(test_hash_table PRIVATE hash_table)

add_test(NAME test_string_view COMMAND test_string_view)
add_test(NAME test_linked_list COMMAND test_linked_list)
add_test(NAME test_hash_table COMMAND test_hash_table)
