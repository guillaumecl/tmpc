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
	 * Returns true if this search references the queue.
	 */
	bool queue_search() const;
private:
	/**
	 * Creates a search.
	 * @param mpd the connection to the mpd server.
	 * @param allow_empty_search if true, allow to search with no parameter.
	 * @param queue_search if true, the search is actually a pointer to the queue.
	 * @param reuse_song_ptr if true, there will be only one pointer to a song for all iterators.
	 */
	search(mpd& mpd, bool allow_empty_search, bool queue_search, bool reuse_song_ptr, bool add_search);

	/**
	 * Connection to the mpd server.
	 */
	mpd &mpd_;

	/**
	 * True if the search is currently empty.
	 * Used to notify mpd to cancel the search.
	 */
	bool empty_;

	/**
	 * If true, there will be only one pointer to a song for all iterators.
	 */
	bool reuse_song_ptr_;

	/**
	 * If true, allow to search with no parameter.
	 */
	bool allow_empty_search_;

	/**
	 * If true, the search is actually a pointer to the queue.
	 */
	bool queue_search_;

	/**
	 * If true, this is a non iterable search that adds its results into the queue.
	 */
	bool add_search_;

	friend class mpd;
};


}

#endif
