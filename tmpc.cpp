#include <exception>
#include <string>
#include <string.h>

#include <iostream>

#include "mpd++/mpd.h"

#include <QApplication>
#include "main_window.h"



int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	tmpc::main_window *window = new tmpc::main_window;
	window->show();

	return app.exec();
}
