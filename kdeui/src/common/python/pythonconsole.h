
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

class CommandEdit;
class KTextEdit;
class PythonInterpreter;
class PythonManager;
class QLabel;

/**
 * A top-level window containing an embedded python interpreter.
 * Python support must be built in for this class to be used.
 *
 * Objects of this class are generally not created directly; instead
 * PacketManager::launchPythonConsole() should be used.
 */
class PythonConsole : public KMainWindow {
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
        CommandEdit* input;
        PythonOutputStream* output;
        PythonOutputStream* error;

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
         *
         * The constructor will leave the console with input disabled.
         * Input can be re-enabled by calling allowInput().
         *
         * Generally python consoles are created through
         * PythonManager::launchPythonConsole(), not by calling this
         * constructor directly.
         */
        PythonConsole(QWidget* parent = 0, PythonManager* useManager = 0,
                const ReginaPrefSet* initialPrefs = 0);

        ~PythonConsole();

        /**
         * Write input, output or error to the session transcript.
         */
        void addInput(const QString& input);
        void addOutput(const QString& output);
        void addError(const QString& error);

        /**
         * Disallow input, specifying an optional message.
         */
        void blockInput(const QString& msg = QString::null);

        /**
         * Allow input, using either the primary or secondary prompt.
         */
        void allowInput(bool primaryPrompt = true,
            const QString& suggestedInput = QString::null);

        /**
         * Configure the python interpreter.
         */
        bool importRegina();
        void setRootPacket(regina::NPacket* packet);
        void setSelectedPacket(regina::NPacket* packet);
        void setVar(const QString& name, regina::NPacket* value);
        void loadAllLibraries();
        void executeLine(const QString& line);
        void executeLine(const std::string& line);
        void executeLine(const char* line);

        /**
         * Compile or run user scripts.
         */
        bool compileScript(const QString& script);
        void executeScript(const QString& script,
            const QString& scriptName = QString::null);

    public slots:
        /**
         * Save a log of the current session.
         */
        void saveLog();

        /**
         * Open the reference manual at the scripting overview.
         */
        void scriptingOverview();

        /**
         * Open the calculation engine documentation.
         */
        void pythonReference();

        /**
         * Update the global preferences.
         */
        void updatePreferences(const ReginaPrefSet& newPrefs);

    private:
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

    private slots:
        /**
         * Process the command currently on the input line.
         */
        void processCommand();

    private:
        /**
         * An output stream that writes data using addOutput().
         */
        class OutputStream : public PythonOutputStream {
            private:
                PythonConsole* console_;

            public:
                OutputStream(PythonConsole* console);

            protected:
                void processOutput(const std::string& data);
        };

        /**
         * An output stream that writes data using addError().
         */
        class ErrorStream : public PythonOutputStream {
            private:
                PythonConsole* console_;

            public:
                ErrorStream(PythonConsole* console);

            protected:
                void processOutput(const std::string& data);
        };
};

inline PythonConsole::OutputStream::OutputStream(PythonConsole* console) :
        console_(console) {
}

inline PythonConsole::ErrorStream::ErrorStream(PythonConsole* console) :
        console_(console) {
}

#endif
