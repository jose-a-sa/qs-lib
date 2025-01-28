# Specify the minimum CMAKE version required
cmake_minimum_required(VERSION 3.24) # or higher if applicable
project(qs-test)

# Main options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Enable CTest testing
enable_testing()

# If googletest is installed in the system use find_package
# find_package(GTest REQUIRED)
# Otherwise use FetchContent:
include(FetchContent)
set(GTEST_VERSION release-1.12.1)
FetchContent_Declare(
   googletest
   GIT_REPOSITORY https://github.com/google/googletest
   GIT_TAG ${GTEST_VERSION} # Pick the desired version of GoogleTest
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "For Windows: Prevent overriding the parent project's compiler/linker settings" FORCE)
# Fetch and build GoogleTest
FetchContent_MakeAvailable(googletest)

# CMake functions to use with googletest
include(GoogleTest)

# Macro that take binary name and folder and creates a test with all .cpp inside  
function(add_test_binary NAME LOCATION_SRC)
    # find all cpp files for testing under the folder

    # single test for the collection above
    add_executable(test_${NAME} ${LOCATION_SRC})

    # add testing flag
    target_compile_definitions(test_${NAME} PRIVATE QS_TESTING)

    # link libraries
    target_link_libraries(test_${NAME} PRIVATE qs::qs fmt::fmt GTest::gmock_main)

    # discover tests
    gtest_discover_tests(test_${NAME})

    # move file
    # install(TARGETS test_${TARGET_FOLDER} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
endfunction()


function(add_test_binary_folder NAME FOLDER)
    # find all cpp files for testing under the folder
    file(GLOB_RECURSE TEST_SRC
            ${FOLDER}/**/test_*.cpp
            ${FOLDER}/test_*.cpp
    )

    # single test for the collection above
    add_test_binary(${NAME} ${TEST_SRC})
endfunction()


add_test_binary(fenwick_tree containers/test_fenwick_tree.cpp)
