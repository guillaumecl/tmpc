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
	search queue(bool reuse_song_ptr = true);

	/**
	 * Initiates a search insides the database.
	 */
	search search_db(bool reuse_song_ptr = true);

	/**
	 * Initiates a search inside the queue.
	 */
	search search_queue(bool reuse_song_ptr = true);

	/**
	 * Returns the current song.
	 */
	song_ptr current_song() const;

	/**
	 * Returns the next song in the search.
	 * Fills the song if specified, instantiate a new song otherwise.
	 */
	song_ptr next_song(song_ptr song);

	/**
	 * Play the specified song inside the queue.
	 */
	void play(unsigned int song_id) const;

	/**
	 * Play the specified song inside the queue.
	 */
	void play(const song& song) const;

	/**
	 * Adds the specified song to the queue.
	 * @returns the added song.
	 */
	song_ptr add(const char *uri);

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


/**
 * Iterator for songs.
 */
class song_iterator: public std::iterator<std::forward_iterator_tag, song>
{
public:
	song_iterator& operator++();

	bool operator==(song_iterator const& rhs) const;

	bool operator!=(song_iterator const& rhs) const;

	song_ptr operator->();

	song& operator*();
private:
	song_iterator(mpd& mpd, bool reuse_song_ptr);

	mpd& mpd_;
	song_ptr current_song_;
	bool reuse_song_ptr_;
	friend class search;
};

/**
 * A search in the database or queue.
 */
class search
{
public:
	typedef song_iterator iterator;
	~search();

	iterator begin();
	iterator end();

	search& operator<<(const tag_contains& tag);
	search& operator<<(const any_tag_contains& tag);

	bool empty() const;

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
