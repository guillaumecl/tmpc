#include "song.h"

#include <mpd/client.h>

namespace mpdpp
{

song::song(mpd_song *s) :
	song_(s)
{
}

song::~song()
{
	mpd_song_free(song_);
}

const char * song::uri() const
{
	return mpd_song_get_uri(song_);
}

const char * song::tag(mpdpp::tag t) const
{
	return mpd_song_get_tag(song_, static_cast<mpd_tag_type>(t), 0);
}

unsigned int song::duration() const
{
	return mpd_song_get_duration(song_);
}

unsigned int song::start() const
{
	return mpd_song_get_start(song_);
}

unsigned int song::end() const
{
	return mpd_song_get_end(song_);
}

unsigned int song::last_modified() const
{
	return mpd_song_get_last_modified(song_);
}

void song::set_pos(unsigned int new_pos)
{
	return mpd_song_set_pos(song_, new_pos);
}

unsigned int song::get_pos() const
{
	return mpd_song_get_pos(song_);
}


unsigned int song::id() const
{
	return mpd_song_get_id(song_);
}

unsigned int song::priority() const
{
	return mpd_song_get_prio(song_);
}

}
