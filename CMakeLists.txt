cmake_minimum_required(VERSION 3.12...3.14 FATAL_ERROR )
project(sigslot)
enable_testing() 

include_directories(${CMAKE_SOURCE_DIR}/include/)
file(GLOB TEST_SOURCE ${CMAKE_SOURCE_DIR}/test/*.cc)
foreach(TEST ${TEST_SOURCE})
    # https://cmake.org/cmake/help/latest/command/cmake_path.html#command:cmake_path
    cmake_path(GET TEST STEM TEST_NAME)
    set_source_files_properties(${TEST_NAME} PROPERTIES LANGUAGE CXX)
    add_executable(${TEST_NAME} ${TEST})
    target_compile_features(${TEST_NAME} PUBLIC cxx_std_20)
endforeach()