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

void mpd::play(const song& song) const
{
	mpd_run_play_id(connection_, song.id());
	throw_if_error();

	mpd_response_finish(connection_);
	throw_if_error();
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

song_ptr_vector mpd::queue() const
{
	std::vector<std::shared_ptr<song> > result;

	mpd_send_list_queue_meta(connection_);
	throw_if_error();

	build_song_response(result);

	return result;
}

void mpd::build_song_response(song_ptr_vector& fill_me) const
{
	mpd_song *new_song;
	while((new_song = mpd_recv_song(connection_)))
	{
		throw_if_error();
		fill_me.emplace_back(std::make_shared<song>(new_song));
	}
	throw_if_error();

	mpd_response_finish(connection_);
	throw_if_error();
}

mpd_connection * mpd::internal_connection() const
{
	return connection_;
}

void mpd::add_search_constraint(tag t, const char *value)
{
	mpd_search_add_tag_constraint(connection_, MPD_OPERATOR_DEFAULT, static_cast<mpd_tag_type>(t), value);
	throw_if_error();
}

void mpd::add_search_constraint(const char *value)
{
	mpd_search_add_any_tag_constraint(connection_, MPD_OPERATOR_DEFAULT, value);
	throw_if_error();
}

song_ptr_vector mpd::operator<<(const commit & /*unused*/)
{
	song_ptr_vector result;

	mpd_search_commit(connection_);
	throw_if_error();

	build_song_response(result);

	return result;
}

song_ptr_vector mpd::commit_search() const
{
	song_ptr_vector result;

	mpd_search_commit(connection_);
	throw_if_error();

	build_song_response(result);

	return result;
}

mpd& mpd::search_queue()
{
	mpd_search_queue_songs(connection_, false);
	throw_if_error();
	return *this;
}

mpd& mpd::search_db()
{
	mpd_search_db_songs(connection_, false);
	throw_if_error();
	return *this;
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
