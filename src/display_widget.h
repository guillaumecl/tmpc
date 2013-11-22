#ifndef TMPC_DISPLAY_WIDGET_H
#define TMPC_DISPLAY_WIDGET_H

#include <QLabel>

#include <mpd++/mpd.h>

class QSlider;
class QTimer;

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

protected:
	virtual void showEvent(QShowEvent *event) override;

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
