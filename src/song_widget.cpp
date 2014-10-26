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

#include "song_widget.h"
#include <mpd++/mpd.h>

#include "song_model.h"

#include <QKeyEvent>
#include <sstream>

using namespace tmpc;

song_widget::song_widget(song_model *model, QWidget *parent)
	: QTreeView(parent),
	  model_(model)
{
	setModel(model);
	setRootIsDecorated(false);

	setColumnWidth(0, 400);
}

void song_widget::mouseDoubleClickEvent(QMouseEvent *event)
{
	const song_storage& song = selection();
	if (not song.valid())
		return QTreeView::mouseDoubleClickEvent(event);

	if (not song.queued)
		emit play_song(song.uri);
	else
		emit play_song(song.id);
}

void song_widget::keyPressEvent(QKeyEvent *event)
{
	const song_storage& song = selection();
	if (not song.valid())
		return QTreeView::keyPressEvent(event);

	switch(event->key())
	{
	case Qt::Key_Return:
		if (not song.queued)
			emit play_song(song.uri);
		else
			emit play_song(song.id);
		break;
	case Qt::Key_Space:
		if (not song.queued)
			emit queue_song(song.uri);
		break;
	case Qt::Key_Plus:
		if (song.queued)
			emit priority_increased(song.id, song.priority);
		else
			emit queue_song(song.uri);
		break;
	case Qt::Key_Minus:
		if (song.queued)
			emit priority_decreased(song.id, song.priority);
		break;
	case Qt::Key_Delete:
		if (song.queued)
			emit remove_song(song.id);
		break;
	default:
		return QTreeView::keyPressEvent(event);
	}
	event->accept();
}

song_storage& song_widget::selection()
{
	return model_->item_at(currentIndex().row());
}

const song_storage& song_widget::selection() const
{
	return model_->item_at(currentIndex().row());
}
