add_library(string_view string_view.c)
target_include_directories(string_view PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(linked_list linked_list.c)
target_include_directories(linked_list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
