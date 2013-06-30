#include <exception>
#include <string>
#include <string.h>

#include <iostream>

#include "mpd++/mpd.h"

#include <QApplication>
#include <QLabel>

#include <sstream>

#include "search_queue_widget.h"
#include "main_window.h"


int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	Q_INIT_RESOURCE(tmpc);

	QStringList arguments = QApplication::arguments();
	mpdpp::mpd mpd;

	if (arguments.contains("--current"))
	{
		mpdpp::song_ptr current = mpd.current_song();
		if(!current)
		{
			return 0;
		}
		std::ostringstream str;
		str << *current;

		QLabel *label = new QLabel(QString::fromUtf8(str.str().c_str()));
		tmpc::main_window *window = new tmpc::main_window(mpd, label);

		window->show();
	}
	else
	{
		tmpc::search_queue_widget *song_widget = new tmpc::search_queue_widget(mpd);
		tmpc::main_window *window = new tmpc::main_window(mpd, song_widget);

		window->connect(song_widget, SIGNAL(quit()), SLOT(close()));
		window->connect(song_widget, SIGNAL(needResize()), SLOT(resizeToFit()));


		window->show();
	}



	return app.exec();
}
