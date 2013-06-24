find_library(MPD_CLIENT mpdclient)

add_library(mpd++ STATIC
  mpd.cpp
  song.cpp
  exception.cpp
)

target_link_libraries(mpd++ ${MPD_CLIENT})
