/******************************************************************************

Copyright (C) 2013  CLÉMENT Guillaume <gclement@baobob.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

******************************************************************************/

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
const option::Descriptor usage[] = {
	{UNKNOWN, 0, "", "",option::Arg::None, "USAGE: tmpc [options]\n\nOptions:" },
	{CURRENT, 0,"c", "current",option::Arg::None, "  --current, -c    Print information about the current song." },
	{HOST, 0,"h","host",option::Arg::Optional,    "  --host, -h host  Host to use." },
	{PORT, 0,"p","port",option::Arg::Optional,    "  --port, -p port  Port to use." },
	{0,0,0,0,0,0}
};

mpdpp::mpd connect(int argc, char **argv, bool& display)
{
	QString message;
	--argc;
	++argv;
	option::Stats  stats(usage, argc, argv);
	option::Option options[stats.options_max];
	option::Option buffer[stats.buffer_max];
	option::Parser parse(usage, argc, argv, options, buffer);

	try {
		const char *host = nullptr;
		int port = 0;

		if (parse.error() || options[UNKNOWN]) {
			for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
				std::cout << "Unknown option: " << std::string(opt->name,opt->namelen) << "\n";

			option::printUsage(std::cerr, usage);
			exit(1);
			return mpdpp::mpd();
		}

		if (options[HOST])
			host = options[HOST].arg;

		if (options[PORT] and options[PORT].arg)
			port = std::atoi(options[PORT].arg);

		display = options[CURRENT];

		return mpdpp::mpd(host, port);
	} catch(const std::exception & e) {
		message = QObject::tr(e.what());
	} catch(...) {
		message = QObject::tr("Unknown exception while connecting to mpd");
	}

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
		widget = new tmpc::display_widget(mpd);
	else
		widget = new tmpc::search_queue_widget(mpd);

	tmpc::main_window *window = new tmpc::main_window(mpd, widget);
	window->connect(widget, SIGNAL(quit()), SLOT(close()));
	window->connect(widget, SIGNAL(needResize()), SLOT(resizeToFit()));

	return app.exec();
}
