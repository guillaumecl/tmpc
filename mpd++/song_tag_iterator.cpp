#include "song_tag_iterator.h"
#include "song.h"

using namespace mpdpp;

void song_tag_iterator::find_next()
{
	while (iter_.valid() and not (tag_ = song_->tag(*iter_)))
	{
		++iter_;
	}
}
