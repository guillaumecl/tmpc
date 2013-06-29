#ifndef MPDPP_SEARCH_H
#define MPDPP_SEARCH_H

#include "mpd.h"
#include "song_iterator.h"

namespace mpdpp
{

/**
 * A search in the database or queue.
 */
class search
{
public:
	typedef song_iterator iterator;

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

private:
	/**
	 * Creates a search.
	 * @param mpd the connection to the mpd server.
	 * @param queue_search if true, the search is actually a pointer to the queue.
	 * @param reuse_song_ptr if true, there will be only one pointer to a song for all iterators.
	 */
	search(mpd& mpd, bool queue_search, bool reuse_song_ptr);

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
	 * If true, the search is actually a pointer to the queue.
	 */
	bool queue_search_;

	friend class mpd;
};


}

#endif
