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

#ifndef MPDPP_EXCEPTION_H
#define MPDPP_EXCEPTION_H

#include <exception>
#include <string>


namespace mpdpp
{

/**
 * Base class for exceptions thrown by mpd++.
 *
 * These can be thrown after most operations that request a network transfer.
 */
class exception : public std::exception
{
public:
	/**
	 * Build an exception from the specified message.
	 */
	exception(const char *message);

	~exception() throw()
	{
	}

	/**
	 * Message.
	 */
	virtual const char * what() const noexcept { return what_.c_str(); }

private:
	/**
	 * Message of the error.
	 */
	std::string what_;
};



}


#endif
