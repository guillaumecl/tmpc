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
	/**
	 * A search is valid if at least a search criteria was added to it or
	 * if it references a whole collection (with no criteria)
	 */
	bool valid() const;

	typedef song_iterator iterator;
	~search();

	iterator begin();
	iterator end();

	search& operator<<(const tag_contains& tag);
	search& operator<<(const any_tag_contains& tag);

private:
	search(mpd& mpd, bool queue_search, bool reuse_song_ptr);

	mpd &mpd_;
	bool empty_;
	bool reuse_song_ptr_;
	bool queue_search_;
	friend class mpd;
};


}

#endif
