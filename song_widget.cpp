#include "song_widget.h"
#include "mpd++/mpd.h"

#include <sstream>

using namespace tmpc;

song_widget::song_widget(QWidget *parent)
	: QTreeWidget(parent)
{
	QStringList labels;

	qRegisterMetaType<mpdpp::song_ptr>("mpdpp::song_ptr");
	setRootIsDecorated(false);
	setColumnCount(3);

	labels << "Title" << "Artist" << "Album";
	setHeaderLabels(labels);

	setColumnWidth(0, 400);
}

void song_widget::fill(mpdpp::search& search)
{
	for (auto it = search.begin(); it != search.end(); ++it)
	{
		mpdpp::song_ptr song = it.steal_ptr();
		item_type *item = new item_type(this);

		const char *title = song->tag(mpdpp::tag::title);
		if (title)
		{
			item->setText(0, QString::fromUtf8(title));
		}
		else
		{
			item->setText(0, QString::fromUtf8(song->uri()));
		}
		item->setText(1, QString::fromUtf8(song->tag(mpdpp::tag::artist)));
		item->setText(2, QString::fromUtf8(song->tag(mpdpp::tag::album)));
		item->setData(Qt::UserRole, 0, QVariant::fromValue(song));
	}
	sortItems(0, Qt::AscendingOrder);
}

mpdpp::song_ptr song_widget::selection() const
{
	item_type* current_item = static_cast<item_type*>(currentItem());

	if (current_item)
	{
		return qvariant_cast<mpdpp::song_ptr>(current_item->data(Qt::UserRole, 0));
	}
	return nullptr;
}
