
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
#include "../python/pybind11/pybind11.h"
#include <compile.h>
#include <eval.h>
#include <sysmodule.h>

#ifdef PYTHON_STATIC_LINK
    // Regina's python module is statically linked into the executable
    // (as opposed to dynamically loaded in the usual way by the python
    // import mechanism).

    // Declare the entry point for Regina's python module:
    #if PY_MAJOR_VERSION >= 3
        #define REGINA_PYTHON_INIT PyInit_regina
    #else
        #define REGINA_PYTHON_INIT initregina
    #endif
    PyMODINIT_FUNC REGINA_PYTHON_INIT();
#endif

// Convert the Python version x.y into the form "x" "y":
#define REGINA_MAKE_STR(x) #x
#define REGINA_STR(x) REGINA_MAKE_STR(x)
#define REGINA_PY_VERSION REGINA_STR(PY_MAJOR_VERSION) REGINA_STR(PY_MINOR_VERSION)

#ifdef PYTHON_STATIC_LINK
    // Regina's python module is statically linked into the executable
    // (as opposed to dynamically loaded in the usual way by the python
    // import mechanism).

    // Declare the entry point for Regina's python module:
    #if PY_MAJOR_VERSION >= 3
        #define REGINA_PYTHON_INIT PyInit_regina
    #else
        #define REGINA_PYTHON_INIT initregina
    #endif
    PyMODINIT_FUNC REGINA_PYTHON_INIT();
#endif

// Convert the Python version x.y into the form "x" "y":
#define REGINA_MAKE_STR(x) #x
#define REGINA_STR(x) REGINA_MAKE_STR(x)
#define REGINA_PY_VERSION REGINA_STR(PY_MAJOR_VERSION) REGINA_STR(PY_MINOR_VERSION)

/**
 * WARNING: We never call Py_Finalize().
 *
 * It can't be called during the program since multiple
 * initialise/finalise sequences cause problems with boost.python
 * modules.
 *
 * It can't be called during global object destruction since this seems to
 * be too late and a crash results (PyThreadState_Get: no current thread).
 *
 * Ideally it is called at program exit, before the destruction of
 * global objects.  This is a hassle though, and since there should be
 * no external resource usage and since memory leaks are reclaimed by the
 * operating system, we simply choose to ignore the problem.
 */

namespace regina {
namespace python {

static PyCompilerFlags pyCompFlags = { PyCF_DONT_IMPLY_DEDENT };

std::mutex PythonInterpreter::globalMutex;
bool PythonInterpreter::pythonInitialised = false;
PyThreadState* mainState;

PythonInterpreter::PythonInterpreter(
        regina::python::PythonOutputStream& pyStdOut,
        regina::python::PythonOutputStream& pyStdErr) :
        output(pyStdOut), errors(pyStdErr) {
    std::lock_guard<std::mutex> lock(globalMutex);

    // Acquire the global interpreter lock.
    if (pythonInitialised)
        PyEval_AcquireThread(mainState);
    else {
#ifdef PYTHON_CORE_IN_ZIP
        // Regina is shipping its own copy of python, which means the
        // core python libraries are bundled as a zip file.
        //
        // We need to manually include the python zip and the path to zlib.pyd
        // on the python path, *before* the first interpreter is initialised.
        //
        // Here we assume that pythonXY.zip and zlib.pyd are installed
        // in the same directory as the regina python module.

        const char* oldPath = getenv("PYTHONPATH");
        std::string newPath("PYTHONPATH=");
        newPath += regina::GlobalDirs::pythonModule();
        #if defined(REGINA_INSTALL_WINDOWS)
            newPath += "/python" REGINA_PY_VERSION ".zip;";
        #else
            newPath += "/python" REGINA_PY_VERSION ".zip:";
        #endif
        
        if (oldPath)
            newPath += oldPath;

        putenv(strdup(newPath.c_str()));
#endif

#ifdef PYTHON_STATIC_LINK
        // Regina's python module is statically linked into the GUI; it
        // is not shipped as a separate module on the filesystem.
        // Tell python how to find it.
        if (PyImport_AppendInittab("regina", &REGINA_PYTHON_INIT) == -1) {
            errors.write("ERROR: PyImport_AppendInittab(\"regina\", ...) "
                "failed.\n");
            errors.flush();
        }
#endif

        Py_Initialize();
        PyEval_InitThreads();
        mainState = PyThreadState_Get();

#if PY_MAJOR_VERSION >= 3
        // Subinterpreters are supposed to share extension modules
        // without repeatedly calling the modules' init functions.
        // In python 3, this seems to fail if all subinterpreters are
        // destroyed, unless we keep the extension module loaded here in
        // the main interpreter also.
        //
        // If this fails, do so silently; we'll see the same error again
        // immediately in the first subinterpreter.
        importReginaIntoNamespace(PyModule_GetDict(PyImport_AddModule(
            "__main__")));

        // With pybind11, it seems that we need to *use* regina's bindings in
        // the first interpreter that imports them; otherwise subsequent
        // subinterpreters cannot see the pybind11 conversion table.
        //
        // Note: the temporary packet that we create here will be
        // destroyed with the pybind11::object destructor.
        pybind11::cast(new regina::Container());
#endif
    }

    // Create the new interpreter.
    state = Py_NewInterpreter();

    // Record the main module.
    mainModule = PyImport_AddModule("__main__"); // Borrowed ref.
    mainNamespace = PyModule_GetDict(mainModule); // Borrowed ref.

    // Redirect stdout and stderr if appropriate.
    try {
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
    std::lock_guard<std::mutex> lock(globalMutex);

    // Acquire the global interpreter lock.
    PyEval_RestoreThread(state);

    // Destroy the interpreter.
    Py_EndInterpreter(state);

    // Release the global interpreter lock.
    PyEval_ReleaseLock();
}

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

    // Acquire the global interpreter lock.
    PyEval_RestoreThread(state);

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
            PyErr_Print();
            PyErr_Clear();
        }

