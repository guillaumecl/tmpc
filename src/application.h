#ifndef TMPC_APPLICATION_H
#define TMPC_APPLICATION_H

#include <QApplication>

namespace tmpc
{

/**
 * Application for tmpc.
 */
class application: public QApplication
{
Q_OBJECT
public:
	/**
	 * Creates an application.
	 */
	application(int &argc, char **argv);

protected:
	/**
	 * Overrides notify of QApplication to handle exceptions.
	 */
	virtual bool notify( QObject* receiver, QEvent* event );

};

}

#endif
