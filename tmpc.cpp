#include <exception>
#include <string>
#include <string.h>

#include <iostream>

#include "mpd++/mpd.h"

#include <QApplication>
#include <QLabel>

#include <QMessageBox>

#include "search_queue_widget.h"
#include "display_widget.h"
#include "main_window.h"
#include "application.h"

mpdpp::mpd connect()
{
	QString message;
	try
	{
		return mpdpp::mpd();
	}
	catch(const std::exception & e)
	{
		message = QString::fromUtf8(e.what());
	}
	catch(...)
	{
		message = "Unknown exception while connecting to mpd";
	}
	QMessageBox::critical(nullptr, "tmpc", message);
	std::terminate();
	return mpdpp::mpd();
}

int main(int argc, char **argv)
{
	tmpc::application app(argc, argv);
	Q_INIT_RESOURCE(tmpc);

    QCoreApplication::setOrganizationName("baobob");
    QCoreApplication::setApplicationName("tmpc");

	QStringList arguments = QApplication::arguments();
	mpdpp::mpd mpd = connect();

	tmpc::main_window *window;
	if (arguments.contains("--current"))
	{
		tmpc::display_widget *display = new tmpc::display_widget(mpd);
		window = new tmpc::main_window(mpd, display);
	}
	else
	{
		tmpc::search_queue_widget *song_widget = new tmpc::search_queue_widget(mpd);
		window = new tmpc::main_window(mpd, song_widget);

		window->connect(song_widget, SIGNAL(quit()), SLOT(close()));
		window->connect(song_widget, SIGNAL(needResize()), SLOT(resizeToFit()));
	}



	return app.exec();
}
