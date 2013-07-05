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
    if (event->key() == Qt::Key_Up)
    {
        if (position_ != 0)
        {
            event->accept();
            --position_;
            setText(history_[position_]);
            handled = true;
        }
    }
    else if (event->key() == Qt::Key_Down)
    {
        if (position_ < history_.size())
        {
            event->accept();
            ++position_;
            if (position_ < history_.size())
            {
                setText(history_[position_]);
            }
            else
            {
                clear();
            }
            handled = true;
        }
    }
    else if (event->key() == Qt::Key_Return)
    {
        history_ << text();
    }

    if (not handled)
    {
        QLineEdit::keyPressEvent(event);
    }
}
