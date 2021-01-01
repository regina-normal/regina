
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
 *
 *  The accompanying source file (pythoninterpreter.cpp) should be built
 *  directly into each user interface that requires it.
 *  It is \e not built into Regina's python module.
 */

#ifndef __PYTHONINTERPRETER_H
#define __PYTHONINTERPRETER_H

#include <Python.h>
#include <mutex>
#include <string>

namespace regina {

class Packet;
class Script;

namespace python {

class PythonOutputStream;

/**
 * A callback object that the user can pass to PythonInterpreter::complete().
 *
 * Subclasses must override addCompletion(), which receives completions
 * one at a time as PythonInterpreter::complete() finds them.
 */
class PythonCompleter {
    public:
        /**
         * Called each time addCompletion() finds a new completion.
         *
         * Note that python's completion code has been known to return the
         * same completion multiple times (in which case the same completion
         * will be passed to this function multiple times in turn).
         *
         * @param s the new completion.  This string will begin with the
         * original text that was passed to PythonCompleter::complete().
         * @return \c true to indicate that PythonCompleter::complete()
         * should continue searching for more completions, or \c false
         * if PythonCompleter::complete() can finish now.
         */
        virtual bool addCompletion(const std::string& s) = 0;
};

/**
 * A python completion callback that computes the longest common prefix
 * for all of the completions that it receives.  This can (for example)
 * be used to decide what text to immediately append to the current line
 * when tab is pressed.
 *
 * The common prefix is computed in terms of unicode characters, not raw bytes.
 */
class PrefixCompleter : public PythonCompleter {
    private:
        bool initialised_;
            /** Has this object received any completions at all yet? */
        std::string prefix_;
            /** The longest common prefix for all completions received. */

    public:
        /**
         * Creates a new callback object.  You will need to create a new
         * object each time you call PythonCompleter::complete(); that is,
         * you cannot reuse the same callback object across multiple
         * calls to PythonCompleter::complete().
         */
        PrefixCompleter();
        /**
         * Returns the longest common prefix for all completions received so
         * far.  If no completions have been received at all, then this
         * will be the empty string.
         *
         * The common prefix is computed in terms of unicode characters,
         * and the return value will be a valid UTF-8 string.
         *
         * @return the longest common prefix for all completions.
         */
        const std::string& prefix() const;

        bool addCompletion(const std::string& s) override;
};

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
        PyObject* completer;
            /**< The current plainCompleter::Completer object, or \c null
                 if one could not be created. */
        PyObject* completerFunc;
            /**< The method completer.complete(), or \c null if this reference
                 could not be acquired. */

        std::string currentCode;
            /**< Any previous statements (such as loop openings) that are
                 waiting to be completed. */

        bool caughtSystemExit;
            /**< Has the interpreter thrown a SystemExit exception? */

        PythonOutputStream& output;
            /**< The stream that we are using for sys.stdout. */
        PythonOutputStream& errors;
            /**< The stream that we are using for sys.stderr. */

    public:
        /**
         * Constructor and destructor.
         */
        PythonInterpreter(
            regina::python::PythonOutputStream& pyStdOut,
            regina::python::PythonOutputStream& pyStdErr);
        ~PythonInterpreter();

        /**
         * Execute a single line of code in the subinterpreter.
         * Returns \c true if no further input is required, or
         * \c false if the interpreter is waiting on further lines of code.
         *
         * This is intended for use in an interactive Python session.
         *
         * You should always test exitAttempted() after executing user code.
         */
        bool executeLine(const std::string& command);

        /**
         * Imports Regina's python module.
         *
         * This function also sets up a completer object, for use with
         * complete().
         *
         * Returns \c true on success or \c false on failure.
         */
        bool importRegina();

        /**
         * Set the given variable in Python's main namespace to
         * represent the given Regina packet.
         *
         * Returns \c true on success or \c false on failure.
         */
        bool setVar(const char* name, regina::Packet* value);

        /**
         * Run the given script packet in Python's main namespace.
         * This involves setting all of the script variables, and then running
         * the script code itself.
         *
         * This routine flushes standard output and standard error.
         *
         * You should always test exitAttempted() after executing user code.
         *
         * Returns \c true on success or \c false on failure.
         */
        bool runScript(const regina::Script* script);

        /**
         * Flushes the standard output and error streams.
         */
        void flush();

        /**
         * Has the subinterpreter attempted to exit?
         *
         * Specifically, this tests whether a \c SystemExit exception
         * has been thrown (typically by calling a Python function such as
         * exit(), quit(), or sys.quit()).  In a command-line python
         * session, such a function would exit the python * session; here it
         * just sets this flag so the enclosing GUI can clean up nicely.
         *
         * This does \e not capture os._exit(), which will instead
         * immediately terminate the parent process (i.e., the enclosing GUI).
         */
        bool exitAttempted() const;

        /**
         * Attempts to complete the given Python string.
         *
         * The completion process uses Regina's \c plainCompleter Python
         * module, which is a copy of Python's own \c rlcompleter module
         * with all \c readline interaction disabled (since \c readline does
         * not play well with subinterpreters and may produce deadlocks).
         * See the Python documentation for \c rlcompleter for what
         * text can be completed and how completion works.
         *
         * Each completion that is found will be passed to \a completer
         * using PythonCompleter::addCompletion(), until either
         * (i) \a completer returns \c false to signify that no more
         * completions are required, or (ii) no more completions can be found.
         *
         * @param text the Python text to complete.
         * @param completer the callback object that will receive the
         * resulting completions (if any).
         * @return the number of completions that were passed to \a completer,
         * or -1 if the completion process failed (e.g., the \c plainCompleter
         * Python module could not be imported).  In particular, if the
         * completion process ran succesfully and determined that the given
         * text has no completions at all, this routine will return 0.
         */
        int complete(const std::string& text, PythonCompleter& completer);

    private:
        /**
         * Run the given Python code in Python's main namespace.
         */
        bool runCode(const char* code);

        /**
         * Is the given command comment or only whitespace?
         */
        static bool isEmptyCommand(const std::string& command);

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

inline PrefixCompleter::PrefixCompleter() : initialised_(false) {
}

inline const std::string& PrefixCompleter::prefix() const {
    return prefix_;
}

inline bool PythonInterpreter::exitAttempted() const {
    return caughtSystemExit;
}

} } // namespace regina::python

#endif
