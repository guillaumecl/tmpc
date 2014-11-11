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

#include "mpd.h"


using namespace mpdpp;


song_iterator::song_iterator(mpd& mpd, bool reuse_song_ptr, bool queue) :
	mpd_(mpd),
	current_song_(nullptr),
	reuse_song_ptr_(reuse_song_ptr),
	queue_(queue)
{
}

song_iterator& song_iterator::operator++()
{
	if (reuse_song_ptr_)
		current_song_ = mpd_.next_song(current_song_, queue_);
	else
		current_song_ = mpd_.next_song(nullptr, queue_);
	return *this;
}

bool song_iterator::operator==(song_iterator const& rhs) const
{
	return current_song_ == rhs.current_song_;
}

bool song_iterator::operator!=(song_iterator const& rhs) const
{
	return current_song_ != rhs.current_song_;
}

song_ptr song_iterator::operator->()
{
	return current_song_;
}

song_ptr song_iterator::steal_ptr()
{
	song_ptr result = current_song_;

	current_song_ = nullptr;
	return result;
}

song& song_iterator::operator*()
{
	return *current_song_;
}
