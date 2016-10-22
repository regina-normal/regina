
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
#import <file/globaldirs.h>
#import <string>
#import <ostream> // This needs to come before boost/python.hpp. *shrug*

// Python includes:
#import <Python.h>
#import <compile.h>
#import <eval.h>
#import <sysmodule.h>

#import <boost/python.hpp>

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

#pragma mark Log stream

@implementation PythonLogStream

- (void)processOutput:(const char*)data
{
    NSLog(@"%s", data);
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

    NSString* _currentCode;
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
            std::string pyZipDir = regina::GlobalDirs::home() + "/python";

            std::string newPath("PYTHONPATH=");
            newPath += pyZipDir;
            newPath += "/python27.zip";

            putenv(strdup(newPath.c_str()));

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

// TODO: Destructor that cleans up Python and deletes _outCpp / _errCpp.

- (bool)executeLine:(NSString *)command
{
    // TODO
    return false;
}

- (bool)importRegina
{
    // TODO
    return false;
}

- (bool)setVar:(NSString *)name value:(regina::Packet *)value
{
    // TODO
    return false;
}

- (bool)compileScript:(NSString *)code
{
    // TODO
    return false;
}

- (bool)runScript:(NSString *)code
{
    PyEval_RestoreThread(_state);

    PyObject* ans = PyRun_String([code UTF8String], Py_file_input, _mainNamespace, _mainNamespace);
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

- (bool)runScript:(NSString *)code filename:(NSString *)filename
{
    // TODO
    return false;
}

@end

