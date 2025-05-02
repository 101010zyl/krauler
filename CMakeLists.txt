cmake_minimum_required(VERSION 3.14)
project(krauler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)

add_executable(krauler src/main.cpp)
target_link_libraries(krauler PRIVATE CURL::libcurl gumbo)
