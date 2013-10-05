#include "display_widget.h"
#include <sstream>

#include <QTimer>

#include <mpd/client.h>

#include <QApplication>

#include <QVBoxLayout>

using namespace tmpc;

display_widget::display_widget(mpdpp::mpd& mpd) :
	mpd_(mpd)
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	label_ = new QLabel(this);

	label_->setWordWrap(true);
	display(mpd_.current_song());


	layout->addWidget(label_);

	setLayout(layout);


	mpd_.monitor(mpdpp::event::player);
	QTimer *timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()),
			this, SLOT(poll()));

	timer->start(250);
}


void display_widget::display(mpdpp::song_ptr song)
{
	if(!song)
	{
		return;
	}
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

	QString text = QString("<center><h1>%1</h1></center>").arg(translated);

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

	label_->setText(text);
	emit needResize();
}


void display_widget::poll()
{
	if (mpd_.stop_monitor())
	{
		display(mpd_.current_song());
	}
	mpd_.monitor(mpdpp::event::player);
}
