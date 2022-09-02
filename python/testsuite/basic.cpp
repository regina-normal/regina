
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
 * This is a very basic Python interpreter, designed to test scenarios
 * where Python is embedded in a C++ program.  It processes input commands
 * one line at a time (so even if a script is passed via the command line,
 * it behaves as though the input were interactive).
 *
 * The main reason for having this basic interpreter is so we can more easily
 * test for problems that might occur in the Qt GUI (e.g., problems related to
 * multithreading, or subinterpreters, or the global interpreter lock).
 */

bool mainThread = true;

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

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " [ -t ] [ <commands> ]\n";
    std::cerr << std::endl;
    std::cerr << "    -t : Execute commands in a different thread\n";
    std::cerr << std::endl;
    std::cerr << "    <script> : Read commands line-by-line from the "
        "given file (otherwise\n";
    std::cerr << "               uses standard input)\n";
    exit(1);
}

void run(regina::python::PythonInterpreter& py, std::istream& input) {
    std::string line;
    while (input) {
        std::getline(input, line);
        if (mainThread) {
            py.executeLine(line);
        } else {
            std::thread([&]() {
                py.executeLine(line);
            }).join();
        }
    }
}

int main(int argc, char* argv[]) {
    std::string input;

    if (argc > 3)
        usage(argv[0]);
    else if (argc == 3) {
        if (strcmp(argv[1], "-t") != 0)
            usage(argv[0]);
        mainThread = false;
        input = argv[2];
    } else if (argc == 2)
        input = argv[1];

    NativeOutputStream out(std::cout);
    NativeOutputStream err(std::cerr);
    regina::python::PythonInterpreter py(out, err);
    if (! py.importRegina()) {
        std::cerr << "ERROR: Could not import regina";
        exit(2);
    }

    py.executeLine("from regina import *");

    if (input.empty()) {
        run(py, std::cin);
    } else {
        std::ifstream in(input);
        if (! in) {
            std::cerr << "ERROR: Could not open input file: " << input
                << std::endl;
            return 1;
        }
        run(py, in);
    }

    return 0;
}

