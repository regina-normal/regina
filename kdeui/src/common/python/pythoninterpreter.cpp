
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

// UI includes:

#include "pythoninterpreter.h"

#include <cstdlib>
#include <cstring>
#include <iostream> // TODO: Remove

long PythonInterpreter::nInterpreters = 0;
bool PythonInterpreter::createdGIL = false;
regina::NMutex PythonInterpreter::globalMutex;

PythonInterpreter::PythonInterpreter() {
    regina::NMutex::MutexLock lock(globalMutex);

    // Acquire the global interpreter lock.
    if (! createdGIL) {
        PyEval_InitThreads();
        createdGIL = true;
    } else
        PyEval_AcquireLock();

    // Create the new interpreter.
    if (nInterpreters == 0)
        Py_Initialize();
    state = Py_NewInterpreter();
    nInterpreters++;

    // Release the global interpreter lock.
    PyEval_SaveThread();
}

PythonInterpreter::~PythonInterpreter() {
    regina::NMutex::MutexLock lock(globalMutex);

    // Acquire the global interpreter lock.
    PyEval_RestoreThread(state);

    // Destroy the interpreter.
    nInterpreters--;
    if (nInterpreters == 0)
        Py_Finalize();
    else
        Py_EndInterpreter(state);

    // Release the global interpreter lock.
    PyEval_ReleaseLock();
}

bool PythonInterpreter::executeLine(const char* command) {
    PyEval_RestoreThread(state);

    // TODO
    char* copy = strdup(command);
    PyRun_SimpleString(copy);
    free(copy);

    state = PyEval_SaveThread();

    // TODO
    return true;
}

void PythonInterpreter::setVar(const char* name, regina::NPacket* value) {
    PyEval_RestoreThread(state);

    // TODO

    state = PyEval_SaveThread();
}

