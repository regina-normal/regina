
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include <condition_variable>
#include <thread>

/**
 * This is a very basic Python interpreter, designed to test scenarios
 * where Python is embedded in a C++ program.  It processes input commands
 * one line at a time (so even if a script is passed via the command line,
 * it behaves as though the input were interactive).
 *
 * If -t <seconds> is passed, the interpreter runs under a strict time limit:
 * it will exit with non-zero error status if the given time limit is exceeded.
 * This is so that deadlock scenarios can be detected and reported effectively.
 *
 * The main reason for having this basic interpreter is so we can more easily
 * test for problems that might occur in the Qt GUI (e.g., problems related to
 * multithreading, or subinterpreters, or the global interpreter lock).
 *
 * This interpreter is designed to be run directly out of the source tree.
 * It does not set LD_LIBRARY_PATH, PYTHONHOME or PYTHONPATH at all; it is
 * up to whoever calls this interpreter to set these paths appropriately.
 */

long timeout = 0; // measured in seconds
bool mainThread = true;

std::condition_variable cond; // used with timeout mechanism
bool finished = false;
std::mutex mutex; // guards finished and cond

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
    std::cerr << "    " << progName << " [ -m ] [ -t <seconds> ] [ <commands> ]\n";
    std::cerr << "    " << progName << " -v\n";
    std::cerr << std::endl;
    std::cerr << "    -m : Execute commands in a different thread\n";
    std::cerr << "    -t : Timeout after the given number of seconds\n";
    std::cerr << "    -v : Output the Python version being used\n";
    std::cerr << std::endl;
    std::cerr << "    <script> : Read commands line-by-line from the "
        "given file (otherwise\n";
    std::cerr << "               uses standard input)\n";
    exit(1);
}

void watcher() {
    std::unique_lock<std::mutex> lock(mutex);

    // At this point, either finished is true, or else the main thread
    // will not be able to notify the condition variable until *after*
    // we wait (thus ensuring we will be properly woken up).

    if (finished)
        return;

    if (cond.wait_for(lock, std::chrono::seconds(timeout)) ==
            std::cv_status::timeout) {
        std::cerr << "ERROR: Timed out after " << timeout  << "s." << std::endl;

        // We assume the python code is deadlocked; we will not be able to
        // clean up Python properly.
        exit(3);
    }
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

    for (int i = 1; i < argc; ++i) {
        if (*argv[i] == '-') {
            if (! (argv[i][1] && ! argv[i][2]))
                usage(argv[0], "Unknown option.");
            switch (argv[i][1]) {
                case 'v':
                    if (argc == 2) {
                        std::cout << PY_MAJOR_VERSION << '.'
                            << PY_MINOR_VERSION << '.'
                            << PY_MICRO_VERSION << std::endl;
                        return 0;
                    } else
                        usage(argv[0],
                            "Argument -v cannot be used with other options.");
                case 'm':
                    mainThread = false;
                    break;
                case 't':
                    if (i == argc - 1)
                        usage(argv[0], "Missing timeout argument.");
                    ++i;
                    {
                        char* err = nullptr;
                        timeout = strtol(argv[i], &err, 10);
                        if (*err)
                            usage(argv[0], "Timeout should be an integer.");
                        else if (timeout <= 0)
                            usage(argv[0],
                                "Timeout should be strictly positive.");
                    }
                    break;
                default:
                    usage(argv[0], "Unknown option.");
            }
        } else {
            if (i != argc - 1)
                usage(argv[0], "The <commands> argument must come last.");
            input = argv[i];
        }
    }

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

    if (input.empty()) {
        if (timeout) {
            std::thread w(watcher);
            run(py, std::cin);
            {
                std::unique_lock<std::mutex> lock(mutex);
                finished = true;
            }
            cond.notify_one();

            w.join();
        } else {
            run(py, std::cin);
        }
    } else {
        std::ifstream in(input);
        if (! in) {
            std::cerr << "ERROR: Could not open input file: " << input
                << std::endl;
            return 1;
        }
        if (timeout) {
            std::thread w(watcher);
            run(py, in);
            {
                std::unique_lock<std::mutex> lock(mutex);
                finished = true;
            }
            cond.notify_one();

            w.join();
        } else {
            run(py, in);
        }
    }

    return 0;
}

