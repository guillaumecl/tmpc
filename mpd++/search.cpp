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

#include <mpd/client.h>

#include "mpd.h"

using namespace mpdpp;



search::search(mpd& mpd, bool allow_empty_search, bool queue_search, bool reuse_song_ptr, bool add_search) :
	mpd_(mpd),
	empty_(true),
	reuse_song_ptr_(reuse_song_ptr),
	allow_empty_search_(allow_empty_search),
	queue_search_(queue_search),
	add_search_(add_search)
{
}

search::~search()
{
	if (not allow_empty_search_ and empty_)
	{
		mpd_search_cancel(mpd_.connection_);
		mpd_.throw_if_error();
	}
	else if (add_search_ and not empty_)
	{
		mpd_search_commit(mpd_.connection_);
		mpd_.throw_if_error();
	}
	mpd_response_finish(mpd_.connection_);
	mpd_.throw_if_error();
}

search::iterator search::begin()
{
	iterator ret = end();

	if (not allow_empty_search_ and not add_search_ and not empty_)
	{
		mpd_search_commit(mpd_.connection_);
		mpd_.throw_if_error();
		++ret;
	}
	else if (allow_empty_search_ and not add_search_)
	{
		++ret;
	}
	return ret;
}


bool search::valid() const
{
	return allow_empty_search_ or not empty_;
}

search::iterator search::end()
{
	return iterator(mpd_, reuse_song_ptr_, queue_search_);
}

bool search::queue_search() const
{
	return queue_search_;
}

search& search::operator<<(const tag_contains & tag)
{
	empty_ = false;
	mpd_search_add_tag_constraint(mpd_.connection_,
								  MPD_OPERATOR_DEFAULT,
								  static_cast<mpd_tag_type>(tag.tag_), tag.value_);
	mpd_.throw_if_error();
	return *this;
}

search& search::operator<<(const any_tag_contains & tag)
{
	empty_ = false;
	mpd_search_add_any_tag_constraint(mpd_.connection_,
									  MPD_OPERATOR_DEFAULT,
									  tag.value_);
	mpd_.throw_if_error();
	return *this;
}

search& search::operator<<(const uri_contains & tag)
{
	empty_ = false;
	mpd_search_add_uri_constraint(mpd_.connection_,
								  MPD_OPERATOR_DEFAULT,
								  tag.value_);
	mpd_.throw_if_error();
	return *this;
}
