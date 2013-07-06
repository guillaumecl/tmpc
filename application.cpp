 #include "application.h"

#include <exception>

#include "mpd++/exception.h"
#include <QMessageBox>

using namespace tmpc;

application::application(int &argc, char **argv) :
	QApplication(argc, argv)
{
}

bool application::notify(QObject *receiver, QEvent *event)
{
	try
	{
		return QApplication::notify(receiver, event);
	}
	catch(const mpdpp::exception &e)
	{
		qDebug("%s", e.what());
		QMessageBox::critical(nullptr, "tmpc", e.what());
	}
	catch(const std::exception &e)
	{
		qDebug("%s", e.what());
		QMessageBox::critical(nullptr, "tmpc", e.what());
	}
	catch(...)
	{
		qDebug("erreur");
		std::terminate();
	}
	return false;
}
