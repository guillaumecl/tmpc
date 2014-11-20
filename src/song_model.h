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

#ifndef SONG_MODEL_H
#define SONG_MODEL_H

#include <mpd++/forward.h>

#include <QAbstractTableModel>
#include <QList>
#include <QIcon>


namespace tmpc
{


struct song_storage
{
	int index;

	unsigned int id;
	unsigned int priority;

	QString title;
	QString album;
	QString artist;
	QString uri;

	bool queued;

	bool valid() const { return index >= 0; }
};

class song_model: public QAbstractTableModel
{
	Q_OBJECT
public:
	song_model(QObject *parent);


	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

	const song_storage& item_at(int position) const;
	song_storage& item_at(int position);

	void update_song(int position, const mpdpp::song& song);
	void delete_song(int position);

public slots:
	/**
	 * Fills the list from a search.
	 */
	void fill(mpdpp::search& search);

	/**
	 * Fills the list from a search.
	 */
	void fill(mpdpp::search&& search);

	void clear();

private:
	void add_song(const mpdpp::song& song);

	QList<song_storage> songs_;
	song_storage invalid_song_;

	QIcon queue_icon_;
	QIcon db_icon_;
};

}


#endif
