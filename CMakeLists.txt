# Specify the minimum CMAKE version required
cmake_minimum_required(VERSION 3.24) # or higher if applicable
project(qs)

# Main options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Add compiler options (more pedantic in errors)
if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(
        -Wall # Enable all warnings
        -Wextra # Enable extra warnings
        -Wpedantic # Enforce standard-compliant code
        -Werror # Treat warnings as errors
        -Wno-gnu-statement-expression
    )
endif()

# Declare additional CMake options
option(BUILD_TESTING "" ON)
option(BUILD_BENCHMARK "" ON)

include(cmake/FetchFmtLib.cmake)

# Add main folder as include directory
include_directories(${CMAKE_SOURCE_DIR})

# Add source library (this can be updated to add to use add_subdirectory too)
file(GLOB_RECURSE qs_SRC "qs/*.cpp" "qs/*.h" "qs/**/*.cpp" "qs/**/*.h")
add_library(qs ${qs_SRC})
target_link_libraries(qs PRIVATE fmt::fmt)
add_library(qs::qs ALIAS qs)

# Build testing binaries
if(BUILD_TESTING)
    # enable_testing() should be called in the top-level CMakeLists.txt
    enable_testing()
    add_subdirectory(test)
endif()

# Build benchmark binaries
if(BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()
