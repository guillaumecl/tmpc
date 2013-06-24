#ifndef TMPC_EXCEPTION_H
#define TMPC_EXCEPTION_H

#include <exception>
#include <string>


namespace tmpc
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
