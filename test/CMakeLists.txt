cmake_minimum_required(VERSION 3.12)

project(test_http_parser)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
find_package(HttpParser REQUIRED)

add_definitions(-D__P_WIN32=1 -D_CRT_SECURE_NO_WARNINGS)

add_executable(${CMAKE_PROJECT_NAME} "test_http.cpp")

add_subdirectory(websrc)

target_link_libraries(
    ${CMAKE_PROJECT_NAME} 
    web_srcs)
