
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file pythonoutputstream.h
 *  \brief Provides an infrastructure for custom python output streams.
 */

#ifndef __PYTHONOUTPUTSTREAM_H
#define __PYTHONOUTPUTSTREAM_H

#include <string>

/**
 * An custom output stream that can be used by python as sys.stdout or
 * sys.stderr.
 *
 * Data is fed to the output stream by calling write().  This data is
 * buffered - each time a newline is sent or flush() is called, any
 * unprocessed data will be sent to the virtual routine processOutput().
 */
class PythonOutputStream {
    private:
        std::string buffer;
            /**< The output data that has yet to be processed. */

    public:
        /**
         * Virtual destructor.
         */
        virtual ~PythonOutputStream();
        /**
         * Writes data to this output stream.  Note that this data will
         * not be processed until a newline is written or flush() is
         * called.
         */
        void write(const std::string& data);
        /**
         * Forces any data that has not yet been processed to be
         * sent to processOutput().
         */
        void flush();

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

#endif
