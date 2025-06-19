
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
 * Python subinterpreters.  The purpose of this test is to ensure that
 * Regina's Python module can be loaded and used in multiple subinterpreters,
 * both sequentially and simultaneously.
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
    {
        Session s(argv[0]);
        s.executeLine("print(ExampleLink.trefoil().jones())");
    }
    {
        Session s(argv[0]);
        s.executeLine("print(ExampleLink.virtualTrefoil().jones())");
    }
    {
        Session s1(argv[0]);
        Session s2(argv[0]);
        s1.executeLine("print(ExampleLink.figureEight().jones())");
        s2.executeLine("print(ExampleLink.whitehead().jones())");
    }
    return 0;
}

