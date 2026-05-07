set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_executable(mini_excel mini_excel.c)
target_link_libraries(mini_excel PRIVATE string_view)
add_custom_command(TARGET mini_excel POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/MiniExcel/input.csv"
    $<TARGET_FILE_DIR:mini_excel>
)
