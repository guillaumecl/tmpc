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
	layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

	setLayout(layout);


	QTimer *timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()),
			this, SLOT(poll()));

	timer->start(250);

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
