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

#include "main_window.h"

#include <QApplication>
#include <QDesktopWidget>

#include <QKeyEvent>
#include <QResizeEvent>

#include <mpd++/mpd.h>

using namespace tmpc;

main_window::main_window(mpdpp::mpd& mpd, QWidget *centralWidget) :
	QMainWindow(nullptr, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint),
	mpd_(mpd),
	user_resized_(false)
{
	setAttribute(Qt::WA_QuitOnClose);
	setAttribute(Qt::WA_DeleteOnClose);

	setCentralWidget(centralWidget);
	resizeToFit();

	show();
}

void main_window::resizeToFit()
{
	if (user_resized_)
		return;

    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry(desktop->screenNumber(QCursor::pos()));

	QSize centralSize = centralWidget()->sizeHint();

	int width = qMax((4*screen.width()) / 5, centralSize.width());
	int height = centralSize.height();

	setGeometry(QRect(
					screen.left() + (screen.width() - width) / 2,
					screen.top() + (screen.height() - height) / 4,
				    width,
                    height));
}


void main_window::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		event->accept();
		close();
	}
	else if(event->modifiers() & Qt::ControlModifier)
	{
	}
	else
	{
		QMainWindow::keyPressEvent(event);
	}
}

void main_window::resizeEvent(QResizeEvent *event)
{
	if (event->spontaneous())
	{
		user_resized_ = true;
	}
	QMainWindow::resizeEvent(event);
}
