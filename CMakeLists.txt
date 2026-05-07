cmake_minimum_required(VERSION 3.22)

project(cprojects C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
if(DEFINED LRU_ENABLE)
    add_compile_definitions(LRU_ENABLE=${LRU_ENABLE})
endif()

enable_testing()

add_subdirectory(common)
add_subdirectory(LRU)
add_subdirectory(tests)
add_subdirectory(FindDupFiles)
add_subdirectory(MiniExcel)
add_subdirectory(DiskUsage)
