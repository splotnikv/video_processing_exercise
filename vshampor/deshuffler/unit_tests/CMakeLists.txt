add_subdirectory(googletest)
include_directories(googletest/googletest)
include_directories(googletest/googletest/include)
include_directories(googletest/googlemock)
include_directories(googletest/googlemock/include)

add_executable(unit_tests 
	deshuffler_test.cpp)
add_dependencies(unit_tests deshuffler gtest_main)
target_link_libraries(unit_tests deshuffler gtest_main)

add_custom_command(
    POST_BUILD
    TARGET unit_tests
    COMMAND unit_tests
    WORKING_DIRECTORY bin/
    COMMENT "Running unit tests..."
)