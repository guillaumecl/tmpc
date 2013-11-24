/******************************************************************************

Copyright (C) 2013  CLÉMENT Guillaume <gclement@baobob.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 2.1
of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

******************************************************************************/

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
