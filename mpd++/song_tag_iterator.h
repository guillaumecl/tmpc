#ifndef MPDPP_SONG_TAG_ITERATOR_H
#define MPDPP_SONG_TAG_ITERATOR_H

#include <iterator>
#include "tag.h"

namespace mpdpp
{

class song;
class song_tags;

/**
 * Allows to iterate on tags from songs.
 */
class song_tag_iterator: public std::iterator<std::forward_iterator_tag, std::pair<mpdpp::tag, const char *>>
{
public:
	/**
	 * Go to the next tag.
	 */
	song_tag_iterator& operator++()
	{
		++iter_;
		find_next();
		return *this;
	}

	/**
	 * Get the current tag. Undefined if *this == end()
	 */
	std::pair<mpdpp::tag, const char *> operator*() const
	{
		return std::make_pair(*iter_, tag_);
	}

	/**
	 * Report equality to another iterator.
	 */
	bool operator== (const song_tag_iterator& rhs) const
	{
		return song_ == rhs.song_ and iter_ == rhs.iter_;
	}

	/**
	 * Report inequality to another iterator.
	 */
	bool operator!= (const song_tag_iterator& rhs) const
	{
		return !(*this == rhs);
	}
private:
	/**
	 * Construct an iterator to a tag.
	 */
	song_tag_iterator(const song *s, tag_iterator it) :
		song_(s),
		iter_(it)
	{
		find_next();
	}

	/**
	 * Move to the next tag.
	 */
	void find_next();

	/**
	 * The current song. nullptr for for invalid tags.
	 */
	const song *song_;

	/**
	 * Current tag iterator.
	 * @internal
	 */
	tag_iterator iter_;

	/**
	 * Current tag name.
	 */
	const char *tag_;
	friend class song_tags;
};

/**
 * Gives access to begin and end to iterate on song tags.
 */
class song_tags
{
public:
	/**
	 * Iterator to the first tag.
	 */
	song_tag_iterator begin()
	{
		return song_tag_iterator(song_, tags().begin());
	}

	/**
	 * Iterator after the last tag.
	 */
	song_tag_iterator end()
	{
		return song_tag_iterator(song_, tags().end());
	}
private:
	/**
	 * Construct a song_tag object from a song.
	 */
	song_tags(const song *s):
		song_(s)
	{
	}

	/**
	 * The song referenced by this object.
	 */
	const song *song_;
	friend class song;
};

}

#endif //MPDPP_SONG_TAG_ITERATOR_H
