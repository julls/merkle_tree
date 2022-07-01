cmake_minimum_required(VERSION 3.16)
project(merkle_tree VERSION 1.0 LANGUAGES CXX)

#######
# lib #
#######
# add_library(${PROJECT_NAME} INTERFACE)
# target_include_directories(${PROJECT_NAME}
#     INTERFACE
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src
# )

#########
# tests #
#########
add_executable(${PROJECT_NAME}_tests src/MerkleTree.hpp tests/MerkleTreeTest.cpp)
set_target_properties(${PROJECT_NAME}_tests
    PROPERTIES 
        CXX_STANDARD 17 
        CXX_EXTENSIONS OFF
)

