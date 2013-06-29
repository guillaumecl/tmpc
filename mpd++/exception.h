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
