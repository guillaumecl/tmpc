#ifndef TMPC_DISPLAY_WIDGET_H
#define TMPC_DISPLAY_WIDGET_H

#include <QLabel>

#include <mpd++/mpd.h>

class QSlider;
class QTimer;

namespace tmpc
{

/**
 * Displays the content of the current song.
 */
class display_widget: public QWidget
{
	Q_OBJECT
public:
	/**
	 * Creates a display widget mith the specified mpd connection.
	 * @param mpd: the mpd connection.
	 */
	display_widget(mpdpp::mpd& mpd);

private slots:
	void display(mpdpp::song_ptr song);
	void poll();
	void seek(int position);

signals:
	/**
	 * Sent when this widget changed some data and potentially needs some resizing.
	 */
	void needResize();

	/**
	 * Sent when a user action should quit the application.
	 */
	void quit();

protected:
	/**
	 * Reimplement showEvent from QWidget.
	 * @param event the event to process.
	 */
	virtual void showEvent(QShowEvent *event) override;

	/**
	 * Reimplement hideEvent from QWidget.
	 * @param event the event to process.
	 */
	virtual void hideEvent(QHideEvent *event) override;


private:
	QLabel *tags_;
	QLabel *title_;
	QSlider *slider_;
	QTimer *timer_;

	int id_;

	mpdpp::mpd &mpd_;
};

}

#endif //TMPC_DISPLAY_WIDGET_H
