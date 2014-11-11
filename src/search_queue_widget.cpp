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

#include "search_queue_widget.h"
#include "song_widget.h"
#include "display_widget.h"
#include "song_model.h"

#include "history_line_edit.h"

#include <QLineEdit>
#include <QVBoxLayout>

#include <QKeyEvent>

#include <sstream>


namespace tmpc
{

search_queue_widget::search_queue_widget(mpdpp::mpd& mpd) :
	mpd_(mpd),
	model_(new song_model(this))
{
	setAttribute(Qt::WA_QuitOnClose);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	display_ = new display_widget(mpd);
	display_->hide();

	layout->addWidget(display_);

	text_ = new history_line_edit("search_queue_widget", this);
	text_->setFrame(false);
	layout->addWidget(text_);

	list_ = new song_widget(model_, this);
	layout->addWidget(list_, 1);

	connect(text_, SIGNAL(textChanged(const QString&)),
		this, SLOT(search(const QString&)));

	connect(list_, SIGNAL(play_song(unsigned int)),
		this, SLOT(play(unsigned int)));

	connect(list_, SIGNAL(play_song(int, QString)),
		this, SLOT(play(int, QString)));

	connect(list_, SIGNAL(priority_increased(int, unsigned int, int)),
		this, SLOT(increase_priority(int, unsigned int, int)));

	connect(list_, SIGNAL(priority_decreased(int, unsigned int, int)),
		this, SLOT(decrease_priority(int, unsigned int, int)));

	connect(list_, SIGNAL(remove_song(int, unsigned int)),
		this, SLOT(remove_song(int, unsigned int)));

	connect(list_, SIGNAL(queue_song(int, QString)),
		this, SLOT(insert_song(int, QString)));

	connect(display_, SIGNAL(needResize()),
		this, SIGNAL(needResize()));
}

void search_queue_widget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Up
		or event->key() == Qt::Key_Down
		or event->key() == Qt::Key_PageUp
		or event->key() == Qt::Key_PageDown) {
		list_->setFocus();
		QWidget::keyPressEvent(event);
	} else if (event->key() == Qt::Key_O
			 and event->modifiers() & Qt::ControlModifier
			 and not queue_search()) {
		QString search_term = text_->text();

		build_search(search_term, mpd_.add_from_db());

		// Remove the ! to make the list match with the content
		search_term.remove(0, 1);
		text_->setText(search_term);
	} else if (event->key() == Qt::Key_P
		and event->modifiers() & Qt::ControlModifier) {
		mpd_.clear_queue();
		if (queue_search())
			model_->clear();
	} else if (event->key() == Qt::Key_F10) {
		display_->setVisible(not display_->isVisible());
		emit needResize();
	} else if (event->key() == Qt::Key_Return) {
		QString search_term = text_->text();
		if (search_term.startsWith('+')) {
			build_search(search_term, mpd_.add_from_db());

			text_->clear();
			event->accept();
		}
	} else {
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
	if (terms.startsWith('!') or terms.startsWith('+')) {
		terms = terms.remove(0, 1);
	}
	for(QString const& it : terms.split(',', QString::SkipEmptyParts)) {
		if (it.size() > 0 and it[0] == ':') {
			QStringList tag_value = it.split(':');
			if (tag_value.count() == 3) {
				const QString& tag_name = tag_value[1];
				const QString& value = tag_value[2];

				if (tag_name == "uri") {
					search << mpdpp::uri_contains(value.toUtf8());
				} else {
					mpdpp::tag tag;
					if (tag_name.size() == 0)
						tag = mpdpp::tag::comment;
					else
						tag = mpdpp::tag_from_string(tag_name.toUtf8());

					if (tag != mpdpp::tag::unknown and value.size() >= 2)
						search << mpdpp::tag_contains(tag, value.toUtf8());
				}
			}
		} else if (it.size() >= 2) {
			search << mpdpp::any_tag_contains(it.toUtf8());
		}
	}
	return search;
}

mpdpp::search search_queue_widget::search_type()
{
	if (text_->text() == "@")
		return mpd_.queue();

	if (queue_search())
		return mpd_.search_queue();
	return mpd_.search_db();
}

void search_queue_widget::search(const QString& str)
{
	/**
	 * Add searches are validated using the Return key.
	 */
	if (str.startsWith('+'))
		return;

	QString search_str = str;

	mpdpp::search search = search_type();
	build_search(search_str, search);

	model_->fill(search);

	emit needResize();
}

QSize search_queue_widget::sizeHint() const
{
	QSize textHint = text_->sizeHint();
	QSize listHint(0,0);

	if (list_->model()->rowCount() > 0)
		listHint = list_->sizeHint();

	QSize size = QSize(qMax(textHint.width(), listHint.width()), textHint.height() + listHint.height());
	return size;
}

void search_queue_widget::play(unsigned int song_id)
{
	mpd_.play(song_id);
	if (not display_->isVisible())
		emit quit();
}

void search_queue_widget::play(int position, QString song_uri)
{
	mpdpp::song_ptr song = mpd_.add(song_uri.toUtf8());
	mpd_.play(song);
	if (not display_->isVisible())
		emit quit();
	else
		model_->update_song(position, *song);
}

void search_queue_widget::insert_song(int position, QString uri)
{
	mpdpp::song_ptr created = mpd_.add(uri.toUtf8());
	model_->update_song(position, *created);
}

bool search_queue_widget::queue_search() const
{
	return not text_->text().startsWith('!') and not text_->text().startsWith('+');
}

void search_queue_widget::increase_priority(int position, unsigned int song_id, int priority)
{
	if (priority < 255) {
		mpd_.set_song_priority(song_id, priority + 1);
		(void)position;
		// TODO update song priority
	}
}

void search_queue_widget::decrease_priority(int position, unsigned int song_id, int priority)
{
	if (priority > 0) {
		mpd_.set_song_priority(song_id, priority - 1);
		(void)position;
		// TODO update song priority
	}
}

void search_queue_widget::remove_song(int position, unsigned int id)
{
	mpd_.delete_song(id);
	(void)position;
	// TODO delete song from the model.
}

}
