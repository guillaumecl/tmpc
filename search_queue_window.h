#ifndef TMPC_SEARCH_QUEUE_WINDOW_H
#define TMPC_SEARCH_QUEUE_WINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "mpd++/mpd.h"

namespace tmpc
{

class song_widget;

class search_queue_window: public QMainWindow
{
	Q_OBJECT
public:
	search_queue_window();

protected:
	void keyPressEvent(QKeyEvent *event);

private slots:
	void search(const QString& str);

private:
	mpdpp::mpd mpd_;
	QLineEdit *text_;
	song_widget *list_;
};

}

#endif
