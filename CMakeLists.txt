cmake_minimum_required(VERSION 3.22)
project(krauler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(EXPORT_COMPILE_COMMANDS ON)

find_package(CURL REQUIRED)
# find_package(Gumbo REQUIRED)
find_package(spdlog REQUIRED)
find_package(Boost 1.74 REQUIRED COMPONENTS system thread atomic)

add_executable(krauler src/main.cpp)

target_sources(krauler PRIVATE 
    src/config.cpp
    src/krauler.cpp
    src/http_client.cpp
    src/html_parser.cpp
    src/filter.cpp
    src/robots.cpp
)

target_link_libraries(krauler PRIVATE 
    CURL::libcurl 
    gumbo 
    spdlog::spdlog
    Boost::system
    Boost::thread
    Boost::atomic
)
target_include_directories(krauler PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external
)