/******************************************************************************

Copyright (C) 2013  CLÉMENT Guillaume <gclement@baobob.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

******************************************************************************/

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
