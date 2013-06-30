#include "main_window.h"

#include <QApplication>
#include <QDesktopWidget>

#include <QKeyEvent>

using namespace tmpc;

main_window::main_window(QWidget *centralWidget) :
	QMainWindow(nullptr, Qt::Tool | Qt::WindowStaysOnTopHint)
{
	setAttribute(Qt::WA_QuitOnClose);

	setCentralWidget(centralWidget);
	resizeToFit();
}

void main_window::resizeToFit()
{
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry(desktop->screenNumber(QCursor::pos()));

	QSize centralSize = centralWidget()->sizeHint();

	int width = (4*screen.width()) / 5;
	int height = centralSize.height();

	setGeometry(QRect(
				(screen.width() - width) / 2,
				(screen.height() - height) / 4,
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
	else
	{
		QMainWindow::keyPressEvent(event);
	}
}
