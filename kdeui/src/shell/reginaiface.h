
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file reginaiface.h
 *  \brief Provides DCOP interfaces for general Regina classes.
 */

#ifndef __REGINAIFACE_H
#define __REGINAIFACE_H

#include <QObject>

/**
 * A D-Bus interface for a top-level Regina window.
 */
class ReginaMainInterface {
    Q_CLASSINFO("D-Bus Interface","org.kde.regina") 

    public Q_SLOTS:
        Q_SCRIPTABLE void newTopology();
        Q_SCRIPTABLE void newPython();
        Q_SCRIPTABLE bool openURL(const QString& url);
        Q_SCRIPTABLE void pythonConsole();
        Q_SCRIPTABLE void close();
        Q_SCRIPTABLE void quit();
};

#endif
