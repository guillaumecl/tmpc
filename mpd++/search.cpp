#include <mpd/client.h>

#include "mpd.h"

using namespace mpdpp;



search::search(mpd& mpd, bool queue_search, bool reuse_song_ptr) :
	mpd_(mpd),
	empty_(true),
	reuse_song_ptr_(reuse_song_ptr),
	queue_search_(queue_search)
{
}

search::~search()
{
	if (not queue_search_ and empty_)
	{
		mpd_search_cancel(mpd_.connection_);
		mpd_.throw_if_error();
	}
	mpd_response_finish(mpd_.connection_);
	mpd_.throw_if_error();
}

search::iterator search::begin()
{
	if (not queue_search_)
	{
		if(empty_)
		{
			return end();
		}
		else
		{
			mpd_search_commit(mpd_.connection_);
			mpd_.throw_if_error();
		}
	}
	return ++iterator(mpd_, reuse_song_ptr_);
}


bool search::empty() const
{
	return empty_;
}

search::iterator search::end()
{
	return iterator(mpd_, reuse_song_ptr_);
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