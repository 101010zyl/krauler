# Collect all source files
set(KRAULER_SOURCES
    config.cpp
    main.cpp
)

# Define the executable
add_executable(krauler ${KRAULER_SOURCES})

# Link dependencies
target_link_libraries(krauler PRIVATE CURL::libcurl gumbo spdlog::spdlog)

# Optionally link headers (redundant if already in top-level)
target_include_directories(krauler PRIVATE 
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/external
)
