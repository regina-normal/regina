
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Command-Line Python Interpreter                                       *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

// This is provided purely for systems where we need to build a Python
// interpreter with special compiler/linker flags.  An example is the macOS
// app bundle, where we need to link an Info.plist into the Python executable.
//
// On most platforms, we do not build this file at all.  In particular, it
// does not appear in CMakeLists.txt.

#include <iostream>
#include <Python.h>

int main(int argc, char** argv) {
    // We have to convert our char* arguments into wchar_t* arguments
    // before passing them to Python.
    wchar_t** wideArgs = new wchar_t*[argc];
    for (int i = 0; i < argc; ++i) {
        wideArgs[i] = Py_DecodeLocale(argv[i], nullptr);
        if (! wideArgs[i]) {
            std::cerr << "ERROR: Could not decode argument: " << argv[i]
                << std::endl;
            return 1;
        }
    }

    // Ask Python to do the real work...
    int retVal = Py_Main(argc, wideArgs);

    // ... and then clean up.
    for (int i = 0; i < argc; ++i)
        PyMem_RawFree(wideArgs[i]);
    delete[] wideArgs;
    return retVal;
}

