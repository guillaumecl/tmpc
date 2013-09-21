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


class search;

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
	 * Steal another mpd connection.
	 */
	mpd(mpd && other) = default;

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
	 * @param reuse_song_ptr if true, don't allocate a new song at each iteration.
	 */
	search queue(bool reuse_song_ptr = true);

	/**
	 * Initiates a search insides the database.
	 * @param reuse_song_ptr if true, don't allocate a new song at each iteration.
	 */
	search search_db(bool reuse_song_ptr = true);

	/**
	 * Initiates a search inside the queue.
	 * @param reuse_song_ptr if true, don't allocate a new song at each iteration.
	 */
	search search_queue(bool reuse_song_ptr = true);

	/**
	 * Initiates a search from the db that adds all the results into the queue.
	 */
	search add_from_db();

	/**
	 * Returns the current song.
	 */
	song_ptr current_song() const;

	/**
	 * Returns the next song in the search.
	 * Fills the song if specified, instantiate a new song otherwise.
	 * @param song to fill if specified and not nullptr
	 * @returns song if not nullptr, the allocated song otherwise.
	 */
	song_ptr next_song(song_ptr song);

	/**
	 * Play the specified song inside the queue.
	 * @param song_id song id the play
	 */
	void play(unsigned int song_id) const;

	/**
	 * Play the specified song inside the queue.
	 * @param song the song to play
	 */
	void play(const song& song) const;

	/**
	 * Adds the specified song to the queue.
	 * @param uri the URI to add.
	 * @returns the added song.
	 */
	song_ptr add(const char *uri);

	/**
	 * Removes all the songs from the queue.
	 */
	void clear_queue();

	/**
	 * Changes the priority of a song.
	 */
	void set_song_priority(song_ptr song, unsigned int priority);


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

	mpd(mpd const& c) = delete;
	mpd& operator=(mpd const& c) = delete;

	/**
	 * The underlying mpd connection.
	 */
	mpd_connection * connection_;

	friend class search;
};

}

#include "search.h"

#endif
