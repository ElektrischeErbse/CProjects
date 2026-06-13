set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
add_executable(compiler compiler.c fsm.c token.c ast.c)
