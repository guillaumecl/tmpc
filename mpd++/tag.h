#ifndef MPDPP_TAG_H
#define MPDPP_TAG_H

namespace mpdpp
{


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


struct tag_contains
{
	tag_contains(tag t, const char *v) :
		tag_(t),
		value_(v)
	{}

	tag tag_;
	const char *value_;
};

struct any_tag_contains
{
	any_tag_contains(const char *v) :
		value_(v)
	{}

	const char * value_;
};


}

#endif
