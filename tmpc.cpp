#include <exception>
#include <string>
#include <string.h>

#include <iostream>

#include "mpd++/mpd.h"

#include <QApplication>
#include "search_queue_window.h"



int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	tmpc::search_queue_window window;
	window.show();

	return app.exec();
}
