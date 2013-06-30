#ifndef TMPC_SEARCH_QUEUE_WIDGET_H
#define TMPC_SEARCH_QUEUE_WIDGET_H

#include <QMainWindow>
#include <QListWidget>
#include "mpd++/mpd.h"

namespace tmpc
{

class song_widget;

class search_queue_widget: public QWidget
{
	Q_OBJECT
public:
	search_queue_widget(mpdpp::mpd& mpd);

	virtual QSize sizeHint() const;

	bool queue_search() const;
protected:
	virtual void keyPressEvent(QKeyEvent *event);

private slots:
	void search(const QString& str);
	void play(mpdpp::song_ptr song);

signals:
	void needResize();
	void quit();

private:
	mpdpp::search search_type();
	mpdpp::search& build_search(const QString& search_terms, mpdpp::search &&search);
	mpdpp::search& build_search(const QString& search_terms, mpdpp::search &search);

	mpdpp::mpd &mpd_;
	QLineEdit *text_;
	song_widget *list_;
};

}

#endif
