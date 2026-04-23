set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
find_package(OpenSSL REQUIRED)

add_executable(dup_files dup_files.c)
target_link_libraries(dup_files PRIVATE
    file_utils
    OpenSSL::SSL
    OpenSSL::Crypto
)
