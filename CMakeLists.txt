cmake_minimum_required (VERSION 3.8)

project (keylogger)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CURL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/curl/include")

add_subdirectory (externals)
add_subdirectory (src)
