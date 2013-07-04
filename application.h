#ifndef TMPC_APPLICATION_H
#define TMPC_APPLICATION_H

#include <QApplication>

namespace tmpc
{

class application: public QApplication
{
Q_OBJECT
public:
	application(int& argc, char **argv);

protected:
	virtual bool notify( QObject* receiver, QEvent* event );

};

}

#endif
