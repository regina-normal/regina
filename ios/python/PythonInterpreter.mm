
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

#import "PythonInterpreter.h"
#import "file/globaldirs.h"
#import "packet/script.h"

// We need to include <ostream> before boost.python - otherwise we get a
// compile error when boost.python includes <ostream> itself.. *shrug*
#import <ostream>

// Python includes:
#import <Python.h>
#import <boost/python.hpp>

// For regina's to_held_type:
#import "../python/safeheldtype.h"

// Declare the entry point for Regina's python module:
PyMODINIT_FUNC initregina();

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

static bool pythonInitialised = false;
static PyCompilerFlags pyCompFlags = { PyCF_DONT_IMPLY_DEDENT };

#pragma mark Helper functions

bool isEmptyCommand(const std::string& command) {
    for (auto it = command.begin(); it != command.end(); ++it) {
        if (isspace(*it))
            continue;
        else if (*it == '#')
            return true;
        else
            return false;
    }
    return true;
}

PyObject* extractErrMsg() {
    PyObject *errType, *errValue, *errTrace, *ans;

    PyErr_Fetch(&errType, &errValue, &errTrace);
    ans = PyObject_Str(errValue);

    Py_XDECREF(errType);
    Py_XDECREF(errValue);
    Py_XDECREF(errTrace);
    return ans;
}

#pragma mark Log stream

@implementation PythonLogStream

- (void)processOutput:(const char*)data
{
    // We can't use %s with NSLog, since this does not default to UTF-8.
    // Instead we jump through some hoops to explicitly specify that data is UTF-8.
    NSLog(@"%@", [NSString stringWithUTF8String:data]);
}

@end

#pragma mark C++ wrapper

class PythonOutputStreamObjC {
    private:
        void* _object;
            /**< The Objective-C delegate that implements processOutput:. */
        std::string buffer;
            /**< The output data that has yet to be processed. */

    public:
        PythonOutputStreamObjC(void* object) : _object(object) {
        }

        /**
         * Writes data to this output stream.  Note that this data will
         * not be processed until a newline is written or flush() is
         * called.
         */
        void write(const std::string& data) {
            buffer.append(data);

            std::string::size_type pos;
            while ((pos = buffer.find('\n')) < buffer.length()) {
                [(__bridge id)_object processOutput:buffer.substr(0, pos + 1).c_str()];
                buffer.erase(0, pos + 1);
            }
        }

        /**
         * Forces any data that has not yet been processed to be
         * sent to processOutput().
         */
        void flush() {
            if (! buffer.empty()) {
                [(__bridge id)_object processOutput:buffer.c_str()];
                buffer.clear();
            }
        }
};

#pragma mark Python interpreter

@implementation PythonInterpreter {
    NSObject<PythonOutputStream>* _out;
    NSObject<PythonOutputStream>* _err;

    PythonOutputStreamObjC* _outCpp;
    PythonOutputStreamObjC* _errCpp;

    PyThreadState* _state;
    PyObject* _mainModule;
    PyObject* _mainNamespace;

    std::string _currentCode;
}

