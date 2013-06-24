#ifndef MPDPP_EXCEPTION_H
#define MPDPP_EXCEPTION_H

#include <exception>
#include <string>


namespace mpdpp
{

class mpd;

class exception : public std::exception
{
public:
	exception(const char *message);

	~exception() throw()
	{
	}

	virtual const char * what() const noexcept { return what_.c_str(); }

private:
	std::string what_;
};



}


#endif
