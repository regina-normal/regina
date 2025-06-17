
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  It is _not_ built into Regina's python module.
 */

#ifndef __PYTHONINTERPRETER_H
#define __PYTHONINTERPRETER_H

#include "regina-config.h" // Required for REGINA_PYBIND11_VERSION

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#if REGINA_PYBIND11_VERSION == 3
#include <pybind11/subinterpreter.h>
#endif
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include "utilities/exception.h"

namespace regina {
    class Packet;
    class Script;
}

namespace regina::python {

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
         * \param s the new completion.  This string will begin with the
         * original text that was passed to PythonCompleter::complete().
         * \return \c true to indicate that PythonCompleter::complete()
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
         * \return the longest common prefix for all completions.
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
#if REGINA_PYBIND11_VERSION == 3
        static std::mutex initMutex;
            /**< A mutual exclusion device to protect the creation of the main
                 interpreter (which happens only on demand). */
        static std::optional<pybind11::scoped_interpreter> mainInterpreter;
            /**< The main python interpreter, or std::nullopt if the python
                 system has not yet been initialised. */
        pybind11::subinterpreter subInterpreter;
            /**< This specific subinterpreter. */
#elif REGINA_PYBIND11_VERSION == 2
        static std::mutex interpreterMutex;
            /**< A mutual exclusion device for the creation and
                 destruction of subinterpreters. */
        static bool pythonInitialised;
            /**< Has the python system been initialised yet? */
        std::thread::id thread;
            /**< The ID of the C++ thread to be used exclusively with this
                 subinterpreter. */
        PyThreadState* state;
            /**< The first thread state created in this particular
                 subinterpreter. */
#else
        #error "Unsupported pybind11 version"
#endif

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
         * Constructor.
         *
         * In the constructor, if \a fixPythonPath is \c true then:
         *
         * - if Regina ships its own copy of Python, then this constructor
         *   will adjust the Python path so that Regina's copy of the core
         *   Python libraries can be found;
         *
         * - if necessary, this constructor will also adjust the Python path
         *   so that Regina's own module can be found.
         *
         * If \a fixPythonPath is \c false then the Python path will not be
         * touched at all.
         */
        PythonInterpreter(
            regina::python::PythonOutputStream& pyStdOut,
            regina::python::PythonOutputStream& pyStdErr,
            bool fixPythonPath = true);

        /**
         * Destructor.
         *
         * \pre This is called from the same C++ thread that created
         * this Python interpreter.
         */
        ~PythonInterpreter();

        /**
         * Execute a single line of code in the subinterpreter.
         * Returns \c true if no further input is required, or
         * \c false if the interpreter is waiting on further lines of code.
         *
         * This is intended for use in an interactive Python session.
         *
         * You should always test exitAttempted() after executing user code.
         *
         * \pre This is called from the same C++ thread that created
         * this Python interpreter.
         */
        bool executeLine(const std::string& command);

        /**
         * Imports Regina's python module.
         *
         * This function also sets up a completer object, for use with
         * complete().
         *
         * If \a fixPythonPath is \c true, then this routine will (before the
         * import) adjust the Python path so that Regina's module can be found.
         *
         * \pre This is called from the same C++ thread that created
         * this Python interpreter.
         *
         * \return \c true on success or \c false on failure.
         */
        bool importRegina(bool fixPythonPath = true);

        /**
         * Set the given variable in Python's main namespace to
         * represent the given Regina packet.
         *
         * \pre This is called from the same C++ thread that created
         * this Python interpreter.
         *
         * \return \c true on success or \c false on failure.
         */
        bool setVar(const char* name, std::shared_ptr<Packet> value);

        /**
         * Run the given script packet in Python's main namespace.
         * This involves setting all of the script variables, and then running
         * the script code itself.
         *
         * This routine flushes standard output and standard error.
         *
         * You should always test exitAttempted() after executing user code.
         *
         * \pre This is called from the same C++ thread that created
         * this Python interpreter.
         *
         * \return \c true on success or \c false on failure.
         */
        bool runScript(const regina::Script* script);

