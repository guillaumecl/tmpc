 #include "application.h"

#include <exception>

#include "mpd++/exception.h"
#include <QMessageBox>

#include <QTranslator>
#include <QLocale>

#include <QTextCodec>

using namespace tmpc;

application::application(int &argc, char **argv) :
	QApplication(argc, argv)
{
	QTranslator *translator = new QTranslator(this);
	translator->load(QLocale::system(), "tmpc", "_");
	installTranslator(translator);

	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}

bool application::notify(QObject *receiver, QEvent *event)
{
	try
	{
		return QApplication::notify(receiver, event);
	}
	catch(const mpdpp::exception &e)
	{
		QMessageBox::critical(nullptr, tr("tmpc"), e.what());
	}
	catch(const std::exception &e)
	{
		QMessageBox::critical(nullptr, tr("tmpc"), e.what());
	}
	return false;
}
