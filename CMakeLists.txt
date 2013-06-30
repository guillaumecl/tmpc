cmake_minimum_required(VERSION 2.6)
project(tmpc)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -Werror")

add_subdirectory(mpd++)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})


QT4_WRAP_CPP(TMPC_HEADERS_MOC
  search_queue_widget.h
  main_window.h
  song_widget.h
)

QT4_WRAP_UI(TMPC_FORMS_HEADERS

)


add_executable(tmpc
  tmpc.cpp
  main_window.cpp
  search_queue_widget.cpp
  song_widget.cpp
  ${TMPC_HEADERS_MOC}
  ${TMPC_FORMS_HEADERS}
)

target_link_libraries(tmpc mpd++ ${QT_LIBRARIES})
