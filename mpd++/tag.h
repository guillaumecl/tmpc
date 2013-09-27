#ifndef MPDPP_TAG_H
#define MPDPP_TAG_H

#include <iterator>

namespace mpdpp
{


/**
 * The different tags that can be retrieved from the songs.
 */
enum class tag
{
	unknown=-1,
	artist,
	album,
	album_artist,
	title,
	track,
	name,
	genre,
	date,
	composer,
	performer,
	comment,
	disc,
	musicbrainz_artist_id,
	musicbrainz_album_id,
	musicbrainz_album_artist_id,
	musicbrainz_track_id,
};

/**
 * Returns a tag from the (case insensitive) name.
 */
tag tag_from_string(const char *string);

/**
 * Returns the human readable name of a tag.
 */
const char * tag_to_string(tag tag);

/**
 * Allows to iterate on tags.
 */
struct tag_iterator: public std::iterator<std::forward_iterator_tag, tag>
{
	/**
	 * Constructs an iterator pointing to a specific tag.
	 */
	tag_iterator(tag t) :
		tag_(t)
	{
	}

	/**
	 * Point to the next tag.
	 */
	tag_iterator& operator++()
	{
		if (tag_ >= tag::musicbrainz_track_id or tag_ == tag::unknown)
		{
			tag_ = tag::unknown;
		}
		else
		{
			int num = static_cast<int>(tag_);
			tag_ = static_cast<tag>(num+1);
		}
		return *this;
	}

	/**
	 * Compare two tag iterators.
	 */
	bool operator==(const tag_iterator& rhs) const
	{
		return tag_ == rhs.tag_;
	}

	/**
	 * Compare two tag iterators.
	 */
	bool operator!=(const tag_iterator& rhs) const
	{
		return !(*this == rhs);
	}

	/**
	 * Get the tag pointed by the iterator.
	 */
	tag operator*() const
	{
		return tag_;
	}
private:
	tag tag_;
};


/**
 * Use this to iterate on tags.
 *
 * for (tag t : tags()) { ... }
 */
struct tags
{
	/**
	 * Iterator to the first tag.
	 */
	static tag_iterator begin()
	{
		return tag_iterator(tag::artist);
	}

	/**
	 * Iterator after the last tag.
	 */
	static tag_iterator end()
	{
		return tag_iterator(tag::unknown);
	}
};


/**
 * Pass this into a search to search a specific tag value.
 */
struct tag_contains
{
	/**
	 * Search for a value for a specific tag.
	 * @param tag the tag to search
	 * @param value the value to search for
	 */
	tag_contains(tag tag, const char *value) :
		tag_(tag),
		value_(value)
	{}

	/**
	 * The tag to search.
	 */
	tag tag_;

	/**
	 * Value of the tag to search.
	 *
	 * This will not be freed, and the pointer must remain valid until the search is done.
	 */
	const char *value_;
};

/**
 * Pass this into a search to search for a value in any tag.
 */
struct any_tag_contains
{
	/**
	 * Search for a value in any tag.
	 * @param value the value to search for
	 */
	any_tag_contains(const char *value) :
		value_(value)
	{}

	/**
	 * Value to search.
	 *
	 * This will not be freed, and the pointer must remain valid until the search is done.
	 */
	const char * value_;
};

/**
 * Pass this into a search to search for a value in the uri.
 */
struct uri_contains
{
	/**
	 * Search for a value in any tag.
	 * @param value the value to search for
	 */
	uri_contains(const char *value) :
		value_(value)
	{}

	/**
	 * Value to search.
	 *
	 * This will not be freed, and the pointer must remain valid until the search is done.
	 */
	const char * value_;
};


}

#endif
