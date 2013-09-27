#ifndef MPDPP_SONG_H
#define MPDPP_SONG_H

#include "forward.h"

#include <ostream>

#include "tag.h"
#include "song_tag_iterator.h"

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
	song(mpd_song *s, bool queue);

	/**
	 * Creates a song from another pointer.
	 */
	song(song &&reused_song);

	/**
	 * Steal another song's data.
	 */
	song& operator=(song &&s);

	/**
	 * Destructor.
	 */
	~song();

	/**
	 * The song URI of the underlying file.
	 *
	 * This can be either relative to the mpd source folder (for local files),
	 * or absolute (for webradios)
	 */
	const char * uri() const;

    /**
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
	unsigned int position() const;

	/**
	 * Id of this song in the playlist.
	 * The value is undefined if you did not obtain this song from the queue.
	 */
	unsigned int id() const;

	/**
	 * Priority of this song in the playlist.
	 * The value is undefined if you did not obtain this song from the queue.
	 * A higher priority means that it will be played first when "random" mode is enabled.
	 */
	unsigned int priority() const;

	/**
	 * True if the song comes from the queue, false if it comes from the database.
	 */
	bool queued() const;

	/**
	 * Operator[] will return the corresponding tag.
	 */
	const char * operator[] (mpdpp::tag tag) const { return this->tag(tag); }

	song_tags tags() const
	{
		return song_tags(this);
	}

private:
	song(const song& s) = delete;
	song& operator=(song &s) = delete;

	/**
	 * Underlying mpd object.
	 */
	mpd_song *song_;

	/**
	 * True if the song references the queue.
	 */
	bool queue_;
};

/**
 * Print the content of the song into the specified stream.
 */
std::ostream& operator<<(std::ostream& out, const song & s);

}

#endif
