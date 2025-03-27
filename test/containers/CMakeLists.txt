# All tests
get_filename_component(CURRENT_FOLDER_BASENAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_test_binary_folder(${CURRENT_FOLDER_BASENAME} ./)

add_test_binary(fenwick_tree test_fenwick_tree.cpp)
