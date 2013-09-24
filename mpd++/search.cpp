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
	if (not allow_empty_search_)
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
	return ++iterator(mpd_, reuse_song_ptr_, queue_search_);
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
