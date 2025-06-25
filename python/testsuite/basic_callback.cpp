
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
 * This test runs some simple Regina/Python commands in multiple embedded
 * Python subinterpreters, where these commands involve pure Python callbacks.
 * The purpose of this test is to ensure that Regina's Python module handles
 * callbacks correctly when used in multiple subinterpreters.
 */

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

struct Session {
    NativeOutputStream out;
    NativeOutputStream err;
    regina::python::PythonInterpreter py;

    Session(std::string exec) :
            out(std::cout), err(std::cerr), py(out, err, false) {
        // We do not adjust the Python path to reflect Regina's installation
        // location, since this test is designed to be run directly out of the
        // source tree.
        if (! py.importRegina(false)) {
            std::cerr << "ERROR: Could not import regina" << std::endl;
            ::exit(2);
        }
        py.executeLine("from regina import *");
        py.executeLine("regina.GlobalDirs.deduceDirs('" + exec + "');");
    }

    void executeLine(const char* line) {
        py.executeLine(line);
    }
};

int main(int argc, char* argv[]) {
    // Tests that do not use GILCallbackManager under the hood:
    {
        Session s(argv[0]);
        s.executeLine("def sortInc(a, b):");
        s.executeLine("    return a.str() < b.str()");
        s.executeLine("");
        s.executeLine("s = NormalSurfaces(Example3.poincare(), NormalCoords.Standard)");
        s.executeLine("print(s.detail())");
        s.executeLine("s.sort(sortInc)");
        s.executeLine("print(s.detail())");
    }
    {
        Session s(argv[0]);
        s.executeLine("def sortInc(a, b):");
        s.executeLine("    return a.str() < b.str()");
        s.executeLine("");
        s.executeLine("s = NormalSurfaces(Example3.poincare(), NormalCoords.Standard)");
        s.executeLine("print(s.detail())");
        s.executeLine("s.sort(sortInc)");
        s.executeLine("print(s.detail())");
    }
    {
        Session s1(argv[0]);
        Session s2(argv[0]);

        s1.executeLine("def sortInc(a, b):");
        s1.executeLine("    return a.str() < b.str()");
        s1.executeLine("");
        s1.executeLine("s = NormalSurfaces(Example3.poincare(), NormalCoords.Standard)");


        s2.executeLine("def sortInc(a, b):");
        s2.executeLine("    return a.str() < b.str()");
        s2.executeLine("");
        s2.executeLine("s = NormalSurfaces(Example3.poincare(), NormalCoords.Standard)");

        s1.executeLine("print(s.detail())");
        s2.executeLine("print(s.detail())");

        s1.executeLine("s.sort(sortInc)");
        s1.executeLine("print(s.detail())");
        s2.executeLine("print(s.detail())");

        s2.executeLine("s.sort(sortInc)");
        s1.executeLine("print(s.detail())");
        s2.executeLine("print(s.detail())");
    }

    // Tests that do use GILCallbackManager under the hood:
    {
        Session s(argv[0]);
        s.executeLine("ans = None");
        s.executeLine("def store(vec):");
        s.executeLine("    global ans");
        s.executeLine("    ans.append(vec)");
        s.executeLine("");
        s.executeLine("def dump():");
        s.executeLine("    global ans");
        s.executeLine("    ans.sort(key = str)");
        s.executeLine("    for v in ans:");
        s.executeLine("        print(v)");
        s.executeLine("    print()");
        s.executeLine("");
        s.executeLine("tri = Example3.lens(3, 1)");
        s.executeLine("m = makeMatchingEquations(tri, NormalCoords.Standard)");
        s.executeLine("c = ValidityConstraints(7, tri.size())");
        s.executeLine("ans = []");
        s.executeLine("DoubleDescription.enumerate(store, m, c)");
        s.executeLine("dump()");
    }

    return 0;
}
