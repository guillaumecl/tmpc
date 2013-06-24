cmake_minimum_required(VERSION 2.6)
project(tmpc)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -Werror")

find_library(MPD_CLIENT mpdclient)

add_executable(tmpc
  tmpc.cpp
  mpd.cpp
  exception.cpp
  song.cpp
)
target_link_libraries(tmpc ${MPD_CLIENT})
