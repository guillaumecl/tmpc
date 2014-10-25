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

#ifndef TMPC_SONG_WIDGET_H
#define TMPC_SONG_WIDGET_H

#include <QTreeView>
#include <QTreeWidgetItem>
#include <QMetaType>
#include <QIcon>

#include <mpd++/forward.h>

namespace tmpc
{

class song_model;
struct song_storage;

/**
 * A list widget that shows songs and can respond to play events.
 */
class song_widget: public QTreeView
{
	Q_OBJECT
public:
	/**
	 * the item type. Use this instead of QTreeWidgetItem in case the class changes later.
	 */
	typedef QTreeWidgetItem item_type;

	/**
	 * Creates a song widget.
	 * @param parent the parent widget.
	 */
	song_widget(song_model *model, QWidget *parent = 0);

public slots:
signals:
	/**
	 * Emitted when a song is selected.
	 */
	void play_song(unsigned int song_id);

	/**
	 * Emitted when a song is selected.
	 */
	void play_song(QString song_uri);

	/**
	 * Emitted when a song is selected.
	 */
	void queue_song(QString song_uri);

	/**
	 * Increase the priority of the specified song.
	 */
	void priority_increased(unsigned int song_id, int priority);

	/**
	 * Decrease the priority of the specified song.
	 */
	void priority_decreased(unsigned int song_id, int priority);

	/**
	 * Remove the specified song from the queue.
	 */
	void remove_song(unsigned int song_id);

private slots:
	void item_double_clicked();

protected:
	/**
	 * Respond to a key event.
	 */
	virtual void keyPressEvent(QKeyEvent *event);

private:
	const song_storage& selection() const;
	song_storage& selection();

	song_model *model_;
};

}

Q_DECLARE_METATYPE(std::shared_ptr<mpdpp::song>)


#endif
