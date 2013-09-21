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
	{
		current_song_ = mpd_.next_song(current_song_, queue_);
	}
	else
	{
		current_song_ = mpd_.next_song(nullptr, queue_);
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
