set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_executable(lru lru.c)
target_link_libraries(lru PRIVATE string_view linked_list hash_table)
add_custom_command(TARGET lru POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/LRU/shakespeare.txt"
    $<TARGET_FILE_DIR:lru>
)
