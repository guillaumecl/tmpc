#include "exception.h"
#include "mpd.h"

using namespace tmpc;

exception::exception(const char *message) :
	what_(message)
{
}
