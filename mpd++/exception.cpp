#include "exception.h"
#include "mpd.h"

using namespace mpdpp;

exception::exception(const char *message) :
	what_(message)
{
}
