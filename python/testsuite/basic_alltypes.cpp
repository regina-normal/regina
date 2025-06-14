
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

#include "../gui/pythoninterpreter.h"
#include "../gui/pythonoutputstream.h"
#include <fstream>
#include <iostream>
#include <thread>

/**
 * This uses the same basic Python interpreter as in basic.cpp, but runs a
 * script that is hard-coded into the executable.  The purpose is to have a
 * standalone executable that can run with no arguments and reproduce the
 * subinterpreter problems that were previously observed with Python ≥ 3.12.
 *
 * This interpreter is designed to be run directly out of the source tree.
 * It does not set LD_LIBRARY_PATH, PYTHONHOME or PYTHONPATH at all; it is
 * up to whoever calls this interpreter to set these paths appropriately.
 */

std::string lines[] = {
    "def classInfo(name, object):",
    "    realName = object.__name__",
    "    if realName == name:",
    "        if name == 'Bitmask256' or name == 'Qitmask256':",
    "            # These classes are only present on 64-bit machines.",
    "            # Omit them here so that this test generates the same output",
    "            # across all platforms.",
    "            return",
    "        if object.__base__ is RuntimeError:",
    "            # Exception classes do not have equalityType.",
    "            print(name, ': exception')",
    "        else:",
    "            print(name, ': class ->', object.equalityType)",
    "    else:",
    "        print(name, ': alias ->', realName)",
    "",
    "def enumInfo(name, object):",
    "    realName = object.__name__",
    "    if realName == name:",
    "        print(name, ': enum')",
    "    else:",
    "        print(name, ': alias -> enum', realName)",
    "",
    "def functionInfo(name, object):",
    "    if not name.startswith('__'):",
    "        print(name, ': function')",
    "",
    "def objectInfo(name, object):",
    "    # print(name, ': object')",
    "    pass",
    "",
    "keys = sorted(regina.__dict__)",
    "",
    "for i in keys:",
    "    object = regina.__dict__[i]",
    "    if isinstance(object, type):",
    "        try:",
    "            tmp = object.__members__ # Exists for enums, not classes",
    "            enumInfo(i, object)",
    "        except AttributeError:",
    "            classInfo(i, object)",
    "    elif 'builtin_function_or_method' in str(object.__class__):",
    "        functionInfo(i, object)",
    "    else:",
    "        objectInfo(i, object)",
    ""
    };

class NativeOutputStream : public regina::python::PythonOutputStream {
    private:
        std::ostream& stream_;

    public:
        NativeOutputStream(std::ostream& stream) : stream_(stream) {}

    protected:
        virtual void processOutput(const std::string& data) override {
            stream_ << data;
            stream_.flush();
        }
};

int main(int argc, char* argv[]) {
    NativeOutputStream out(std::cout);
    NativeOutputStream err(std::cerr);
    // In the new python interpreter, do not adjust the Python path to reflect
    // Regina's installation location, since this tool is designed to be
    // run directly out of the source tree.
    regina::python::PythonInterpreter py(out, err, false);
    if (! py.importRegina(false)) {
        std::cerr << "ERROR: Could not import regina" << std::endl;
        exit(2);
    }

    py.executeLine("from regina import *");
    std::string exec = argv[0];
    py.executeLine("regina.GlobalDirs.deduceDirs('" + exec + "');");

    for (const auto& line : lines)
        py.executeLine(line);

    return 0;
}

