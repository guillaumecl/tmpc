#include <mpd/client.h>

#include "mpd.h"

using namespace mpdpp;

mpd::mpd()
{
	connection_ = mpd_connection_new(nullptr, 0, 0);
	throw_if_error();
}

mpd::~mpd()
{
	if(connection_)
	{
		mpd_connection_free(connection_);
	}
}

void mpd::throw_if_error() const
{
	if (error())
	{
		throw exception(mpd_connection_get_error_message(connection_));
	}
}

void mpd::play() const
{
	mpd_send_play(connection_);
	throw_if_error();

	mpd_response_finish(connection_);
	throw_if_error();
}

void mpd::play(unsigned int song_id) const
{
	mpd_run_play_id(connection_, song_id);
	throw_if_error();

	mpd_response_finish(connection_);
	throw_if_error();
}

void mpd::play(const song& song) const
{
	play(song.id());
}

song_ptr mpd::add(const char *uri)
{
	unsigned int id = mpd_run_add_id(connection_, uri);
	throw_if_error();

	mpd_song *new_song = mpd_run_get_queue_song_id(connection_, id);
	throw_if_error();

	return std::make_shared<song>(new_song);
}

unsigned int mpd::error() const
{
	return mpd_connection_get_error(connection_);
}

std::string mpd::error_message() const
{
	return std::string(mpd_connection_get_error_message(connection_));
}

mpd_connection * mpd::internal_connection() const
{
	return connection_;
}

search mpd::queue(bool reuse_song_ptr)
{
	mpd_send_list_queue_meta(connection_);
	throw_if_error();
	return search(*this, true, reuse_song_ptr);
}

search mpd::search_queue(bool reuse_song_ptr)
{
	mpd_search_queue_songs(connection_, false);
	throw_if_error();
	return search(*this, false, reuse_song_ptr);
}

search mpd::search_db(bool reuse_song_ptr)
{
	mpd_search_db_songs(connection_, false);
	throw_if_error();
	return search(*this, false, reuse_song_ptr);
}

song_ptr mpd::current_song() const
{
	mpd_song *s = mpd_run_current_song(connection_);
	throw_if_error();
	if (s)
	{
		return std::make_shared<song>(s);
	}
	return nullptr;
}

song_ptr mpd::next_song(song_ptr existing_song)
{
	mpd_song *s = mpd_recv_song(connection_);
	throw_if_error();
	if (!s)
	{
		return nullptr;
	}
	if (existing_song)
	{
		*existing_song = song(s);
		return existing_song;
	}
	return std::make_shared<song>(s);
}



song_iterator::song_iterator(mpd& mpd, bool reuse_song_ptr) :
	mpd_(mpd),
	current_song_(nullptr),
	reuse_song_ptr_(reuse_song_ptr)
{
}

song_iterator& song_iterator::operator++()
{
	if (reuse_song_ptr_)
	{
		current_song_ = mpd_.next_song(current_song_);
	}
	else
	{
		current_song_ = mpd_.next_song(nullptr);
	}
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

song& song_iterator::operator*()
{
	return *current_song_;
}



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
