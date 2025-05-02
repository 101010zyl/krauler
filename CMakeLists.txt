cmake_minimum_required(VERSION 3.22)
project(krauler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(EXPORT_COMPILE_COMMANDS ON)

find_package(CURL REQUIRED)
# find_package(Gumbo REQUIRED)
find_package(spdlog REQUIRED)

add_executable(krauler src/main.cpp)

target_sources(krauler PRIVATE 
    src/config.cpp
    src/krauler.cpp
)
target_link_libraries(krauler PRIVATE 
    CURL::libcurl 
    gumbo 
    spdlog::spdlog
)
target_include_directories(krauler PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external
)