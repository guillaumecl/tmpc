find_library(MPD_CLIENT mpdclient)

add_library(mpd++ STATIC
  mpd.cpp
  song.cpp
  exception.cpp
  song_iterator.cpp
  song_tag_iterator.cpp
  search.cpp
  tag.cpp
  status.cpp
)

target_link_libraries(mpd++ ${MPD_CLIENT})
