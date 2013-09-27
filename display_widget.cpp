#include "display_widget.h"
#include <sstream>

#include <QTimer>

#include <mpd/client.h>


using namespace tmpc;

display_widget::display_widget(mpdpp::mpd& mpd) :
	mpd_(mpd)
{
	display(mpd_.current_song());

	mpd_.monitor(mpdpp::event::player);
	QTimer *timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()),
			this, SLOT(poll()));

	timer->start(250);
}


void display_widget::display(mpdpp::song_ptr song)
{
	if(!song)
	{
		return;
	}
	std::ostringstream str;
	str << "<center><h1>" << *song << "</h1></center>";

	bool first = true;
	for (const auto &pair : song->tags())
	{
		if (first)
		{
			first = false;
			str << "<hr />";
		}
		else
		{
			str << "<br />";
		}
		str << "<b>" << tag_to_string(pair.first) << "</b>: ";
		str << pair.second;
	}

	setText(QString::fromUtf8(str.str().c_str()));
	emit needResize();
}


void display_widget::poll()
{
	if (mpd_.stop_monitor())
	{
		display(mpd_.current_song());
	}
	mpd_.monitor(mpdpp::event::player);
}
