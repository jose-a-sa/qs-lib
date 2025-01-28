cmake_minimum_required(VERSION 3.24) # or higher if applicable
project(qs-benchmark)

# Main options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# If benchmark is installed in the system use find_package
# find_package(benchmark REQUIRED)
# Otherwise use FetchContent:
include(FetchContent)
set(BENCHMARK_VERSION v1.8.4)
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG ${BENCHMARK_VERSION}
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "For Windows: Prevent overriding the parent project's compiler/linker settings" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
# # Fetch and build benchmark
FetchContent_MakeAvailable(benchmark)

# Macro that take binary name and folder and creates a test with all .cpp inside
function(add_bm_binary NAME SRC)
    # single test for the collection above
    add_executable(bm_${NAME} ${SRC})

    # link libraries
    target_link_libraries(bm_${NAME} PRIVATE qs::qs fmt::fmt benchmark::benchmark)

    # move file
    # install(TARGETS test_${TARGET_FOLDER} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
endfunction()


add_bm_binary(fenwick_tree bm_fenwick_tree.cpp)
add_bm_binary(compiler_specific bm_compiler.cpp)