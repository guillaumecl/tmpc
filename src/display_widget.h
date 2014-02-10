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

#ifndef TMPC_DISPLAY_WIDGET_H
#define TMPC_DISPLAY_WIDGET_H

#include <QLabel>

#include <mpd++/mpd.h>

class QSlider;
class QTimer;

namespace tmpc
{

/**
 * Displays the content of the current song.
 */
class display_widget: public QWidget
{
	Q_OBJECT
public:
	/**
	 * Creates a display widget mith the specified mpd connection.
	 * @param mpd: the mpd connection.
	 */
	display_widget(mpdpp::mpd& mpd);

private slots:
	void display(mpdpp::song_ptr song);
	void poll();
	void seek(int position);

signals:
	/**
	 * Sent when this widget changed some data and potentially needs some resizing.
	 */
	void needResize();

	/**
	 * Sent when a user action should quit the application.
	 */
	void quit();

protected:
	/**
	 * Reimplement showEvent from QWidget.
	 * @param event the event to process.
	 */
	virtual void showEvent(QShowEvent *event) override;

	/**
	 * Reimplement hideEvent from QWidget.
	 * @param event the event to process.
	 */
	virtual void hideEvent(QHideEvent *event) override;


private:
	QLabel *tags_;
	QLabel *title_;
	QSlider *slider_;
	QTimer *timer_;

	int id_;

	mpdpp::mpd &mpd_;
};

}

#endif //TMPC_DISPLAY_WIDGET_H
