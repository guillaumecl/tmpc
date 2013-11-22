#ifndef TMPC_MAIN_WINDOW_H
#define TMPC_MAIN_WINDOW_H

#include <QMainWindow>

#include <mpd++/mpd.h>

namespace tmpc
{

/**
 * The main window of tmpc.
 *
 * Basically, this is just here to show a widget on screen at an absolute position.
 */
class main_window: public QMainWindow
{
	Q_OBJECT
public:
	/**
	 * Creates a main window.
	 * @param mpd the underlying mpd connection
	 * @param centralWidget the widget to display.
	 */
	main_window(mpdpp::mpd& mpd, QWidget *centralWidget);

public slots:
	/**
	 * Resize the window and move it to the proper place.
	 * @todo maybe rename this since this does not match what the documentation says.
	 */
	void resizeToFit();

protected:
	/**
	 * Respond to a keyPressEvent.
	 */
	void keyPressEvent(QKeyEvent *event);

private:
	QWidget *main_widget_;
	mpdpp::mpd& mpd_;
};


}

#endif
