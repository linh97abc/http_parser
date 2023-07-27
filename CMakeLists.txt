cmake_minimum_required(VERSION 3.1)

project(automic)

add_definitions(-D__P_WIN32=1 -D_CRT_SECURE_NO_WARNINGS)
# add_executable(${CMAKE_PROJECT_NAME} main.cpp websocket_server.cpp )

# add_executable(${CMAKE_PROJECT_NAME} "test.cpp" "src/http_parser.c")
add_executable(${CMAKE_PROJECT_NAME} "test_http.cpp" "src/http_parser.c" "src/webutil.c")

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)