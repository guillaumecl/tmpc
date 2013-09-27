#ifndef MPDPP_SONG_TAG_ITERATOR_H
#define MPDPP_SONG_TAG_ITERATOR_H

#include <iterator>
#include "tag.h"

namespace mpdpp
{

class song;
class song_tags;

class song_tag_iterator: public std::iterator<std::forward_iterator_tag, std::pair<mpdpp::tag, const char *>>
{
public:
	song_tag_iterator& operator++()
	{
		++iter_;
		find_next();
		return *this;
	}

	std::pair<mpdpp::tag, const char *> operator*() const
	{
		return std::make_pair(*iter_, tag_);
	}

	bool operator== (const song_tag_iterator& rhs) const
	{
		return song_ == rhs.song_ and iter_ == rhs.iter_;
	}

	bool operator!= (const song_tag_iterator& rhs) const
	{
		return !(*this == rhs);
	}
private:
	song_tag_iterator(const song *s, tag_iterator it) :
		song_(s),
		iter_(it)
	{
		find_next();
	}

	void find_next();

	const song *song_;
	tag_iterator iter_;
	const char *tag_;
	friend class song_tags;
};

class song_tags
{
public:
	song_tag_iterator begin()
	{
		return song_tag_iterator(song_, tags().begin());
	}

	song_tag_iterator end()
	{
		return song_tag_iterator(song_, tags().end());
	}
private:
	song_tags(const song *s):
		song_(s)
	{
	}

	const song *song_;
	friend class song;
};

}

#endif //MPDPP_SONG_TAG_ITERATOR_H
