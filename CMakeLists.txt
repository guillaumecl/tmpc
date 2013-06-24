cmake_minimum_required(VERSION 2.6)
project(tmpc)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -Werror")

add_subdirectory(mpd++)

add_executable(tmpc
  tmpc.cpp
)
target_link_libraries(tmpc mpd++)
