#include "main_window.h"

#include <QApplication>
#include <QDesktopWidget>

#include <QKeyEvent>

#include "mpd++/mpd.h"

using namespace tmpc;

main_window::main_window(mpdpp::mpd& mpd, QWidget *centralWidget) :
	QMainWindow(nullptr, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint),
	mpd_(mpd)
{
	setAttribute(Qt::WA_QuitOnClose);
	setAttribute(Qt::WA_DeleteOnClose);

	setCentralWidget(centralWidget);
	resizeToFit();

	show();
}

void main_window::resizeToFit()
{
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry(desktop->screenNumber(QCursor::pos()));

	QSize centralSize = centralWidget()->sizeHint();

	int width = qMax((4*screen.width()) / 5, centralSize.width());
	int height = centralSize.height();

	setGeometry(QRect(
					screen.left() + (screen.width() - width) / 2,
					screen.top() + (screen.height() - height) / 4,
				    width,
                    height));
}


void main_window::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		event->accept();
		close();
	}
	else if(event->modifiers() & Qt::ControlModifier)
	{
	}
	else
	{
		QMainWindow::keyPressEvent(event);
	}
}
