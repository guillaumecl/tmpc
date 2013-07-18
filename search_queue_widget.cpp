#include "search_queue_widget.h"
#include "song_widget.h"
#include "history_line_edit.h"

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

	text_ = new history_line_edit("search_queue_widget", this);
	text_->setFrame(false);
	layout->addWidget(text_);

	list_ = new song_widget(this);
	layout->addWidget(list_);

	connect(text_, SIGNAL(textChanged(const QString&)),
			this, SLOT(search(const QString&)));

	connect(list_, SIGNAL(song_selected(mpdpp::song_ptr)),
			this, SLOT(play(mpdpp::song_ptr)));
}

void search_queue_widget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Up
		or event->key() == Qt::Key_Down
		or event->key() == Qt::Key_PageUp
		or event->key() == Qt::Key_PageDown)
	{
		list_->setFocus();
		QWidget::keyPressEvent(event);
	}
	else if (event->key() == Qt::Key_O
			 and event->modifiers() & Qt::ControlModifier
			 and not queue_search())
	{
		QString search_term = text_->text();

		build_search(search_term, mpd_.add_from_db());

		// Remove the ! to make the list match with the content
		search_term.remove(0, 1);
		text_->setText(search_term);
	}
	else if (event->key() == Qt::Key_P
			 and event->modifiers() & Qt::ControlModifier)
	{
		mpd_.clear_queue();
		if (queue_search())
		{
			list_->clear();
		}
	}
	else
	{
		text_->setFocus();
		QWidget::keyPressEvent(event);
	}
}

mpdpp::search& search_queue_widget::build_search(const QString& search_terms, mpdpp::search &&search)
{
	return build_search(search_terms, search);
}

mpdpp::search& search_queue_widget::build_search(const QString& search_terms, mpdpp::search &search)
{
	QString terms = search_terms;
	if (terms.startsWith('!'))
	{
		terms = terms.remove(0, 1);
	}
	for(QString const& it : terms.split(',', QString::SkipEmptyParts))
	{
		if (it.size() > 0 and it[0] == ':')
		{
			QStringList tag_value = it.split(':', QString::SkipEmptyParts);
			if (tag_value.count() == 2)
			{
				if (tag_value[0] == "uri")
				{
					search << mpdpp::uri_contains(tag_value[1].toUtf8());
				}
				else
				{
					mpdpp::tag tag = mpdpp::tag_from_string(tag_value[0].toUtf8());
					const QString& value = tag_value[1];
					if (tag != mpdpp::tag::unknown and value.size() >= 2)
					{
						search << mpdpp::tag_contains(tag,
													  value.toUtf8());
					}
				}
			}
		}
		else if (it.size() >= 2)
		{
			search << mpdpp::any_tag_contains(it.toUtf8());
		}
	}
	return search;
}

mpdpp::search search_queue_widget::search_type()
{
	if (text_->text() == "@")
	{
		return mpd_.queue();
	}
	if (queue_search())
	{
		return mpd_.search_queue();
	}
	else
	{
		return mpd_.search_db();
	}
}

void search_queue_widget::search(const QString& str)
{
	list_->clear();
	QString search_str = str;

	mpdpp::search search = search_type();
	build_search(search_str, search);

	list_->fill(search);

	emit needResize();
}

QSize search_queue_widget::sizeHint() const
{
	QSize textHint = text_->sizeHint();
	QSize listHint(0,0);

	if (list_->model()->rowCount() > 0)
	{
		listHint = list_->sizeHint();
	}
	QSize size = QSize(qMax(textHint.width(), listHint.width()), textHint.height() + listHint.height());
	return size;
}

void search_queue_widget::play(mpdpp::song_ptr song)
{
	if (song)
	{
		if (queue_search())
		{
			mpd_.play(*song);
			emit quit();
		}
		else
		{
			mpd_.play(*mpd_.add(song->uri()));
		}
	}
}

bool search_queue_widget::queue_search() const
{
	return not text_->text().startsWith('!');
}

}
