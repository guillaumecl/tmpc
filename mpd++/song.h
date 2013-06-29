#ifndef MPDPP_SONG_H
#define MPDPP_SONG_H

#include "forward.h"

#include <ostream>

#include "tag.h"

struct mpd_song;

namespace mpdpp
{

class song;


/**
 * Represents a song inside a playlist.
 */
class song: public std::enable_shared_from_this<song>
{
public:
	/**
	 * Constructs a song from the underlying mpd object.
	 */
	song(mpd_song *s);

	/**
	 * Creates a song from another pointer.
	 */
	song(song &&reused_song);

	song& operator=(song &&s);

	~song();

	/**
	 * The song URI of the underlying file.
	 *
	 * This can be either relative to the mpd source folder (for local files),
	 * or absolute (for webradios)
	 */
	const char * uri() const;

    /*
	 * Retrieves a specific tag from the file.
	 */
	const char * tag(mpdpp::tag tag) const;

	/**
	 * Duration of the song in seconds. 0 for unknown size.
	 */
	unsigned int duration() const;

	/**
	 * Start of the song inside the file (useful for bulked files)
	 */
	unsigned int start() const;

	/**
	 * End of the song inside the file (useful for bulked files).
	 * 0 means this is the last song in the file.
	 */
	unsigned int end() const;

	/**
	 * POSIX UTC time stamp of the last modification, or 0 if that is unknown.
	 */
	unsigned int last_modified() const;

	/**
	 * Sets the position within the queue.
	 * This value is not used for songs which are not in the queue.
	 */
	void set_pos(unsigned int new_pos);

	/**
	 * Position of this song in the queue.
	 * The value is undefined if you did not obtain this song from the queue.
	 */
	unsigned int get_pos() const;

	/**
	 * Id of this song in the playlist.
	 * The value is undefined if you did not obtain this song from the queue.
	 */
	unsigned int id() const;

	/**
	 * Priority of this song in the playlist.
	 * The value is undefined if you did not obtain this song from the queue.
	 */
	unsigned int priority() const;

	/**
	 * Operator[] will return the corresponding tag.
	 */
	const char * operator[] (mpdpp::tag tag) const { return this->tag(tag); }

private:
	song(const song& s) = delete;
	song& operator=(song &s) = delete;

	/**
	 * Underlying mpd object.
	 */
	mpd_song *song_;
};

std::ostream& operator<<(std::ostream& out, const song & s);

}

#endif
