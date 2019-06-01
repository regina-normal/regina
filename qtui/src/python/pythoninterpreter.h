
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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

/*! \file pythoninterpreter.h
 *  \brief Provides an interface to the python interpreter and
 *  subinterpreters.
 */

#ifndef __PYTHONINTERPRETER_H
#define __PYTHONINTERPRETER_H

#include <Python.h>
#include <mutex>
#include <string>

class PythonOutputStream;

namespace regina {
    class Packet;
}

/**
 * A single python subinterpreter.  Multiple subinterpreters are independent
 * and may exist simultaneously.
 *
 * Each new subinterpreter corresponds to a new call to
 * Py_NewInterpreter().  The global routine Py_Initialize() is called when
 * the first interpreter is created, and Py_Finalize() is never called.
 */
class PythonInterpreter {
    private:
        static std::mutex globalMutex;
            /**< A mutual exclusion device for the creation and
                 destruction of subinterpreters. */
        static bool pythonInitialised;
            /**< Has the python system been initialised yet? */

        PyThreadState* state;
            /**< The first thread state created in this particular
                 subinterpreter. */

        PyObject* mainModule;
            /**< The __main__ module. */
        PyObject* mainNamespace;
            /**< The global namespace. */

        std::string currentCode;
            /**< Any previous statements (such as loop openings) that are
                 waiting to be completed. */

    public:
        /**
         * Constructor and destructor.
         */
        PythonInterpreter(PythonOutputStream* pyStdOut = 0,
                PythonOutputStream* pyStdErr = 0);
        ~PythonInterpreter();

        /**
         * Execute the given line in the subinterpreter.
         * Returns \c true if no further input is required, or
         * \c false if the interpreter is waiting on further lines of code.
         */
        bool executeLine(const std::string& command);

        /**
         * Configuration of the subinterpreter.
         * Each of these routines returns \c true on success and
         * \c false on failure.
         */
        bool importRegina();
        bool setVar(const char* name, regina::Packet* value);
        bool runScript(const char* code);

    private:
        /**
         * Is the given command comment or only whitespace?
         */
        static bool isEmptyCommand(const std::string& command);

        /**
         * Write the given error message to std::cerr along with a
         * request to report the problem.
         */
        static void pleaseReport(const char* msg);

        /**
         * Extract the current error message.  A new python reference is
         * returned.
         */
        static PyObject* extractErrMsg();

        /**
         * Push regina's python path onto the beginning of python's sys.path.
         *
         * This routine assumes that the global interpreter lock is already
         * held by the current thread, and it acts on the (sub-)interpreter
         * that is referenced by the current python thread state.
         */
        static void prependReginaToSysPath();

        /**
         * Attempt to import regina into the given namespace.  Typically
         * \a useNamespace would be the main namespace for either the
         * main interpreter or a subinterpreter.
         *
         * This routine assumes that the global interpreter lock is already
         * held by the current thread.  It acts on the (sub-)interpreter
         * that is referenced by the current python thread state, to which
         * the given namespace must belong.
         *
         * Returns \c true on success or \c false on failure.
         */
        static bool importReginaIntoNamespace(PyObject* useNamespace);
};

#endif
