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

#include "history_line_edit.h"

#include <QKeyEvent>
#include <QSettings>

using namespace tmpc;



history_line_edit::history_line_edit(const QString& history_name, QWidget *parent) :
    QLineEdit(parent),
    history_name_(history_name)
{
    QSettings settings;
    history_ = settings.value(history_name_).toStringList();
    position_ = history_.size();
}

history_line_edit::~history_line_edit()
{
    QSettings settings;
    settings.setValue(history_name_, history_);
}

void history_line_edit::keyPressEvent(QKeyEvent *event)
{
    bool handled = false;
    if (event->key() == Qt::Key_Up) {
        if (position_ != 0) {
            event->accept();
            --position_;
            setText(history_[position_]);
            handled = true;
        }
    } else if (event->key() == Qt::Key_Down) {
        if (position_ < history_.size()) {
            event->accept();
            ++position_;
            if (position_ < history_.size())
                setText(history_[position_]);
            else
                clear();
            handled = true;
        }
    } else if (event->key() == Qt::Key_Return) {
        history_ << text();
    }

    if (not handled)
        QLineEdit::keyPressEvent(event);
}
