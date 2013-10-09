#include <mpd/client.h>

#include "mpd.h"

using namespace mpdpp;

mpd::mpd(const char *host, int port)
{
	connection_ = mpd_connection_new(host, port, 0);
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

void mpd::play(song_ptr song) const
{
	if (not song->queued())
	{
		song->replace(add(song->uri()));
	}
	play(song->id());
}

song_ptr mpd::add(const char *uri) const
{
	unsigned int id = mpd_run_add_id(connection_, uri);
	throw_if_error();

	mpd_song *new_song = mpd_run_get_queue_song_id(connection_, id);
	throw_if_error();

	return std::make_shared<song>(new_song, true);
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
	return search(*this, true, true, reuse_song_ptr, false);
}

void mpd::clear_queue()
{
	mpd_run_clear(connection_);
	throw_if_error();
}

search mpd::search_queue(bool reuse_song_ptr)
{
	mpd_search_queue_songs(connection_, false);
	throw_if_error();
	return search(*this, false, true, reuse_song_ptr, false);
}

search mpd::search_db(bool reuse_song_ptr)
{
	mpd_search_db_songs(connection_, false);
	throw_if_error();
	return search(*this, false, false, reuse_song_ptr, false);
}

search mpd::add_from_db()
{
	mpd_search_add_db_songs(connection_, false);
	throw_if_error();
	return search(*this, false, false, false, true);
}

song_ptr mpd::current_song() const
{
	mpd_song *s = mpd_run_current_song(connection_);
	throw_if_error();
	if (s)
	{
		return std::make_shared<song>(s, true);
	}
	return nullptr;
}

song_ptr mpd::next_song(song_ptr existing_song, bool queue)
{
	mpd_song *s = mpd_recv_song(connection_);
	throw_if_error();
	if (!s)
	{
		return nullptr;
	}
	if (existing_song)
	{
		*existing_song = song(s, queue);
		return existing_song;
	}
	return std::make_shared<song>(s, queue);
}

void mpd::set_song_priority(song_ptr song, unsigned int priority)
{
#if ((LIBMPDCLIENT_MAJOR_VERSION == 2 && LIBMPDCLIENT_MINOR_VERSION > 7) || LIBMPDCLIENT_MAJOR_VERSION > 2)
	mpd_run_prio_id(connection_, priority, song->id());
	throw_if_error();

	auto ptr = mpd_run_get_queue_song_id(connection_, song->id());
	*song = mpdpp::song(ptr, song->queued());
	throw_if_error();
#else
    if (priority)
    {
        song = 0;
    }
#endif
}

void mpd::set_queue_priority(unsigned int priority)
{
#if ((LIBMPDCLIENT_MAJOR_VERSION == 2 && LIBMPDCLIENT_MINOR_VERSION > 7) || LIBMPDCLIENT_MAJOR_VERSION > 2)
	mpd_run_prio_range(connection_, priority, 0, -1);
	throw_if_error();
#else
    if (priority)
    {
        return;
    }
#endif
}

void mpd::delete_song(song_ptr song)
{
	mpd_run_delete_id(connection_, song->id());
	throw_if_error();
}


void mpd::monitor(event flags)
{
	mpd_send_idle_mask(connection_, (mpd_idle)flags);
	throw_if_error();
}

event mpd::stop_monitor()
{
	event result = (event) mpd_run_noidle(connection_);
	throw_if_error();

	return result;
}

event mpd::wait_events()
{
	event result = (event) mpd_recv_idle(connection_, true);
	throw_if_error();

	return result;
}

void mpd::seek(int song_id, unsigned int position)
{
	mpd_run_seek_id(connection_, song_id, position);
	throw_if_error();

	mpd_response_finish(connection_);
	throw_if_error();

}

status mpd::status()
{
	mpd_status *content = mpd_run_status(connection_);
	throw_if_error();

	mpd_response_finish(connection_);
	throw_if_error();

	return mpdpp::status(content);
}