        // Clean up.
        Py_DECREF(code);
        state = PyEval_SaveThread();

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
        state = PyEval_SaveThread();

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
        state = PyEval_SaveThread();

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
            state = PyEval_SaveThread();

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
            state = PyEval_SaveThread();

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
        state = PyEval_SaveThread();

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
#if PY_MAJOR_VERSION >= 3
            PyObject* regModuleDirPy =
                PyUnicode_DecodeFSDefault(regModuleDir.c_str());
#else
            PyObject* regModuleDirPy =
                PyString_FromString(regModuleDir.c_str());
#endif
            PyList_Insert(path, 0, regModuleDirPy);
            Py_DECREF(regModuleDirPy);
        }
    }
}

bool PythonInterpreter::importRegina() {
    PyEval_RestoreThread(state);

    bool ok = importReginaIntoNamespace(mainNamespace);
    if (! ok) {
        PyErr_Print();
        PyErr_Clear();
    }

    state = PyEval_SaveThread();
    return ok;
}

bool PythonInterpreter::importReginaIntoNamespace(PyObject* useNamespace) {
#ifndef PYTHON_STATIC_LINK
    // Adjust the python path if we need to.
    prependReginaToSysPath();
#endif

    // Import the module.
    PyObject* regModule = PyImport_ImportModule("regina"); // New ref.
    if (regModule) {
        PyDict_SetItemString(useNamespace, "regina", regModule);
        Py_DECREF(regModule);
        return true;
    } else {
        return false;
    }
}

bool PythonInterpreter::setVar(const char* name, regina::Packet* value) {
    PyEval_RestoreThread(state);

    bool ok = false;
    try {
        pybind11::object obj = pybind11::cast(value);
        if (obj.ptr()) {
#if PY_MAJOR_VERSION >= 3
            // PyUnicode_FromString assumes UTF-8 encoding.
            PyObject* nameStr = PyUnicode_FromString(name); // New ref.
#else
            PyObject* nameStr = PyString_FromString(name); // New ref.
#endif
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

    state = PyEval_SaveThread();
    return ok;
}

bool PythonInterpreter::runCode(const char* code) {
    PyEval_RestoreThread(state);

    PyObject* ans = PyRun_String(const_cast<char*>(code), Py_file_input,
        mainNamespace, mainNamespace);
    if (ans) {
        Py_DECREF(ans);
        state = PyEval_SaveThread();
        return true;
    } else {
        PyErr_Print();
        PyErr_Clear();
        state = PyEval_SaveThread();
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
    for (auto it = command.begin(); it != command.end(); it++) {
        if (isspace(*it))
            continue;
        else if (*it == '#')
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

} } // namespace regina::python
