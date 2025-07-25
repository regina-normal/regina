
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "regina-config.h"
#include "file/globaldirs.h"
#include "packet/container.h"
#include "packet/script.h"

#include "pythoninterpreter.h"
#include "pythonoutputstream.h"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>

// Python includes:
#include <compile.h>
#include <sysmodule.h>

#ifdef PYTHON_STATIC_LINK
    // Regina's python module is statically linked into the executable
    // (as opposed to dynamically loaded in the usual way by the python
    // import mechanism).

    // Declare the entry point for Regina's python module:
    PyMODINIT_FUNC PyInit_regina();
#endif

// Convert the Python version x.y into the form "x" "y":
#define REGINA_MAKE_STR(x) #x
#define REGINA_STR(x) REGINA_MAKE_STR(x)
#define REGINA_PY_VERSION REGINA_STR(PY_MAJOR_VERSION) REGINA_STR(PY_MINOR_VERSION)

/**
 * WARNING: We never call Py_Finalize().
 *
 * It can't be called during the program since multiple
 * initialise/finalise sequences cause problems with our python modules.
 *
 * It can't be called during global object destruction since this seems to
 * be too late and a crash results (PyThreadState_Get: no current thread).
 *
 * Ideally it is called at program exit, before the destruction of
 * global objects.  This is a hassle though, and since there should be
 * no external resource usage and since memory leaks are reclaimed by the
 * operating system, we simply choose to ignore the problem.
 */

