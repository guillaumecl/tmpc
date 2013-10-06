#include "song_widget.h"
#include "mpd++/mpd.h"


#include <QKeyEvent>
#include <sstream>

using namespace tmpc;

song_widget::song_widget(QWidget *parent)
	: QTreeWidget(parent),
	  queue_icon_(":/icons/song"),
	  db_icon_(":/icons/db"),
	  queue_fed_(false)
{
	QStringList labels;

	qRegisterMetaType<mpdpp::song_ptr>("mpdpp::song_ptr");
	setRootIsDecorated(false);

	setColumnCount(4);

	labels << tr("Title") << tr("Artist") << tr("Album") << tr("Priority");
	setHeaderLabels(labels);

	setColumnWidth(0, 400);

	hideColumn(3);

	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
			this, SLOT(item_double_clicked()));
}

void song_widget::add_song(mpdpp::song_ptr song)
{
	if (song->queued())
	{
		queue_.insert(song->uri());
	}

	item_type *item = new item_type(this);

	if (song->queued())
	{
		item->setIcon(0, queue_icon_);
		item->setData(3, Qt::DisplayRole, song->priority());
	}
	else if (queue_.contains(song->uri()))
	{
		item->setIcon(0, queue_icon_);
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
		if (not song->queued())
		{
			currentItem()->setData(0, Qt::DecorationRole, queue_icon_);
			queue_.insert(song->uri());
		}
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
			if (not song->queued())
			{
				currentItem()->setData(0, Qt::DecorationRole, queue_icon_);
				queue_.insert(song->uri());
			}
			emit song_selected(song);
			return;
		}
	}
	else if (event->key() == Qt::Key_Space)
	{
		mpdpp::song_ptr song = selection();
		if (song)
		{
			event->accept();
			if (not song->queued())
			{
				currentItem()->setData(0, Qt::DecorationRole, queue_icon_);
				queue_.insert(song->uri());
				emit song_inserted(song);
			}
		}
	}
	else if (event->key() == Qt::Key_Plus)
	{
		mpdpp::song_ptr song = selection();
		if (song)
		{
			event->accept();
			if (song->queued())
			{
				emit priority_increased(song);
				currentItem()->setData(3, Qt::DisplayRole, song->priority());
			}
			else
			{
				currentItem()->setData(0, Qt::DecorationRole, queue_icon_);
				queue_.insert(song->uri());
				emit song_inserted(song);
			}
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
			currentItem()->setData(3, Qt::DisplayRole, song->priority());
			return;
		}
	}
	else if (event->key() == Qt::Key_Delete)
	{
		mpdpp::song_ptr song = selection();
		if (song and song->queued())
		{
			event->accept();
			queue_.remove(song->uri());
			emit song_removed(song);
			QModelIndex selected = currentIndex();
			model()->removeRow(selected.row(), selected.parent());
			return;
		}
	}
	QTreeWidget::keyPressEvent(event);
}

void song_widget::feed_queue(mpdpp::search&& search)
{
	feed_queue(search);
}

void song_widget::feed_queue(mpdpp::search& search)
{
	queue_.clear();
	for (const mpdpp::song& song : search)
	{
		queue_.insert(song.uri());
	}
	queue_fed_ = true;

	if (queue_.empty() and topLevelItemCount() > 0)
	{
		for (int i = 0; i < topLevelItemCount() ; ++i)
		{
			QTreeWidgetItem *item = topLevelItem(i);
			item->setData(0, Qt::DecorationRole, db_icon_);
		}
	}
}

bool song_widget::queue_fed() const
{
	return queue_fed_;
}
