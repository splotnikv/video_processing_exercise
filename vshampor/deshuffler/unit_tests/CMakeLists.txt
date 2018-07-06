add_subdirectory(googletest)
include_directories(googletest/googletest)
include_directories(googletest/googletest/include)
include_directories(googletest/googlemock)
include_directories(googletest/googlemock/include)

add_executable(unit_tests 
	src/deshuffler_test.cpp
	src/yuv_reader_seek_i420_test.cpp)
add_dependencies(unit_tests deshuffler gtest_main)
target_link_libraries(unit_tests deshuffler gtest_main)

