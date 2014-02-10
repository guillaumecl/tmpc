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

#ifndef TMPC_MAIN_WINDOW_H
#define TMPC_MAIN_WINDOW_H

#include <QMainWindow>

#include <mpd++/mpd.h>

namespace tmpc
{

/**
 * The main window of tmpc.
 *
 * Basically, this is just here to show a widget on screen at an absolute position.
 */
class main_window: public QMainWindow
{
	Q_OBJECT
public:
	/**
	 * Creates a main window.
	 * @param mpd the underlying mpd connection
	 * @param centralWidget the widget to display.
	 */
	main_window(mpdpp::mpd& mpd, QWidget *centralWidget);

public slots:
	/**
	 * Resize the window and move it to the proper place.
	 * @todo maybe rename this since this does not match what the documentation says.
	 */
	void resizeToFit();

protected:
	/**
	 * Respond to a keyPressEvent.
	 */
	void keyPressEvent(QKeyEvent *event);

private:
	QWidget *main_widget_;
	mpdpp::mpd& mpd_;
};


}

#endif
