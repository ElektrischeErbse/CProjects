add_library(string_view string_view.c)
target_include_directories(string_view PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(linked_list linked_list.c)
target_include_directories(linked_list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(hash_table hash_table.c)
target_include_directories(hash_table PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hash_table PUBLIC linked_list)

add_library(file_utils file_utils.c)
target_include_directories(file_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
