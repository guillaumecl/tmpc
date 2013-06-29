#ifndef MPDPP_SONG_ITERATOR_H
#define MPDPP_SONG_ITERATOR_H

#include <iterator>


namespace mpdpp
{


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

}

#endif
