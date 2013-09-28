 #include "application.h"

#include <exception>

#include "mpd++/exception.h"
#include <QMessageBox>

#include <QTranslator>
#include <QLocale>

using namespace tmpc;

application::application(int &argc, char **argv) :
	QApplication(argc, argv)
{
	QTranslator *translator = new QTranslator(this);
	translator->load(QLocale::system(), "tmpc", "_");
	installTranslator(translator);
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
		QMessageBox::critical(nullptr, tr("tmpc"), e.what());
	}
	catch(const std::exception &e)
	{
		qDebug("%s", e.what());
		QMessageBox::critical(nullptr, "tmpc", e.what());
	}
	return false;
}
