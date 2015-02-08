find_package(Qt4 REQUIRED)

# Allows referencing mpd++ headers with <mpd++/file.h> like it's a real library.
include_directories(${CMAKE_SOURCE_DIR})

file(GLOB SRC_TRANSLATIONS "${CMAKE_CURRENT_SOURCE_DIR}/*.ts")

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})


QT4_WRAP_CPP(TMPC_HEADERS_MOC
  application.h
  search_queue_widget.h
  main_window.h
  song_model.h
  song_widget.h
  history_line_edit.h
  display_widget.h
)

QT4_WRAP_UI(TMPC_FORMS_HEADERS

)

QT4_ADD_RESOURCES(TMPC_RESOURCES
  tmpc.qrc
)

SET(TMPC_TS
  ${SRC_TRANSLATIONS}
)

QT4_ADD_TRANSLATION(TMPC_TRANSLATIONS
  ${TMPC_TS}
)

add_executable(tmpc
  application.cpp
  tmpc.cpp
  main_window.cpp
  search_queue_widget.cpp
  song_model.cpp
  song_widget.cpp
  history_line_edit.cpp
  display_widget.cpp
  ${TMPC_HEADERS_MOC}
  ${TMPC_FORMS_HEADERS}
  ${TMPC_RESOURCES}
  ${TMPC_TRANSLATIONS}
)

target_link_libraries(tmpc mpd++ ${QT_LIBRARIES})



add_custom_target(lupdate
  COMMAND ${CMAKE_SOURCE_DIR}/scripts/update_translations.sh "${CMAKE_BINARY_DIR}/translations"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(tmpc lupdate)

file(GLOB TRANSLATIONS "${CMAKE_CURRENT_BINARY_DIR}/*.qm")

install(TARGETS tmpc DESTINATION bin)
install(FILES "${TRANSLATIONS}" DESTINATION share/tmpc)
