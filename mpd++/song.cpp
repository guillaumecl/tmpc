/******************************************************************************

Copyright (C) 2013  CLÉMENT Guillaume <gclement@baobob.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 2.1
of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

******************************************************************************/

#include "song.h"

#include <mpd/client.h>

namespace mpdpp
{

song::song(mpd_song *s, bool queue) :
	song_(s),
	queue_(queue)
{
}

song::song(song && reused_song)
{
	std::swap(song_, reused_song.song_);
	queue_ = reused_song.queue_;
}

song& song::operator=(song && reused_song)
{
	std::swap(song_, reused_song.song_);
	queue_ = reused_song.queue_;
	return *this;
}

void song::replace(song_ptr other)
{
	*this = std::move(*other);
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

unsigned int song::position() const
{
	return mpd_song_get_pos(song_);
}


unsigned int song::id() const
{
	return mpd_song_get_id(song_);
}

unsigned int song::priority() const
{
#if ((LIBMPDCLIENT_MAJOR_VERSION == 2 && LIBMPDCLIENT_MINOR_VERSION > 7) || LIBMPDCLIENT_MAJOR_VERSION > 2)
	return mpd_song_get_prio(song_);
#else
return 0;
#endif
}

bool song::queued() const
{
	return queue_;
}

std::ostream& operator<<(std::ostream& out, const song & s)
{
	const char * title = s[mpdpp::tag::title];
	if (title)
	{
		out << title;
	}
	else
	{
		out << s.uri();
	}

	const char *artist = s[mpdpp::tag::artist];
	if (artist)
	{
		out << " by " << artist;
	}

	const char * album = s[mpdpp::tag::album];
	if (album)
	{
		out << " (" << album << ")";
	}
	return out;
}

}
