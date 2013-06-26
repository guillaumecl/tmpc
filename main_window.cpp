#include "main_window.h"

#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QVBoxLayout>

#include <QKeyEvent>

#include <sstream>

namespace tmpc
{

main_window::main_window() :
	QMainWindow(nullptr, Qt::Tool | Qt::WindowStaysOnTopHint)
{
	setAttribute(Qt::WA_QuitOnClose);
	setAttribute(Qt::WA_DeleteOnClose);

    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry(desktop->screenNumber(QCursor::pos()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);


	text_ = new QLineEdit;
	text_->setFrame(false);
	layout->addWidget(text_);

	text_->setMinimumWidth(screen.width() / 2);

	list_ = new QListWidget;
	list_->setVisible(false);
	layout->addWidget(list_);

	setCentralWidget(new QWidget);
	centralWidget()->setLayout(layout);

	resize(text_->sizeHint());
	move((screen.width() - text_->width()) / 2, (screen.height() - height()) / 4);

	connect(text_, SIGNAL(textChanged(const QString&)), this, SLOT(search(const QString&)));
}

void main_window::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		event->accept();
		close();
	}
	else if (event->key() == Qt::Key_Return
			 and list_->currentItem())
	{
		unsigned int id = list_->currentItem()->data(Qt::UserRole).toInt();

		mpd_.play(id);
		qDebug("Playing song %d", id);
		event->accept();
		close();
	}
	else if (event->key() == Qt::Key_Up
			 or event->key() == Qt::Key_Down
			 or event->key() == Qt::Key_PageUp
			 or event->key() == Qt::Key_PageDown)
	{
		list_->setFocus();
		QMainWindow::keyPressEvent(event);
	}
	else
	{
		text_->setFocus();
		QMainWindow::keyPressEvent(event);
	}
}

void main_window::search(const QString& str)
{
	list_->clear();
	if (str.size() > 2)
	{
		mpd_.search_queue() << mpdpp::any_tag_contains(str.toLatin1());

		for (mpdpp::song_ptr song : mpd_.commit_search())
		{
			std::ostringstream stream;
			stream << *song;
			QListWidgetItem *item = new QListWidgetItem(QString::fromUtf8(stream.str().c_str()), list_);

			item->setData(Qt::UserRole, song->id());
		}
	}
	if (list_->count() > 0)
	{
		list_->setVisible(true);
		list_->sortItems();
		QSize textHint = text_->sizeHint();
		QSize listHint = list_->sizeHint();

		QSize hint(qMax(textHint.width(), listHint.width()), textHint.height() + listHint.height());

		resize(hint);
	}
	else
	{
		list_->setVisible(false);
		resize(text_->sizeHint());
	}

    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry(desktop->screenNumber(QCursor::pos()));
	move(screen.x() + (screen.width() - width()) / 2, screen.y() + (screen.height() - height()) / 4);
}

}
