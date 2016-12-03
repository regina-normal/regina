
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
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

#import <Foundation/Foundation.h>

namespace regina {
    class Packet;
    class Script;
}

/**
 * A protocol for objects that can act as Python output streams
 * sys.stdout and/or sys.stderr.
 */
@protocol PythonOutputStream <NSObject>
@required
/**
 * Process a chunk of data that was sent to this output stream.
 * This routine might for instance display the data to the user
 * or write it to a log file.
 *
 * You should assume that \a data is encoded in UTF-8.
 */
- (void)processOutput:(const char*)data;
@end

/**
 * A Python output stream that sends data to NSLog().
 */
@interface PythonLogStream : NSObject<PythonOutputStream>

- (void)processOutput:(const char*)data;

@end

/**
 * A single Python subinterpreter.  Multiple subinterpreters are independent
 * and may exist simultaneously.
 *
 * Each new subinterpreter corresponds to a new call to Py_NewInterpreter().
 * The global routine Py_Initialize() is called when the first interpreter is
 * created.  The global routine Py_Finalize() is never called (which is
 * bad behaviour), since Regina has no idea which interpreter will be the last.
 */
@interface PythonInterpreter : NSObject

/**
 * Initialises a new subinterpreter.
 *
 * \param out The output stream to use as sys.stdout.  This may be \c nil,
 * in which case a PythonLogStream will be used.
 * \param err The output stream to use as sys.stderr.  This may be \c nil,
 * in which case a PythonLogStream will be used.  This is also allowed to be
 * the same as \a out.
 */
- (id)initWithOut:(id<PythonOutputStream>)out err:(id<PythonOutputStream>)err;

/**
 * Execute a single line of code.  This is intended for use in an interactive
 * Python session.
 */
- (bool)executeLine:(const char*)command;

/**
 * Import Regina's Python module.
 */
- (bool)importRegina;

/**
 * Set the given variable in Python's main namespace to represent the given
 * Regina packet.
 */
- (bool)setVar:(const char*)name value:(regina::Packet*)value;

/**
 * Run the given Python code in Python's main namespace.
 */
- (bool)runCode:(const char*)code;

/**
 * Run the given script packet in Python's main namespace.
 * This involves setting all of the script variables, and then running
 * the script code itself.
 */
- (bool)runScript:(regina::Script*)script;

/**
 * Flush the standard output and error streams.
 */
- (void)flush;

@end