- (id)initWithOut:(id<PythonOutputStream>)out err:(id<PythonOutputStream>)err
{
    self = [super init];
    if (self) {
        if (out)
            _out = out;
        else
            _out = [[PythonLogStream alloc] init];

        if (err)
            _err = err;
        else
            _err = [[PythonLogStream alloc] init];

        _outCpp = new PythonOutputStreamObjC((__bridge void*)_out);
        _errCpp = new PythonOutputStreamObjC((__bridge void*)_err);

        if (pythonInitialised)
            PyEval_AcquireLock();
        else {
            // Make sure Python can find its own modules.
            std::string pyZipDir = regina::GlobalDirs::home() + "/python";

            std::string newPath("PYTHONPATH=");
            newPath += pyZipDir;
            newPath += "/python27.zip";

            putenv(strdup(newPath.c_str()));

            // Make sure Python can find Regina's module also.
            if (PyImport_AppendInittab("regina", &initregina) == -1)
                NSLog(@"ERROR: PyImport_AppendInittab(\"regina\", ...) failed.");

            // Go ahead and initialise Python.
            PyEval_InitThreads();
            Py_Initialize();
            pythonInitialised = true;
        }

        // Create the new interpreter.
        _state = Py_NewInterpreter();

        // Record the main module.
        _mainModule = PyImport_AddModule("__main__"); // Borrowed ref.
        _mainNamespace = PyModule_GetDict(_mainModule); // Borrowed ref.

        // Redirect stdout and stderr if appropriate.
        try {
            boost::python::class_<PythonOutputStreamObjC, boost::noncopyable>
                ("PythonOutputStream", boost::python::no_init)
                .def("write", &PythonOutputStreamObjC::write)
                .def("flush", &PythonOutputStreamObjC::flush);

            boost::python::reference_existing_object::apply<PythonOutputStreamObjC*>::type conv;

            PySys_SetObject(const_cast<char*>("stdout"), conv(_outCpp));
            PySys_SetObject(const_cast<char*>("stderr"), conv(_errCpp));
        } catch (const boost::python::error_already_set&) {
            PyErr_Print();
            PyErr_Clear();
        }

        // Release the global interpreter lock.
        PyEval_SaveThread();
    }
    return self;
}

- (void)dealloc
{
    NSLog(@"Python interpreter being deallocated");

    // Acquire the global interpreter lock.
    PyEval_RestoreThread(_state);

    // Destroy the interpreter.
    Py_EndInterpreter(_state);

    // Release the global interpreter lock.
    PyEval_ReleaseLock();

    // Delete C++ resources that are not managed by ARC.
    delete _outCpp;
    delete _errCpp;
}

