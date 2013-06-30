#ifndef TMPC_MAIN_WINDOW_H
#define TMPC_MAIN_WINDOW_H

#include <QMainWindow>

#include "mpd++/forward.h"

namespace tmpc
{

class main_window: public QMainWindow
{
	Q_OBJECT
public:
	main_window(mpdpp::mpd& mpd, QWidget *centralWidget);

public slots:
	void resizeToFit();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QWidget *main_widget_;
	mpdpp::mpd& mpd_;
};


}

#endif
