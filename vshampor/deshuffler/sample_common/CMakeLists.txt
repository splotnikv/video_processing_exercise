include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB_RECURSE SRC_LIST "*.cpp")

add_library(sample_common STATIC ${SRC_LIST})