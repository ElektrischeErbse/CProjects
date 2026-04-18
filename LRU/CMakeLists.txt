set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_executable(lru lru.c)
target_link_libraries(lru PRIVATE string_view linked_list)
