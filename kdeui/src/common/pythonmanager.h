
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file pythonmanager.h
 *  \brief Provides management for python consoles.
 *
 *  This header may be used whether or not python scripting has been built in.
 */

#ifndef __PYTHONMANAGER_H
#define __PYTHONMANAGER_H

#include <set>

class PythonConsole;
class QWidget;

namespace regina {
    class NPacket;
}

/**
 * Provides simple management for python consoles.  Consoles launched
 * through a PythonManager are tracked, so that when a PythonManager is
 * destroyed it simultaneously destroys any consoles that it launched
 * that still exist.
 *
 * This class may be used whether or not python scripting has been built in.
 */
class PythonManager {
    private:
        std::set<PythonConsole*> consoles;
            /**< The set of currently registered consoles. */

    public:
        /**
         * Destroys this python manager.  Any consoles that were either
         * launched through this python manager or externally
         * registered with this python manager will also be destroyed
         * if they still exist.
         */
        virtual ~PythonManager();

        /**
         * Launches a new python console to interact with the given packet tree
         * and selected packet.  Either of these packets may be null.  The new
         * console is automatically registered with this python manager.
         *
         * If python scripting is not built in, a notice is displayed
         * to the user.
         */
        void launchPythonConsole(QWidget* parent = 0,
                regina::NPacket* tree = 0,
                regina::NPacket* selectedPacket = 0);

        /**
         * Asks this python manager to claim responsibility for the
         * given console.  When this python manager is destroyed, if the
         * given console still exists then it is destroyed also.
         */
        void registerConsole(PythonConsole* console);
        /**
         * Asks this python manager to relinquish responsibility for the
         * given console.
         */
        void deregisterConsole(PythonConsole* console);
};

#endif

