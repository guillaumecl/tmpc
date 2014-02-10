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

#include "status.h"

#include <mpd/client.h>

#include <algorithm>

using namespace mpdpp;


status::status(mpd_status *status) :
	status_(status)
{
}

status::status(status&& status) :
	status_(nullptr)
{
	std::swap(status_, status.status_);
}

status::~status()
{
	if (status_)
	{
		mpd_status_free(status_);
	}
}

int status::volume() const
{
	return mpd_status_get_volume(status_);
}

bool status::repeat() const
{
	return mpd_status_get_repeat(status_);
}


bool status::random() const
{
	return mpd_status_get_random(status_);
}


bool status::single() const
{
	return mpd_status_get_single(status_);
}


bool status::consume() const
{
	return mpd_status_get_consume(status_);
}


unsigned int status::queue_length() const
{
	return mpd_status_get_queue_length(status_);
}


unsigned int status::queue_version() const
{
	return mpd_status_get_queue_version(status_);
}


state status::state() const
{
	return static_cast<mpdpp::state>(mpd_status_get_state(status_));
}


unsigned int status::crossfade() const
{
	return mpd_status_get_crossfade(status_);
}


float status::mixrampdb() const
{
	return mpd_status_get_mixrampdb(status_);
}


float status::mixrampdelay() const
{
	return mpd_status_get_mixrampdelay(status_);
}


int status::song_pos() const
{
	return mpd_status_get_song_pos(status_);
}


int status::song_id() const
{
	return mpd_status_get_song_id(status_);
}


int status::next_song_pos() const
{
#if ((LIBMPDCLIENT_MAJOR_VERSION == 2 && LIBMPDCLIENT_MINOR_VERSION > 7) || LIBMPDCLIENT_MAJOR_VERSION > 2)
	return mpd_status_get_next_song_pos(status_);
#else
    return 0;
#endif
}

int status::next_song_id() const
{
#if ((LIBMPDCLIENT_MAJOR_VERSION == 2 && LIBMPDCLIENT_MINOR_VERSION > 7) || LIBMPDCLIENT_MAJOR_VERSION > 2)
	return mpd_status_get_next_song_id(status_);
#else
    return 0;
#endif
}



unsigned int status::elapsed_time() const
{
	return mpd_status_get_elapsed_time(status_);
}


unsigned int status::elapsed_ms() const
{
	return mpd_status_get_elapsed_ms(status_);
}


unsigned int status::total_time() const
{
	return mpd_status_get_total_time(status_);
}


unsigned int status::kbit_rate() const
{
	return mpd_status_get_kbit_rate(status_);
}


// audio_format status::audio_format() const;

unsigned int status::update_id() const
{
	return mpd_status_get_update_id(status_);
}


const char *status::error() const
{
	return mpd_status_get_error(status_);
}
