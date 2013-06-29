#ifndef MPDPP_FORWARD_H
#define MPDPP_FORWARD_H

#include <memory>
#include <vector>

namespace mpdpp
{

class mpd;
class song;
class search;
enum class tag;
class exception;
class song_iterator;

typedef std::shared_ptr<song> song_ptr;
typedef std::vector<song_ptr> song_ptr_vector;

}

#endif
