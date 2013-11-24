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

#include "display_widget.h"
#include <sstream>

#include <QTimer>

#include <mpd/client.h>

#include <QApplication>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QSlider>

using namespace tmpc;

display_widget::display_widget(mpdpp::mpd& mpd) :
	id_(-1),
	mpd_(mpd)
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	title_ = new QLabel(this);
	title_->setFont(QFont("Times"));
	title_->setAutoFillBackground(true);
	title_->setBackgroundRole(QPalette::Highlight);
	title_->setForegroundRole(QPalette::HighlightedText);
	tags_ = new QLabel(this);
	slider_ = new QSlider(Qt::Horizontal, this);

	slider_->setTickInterval(60000);
	slider_->setTickPosition(QSlider::TicksBelow);

	title_->setWordWrap(true);
	tags_->setWordWrap(true);
	display(mpd_.current_song());


	layout->addWidget(title_);
	layout->addWidget(slider_);
	layout->addWidget(tags_);
	layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

	setLayout(layout);


	timer_ = new QTimer(this);

	connect(timer_, SIGNAL(timeout()),
			this, SLOT(poll()));

	connect(slider_, SIGNAL(sliderMoved(int)),
			this, SLOT(seek(int)));
}


void display_widget::display(mpdpp::song_ptr song)
{
	if(!song)
	{
		return;
	}
	id_ = song->id();
	const char *title = song->tag(mpdpp::tag::title);
	const char *artist = song->tag(mpdpp::tag::artist);
	const char *album = song->tag(mpdpp::tag::album);
	if (not title)
	{
		title =  song->uri();
	}

	QString translated;
	if (artist and album)
	{
		translated = tr("%1 by %2 (%3)").arg(title, artist, album);
	}
	else if (artist)
	{
		translated = tr("%1 by %2").arg(title, artist);
	}
	else if (album)
	{
		translated = tr("%1 (%2)").arg(title, album);
	}
	else
	{
		translated = title;
	}

	title_->setText(QString("<center><h1>%1</h1></center>").arg(translated));
	window()->setWindowTitle(translated);

	slider_->setMaximum(song->duration());

	QString text;
	bool first = true;
	for (const auto &pair : song->tags())
	{
		if (first)
		{
			first = false;
			text.append("<hr />");
		}
		else
		{
			text.append("<br />");
		}
		text.append("<b>").append(qApp->translate("tag", tag_to_string(pair.first))).append("</b>: ");
		text.append(pair.second);
	}

	tags_->setText(text);
	emit needResize();
}


void display_widget::poll()
{
	mpdpp::status state = mpd_.status();

	auto old_id = id_;
	id_ = -1;

	// Set the id to -1 so that slider values don't interfere with polling

	slider_->setMaximum(state.total_time()*1000);
	slider_->setValue(state.elapsed_ms());

	id_ = old_id;
	if (id_ != state.song_id())
	{
		display(mpd_.current_song());
	}
}

void display_widget::seek(int position)
{
	if (id_ != -1)
	{
		mpd_.seek(id_, position/1000);
	}
}

void display_widget::showEvent(QShowEvent *)
{
	timer_->start(250);
}

void display_widget::hideEvent(QHideEvent *)
{
	timer_->stop();
}
