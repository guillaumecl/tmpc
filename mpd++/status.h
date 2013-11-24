/******************************************************************************

Copyright (C) 2013  CLÉMENT Guillaume <gclement@baobob.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 2.1
of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

******************************************************************************/

#ifndef MPDPP_STATUS_H
#define MPDPP_STATUS_H

struct mpd_status;

namespace mpdpp
{

enum class state
{
	unknown,
	mpd_state_stop,
	mpd_state_play,
	mpd_state_pause,
};

/**
 * Current status of mpd.
 */
class status
{
public:
	/**
	 * Create a status.
	 */
	status(mpd_status *status);

	/**
	 * Status.
	 */
	status(status&& status);

	/**
	 * Destructor.
	 */
	~status();

	/**
	 * Returns the current volume: 0-100, or -1 when there is no volume support.
	 */
	int volume() const;

	/**
	 * Returns true if repeat mode is on.
	 */
	bool repeat() const;

	/**
	 * Returns true if random mode is on.
	 */
	bool random() const;

	/**
	 * Returns true if single mode is on.
	 */
	bool single() const;

	/**
	 * Returns true if consume mode is on.
	 */
	bool consume() const;

	/**
	 * Returns the number of songs in the queue. If MPD did not specify that, this function returns 0.
	 */
	unsigned int queue_length() const;

	/**
	 * Returns queue version number. You may use this to determine when the queue has changed since you have last queried it.
	 */
	unsigned int queue_version() const;

	/**
	 * Returns the state of the player: either stopped, playing or paused.
	 */
	mpdpp::state state() const;

	/**
	 * Returns crossfade setting in seconds. 0 means crossfading is disabled.
	 */
	unsigned int crossfade() const;

	/**
	 * Returns mixrampdb setting in db.
	 */
	float mixrampdb() const;

	/**
	 * Returns mixrampdelay setting in seconds. Negative means mixramp is disabled.
	 */
	float mixrampdelay() const;

	/**
	 * Returns the position of the currently playing song in the queue (beginning with 0) if a song is currently selected (always the case when state is PLAY or PAUSE). If there is no current song, -1 is returned.
	 */
	int song_pos() const;

	/**
	 * Returns the id of the current song. If there is no current song, -1 is returned.
	 */
	int song_id() const;

	/**
	 * The same as mpd_status_get_next_song_pos, but for the next song to be played.
	 */
	int next_song_id() const;

	/**
	 * Returns the id of the next song to be played. If it is not known, -1 is returned.
	 */
	int next_song_pos() const;

	/**
	 * Returns time in seconds that have elapsed in the currently playing/paused song.
	 */
	unsigned int elapsed_time() const;

	/**
	 * Returns time in milliseconds that have elapsed in the currently playing/paused song.
	 */
	unsigned int elapsed_ms() const;

	/**
	 * Returns the length in seconds of the currently playing/paused song.
	 */
	unsigned int total_time() const;

	/**
	 * Returns current bit rate in kbps. 0 means unknown.
	 */
	unsigned int kbit_rate() const;

	/**
	 * Returns audio format which MPD is currently playing. May return NULL if MPD is not playing or if the audio format is unknown.
	 */
	// audio_format audio_format() const;

	/**
	 * Returns 1 if mpd is updating, 0 otherwise.
	 */
	unsigned int update_id() const;

	/**
	 * Returns the error message.
	 */
	const char *error() const;
private:
	mpd_status *status_;
};

}

#endif // MPDPP_STATUS_H
