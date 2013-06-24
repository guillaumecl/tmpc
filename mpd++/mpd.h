#ifndef MPDPP_CONNECTION_H
#define MPDPP_CONNECTION_H

#include <vector>
#include <memory>

#include "exception.h"
#include "song.h"
#include "tag.h"

struct mpd_connection;

namespace mpdpp
{

/**
 * Pass this to operator<< of mpd to commit the current search and retrieve the results.
 */
struct commit
{
};



/**
 * Connection to a MPD server.
 */
class mpd
{
public:
	/**
	 * Creates a connection to a MPD server, effectively connecting to it.
	 * @throws mpdpp::exception if the connection fails.
	 */
	mpd();

	/**
	 * Destroys an mpd object, effectively closing the connection and freeing all memory.
	 */
	~mpd();

	/**
	 * Play the current song.
	 */
	void play() const;

	/**
	 * This is const because we can't know if it's const or not, and some uses are legally const.
	 * This should not be abused to do non-const actions as this could potentially lead to undefined behaviour.
	 */
	mpd_connection * internal_connection() const;

	/**
	 * The current queue of songs.
	 */
	song_ptr_vector queue() const;

	/**
	 * Adds a search constraint to the current operation.
	 * Equivalent to the << operator.
	 */
	void add_search_constraint(tag t, const char *value);

	/**
	 * Adds a search constraint to the current operation.
	 * Equivalent to the << operator.
	 */
	void add_search_constraint(const char *value);


	mpd& search_db();
	mpd& search_queue();

	song_ptr_vector commit_search() const;

	song_ptr current_song() const;

	void play(const song& song) const;

	song_ptr add(const char *uri);

	song_ptr_vector operator<<(const commit & );

private:
	/**
	 * Returns the errors that happened at the last call to mpd.
	 * 0 means success.
	 */
	unsigned int error() const;

	/**
	 * Returns the message associated to the last error. Don't call this if error() returns 0.
	 */
	std::string error_message() const;

	/**
	 * Throw an exception if an error occurred.
	 */
	void throw_if_error() const;

	/**
	 * After a request that should send songs, call this to fill in a song_ptr_vector..
	 */
	void build_song_response(song_ptr_vector & fill_me) const;


	mpd(mpd const& c) = delete;
	mpd& operator=(mpd const& c) = delete;

	/**
	 * The underlying mpd connection.
	 */
	mpd_connection * connection_;

};

inline mpd& operator<<(mpd &out, const tag_contains & tag)
{
	out.add_search_constraint(tag.tag_, tag.value_);
	return out;
}

inline mpd& operator<<(mpd &out, const any_tag_contains & tag)
{
	out.add_search_constraint(tag.value_);
	return out;
}

}


#endif
