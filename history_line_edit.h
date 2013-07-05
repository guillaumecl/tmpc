#ifndef TMPC_HISTORY_LINE_EDIT_H
#define TMPC_HISTORY_LINE_EDIT_H

#include <QLineEdit>


namespace tmpc
{

/**
 * A line editor that manages its history.
 */
class history_line_edit: public QLineEdit
{
Q_OBJECT
public:
    /**
     * Creates a history_line_edit object.
     * @param history_name a name that uniqeuly references this widget in the application.
     * Allows to save and store the history for later use.
     * @param parent the parent widget.
     */
    history_line_edit(const QString& history_name, QWidget *parent=nullptr);

    /**
     * Destroys a history_line_edit object.
     * Stores the history to disk.
     */
    ~history_line_edit();

protected:
	/**
	 * Respond to a keyPressEvent.
	 */
	void keyPressEvent(QKeyEvent *event);

private:
    QString history_name_;
    QStringList history_;
    int position_;
};

}

#endif