        /**
         * Flushes the standard output and error streams.
         *
         * This may be called regardless of whether or not the Python
         * global interpreter lock is currently held.
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
         * This does _not_ capture os._exit(), which will instead
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
         * \pre This is called from the same C++ thread that created
         * this Python interpreter.
         *
         * \param text the Python text to complete.
         * \param completer the callback object that will receive the
         * resulting completions (if any).
         * \return the number of completions that were passed to \a completer,
         * or -1 if the completion process failed (e.g., the \c plainCompleter
         * Python module could not be imported).  In particular, if the
         * completion process ran succesfully and determined that the given
         * text has no completions at all, this routine will return 0.
         */
        int complete(const std::string& text, PythonCompleter& completer);

    private:
#if REGINA_PYBIND11_VERSION == 2
        /**
         * Ensures that the current C++ thread is in fact the thread that
         * created this subinterpreter.
         *
         * When this function is called, it is typically verifying a
         * precondition to some other member function of PythonInterpreter.
         * Therefore the behaviour when this check _fails_ is subject to
         * change in future versions of Regina (since in theory, it should
         * never happen).
         */
        void verifyThread() const;
#endif

        /**
         * Run the given Python code in Python's main namespace.
         *
         * \pre This is called from the same C++ thread that created
         * this Python interpreter.
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
         * If \a fixPythonPath is \c true, then this routine will (before the
         * import) adjust the Python path so that Regina's module can be found.
         *
         * Returns \c true on success or \c false on failure.
         */
        static bool importReginaIntoNamespace(PyObject* useNamespace,
            bool fixPythonPath = true);

#if REGINA_PYBIND11_VERSION == 2
        /**
         * An object that calls PyEval_RestoreThread() on construction
         * and PyEval_SaveThread() on destruction.  This manages both
         * (i) the Python thread state for the subinterpreter being used,
         * and also (ii) the Python global interpreter lock.
         *
         * This class insists that the same C++ that created the Python
         * subinterpreter is also used for both construction and destruction
         * of the ScopedThreadRestore object.
         *
         * An object of this type cannot be copied or moved.
         */
        class ScopedThreadRestore {
            private:
                PythonInterpreter& interpreter_;
                    /**< The Python subinterpreter being used. */

            public:
                /**
                 * Restores the primary Python thread for the given Python
                 * subinterpreter.
                 *
                 * \pre This constructor is called from the same C++ thread
                 * that created the given interpreter.
                 */
                ScopedThreadRestore(PythonInterpreter& interpreter) :
                        interpreter_(interpreter) {
                    interpreter.verifyThread();
                    PyEval_RestoreThread(interpreter.state);
                }

                /**
                 * Saves the current Python thread.
                 *
                 * \pre This destructor is called from the same C++ thread
                 * as the corresponding constructor.
                 */
                ~ScopedThreadRestore() {
                    interpreter_.state = PyEval_SaveThread();
                }

                ScopedThreadRestore(const ScopedThreadRestore&) = delete;
                ScopedThreadRestore& operator = (const ScopedThreadRestore&)
                    = delete;
        };
#endif
};

inline PrefixCompleter::PrefixCompleter() : initialised_(false) {
}

inline const std::string& PrefixCompleter::prefix() const {
    return prefix_;
}

inline bool PythonInterpreter::exitAttempted() const {
    return caughtSystemExit;
}

#if REGINA_PYBIND11_VERSION == 2
inline void PythonInterpreter::verifyThread() const {
    if (std::this_thread::get_id() != thread) {
        /*
        std::cerr << "ERROR: A PythonInterpreter was used from the wrong "
            "C++ thread.\n\n"
            "Please report this to the Regina developers!\n"
            << std::endl;
        */
        throw regina::FailedPrecondition(
            "PythonInterpreter used from the wrong C++ thread");
    }
}
#endif

} // namespace regina::python

#endif
