#include "song_model.h"

#include <mpd++/mpd.h>

using namespace tmpc;

enum columns
{
	Title,
	Artist,
	Album,
	Priority
};

song_model::song_model(QObject *parent) :
	QAbstractTableModel(parent),
	queue_icon_(":/icons/song"),
	db_icon_(":/icons/db")
{
	invalid_song_.index = -1;
}

QVariant song_model::data(const QModelIndex &index, int role) const
{
	if (index.row() < 0 or index.row() >= songs_.size())
		return QVariant();

	const song_storage& song = songs_[index.row()];
	if (role == Qt::DecorationRole)
	{
		if (index.column() == Title)
			return song.queued ? queue_icon_ : db_icon_;
		return QVariant();
	}
	else if (role != Qt::DisplayRole)
	{
		return QVariant();
	}

	switch(index.column())
	{
	case columns::Title:
		return song.title;
	case columns::Album:
		return song.album;
	case columns::Artist:
		return song.artist;
	case columns::Priority:
		return QString("%1").arg(song.priority);
	}
	return QVariant();
}

QVariant song_model::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical or role != Qt::DisplayRole)
		return QVariant();
	switch(section)
	{
	case columns::Title:
		return tr("Title");
	case columns::Artist:
		return tr("Artist");
	case columns::Album:
		return tr("Album");
	case columns::Priority:
		return tr("Priority");
	}
	return QVariant();
}

int song_model::columnCount(const QModelIndex &/*parent*/) const
{
	return 3;
}

int song_model::rowCount(const QModelIndex &/*parent*/) const
{
	return songs_.size();
}

void song_model::add_song(const mpdpp::song& song)
{
	const char *title = song.tag(mpdpp::tag::title);
	if (!title)
		title = song.uri();
	bool queued = song.queued();
	songs_.append({
			songs_.size(),
			song.id(),
			song.priority(),
			title,
			song.tag(mpdpp::tag::album),
			song.tag(mpdpp::tag::artist),
			song.uri(),
			queued});
}

void song_model::update_song(int position, const mpdpp::song& song)
{
	song_storage& s = item_at(position);
	if (not s.valid())
		return;

	s.id = song.id();
	s.priority = song.priority();
	s.queued = song.queued();

	emit dataChanged(index(position, 0), index(position, columns::Priority));
}


void song_model::fill(mpdpp::search& search)
{
	beginResetModel();
	songs_.clear();

	for (const mpdpp::song& s:search)
		add_song(s);

	endResetModel();
}

void song_model::fill(mpdpp::search&& search)
{
	// this is not an infinite loop, search is a lvalue so the above will be called.
	fill(search);
}

void song_model::clear()
{
	beginResetModel();
	songs_.clear();
	endResetModel();
}

const song_storage& song_model::item_at(int position) const
{
	if (position < 0 or position >= rowCount())
		return invalid_song_;

	return songs_[position];
}

song_storage& song_model::item_at(int position)
{
	if (position < 0 or position >= rowCount())
		return invalid_song_;

	return songs_[position];
}