namespace regina::python {

#if PY_VERSION_HEX >= 0x030a0300
// Unfortunately python versions 3.10.[0-2] are broken: they reject incomplete
// code (e.g., bracketed lists that span multiple lines) as a syntax error.
// This is fixed with the PyCF_DONT_IMPLY_DEDENT flag that was introduced
// in python 3.10.3.
static PyCompilerFlags pyCompFlags =
    { PyCF_DONT_IMPLY_DEDENT | PyCF_ALLOW_INCOMPLETE_INPUT };
#else
// Python 3.9 and earlier used a different method for compilation, which
// does not have such problems.
static PyCompilerFlags pyCompFlags = { PyCF_DONT_IMPLY_DEDENT };
#endif

bool PythonInterpreter::pythonInitialised = false;

#if REGINA_PYBIND11_VERSION == 3
std::mutex PythonInterpreter::initMutex;
#elif REGINA_PYBIND11_VERSION == 2
std::mutex PythonInterpreter::interpreterMutex;
PyThreadState* mainState;
#endif

#if REGINA_PYBIND11_VERSION == 3
PythonInterpreter::PythonInterpreter(
        regina::python::PythonOutputStream& pyStdOut,
        regina::python::PythonOutputStream& pyStdErr,
        bool fixPythonPath) :
        caughtSystemExit(false),
        output(pyStdOut), errors(pyStdErr),
        completer(nullptr), completerFunc(nullptr) {
    std::lock_guard<std::mutex> lock(initMutex);

    // Acquire the global interpreter lock.
    if (! pythonInitialised) {
#ifdef PYTHON_CORE_IN_ZIP
        if (fixPythonPath) {
            // Regina is shipping its own copy of python, which means the
            // core python libraries are bundled as a zip file.
            //
            // We need to manually include the python zip and the path to
            // zlib.pyd on the python path, *before* the first interpreter
            // is initialised.
            //
            // Here we assume that pythonXY.zip and zlib.pyd are installed
            // in the same directory as the regina python module.

            const char* oldPath = getenv("PYTHONPATH");
            std::string newPath("PYTHONPATH=");
            newPath += regina::GlobalDirs::pythonModule();
            #if defined(REGINA_INSTALL_WINDOWS)
                newPath += ";";
                newPath += regina::GlobalDirs::pythonModule();
                newPath += "/python" REGINA_PY_VERSION ".zip;";
            #else
                newPath += ":";
                newPath += regina::GlobalDirs::pythonModule();
                newPath += "/python" REGINA_PY_VERSION ".zip:";
            #endif

            if (oldPath)
                newPath += oldPath;

            putenv(strdup(newPath.c_str()));
        }
#endif

#ifdef PYTHON_STATIC_LINK
        // Regina's python module is statically linked into the GUI; it
        // is not shipped as a separate module on the filesystem.
        // Tell python how to find it.
        if (PyImport_AppendInittab("regina", &PyInit_regina) == -1) {
            errors.write("ERROR: PyImport_AppendInittab(\"regina\", ...) "
                "failed.\n");
            errors.flush();
        }
#endif

        // Create the main interpreter.
        //
        // Currently we _never_ call pybind11::finalize_interpreter(), even
        // when the entire program is exiting and static variables are being
        // destroyed.  This is because, at present, this is causing a crash
        // somewhere inside the pybind11 internals.  (It appears as a
        // UnicodeDecodeError - "'utf-8' codec can't decode byte 0xe9 in
        // position 0: invalid continuation byte" - however, it looks like a
        // bad pointer access within pybind11::detail::internals_pp_manager.)
        //
        // At some point it would be good to understand whether the crash is
        // due to pybind11 or due to my own misuse of pybind11.  In the
        // meantime, since this is only relevant when the program is exiting,
        // this should be relatively harmless.
        pybind11::initialize_interpreter();

        // In Python 3.13 (but not Python 3.12), the first subinterpreter
        // (but only the first) is unable to see the pybind11 conversion table.
        // I have no idea why, and this definitely needs looking into.
        // In the meantime, a workaround is to load regina's extension module
        // here in the main interpreter also.
        //
        // If this import fails, do so silently; we'll see the same error
        // again immediately in the first subinterpreter.
        importReginaIntoNamespace(PyModule_GetDict(PyImport_AddModule(
            "__main__")), fixPythonPath);
    }

    subInterpreter = pybind11::subinterpreter::create();
    pybind11::subinterpreter_scoped_activate guard(subInterpreter);

    // Record the main module.
    mainModule = PyImport_AddModule("__main__"); // Borrowed ref.
    mainNamespace = PyModule_GetDict(mainModule); // Borrowed ref.

    // Redirect stdout and stderr if appropriate.
    try {
        // For pybind11 v3, we need to bind PythonOutputStream in every
        // subinterpreter.
        regina::python::PythonOutputStream::addBindings();
        pyStdOut.install("stdout");
        pyStdErr.install("stderr");
    } catch (std::runtime_error& e) {
        pyStdErr.write("ERROR: Could not redirect output streams: ");
        pyStdErr.write(e.what());
        pyStdErr.write("\n");
        pyStdErr.flush();
    }

    pythonInitialised = true;
}

PythonInterpreter::~PythonInterpreter() {
    pybind11::subinterpreter_scoped_activate guard(subInterpreter);

    Py_XDECREF(completer);
    Py_XDECREF(completerFunc);
}
#elif REGINA_PYBIND11_VERSION == 2
PythonInterpreter::PythonInterpreter(
        regina::python::PythonOutputStream& pyStdOut,
        regina::python::PythonOutputStream& pyStdErr,
        bool fixPythonPath) :
        caughtSystemExit(false),
        output(pyStdOut), errors(pyStdErr),
        completer(nullptr), completerFunc(nullptr) {
    std::lock_guard<std::mutex> lock(interpreterMutex);

    // Acquire the global interpreter lock.
    if (pythonInitialised)
        PyEval_AcquireThread(mainState);
    else {
#ifdef PYTHON_CORE_IN_ZIP
        if (fixPythonPath) {
            // Regina is shipping its own copy of python, which means the
            // core python libraries are bundled as a zip file.
            //
            // We need to manually include the python zip and the path to
            // zlib.pyd on the python path, *before* the first interpreter
            // is initialised.
            //
            // Here we assume that pythonXY.zip and zlib.pyd are installed
            // in the same directory as the regina python module.

            const char* oldPath = getenv("PYTHONPATH");
            std::string newPath("PYTHONPATH=");
            newPath += regina::GlobalDirs::pythonModule();
            #if defined(REGINA_INSTALL_WINDOWS)
                newPath += ";";
                newPath += regina::GlobalDirs::pythonModule();
                newPath += "/python" REGINA_PY_VERSION ".zip;";
            #else
                newPath += ":";
                newPath += regina::GlobalDirs::pythonModule();
                newPath += "/python" REGINA_PY_VERSION ".zip:";
            #endif

            if (oldPath)
                newPath += oldPath;

            putenv(strdup(newPath.c_str()));
        }
#endif

#ifdef PYTHON_STATIC_LINK
        // Regina's python module is statically linked into the GUI; it
        // is not shipped as a separate module on the filesystem.
        // Tell python how to find it.
        if (PyImport_AppendInittab("regina", &PyInit_regina) == -1) {
            errors.write("ERROR: PyImport_AppendInittab(\"regina\", ...) "
                "failed.\n");
            errors.flush();
        }
#endif

        // We call pybind11::initialize_interpreter() instead of calling
        // Py_Initialize() directly, since this allows pybind11 to do some of
        // its own internal setup also.  For now we will never call
        // pybind11::finalize_interpreter(), since we don't know when we will
        // or will not need more subinterpreters.  Probably it would be good
        // to fix this.
        pybind11::initialize_interpreter();

        // Subinterpreters are supposed to share extension modules
        // without repeatedly calling the modules' init functions.
        // In python 3, this seems to fail if all subinterpreters are
        // destroyed, unless we keep the extension module loaded here in
        // the main interpreter also.
        //
        // If this import fails, do so silently; we'll see the same error
        // again immediately in the first subinterpreter.
        importReginaIntoNamespace(PyModule_GetDict(PyImport_AddModule(
            "__main__")), fixPythonPath);

        // With pybind11, it seems that we need to *use* regina's bindings in
        // the first interpreter that imports them; otherwise subsequent
        // subinterpreters cannot see the pybind11 conversion table.
        //
        // Note: the temporary packet that we create here will be
        // destroyed with the pybind11::object destructor.
        pybind11::cast(new regina::Container());

        mainState = PyThreadState_Get();
    }

    // Create the new interpreter and note the thread that it should be used
    // with.
    state = Py_NewInterpreter();
    thread = std::this_thread::get_id();

    // Record the main module.
    mainModule = PyImport_AddModule("__main__"); // Borrowed ref.
    mainNamespace = PyModule_GetDict(mainModule); // Borrowed ref.

    // Redirect stdout and stderr if appropriate.
    try {
        // For pybind11 v2, we only need to bind PythonOutputStream once.
        if (! pythonInitialised)
            regina::python::PythonOutputStream::addBindings();
        pyStdOut.install("stdout");
        pyStdErr.install("stderr");
    } catch (std::runtime_error& e) {
        pyStdErr.write("ERROR: Could not redirect output streams: ");
        pyStdErr.write(e.what());
        pyStdErr.write("\n");
        pyStdErr.flush();
    }

    pythonInitialised = true;

    // Release the global interpreter lock.
    PyEval_SaveThread();
}

PythonInterpreter::~PythonInterpreter() {
    std::lock_guard<std::mutex> lock(interpreterMutex);

    // Acquire the global interpreter lock.
    PyEval_RestoreThread(state);

    // Destroy the interpreter.
    Py_XDECREF(completer);
    Py_XDECREF(completerFunc);
    Py_EndInterpreter(state);

    // Return to the main thread and release the global interpreter lock.
    // Note: Just calling PyEval_ReleaseLock() crashes python3.9 on debian
    // (and PyEval_ReleaseLock() is now deprecated anyway).
    PyThreadState_Swap(mainState);
    PyEval_ReleaseThread(mainState);
}
#endif

bool PythonInterpreter::executeLine(const std::string& command) {
    /**
     * Part of the code for this routine is based upon pyinterp.cpp by
     * Anthony Martin <amartin@varlog.com>.  The file pyinterp.cpp is
     * licensed under the wxWindows License, which is essentially LGPL
     * with an exception regarding binary object code.  Note that this
     * exception only applies to the original pyinterp.cpp, and is not
     * offered for the PythonInterpreter source code listed here.
     */

    // Join the new command with any previous code.
    std::string fullCommand = currentCode + command;

    // Is it an empty command?
    if (isEmptyCommand(fullCommand)) {
        currentCode.clear();
        return true;
    }

    // Create a char* version of the command to which we might choose to
    // add one or two additional newlines.
    char* cmdBuffer = new char[fullCommand.length() + 3];
    strcpy(cmdBuffer, fullCommand.c_str());

#if REGINA_PYBIND11_VERSION == 3
    pybind11::subinterpreter_scoped_activate guard(subInterpreter);
#elif REGINA_PYBIND11_VERSION == 2
    // Acquire the global interpreter lock.
    ScopedThreadRestore pyThread(*this);
#endif

    // Attempt to compile the command with no additional newlines.
    PyObject* code = Py_CompileStringFlags(
        cmdBuffer, "<console>", Py_single_input, &pyCompFlags);
    if (code) {
        // Run the code!
#if PY_VERSION_HEX >= 0x03020000
        PyObject* ans = PyEval_EvalCode(code, mainNamespace, mainNamespace);
#else
        PyObject* ans = PyEval_EvalCode((PyCodeObject*)code,
            mainNamespace, mainNamespace);
#endif
        if (ans)
            Py_DECREF(ans);
        else {
            // If the user called exit(), this will have thrown a SystemExit
            // exception, which would cause PyErr_Print() to terminate *this*
            // process (the Regina GUI).
            if(PyErr_ExceptionMatches(PyExc_SystemExit)) {
                caughtSystemExit = true;
            } else {
                PyErr_Print();
            }
            PyErr_Clear();
        }

        // Clean up.
        Py_DECREF(code);

        delete[] cmdBuffer;
        currentCode.clear();
        return true;
    }

    // Clear the error so it doesn't stay with the thread state and
    // cause wacky syntax errors later on.  Such behaviour has been
    // observed on macosx/fink.
    PyErr_Clear();

    // Attempt to compile the command with final newline.
    cmdBuffer[fullCommand.length()] = '\n';
    cmdBuffer[fullCommand.length() + 1] = 0;

    code = Py_CompileStringFlags(
        cmdBuffer, "<console>", Py_single_input, &pyCompFlags);
    if (code) {
        // We're waiting on more code.
        Py_DECREF(code);

        delete[] cmdBuffer;
        currentCode = currentCode + command + '\n';
        return false;
    }

    // Extract the full error details in case we wish to display them later.
    // Note that PyErr_Fetch() has the side-effect of clearing the error also.
    PyObject *errType, *errValue, *errTrace;
    PyErr_Fetch(&errType, &errValue, &errTrace);

    PyObject* errStr1 = PyObject_Str(errValue);

    // Attempt to compile the command with two final newlines.
    cmdBuffer[fullCommand.length() + 1] = '\n';
    cmdBuffer[fullCommand.length() + 2] = 0;

    code = Py_CompileStringFlags(
        cmdBuffer, "<console>", Py_single_input, &pyCompFlags);
    if (code) {
        // We're waiting on more code.
        Py_DECREF(code);
        Py_XDECREF(errType);
        Py_XDECREF(errValue);
        Py_XDECREF(errTrace);
        Py_XDECREF(errStr1);

        delete[] cmdBuffer;
        currentCode = currentCode + command + '\n';
        return false;
    }

    PyObject* errStr2 = extractErrMsg(); // Also clears the error.

    // Compare the two compile errors.
    if (errStr1 && errStr2) {
        // Note: rich comparison returns -1 on error, or 0/1 for false/true.
        // Since we are passing two python strings, we assume no error here.
        if (PyObject_RichCompareBool(errStr1, errStr2, Py_NE) == 1) {
            // Errors are different.  We must be waiting on more code.
            Py_XDECREF(errType);
            Py_XDECREF(errValue);
            Py_XDECREF(errTrace);
            Py_DECREF(errStr1);
            Py_DECREF(errStr2);

            delete[] cmdBuffer;
            currentCode = currentCode + command + '\n';
            return false;
        } else {
            // Errors are the same.  It must be a real compile error.
            PyErr_Restore(errType, errValue, errTrace);
            PyErr_Print();
            PyErr_Clear();

            Py_DECREF(errStr1);
            Py_DECREF(errStr2);

            delete[] cmdBuffer;
            currentCode.clear();
            return true;
        }
    } else {
        // This shouldn't happen either.
        Py_XDECREF(errType);
        Py_XDECREF(errValue);
        Py_XDECREF(errTrace);
        Py_XDECREF(errStr1);
        Py_XDECREF(errStr2);

        errors.write("ERROR: Python compile error details "
            "are not available.\n");
        errors.write("Please report this to the authors, "
            "since this should never occur.\n");
        errors.flush();

        delete[] cmdBuffer;
        currentCode.clear();
        return true;
    }
}

void PythonInterpreter::prependReginaToSysPath() {
    std::string regModuleDir = regina::GlobalDirs::pythonModule();
    if (! regModuleDir.empty()) {
        PyObject* path = PySys_GetObject(
            const_cast<char*>("path")); // Borrowed reference.
        if (path) {
            // Ensure that regina's path gets pushed to the beginning
            // of sys.path, not the end - this ensures that different
            // installations can live happily side-by-side.

            // Since this is a filesystem path, we assume it comes direct from
            // the filesystem, and is not necessary encoded using UTF-8.
            PyObject* regModuleDirPy =
                PyUnicode_DecodeFSDefault(regModuleDir.c_str());
            PyList_Insert(path, 0, regModuleDirPy);
            Py_DECREF(regModuleDirPy);
        }
    }
}

bool PythonInterpreter::importRegina(bool fixPythonPath) {
#if REGINA_PYBIND11_VERSION == 3
    pybind11::subinterpreter_scoped_activate guard(subInterpreter);
#elif REGINA_PYBIND11_VERSION == 2
    ScopedThreadRestore pyThread(*this);
#endif

    bool ok = importReginaIntoNamespace(mainNamespace, fixPythonPath);

    // Also set up a completer if we can, but if not then just fail silently.
    if (ok) {
        try {
            pybind11::object c = pybind11::module_::import(
                "regina.plainCompleter").attr("Completer")();
            pybind11::object f = c.attr("complete");

            if (c.ptr() && f.ptr()) {
                // Keep references to both until we destroy the interpreter.
                Py_INCREF(completer = c.ptr());
                Py_INCREF(completerFunc = f.ptr());
            }
        } catch (std::runtime_error&) {
        }
    } else {
        PyErr_Print();
        PyErr_Clear();
    }

    return ok;
}

bool PythonInterpreter::importReginaIntoNamespace(PyObject* useNamespace,
        bool fixPythonPath) {
    if (fixPythonPath) {
#ifndef PYTHON_STATIC_LINK
        // Adjust the python path if we need to.
        prependReginaToSysPath();
#endif
    }

#if REGINA_PYBIND11_VERSION == 3
    try {
        if (auto regina = pybind11::module_::import("regina")) {
            PyDict_SetItemString(useNamespace, "regina", regina.ptr());
            return true;
        } else {
            // Is this actually possible?
            // What I'm observing is that when the import fails, pybind11
            // throws an exception (which we catch below).
            return false;
        }
    } catch (const pybind11::error_already_set& err) {
        // Keep this diagnostic message here for now, so that we can see the
        // details of the Python exception.
        std::cerr << "Import failed: " << err.what() << std::endl;
        return false;
    }
#elif REGINA_PYBIND11_VERSION == 2
    // Import the module.
    PyObject* regModule = PyImport_ImportModule("regina"); // New ref.
    if (regModule) {
        PyDict_SetItemString(useNamespace, "regina", regModule);
        Py_DECREF(regModule);
        return true;
    } else {
        return false;
    }
#endif
}

bool PythonInterpreter::setVar(const char* name,
        std::shared_ptr<Packet> value) {
#if REGINA_PYBIND11_VERSION == 3
    pybind11::subinterpreter_scoped_activate guard(subInterpreter);
#elif REGINA_PYBIND11_VERSION == 2
    ScopedThreadRestore pyThread(*this);
#endif

    bool ok = false;
    try {
        pybind11::object obj = pybind11::cast(value);
        if (obj.ptr()) {
            // PyUnicode_FromString assumes UTF-8 encoding.
            PyObject* nameStr = PyUnicode_FromString(name); // New ref.
            if (PyDict_SetItem(mainNamespace, nameStr, obj.ptr())) {
                PyErr_Print();
                PyErr_Clear();
            } else
                ok = true;
            Py_DECREF(nameStr);
        } else {
            errors.write("ERROR: Null PyObject\n");
            errors.flush();
        }
    } catch (std::runtime_error& e) {
        errors.write("ERROR: ");
        errors.write(e.what());
        errors.write("\n");
        errors.flush();
    }

    return ok;
}

bool PythonInterpreter::runCode(const char* code) {
#if REGINA_PYBIND11_VERSION == 3
    pybind11::subinterpreter_scoped_activate guard(subInterpreter);
#elif REGINA_PYBIND11_VERSION == 2
    ScopedThreadRestore pyThread(*this);
#endif

    PyObject* ans = PyRun_String(const_cast<char*>(code), Py_file_input,
        mainNamespace, mainNamespace);
    if (ans) {
        Py_DECREF(ans);
        return true;
    } else {
        // If the user called exit(), this will have thrown a SystemExit
        // exception, which would cause PyErr_Print() to terminate *this*
        // process (the Regina GUI).
        if(PyErr_ExceptionMatches(PyExc_SystemExit)) {
            caughtSystemExit = true;
        } else {
            PyErr_Print();
        }
        PyErr_Clear();
        return false;
    }
}

bool PythonInterpreter::runScript(const regina::Script* script) {
    bool result = true;
    for (size_t i = 0; i < script->countVariables(); ++i)
        if (! setVar(script->variableName(i).c_str(), script->variableValue(i)))
            result = false;

    if (! runCode((script->text() + "\n\n").c_str()))
        result = false;

    output.flush();
    errors.flush();

    return result;
}

bool PythonInterpreter::isEmptyCommand(const std::string& command) {
    for (char c : command) {
        if (isspace(c))
            continue;
        else if (c == '#')
            return true;
        else
            return false;
    }
    return true;
}

PyObject* PythonInterpreter::extractErrMsg() {
    PyObject *errType, *errValue, *errTrace, *ans;

    PyErr_Fetch(&errType, &errValue, &errTrace);
    ans = PyObject_Str(errValue);

    Py_XDECREF(errType);
    Py_XDECREF(errValue);
    Py_XDECREF(errTrace);
    return ans;
}

void PythonInterpreter::flush() {
    output.flush();
    errors.flush();
}

int PythonInterpreter::complete(const std::string& text, PythonCompleter& c) {
    if (! completerFunc)
        return -1;

#if REGINA_PYBIND11_VERSION == 3
    pybind11::subinterpreter_scoped_activate guard(subInterpreter);
#elif REGINA_PYBIND11_VERSION == 2
    ScopedThreadRestore pyThread(*this);
#endif

    try {
        pybind11::handle func(completerFunc);
        int which = 0;
        while (true) {
            pybind11::object ans = func(text, which);
            if (ans.is_none()) {
                // No more completions available.
                return which;
            }
            ++which;
            if (! c.addCompletion(pybind11::cast<std::string>(ans))) {
                // The PythonCompleter object does not want more completions.
                return which;
            }
        }
    } catch (std::runtime_error& e) {
        return -1;
    }
}

bool PrefixCompleter::addCompletion(const std::string& s) {
    if (! initialised_) {
        initialised_ = true;
        prefix_ = s;
    } else {
        // Find the longest common prefix between prefix_ and s.
        auto pit = prefix_.begin();
        auto sit = s.begin();

        while (pit != prefix_.end() && sit != s.end()) {
            if (*pit != *sit)
                break;

            // The first bytes are the same.
            // However, we need to compare unicode characters, not bytes.

            if (! (*pit & 0x80)) {
                // 0xxxxxxx: 1-byte code.
                // These are equal and valid unicode characters.
                ++pit; ++sit;
                continue;
            }

            // We have a multi-byte unicode character.
            // Leave pit pointing at the first byte until we're
            // satisfied that both unicode characters are valid and equal.

            if (! (*pit & 0x40)) {
                // 10xxxxxx: Invalid code
                break;
            }

            auto tmp = pit;
            ++tmp; ++sit; // move to second byte

            // Check the second byte.
            if (tmp == prefix_.end() || sit == s.end() || // incomplete
                    *tmp != *sit || // unequal unicode characters
                    (*tmp & 0xC0) != 0x80 || (*sit & 0xC0) != 0x80) // invalid
                break;

            ++tmp; ++sit; // move to third byte

            if (! (*pit & 0x20)) {
                // 110xxxxx: 2-byte codes, and they are valid and equal.
                pit = tmp;
                continue;
            }

            // Check the third byte.
            if (tmp == prefix_.end() || sit == s.end() || // incomplete
                    *tmp != *sit || // unequal unicode characters
                    (*tmp & 0xC0) != 0x80 || (*sit & 0xC0) != 0x80) // invalid
                break;

            ++tmp; ++sit; // move to fourth byte

            if (! (*pit & 0x10)) {
                // 1110xxxx: 3-byte codes, and they are valid and equal.
                pit = tmp;
                continue;
            }

            // Check the fourth byte.
            if (tmp == prefix_.end() || sit == s.end() || // incomplete
                    *tmp != *sit || // unequal unicode characters
                    (*tmp & 0xC0) != 0x80 || (*sit & 0xC0) != 0x80) // invalid
                break;

            ++tmp; ++sit; // move beyond the fourth byte

            if (! (*pit & 0x08)) {
                // 11110xxx: 4-byte codes, and they are valid and equal.
                pit = tmp;
                continue;
            }

            // 11111xxx: Invalid codes!
            break;
        }

        prefix_.erase(pit, prefix_.end());
    }
    return ! prefix_.empty();
}

} // namespace regina::python