- (bool)executeLine:(const char*)command
{
    /**
     * Part of the code for this routine is based upon pyinterp.cpp by
     * Anthony Martin <amartin@varlog.com>.  The file pyinterp.cpp is
     * licensed under the wxWindows License, which is essentially LGPL
     * with an exception regarding binary object code.  Note that this
     * exception only applies to the original pyinterp.cpp, and is not
     * offered for the PythonInterpreter source code listed here.
     */

    // Join the new command with any previous code.
    std::string fullCommand = _currentCode + command;

    // Is it an empty command?
    if (isEmptyCommand(fullCommand)) {
        _currentCode.clear();
        return true;
    }

    // Create a char* version of the command to which we might choose to
    // add one or two additional newlines.
    char* cmdBuffer = new char[fullCommand.length() + 3];
    strcpy(cmdBuffer, fullCommand.c_str());

    // Acquire the global interpreter lock.
    PyEval_RestoreThread(_state);

    // Attempt to compile the command with no additional newlines.
    PyObject* code = Py_CompileStringFlags(cmdBuffer, "<console>", Py_single_input, &pyCompFlags);
    if (code) {
        // Run the code!
        PyObject* ans = PyEval_EvalCode((PyCodeObject*)code, _mainNamespace, _mainNamespace);
        if (ans)
            Py_DECREF(ans);
        else {
            PyErr_Print();
            PyErr_Clear();
        }

        // Clean up.
        Py_DECREF(code);
        _state = PyEval_SaveThread();

        delete[] cmdBuffer;
        _currentCode.clear();
        return true;
    }

    // Clear the error so it doesn't stay with the thread state and
    // cause wacky syntax errors later on.  Such behaviour has been
    // observed on macosx/fink.
    PyErr_Clear();

    // Attempt to compile the command with final newline.
    cmdBuffer[fullCommand.length()] = '\n';
    cmdBuffer[fullCommand.length() + 1] = 0;

    code = Py_CompileStringFlags(cmdBuffer, "<console>", Py_single_input, &pyCompFlags);
    if (code) {
        // We're waiting on more code.
        Py_DECREF(code);
        _state = PyEval_SaveThread();

        delete[] cmdBuffer;
        _currentCode = _currentCode + command + '\n';
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

    code = Py_CompileStringFlags(cmdBuffer, "<console>", Py_single_input, &pyCompFlags);
    if (code) {
        // We're waiting on more code.
        Py_DECREF(code);
        Py_XDECREF(errType);
        Py_XDECREF(errValue);
        Py_XDECREF(errTrace);
        Py_XDECREF(errStr1);
        _state = PyEval_SaveThread();

        delete[] cmdBuffer;
        _currentCode = _currentCode + command + '\n';
        return false;
    }

    PyObject* errStr2 = extractErrMsg(); // Also clears the error.

    // Compare the two compile errors.
    if (errStr1 && errStr2) {
        if (PyObject_Compare(errStr1, errStr2)) {
            // Errors are different.  We must be waiting on more code.
            Py_XDECREF(errType);
            Py_XDECREF(errValue);
            Py_XDECREF(errTrace);
            Py_DECREF(errStr1);
            Py_DECREF(errStr2);
            _state = PyEval_SaveThread();

            delete[] cmdBuffer;
            _currentCode = _currentCode + command + '\n';
            return false;
        } else {
            // Errors are the same.  It must be a real compile error.
            PyErr_Restore(errType, errValue, errTrace);
            PyErr_Print();
            PyErr_Clear();

            Py_DECREF(errStr1);
            Py_DECREF(errStr2);
            _state = PyEval_SaveThread();

            delete[] cmdBuffer;
            _currentCode.clear();
            return true;
        }
    } else {
        // This shouldn't happen either.
        Py_XDECREF(errType);
        Py_XDECREF(errValue);
        Py_XDECREF(errTrace);
        Py_XDECREF(errStr1);
        Py_XDECREF(errStr2);
        _state = PyEval_SaveThread();
        
        NSLog(@"ERROR: Python compile error details are not available.");
        
        delete[] cmdBuffer;
        _currentCode.clear();
        return true;
    }
}

- (bool)importRegina
{
    PyEval_RestoreThread(_state);

    bool ok = false;
    try {
        PyObject* regModule = PyImport_ImportModule("regina"); // New ref.
        if (regModule) {
            PyDict_SetItemString(_mainNamespace, "regina", regModule);
            Py_DECREF(regModule);
            ok = true;
        } else {
            PyErr_Print();
            PyErr_Clear();
        }
    } catch (const boost::python::error_already_set&) {
        PyErr_Print();
        PyErr_Clear();
    }

    _state = PyEval_SaveThread();
    return ok;
}

- (bool)setVar:(const char*)name value:(regina::Packet *)value
{
    PyEval_RestoreThread(_state);

    bool ok = false;
    try {
        regina::python::to_held_type<>::apply<regina::Packet*>::type conv;
        PyObject* pyValue = conv(value);

        if (pyValue) {
            PyObject* nameStr = PyString_FromString(name); // New ref.
            PyDict_SetItem(_mainNamespace, nameStr, pyValue);
            Py_DECREF(nameStr);
            Py_DECREF(pyValue);
            ok = true;
        }
    } catch (const boost::python::error_already_set&) {
        PyErr_Print();
        PyErr_Clear();
    }

    _state = PyEval_SaveThread();
    return ok;
}

- (bool)runCode:(const char*)code
{
    PyEval_RestoreThread(_state);

    PyObject* ans = PyRun_String(code, Py_file_input, _mainNamespace, _mainNamespace);
    if (ans) {
        Py_DECREF(ans);
        _state = PyEval_SaveThread();
        return true;
    } else {
        PyErr_Print();
        PyErr_Clear();
        _state = PyEval_SaveThread();
        return false;
    }
}

- (bool)runScript:(regina::Script*)script
{
    bool result = true;
    for (size_t i = 0; i < script->countVariables(); ++i)
        if (! [self setVar:script->variableName(i).c_str() value:script->variableValue(i)])
            result = false;

    if (! [self runCode:(script->text() + "\n\n").c_str()])
        result = false;

    return result;
}

- (void)flush
{
    _outCpp->flush();
    _errCpp->flush();
}

@end

