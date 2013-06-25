#ifndef TMPC_MAIN_WINDOW_H
#define TMPC_MAIN_WINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "mpd++/mpd.h"

namespace tmpc
{

class main_window: public QMainWindow
{
	Q_OBJECT
public:
	main_window();

protected:
	void keyPressEvent(QKeyEvent *event);

private slots:
	void search(const QString& str);

private:
	mpdpp::mpd mpd_;
	QLineEdit *text_;
	QListWidget *list_;
};

}

#endif
