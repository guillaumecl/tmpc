#include "search_queue_widget.h"
#include "song_widget.h"

#include <QLineEdit>
#include <QVBoxLayout>

#include <QKeyEvent>

#include <sstream>

namespace tmpc
{

search_queue_widget::search_queue_widget(mpdpp::mpd& mpd) :
	mpd_(mpd)
{
	setAttribute(Qt::WA_QuitOnClose);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);


	text_ = new QLineEdit;
	text_->setFrame(false);
	layout->addWidget(text_);

//	text_->setMinimumWidth(screen.width() / 2);

	list_ = new song_widget;
	layout->addWidget(list_);

	connect(text_, SIGNAL(textChanged(const QString&)), this, SLOT(search(const QString&)));
}

void search_queue_widget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return
			 and list_->selection())
	{
		mpd_.play(*list_->selection());
		event->accept();
		emit quit();
	}
	else if (event->key() == Qt::Key_Up
			 or event->key() == Qt::Key_Down
			 or event->key() == Qt::Key_PageUp
			 or event->key() == Qt::Key_PageDown)
	{
		list_->setFocus();
		QWidget::keyPressEvent(event);
	}
	else
	{
		text_->setFocus();
		QWidget::keyPressEvent(event);
	}
}

void search_queue_widget::search(const QString& str)
{
	list_->clear();

	mpdpp::search search = mpd_.search_queue();

    for(QString const& it : str.split(','))
    {
        if (it.size() >= 2)
        {
            search << mpdpp::any_tag_contains(it.toUtf8());
        }
    }

	list_->fill(search);

	emit needResize();
}

QSize search_queue_widget::sizeHint() const
{
	QSize textHint = text_->sizeHint();
	QSize listHint = list_->sizeHint();

	QSize size = QSize(qMax(textHint.width(), listHint.width()), textHint.height() + listHint.height());
	return size;
}

}
