#include "search_queue_window.h"
#include "song_widget.h"

#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QVBoxLayout>

#include <QKeyEvent>

#include <sstream>

namespace tmpc
{

search_queue_window::search_queue_window() :
	QMainWindow(nullptr, Qt::Tool | Qt::WindowStaysOnTopHint)
{
	setAttribute(Qt::WA_QuitOnClose);

    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry(desktop->screenNumber(QCursor::pos()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);


	text_ = new QLineEdit;
	text_->setFrame(false);
	layout->addWidget(text_);

	text_->setMinimumWidth(screen.width() / 2);

	list_ = new song_widget;
	layout->addWidget(list_);

	setCentralWidget(new QWidget);
	centralWidget()->setLayout(layout);

	resize(text_->sizeHint());
	move((screen.width() - text_->width()) / 2, (screen.height() - height()) / 4);

	connect(text_, SIGNAL(textChanged(const QString&)), this, SLOT(search(const QString&)));
}

void search_queue_window::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		event->accept();
		close();
	}
	else if (event->key() == Qt::Key_Return
			 and list_->selection())
	{
		mpd_.play(*list_->selection());
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

void search_queue_window::search(const QString& str)
{
	list_->clear();

	mpdpp::search search = mpd_.search_queue(false);

    for(QString const& it : str.split(','))
    {
        if (it.size() >= 2)
        {
            search << mpdpp::any_tag_contains(it.toUtf8());
        }
    }

	list_->fill(search);

	QSize textHint = text_->sizeHint();
	QSize listHint = list_->sizeHint();

	QSize hint(qMax(textHint.width(), listHint.width()), textHint.height() + listHint.height());

	resize(hint);

    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry(desktop->screenNumber(QCursor::pos()));
	move(screen.x() + (screen.width() - width()) / 2, screen.y() + (screen.height() - height()) / 4);
}

}
