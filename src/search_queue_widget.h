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

#ifndef TMPC_SEARCH_QUEUE_WIDGET_H
#define TMPC_SEARCH_QUEUE_WIDGET_H

#include <QMainWindow>
#include <QListWidget>

#include <mpd++/mpd.h>

namespace tmpc
{

class song_model;
class song_widget;
class history_line_edit;
class display_widget;

/**
 * Widget that shows the result of a search.
 */
class search_queue_widget: public QWidget
{
	Q_OBJECT
public:
	/**
	 * Constructs a search_queue_widget from an mpd connection.
	 * @param mpd the underlying mpd connection.
	 */
	search_queue_widget(mpdpp::mpd& mpd);

	/**
	 * Redefines the size hint so that the main window resizes accordingly.
	 */
	virtual QSize sizeHint() const;

	/**
	 * true if this is a search concerning the queue.
	 */
	bool queue_search() const;
protected:
	/**
	 * Respond to a keyPressEvent.
	 */
	virtual void keyPressEvent(QKeyEvent *event);

private slots:
	/**
	 * Searches the specified string.
	 * @param str the string to search. if "@", returns the whole queue. If this begins with !, searches the databse.
	 * Searches by tags may be specified with colons like this: "artist:Red Hot". Several constraints can be added
	 * by adding commas between requests, like this: "artist:Red Hot,album:live".
	 */
	void search(const QString& str);

	/**
	 * Plays the specified song.
	 * @param song a pointer to the song to play.
	 */
	void play(unsigned int id);

	/**
	 * Plays the specified song.
	 * @param song a pointer to the song to play.
	 */
	void play(QString uri);

	/**
	 * Increase the priority of the specified song.
	 * @param song a pointer to the song to modify.
	 */
	void increase_priority(unsigned int song_id, int priority);

	/**
	 * Decrease the priority of the specified song.
	 * @param song a pointer to the song to modify.
	 */
	void decrease_priority(unsigned int song_id, int priority);

	/**
	 * Removes a song from the queue.
	 * @param song a pointer to the song to delete.
	 */
	void remove_song(unsigned int id);

	/**
	 * Inserts a song from the queue.
	 * @param song a pointer to the song to insert. Change the contents to reflect the position in the queue.
	 */
	void insert_song(QString uri);

signals:
	/**
	 * Emitted when this widget size has changed. The parent may respond to this by resizing itself accordingly.
	 */
	void needResize();

	/**
	 * Emitted when a song has been selected and the application should terminate.
	 */
	void quit();

private:
	/**
	 * Creates the proper search type for the current request.
	 */
	mpdpp::search search_type();

	/**
	 * Builds a search from the specified terms.
	 * Takes a rvalue reference, which allows to do something like :
	 *
	 * build_search(term, mpd.search_queue());
	 * @param search_terms the string to search.
	 * @param search the search used.
	 * @see search_queue_widget::search_type()
	 * @see search_queue_widget::search(const QString&)
	 */
	mpdpp::search& build_search(const QString& search_terms, mpdpp::search &&search);

	/**
	 * Builds a search from the specified terms.
	 *
	 * @param search_terms the string to search.
	 * @param search the search used.
	 * @see search_queue_widget::search_type()
	 * @see search_queue_widget::search(const QString&)
	 */
	mpdpp::search& build_search(const QString& search_terms, mpdpp::search &search);

	mpdpp::mpd &mpd_;
	history_line_edit *text_;
	song_model *model_;
	song_widget *list_;
	display_widget *display_;
};

}

#endif
