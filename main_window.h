#ifndef TMPC_MAIN_WINDOW_H
#define TMPC_MAIN_WINDOW_H

#include <QMainWindow>

namespace tmpc
{

class main_window: public QMainWindow
{
	Q_OBJECT
public:
	main_window(QWidget *centralWidget);

public slots:
	void resizeToFit();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QWidget *main_widget_;
};


}

#endif
