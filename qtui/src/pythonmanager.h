
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

/*! \file pythonmanager.h
 *  \brief Provides management for python consoles.
 *
 *  This header may be used whether or not python scripting has been built in.
 */

#ifndef __PYTHONMANAGER_H
#define __PYTHONMANAGER_H

#include <qstring.h>
#include <qlinkedlist.h>
#include <set>

class PythonConsole;
class QString;
class QWidget;

namespace regina {
    class Packet;
}

/**
 * Represents a single python variable.
 */
struct PythonVariable {
    QString name;
        /**< The name of the variable. */
    regina::Packet* value;
        /**< The value of the variable.  This may be 0, in which case
             the corresponding Python value will be None. */

    /**
     * Basic constructors.
     */
    PythonVariable();
    PythonVariable(const QString& newName, regina::Packet* newValue);
};

typedef QLinkedList<PythonVariable> PythonVariableList;

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
         * to the user and 0 is returned.
         */
        PythonConsole* launchPythonConsole(QWidget* parent,
                regina::Packet* tree = 0,
                regina::Packet* selectedPacket = 0);

        /**
         * Launches a new python console to run the given script.
         * The new console is automatically registered with this python
         * manager.
         *
         * If python scripting is not built in, a notice is displayed
         * to the user and 0 is returned.
         */
        PythonConsole* launchPythonConsole(QWidget* parent,
                const QString& script,
                const PythonVariableList& initialVariables);

        /**
         * Attempts to compile the given python script.  If the compile
         * succeeds, 0 is returned.  If the compile fails, a new python
         * console is opened displaying the appropriate error and this new
         * console is returned.
         *
         * If a new console is opened, it is automatically registered with
         * this python manager.
         *
         * If python scripting is not built in, a notice is displayed
         * to the user and 0 is returned.
         */
        PythonConsole* compileScript(QWidget* parent, const QString& script);

        /**
         * Destroys any consoles still in existence that were either
         * launched through this python manager or externally registered
         * with this python manager.
         */
        void closeAllConsoles();

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

        /**
         * Opens a new browser displaying the calculation engine
         * documentation.
         */
        static void openPythonReference(QWidget* topLevelWindow);
};

inline PythonVariable::PythonVariable() : value(0) {
}

inline PythonVariable::PythonVariable(const QString& newName,
        regina::Packet* newValue) : name(newName), value(newValue) {
}

#endif

