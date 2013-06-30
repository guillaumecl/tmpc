#include "tag.h"

#include <mpd/client.h>


namespace mpdpp
{


tag tag_from_string(const char *string)
{
	return static_cast<tag>(mpd_tag_name_iparse(string));
}

const char * tag_to_string(tag tag)
{
	return mpd_tag_name(static_cast<mpd_tag_type>(tag));
}


}
