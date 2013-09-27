#ifndef TMPC_DISPLAY_WIDGET_H
#define TMPC_DISPLAY_WIDGET_H

#include <QLabel>

#include "mpd++/mpd.h"

namespace tmpc
{

class display_widget: public QLabel
{
	Q_OBJECT
public:
	display_widget(mpdpp::mpd& mpd);

private slots:
	void display(mpdpp::song_ptr song);

private:
	mpdpp::mpd &mpd_;
};

}

#endif //TMPC_DISPLAY_WIDGET_H
