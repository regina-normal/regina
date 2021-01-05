
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

/*! \file pythonoutputstream.h
 *  \brief Supports customised output streams for use with embedded
 *  python interpreters.
 *
 *  The accompanying source file (pythonoutputstream.cpp) should be built
 *  directly into each user interface that requires it.
 *  It is \e not built into Regina's python module.
 */

#ifndef __PYTHONOUTPUTSTREAM_H
#define __PYTHONOUTPUTSTREAM_H

#include <string>

namespace regina {

class Packet;

namespace python {

/**
 * A custom output stream that can be used by python as sys.stdout or
 * sys.stderr.
 *
 * Data is fed to the output stream by calling write().  This data is
 * buffered - each time a newline is sent or flush() is called, any
 * unprocessed data will be sent to the virtual routine processOutput().
 *
 * Typically a user interface will create a subclass of PythonOutputStream
 * that writes output to an appropriate location (e.g., a python window,
 * or a logger), and call install() to redirect Python's standard output
 * and/or standard error to some object of this subclass.
 */
class PythonOutputStream {
    private:
        std::string buffer;
            /**< The output data that has yet to be processed. */

    public:
        /**
         * Virtual destructor.
         */
        inline virtual ~PythonOutputStream();

        /**
         * Writes data to this output stream.  Note that this data will
         * not be processed until a newline is written or flush() is called.
         */
        void write(const std::string& data);

        /**
         * Forces any data that has not yet been processed to be
         * sent to processOutput().
         */
        void flush();

        /**
         * Uses this object for either standard output or standard error.
         *
         * This will set <tt>sys.&lt;streamname&gt;</tt> to this object.
         *
         * \pre \a streamName must be either "stdout" or "stderr".
         * \pre The python bindings for PythonOutputStream have already been
         * set up by calling the static routine addBindings().
         *
         * May throw a std::runtime_error if an error occurs.
         */
        void install(const char* streamName);

        /**
         * Adds python bindings for the base class PythonOutputStream.
         *
         * The class name PythonOutputStream is not added to the main
         * namespace, so the only simple way to access the Python
         * wrapper class is by having an object of that type (such as
         * a redirected sys.stdout or sys.stderr).
         *
         * May throw a std::runtime_error if an error occurs.
         */
        static void addBindings();

    protected:
        /**
         * Process a chunk of data that was sent to this output stream.
         * This routine might for instance display the data to the user
         * or write it to a log file.
         */
        virtual void processOutput(const std::string& data) = 0;
};

inline PythonOutputStream::~PythonOutputStream() {
}

} } // namespace regina::python

#endif
