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

#ifndef MPDPP_SEARCH_H
#define MPDPP_SEARCH_H

#include "song_iterator.h"

namespace mpdpp
{

enum search_flags
{
	none = 0,
	/* allow to search with no parameter.*/
	allow_empty_search = 0x1,
	/* search is a pointer to the queue. */
	queue_search = 0x2,
	/* reuse memory for all iterators. */
	reuse_song_ptr = 0x4,
	/* add results into the queue instead of returning them */
	add_search = 0x8
};


/**
 * A search in the database or queue.
 */
class search
{
public:
	/**
	 * Iterator to the search.
	 * For now this is a song_iterator but this might change if needed.
	 */
	typedef song_iterator iterator;

	/**
	 * Move constructor for searches.
	 */
	search(search&& other) = default;

	/**
	 * Destructor.
	 */
	~search();

	/**
	 * A search is valid if at least a search criteria was added to it or
	 * if it references a whole collection (with no criteria)
	 */
	bool valid() const;

	/**
	 * Iterator to the first element of the search.
	 */
	iterator begin();

	/**
	 * Iterator to the end of the search.
	 */
	iterator end();

	/**
	 * Add a constraint to the search.
	 */
	search& operator<<(const tag_contains& tag);

	/**
	 * Add a constraint to the search.
	 */
	search& operator<<(const any_tag_contains& tag);

	/**
	 * Add a constraint to the search.
	 */
	search& operator<<(const uri_contains& tag);

	/**
	 * If true, there will be only one pointer to a song for all iterators.
	 */
	bool reuse_song_ptr() const { return flags_ & search_flags::reuse_song_ptr;}

	/**
	 * If true, allow to search with no parameter.
	 */
	bool allow_empty_search() const { return flags_ & search_flags::allow_empty_search;}

	/**
	 * If true, the search is actually a pointer to the queue.
	 */
	bool queue_search() const { return flags_ & search_flags::queue_search;}

	/**
	 * If true, this is a non iterable search that adds its results into the queue.
	 */
	bool add_search() const { return flags_ & search_flags::add_search;}

private:
	/**
	 * Creates a search.
	 * @param mpd the connection to the mpd server.
	 * @param flags search_flags
	 * @see search_flags
	 */
	search(mpd& mpd, search_flags flag1,
		search_flags flag2 = search_flags::none,
		search_flags flag3 = search_flags::none);

	/**
	 * Connection to the mpd server.
	 */
	mpd &mpd_;

	/**
	 * True if the search is currently empty.
	 * Used to notify mpd to cancel the search.
	 */
	bool empty_;

	search_flags flags_;

	friend class mpd;
};


}

#endif
