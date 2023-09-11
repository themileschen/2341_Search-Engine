cmake_minimum_required(VERSION 3.15)
project(FinalProjExamples)

# filesystem.h require C++17
set(CMAKE_CXX_STANDARD 17)

file(COPY sample_data/ DESTINATION sample_data/)
#file(COPY test_data/ DESTINATION test_data/)

# show compiler output and enable warnings
set(CMAKE_VERBOSE_MAKEFILE ON)
add_compile_options(-Wall -Wextra -pedantic)

add_executable(rapidJSONExample rapidJSONExample.cpp)
add_executable(supersearch main.cpp DocumentParser.cpp IndexHandler.cpp QueryEngine.cpp SearchEngine.cpp porter2_stemmer.cpp AVLTree.h MapValue.h) # add MapValue.h and AVLTree.h?
add_executable(treeTest Tests.cpp AVLTree.h MapValue.h IndexHandler.cpp DocumentParser.cpp SearchEngine.cpp QueryEngine.cpp porter2_stemmer.cpp)
add_executable(testingOnly AVLTreeTest.cpp AVLTree.h MapValue.h) #testing only 
add_executable(testingOnly2 IndexHandlerTesting.cpp AVLTree.h IndexHandler.cpp MapValue.h DocumentParser.cpp porter2_stemmer.cpp) #testing only 

# this makes sure we also link rapidjson
target_include_directories(rapidJSONExample PRIVATE rapidjson/)

set(CMAKE_VERBOSE_MAKEFILE OFF)
