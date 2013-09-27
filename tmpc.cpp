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

	QWidget *widget;
	if (arguments.contains("--current"))
	{
		widget = new tmpc::display_widget(mpd);
	}
	else
	{
		widget = new tmpc::search_queue_widget(mpd);
	}

	tmpc::main_window *window = new tmpc::main_window(mpd, widget);
	window->connect(widget, SIGNAL(quit()), SLOT(close()));
	window->connect(widget, SIGNAL(needResize()), SLOT(resizeToFit()));

	return app.exec();
}
