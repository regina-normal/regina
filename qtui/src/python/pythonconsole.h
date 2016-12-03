
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

/*! \file pythonconsole.h
 *  \brief Provides a console window containing an embedded python
 *  interpreter.
 */

#ifndef __PYTHONCONSOLE_H
#define __PYTHONCONSOLE_H

#include "pythonoutputstream.h"
#include "../reginaprefset.h"

#include <packet/packet.h>
#include <QMainWindow>

class CommandEdit;
class PythonInterpreter;
class PythonManager;
class QCompleter;
class QLabel;
class QTextEdit;

/**
 * A top-level window containing an embedded python interpreter.
 * Python support must be built in for this class to be used.
 *
 * Objects of this class are generally not created directly; instead
 * PacketManager::launchPythonConsole() should be used.
 */
class PythonConsole : public QMainWindow {
    Q_OBJECT

    friend class PythonOutputStream;
    friend class CommandEdit;
    private:
        /**
         * Possible types of prompt.
         */
        enum PromptMode { PRIMARY, SECONDARY, PROCESSING };

        /**
         * Internal components
         */
        QTextEdit* session;
        QLabel* prompt;
        CommandEdit* input;
        PythonOutputStream* output;
        PythonOutputStream* error;

        /**
         * Actions
         */
        QAction* actCut;
        QAction* actCopy;
        QAction* actPaste;

        /**
         * Python components
         */
        PythonInterpreter* interpreter;
        PythonManager* manager;

        /**
         * Configuration and attributes
         */
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
        PythonConsole(QWidget* parent = 0, PythonManager* useManager = 0);

        ~PythonConsole();

        /**
         * Write input, output or error to the session transcript.
         */
        void addInput(const QString& input);
        void addOutput(const QString& output);
        void addInfo(const QString& info);
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
        void setRootPacket(regina::Packet* packet);
        void setSelectedPacket(regina::Packet* packet);
        void setVar(const QString& name, regina::Packet* value);
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
         * Open the users' handbook at the scripting overview.
         */
        void scriptingOverview();

        /**
         * Open the calculation engine documentation.
         */
        void pythonReference();

        /**
         * Update the global preferences.
         */
        void updatePreferences();

        /**
         * Help -> What's this has been acctivated.
         */
        void contextHelpActivated();

        /**
         * Cut selected text from the command area.
         */
        void cut();

        /**
         * Copy selected text, whether it be in the session log or the
         * input area.
         */
        void copy();

        /**
         * Paste text from the clipboard into the command area.
         */
        void paste();

        /**
         * Select all text in the session log.
         */
        void selectAll();

    protected:
        /**
         * Qt overrides.
         */
         virtual QSize sizeHint() const;
        
        /**
         * Track who is getting output from python. 
         */
        bool outputToTabCompletion;
        /**
         * The list of possible completions. 
         */
        QStringList* completions;
        /**
         * Track what index into the completions has
         * been returned by the python.
         */
        int nextCompletion;
        /**
         * A completer object to handle python tab completion.
         */
        QCompleter* completer;
        /**
         * Tab completion results are in, display them.
         */
        void completionsFinished();
        /**
         * Reqest another completion. Needs to be a function
         * that the output stream can call, to emit a signal so
         * the request isn't blocking.
         */
        void requestNextCompletion(); 

    signals:
        /**
         * The signal sent when requesting another completion.
         */
        void doNextCompletion();

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

        /**
         * Update the enabled state of clipboard-related actions
         * and update selections in the session log and/or input area.
         */
        void inputSelectionChanged();
        void sessionSelectionChanged();
        void clipboardChanged();

        /**
         * Process tab completion on the partial line. 
         **/
        void processCompletion();
        /**
         * Request the next tab completion option from pythons readline
         */
        void getNextCompletion(); 
        

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
