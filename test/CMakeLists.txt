# Specify the minimum CMAKE version required
cmake_minimum_required(VERSION 3.24) # or higher if applicable
project(qs_test)

# Main options
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

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
    add_executable(test_${NAME} ${TEST_SRC})

    # add testing flag
    target_compile_definitions(test_${NAME} PRIVATE QS_TESTING)

    # link libraries
    target_link_libraries(test_${NAME} PRIVATE qs::qs fmt::fmt GTest::gmock_main)

    # discover tests
    gtest_discover_tests(test_${NAME})

    # move file
    # install(TARGETS test_${TARGET_FOLDER} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
endfunction()

add_test_binary_folder(all ./)
add_test_binary(span test_span.cpp)

add_test_binary_folder(traits traits)

add_test_binary_folder(containers containers)

add_test_binary_folder(utils utils)


# Loop through the specified C++ standard versions
foreach(VER 11 14 17 20)
    # Set the C++ standard version as a variable
    set(CXX_STD_VER "cxx_std_${VER}")
    # Define a unique target name for each C++ standard version
    set(TARGET_NAME test_${CXX_STD_VER})
    # Create an executable target for the playground test
    add_executable(${TARGET_NAME} cxx_version_playground.cpp)
    # Add a preprocessor definition for testing
    target_compile_definitions(${TARGET_NAME} PRIVATE QS_TESTING)
    # Specify the required C++ standard version for the target
    target_compile_features(${TARGET_NAME} PRIVATE ${CXX_STD_VER})
    # Link the necessary libraries to the target
    target_link_libraries(${TARGET_NAME} PRIVATE qs::qs fmt::fmt GTest::gmock_main)
endforeach()
