#ifndef TMPC_SONG_WIDGET_H
#define TMPC_SONG_WIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMetaType>

#include "mpd++/forward.h"

namespace tmpc
{

/**
 * A list widget that shows songs and can respond to play events.
 */
class song_widget: public QTreeWidget
{
Q_OBJECT
public:
	typedef QTreeWidgetItem item_type;
	song_widget(QWidget *parent = 0);

	void add_song(mpdpp::song_ptr song);

	mpdpp::song_ptr selection() const;

public slots:
	void fill(mpdpp::search& search);

signals:
	void song_selected(mpdpp::song_ptr song);


};

}

Q_DECLARE_METATYPE(std::shared_ptr<mpdpp::song>)


#endif
