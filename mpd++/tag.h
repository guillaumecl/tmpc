#ifndef MPDPP_TAG_H
#define MPDPP_TAG_H

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

tag tag_from_string(const char *string);

const char * tag_to_string(tag tag);

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
