#include "song_widget.h"
#include "mpd++/mpd.h"


#include <QKeyEvent>
#include <sstream>

using namespace tmpc;

song_widget::song_widget(QWidget *parent)
	: QTreeWidget(parent),
	  queue_icon_(":/icons/song"),
	  db_icon_(":/icons/db")
{
	QStringList labels;

	qRegisterMetaType<mpdpp::song_ptr>("mpdpp::song_ptr");
	setRootIsDecorated(false);
	setColumnCount(3);

	labels << tr("Title") << tr("Artist") << tr("Album") << tr("Priority");
	setHeaderLabels(labels);

	setColumnWidth(0, 400);

	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
			this, SLOT(item_double_clicked()));
}

void song_widget::add_song(mpdpp::song_ptr song)
{
	item_type *item = new item_type(this);

	if (song->queued())
	{
		item->setIcon(0, queue_icon_);
		item->setText(3, QString::number(song->priority()));
	}
	else
	{
		item->setIcon(0, db_icon_);
	}

	const char *title = song->tag(mpdpp::tag::title);
	if (title)
	{
		item->setText(0, title);
	}
	else
	{
		item->setText(0, song->uri());
	}
	item->setText(1, song->tag(mpdpp::tag::artist));
	item->setText(2, song->tag(mpdpp::tag::album));
	item->setData(Qt::UserRole, 0, QVariant::fromValue(song));
}

void song_widget::fill(mpdpp::search& search)
{
	clear();
	setSortingEnabled(false);
	if (search.queue_search())
	{
		setColumnCount(4);
	}
	else
	{
		setColumnCount(3);
	}
	for (auto it = search.begin(); it != search.end(); ++it)
	{
		add_song(it.steal_ptr());
	}
	if (search.queue_search())
	{
		sortItems(3, Qt::DescendingOrder);
	}
	else
	{
		sortItems(0, Qt::AscendingOrder);
	}
	setSortingEnabled(true);
}

void song_widget::fill(mpdpp::search&& search)
{
	// this is not an infinite loop, search is a lvalue so the above will be called.
	fill(search);
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

void song_widget::item_double_clicked()
{
	mpdpp::song_ptr song = selection();
	if (song)
	{
		emit song_selected(song);
	}
}


void song_widget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return)
	{
		mpdpp::song_ptr song = selection();
		if (song)
		{
			event->accept();
			emit song_selected(song);
			return;
		}
	}
	else if (event->key() == Qt::Key_Plus)
	{
		mpdpp::song_ptr song = selection();
		if (song and song->queued())
		{
			event->accept();
			emit priority_increased(song);
			currentItem()->setText(3, QString::number(song->priority()));
			return;
		}
	}
	else if (event->key() == Qt::Key_Minus)
	{
		mpdpp::song_ptr song = selection();
		if (song and song->queued())
		{
			event->accept();
			emit priority_decreased(song);
			currentItem()->setText(3, QString::number(song->priority()));
			return;
		}
	}
	else if (event->key() == Qt::Key_Delete)
	{
		mpdpp::song_ptr song = selection();
		if (song and song->queued())
		{
			event->accept();
			emit song_removed(song);
			QModelIndex selected = currentIndex();
			model()->removeRow(selected.row(), selected.parent());
			return;
		}
	}
	QTreeWidget::keyPressEvent(event);
}
