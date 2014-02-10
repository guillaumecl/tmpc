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

#include "application.h"

#include <exception>

#include <mpd++/exception.h>
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
