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
