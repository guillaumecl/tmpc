#ifndef TMPC_DISPLAY_WIDGET_H
#define TMPC_DISPLAY_WIDGET_H

#include <QLabel>

#include "mpd++/mpd.h"

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
signals:
	void needResize();
	void quit();

private:
	QLabel *label_;
	QSlider *slider_;

	mpdpp::mpd &mpd_;
};

}

#endif //TMPC_DISPLAY_WIDGET_H
