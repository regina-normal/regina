
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

/*! \file pythonconsole.h
 *  \brief Provides a console window containing an embedded python
 *  interpreter.
 */

#ifndef __PYTHONCONSOLE_H
#define __PYTHONCONSOLE_H

#include "pythonoutputstream.h"
#include "../reginaprefset.h"

#include <kmainwindow.h>

class KLineEdit;
class KTextEdit;
class PythonInterpreter;
class PythonManager;
class QLabel;

/**
 * A top-level window containing an embedded python interpreter.
 * Python support must be built in for this class to be used.
 *
 * Generally objects of this class are not created directly; instead
 * PacketManager::launchPythonConsole() is used.
 */
class PythonConsole : public KMainWindow, public PythonOutputStream {
    Q_OBJECT

    private:
        /**
         * Possible types of prompt.
         */
        enum PromptMode { PRIMARY, SECONDARY, PROCESSING };

        /**
         * Internal components
         */
        KTextEdit* session;
        QLabel* prompt;
        KLineEdit* input;

        /**
         * Python components
         */
        PythonInterpreter* interpreter;
        PythonManager* manager;

        /**
         * Configuration and attributes
         */
        ReginaPrefSet prefs;
        QString lastIndent;

    public:
        /**
         * Constructor and destructor.
         */
        PythonConsole(QWidget* parent = 0, PythonManager* useManager = 0,
                regina::NPacket* tree = 0, regina::NPacket* selectedPacket = 0);
        ~PythonConsole();

    public slots:
        /**
         * Process the command currently on the input line.
         */
        void processCommand();

        /**
         * Save a log of the current session.
         */
        void saveLog();

        /**
         * Update the global preferences.
         */
        void updatePreferences(const ReginaPrefSet& newPrefs);

    protected:
        /**
         * PythonOutputStream overrides.
         */
        void processOutput(const std::string& data);

    private:
        /**
         * Initialise the python interpreter and otherwise prepare the
         * console for use.
         */
        void init();

        /**
         * Change the visible prompt.
         */
        void setPromptMode(PromptMode mode = PRIMARY);

        /**
         * Write input or output to the session transcript.
         */
        void addInput(const QString& input);
        void addOutput(const QString& output);

        /**
         * Encode special characters so that the given text can be
         * appended to the session transcript without causing HTML
         * confusion.
         */
        static QString encode(const QString& plaintext);

        /**
         * Calculate the indent at the beginning of the given line.
         */
        static QString initialIndent(const QString& line);
};

inline void PythonConsole::updatePreferences(const ReginaPrefSet& newPrefs) {
    prefs = newPrefs;
}

#endif
