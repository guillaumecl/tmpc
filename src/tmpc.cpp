#include <exception>
#include <string>
#include <string.h>

#include <iostream>

#include <mpd++/mpd.h>

#include <QApplication>
#include <QLabel>

#include <QMessageBox>

#include "search_queue_widget.h"
#include "display_widget.h"
#include "main_window.h"
#include "application.h"
#include "optionparser.h"

enum optionIndex { UNKNOWN, CURRENT, HOST, PORT };
const option::Descriptor usage[] =
{
	{UNKNOWN, 0, "", "",option::Arg::None, "USAGE: tmpc [options]\n\nOptions:" },
	{CURRENT, 0,"c", "current",option::Arg::None, "  --current, -c  \tPrint information about the current song." },
	{HOST, 0,"h","host",option::Arg::Optional, "  --host, -h host \tHost to use." },
	{PORT, 0,"p","port",option::Arg::Optional, "  --port, -p port \tPort to use." },
	{0,0,0,0,0,0}
};

mpdpp::mpd connect(int argc, char **argv, bool& display)
{
	QString message;
	--argc;
	++argv;
	option::Stats  stats(usage, argc, argv);
	option::Option *options = new option::Option[stats.options_max];
	option::Option *buffer = new option::Option[stats.buffer_max];
	option::Parser parse(usage, argc, argv, options, buffer);

	try
	{
		const char *host = nullptr;
		int port = 0;

		if (parse.error() || options[UNKNOWN])
		{
			for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
				std::cout << "Unknown option: " << std::string(opt->name,opt->namelen) << "\n";

			option::printUsage(std::cerr, usage);
			delete[] options;
			delete[] buffer;
			exit(1);
			return mpdpp::mpd();
		}

		if (options[HOST])
		{
			host = options[HOST].arg;
		}
		if (options[PORT] and options[PORT].arg)
		{
			port = std::atoi(options[PORT].arg);
		}
		display = options[CURRENT];

		delete[] options;
		delete[] buffer;

		return mpdpp::mpd(host, port);
	}
	catch(const std::exception & e)
	{
		message = QObject::tr(e.what());
	}
	catch(...)
	{
		message = QObject::tr("Unknown exception while connecting to mpd");
	}

	delete[] options;
	delete[] buffer;

	QMessageBox::critical(nullptr, QObject::tr("tmpc"), message);
	exit(2);
	return mpdpp::mpd();
}

int main(int argc, char **argv)
{
	tmpc::application app(argc, argv);
	Q_INIT_RESOURCE(tmpc);

    QCoreApplication::setOrganizationName("baobob");
    QCoreApplication::setApplicationName("tmpc");
	QWidget *widget;
	bool display;
	mpdpp::mpd mpd = connect(argc, argv, display);

	if (display)
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
