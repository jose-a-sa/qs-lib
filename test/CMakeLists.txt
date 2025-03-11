# Specify the minimum CMAKE version required
cmake_minimum_required(VERSION 3.24) # or higher if applicable
project(qs-test)

# Main options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Enable CTest testing
enable_testing()

include(../cmake/FetchGoogleTest.cmake)

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

add_test_binary(lifetime_tracker utils/test_lifetime_tracker.cpp)

add_test_binary(span test_span.cpp)