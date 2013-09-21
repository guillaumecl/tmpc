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

	/**
	 * Next element.
	 */
	song_iterator& operator++();

	/**
	 * Compares two iterators.
	 */
	bool operator==(song_iterator const& rhs) const;

	/**
	 * Compares two iterators.
	 */
	bool operator!=(song_iterator const& rhs) const;

	/**
	 * Access the underlying object of the iterator.
	 * Obviously, don't use this if the iterator is invalid (i.e. this == end())
	 */
	song_ptr operator->();

	/**
	 * Access the underlying object of the iterator.
	 * Obviously, don't use this if the iterator is invalid (i.e. this == end())
	 */
	song& operator*();

	/**
	 * Steal the underlying pointer.
	 * The iterator will not point to a song anymore. operator++ can still be called, though.
	 * This is useful if the iterator is configured to reuse the pointers and you still need to get the ownership of the pointer.
	 */
	song_ptr steal_ptr();
private:
	/**
	 * Build an iterator.
	 * @param mpd the underlying connection.
	 * @param reuse_song_ptr if true, the iterator will not allocate pointers but instead fill the same pointer with each song information.
	 * @param queue if true, songs in this iterator reference the queue
	 */
	song_iterator(mpd& mpd, bool reuse_song_ptr, bool queue);

	/**
	 * The underlying mpd connection.
	 */
	mpd& mpd_;

	/**
	 * The current song in the iterator.
	 */
	song_ptr current_song_;

	/**
	 * If true, the iterator will not allocate pointers but instead fill the same pointer with each song information.
	 */
	bool reuse_song_ptr_;

	/**
	 * If true, songs in this iterator references the queue,
	 */
	bool queue_;

	/**
	 * search is a friend because it's the only class that should be able to create song_iterators for now.
	 */
	friend class search;
};

}

#endif
