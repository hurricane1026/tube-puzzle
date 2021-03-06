#
#
#
cmake_minimum_required(VERSION 3.0.0)
project(puzzle)

set(PUZZLE_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})


## make sure gtest/gmock exist.
find_package(GTest REQUIRED)

### bernoulli's headers
include_directories(${PUZZLE_SOURCE_DIR}/)
include_directories(${PUZZLE_SOURCE_DIR}/yaml-cpp/include)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fexceptions -fsized-deallocation")
set(COMMON_WARNING_FLAGS "${COMMON_WARNING_FLAGS} -Wall -Werror -Wextra -Wendif-labels -Wshadow")
set(CMAKE_CXX_EXTENSIONS OFF)

### unit tests code
add_subdirectory(yaml-cpp)
add_subdirectory(tests)


add_executable(solver solver.cpp)
target_link_libraries(solver yaml-cpp fmt pthread)

file(COPY ${PUZZLE_SOURCE_DIR}/game.yaml
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


