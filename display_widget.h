#ifndef TMPC_DISPLAY_WIDGET_H
#define TMPC_DISPLAY_WIDGET_H

#include <QLabel>

#include "mpd++/mpd.h"

#include <mutex>

class QSlider;

namespace tmpc
{

class display_widget: public QWidget
{
	Q_OBJECT
public:
	display_widget(mpdpp::mpd& mpd);

private slots:
	void display(mpdpp::song_ptr song);
	void poll();
	void seek(int position);

signals:
	void needResize();
	void quit();

private:
	QLabel *tags_;
	QLabel *title_;
	QSlider *slider_;

	int id_;
	std::mutex mutex_;

	mpdpp::mpd &mpd_;
};

}

#endif //TMPC_DISPLAY_WIDGET_H
