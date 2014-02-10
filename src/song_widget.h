#ifndef TMPC_SONG_WIDGET_H
#define TMPC_SONG_WIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMetaType>
#include <QIcon>

#include <mpd++/forward.h>

namespace tmpc
{

/**
 * A list widget that shows songs and can respond to play events.
 */
class song_widget: public QTreeWidget
{
	Q_OBJECT
public:
	/**
	 * the item type. Use this instead of QTreeWidgetItem in case the class changes later.
	 */
	typedef QTreeWidgetItem item_type;

	/**
	 * Creates a song widget.
	 * @param parent the parent widget.
	 */
	song_widget(QWidget *parent = 0);

	/**
	 * The current selection of the widget.
	 */
	mpdpp::song_ptr selection() const;

public slots:
	/**
	 * Fills the list from a search.
	 */
	void fill(mpdpp::search& search);

	/**
	 * Fills the list from a search.
	 */
	void fill(mpdpp::search&& search);

	/**
	 * Fills the queue cache.
	 */
	void feed_queue(mpdpp::search& search);

	/**
	 * Fills the queue cache.
	 */
	void feed_queue(mpdpp::search&& search);

	/**
	 * True if the queue has been calculated.
	 */
	bool queue_fed() const;

    /**
     * Clear the cached queue.
     */
    void clear_queue();

    /**
     * Modify the fed queue status.
     */
    void set_queue_fed(bool fed);

signals:
	/**
	 * Emitted when a song is selected.
	 */
	void song_selected(mpdpp::song_ptr song);

	/**
	 * Increase the priority of the specified song.
	 */
	void priority_increased(mpdpp::song_ptr song);

	/**
	 * Decrease the priority of the specified song.
	 */
	void priority_decreased(mpdpp::song_ptr song);

	/**
	 * Remove the specified song from the queue.
	 */
	void song_removed(mpdpp::song_ptr song);

	/**
	 * Insert the specified song into the queue.
	 */
	void song_inserted(mpdpp::song_ptr song);

private slots:
	void item_double_clicked();
	void add_song(mpdpp::song_ptr song);

protected:
	/**
	 * Respond to a key event.
	 */
	virtual void keyPressEvent(QKeyEvent *event);

private:
	QIcon queue_icon_;
	QIcon db_icon_;

	QSet<QString> queue_;
	bool queue_fed_;
};

}

Q_DECLARE_METATYPE(std::shared_ptr<mpdpp::song>)


#endif
