#ifndef MPDPP_EVENTS_H
#define MPDPP_EVENTS_H

namespace mpdpp
{

enum event
{
	database = 0x1,
	playlist = 0x2,
	queue = 0x4,
	player = 0x8,
	mixer = 0x10,
	output = 0x20,
	options = 0x40,
	update = 0x80,
	sticker = 0x100,
	subscription = 0x200,
	message = 0x400,
};

}

#endif //MPDPP_EVENTS_H
