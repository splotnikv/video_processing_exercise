cmake_minimum_required (VERSION 3.11)
project (deshuffler)
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_BINARY_DIR bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH lib)

include_directories(include)
include_directories(msdk_api/include)
include_directories(sample_common/include)

add_subdirectory(unit_tests)

add_subdirectory(sample_common)

add_library(deshuffler STATIC
		src/deshuffler.cpp
		src/input_params.cpp
		src/permutation_data.cpp
		src/yuv_reader_seek_i420.cpp)
add_dependencies(deshuffler sample_common)
target_link_libraries(deshuffler sample_common)

add_executable(deshuffler_cl src/main.cpp)
add_dependencies(deshuffler_cl deshuffler)
target_link_libraries(deshuffler_cl deshuffler)

add_custom_command(
    POST_BUILD
    TARGET deshuffler_cl
    COMMAND unit_tests
    WORKING_DIRECTORY unit_tests/bin/)