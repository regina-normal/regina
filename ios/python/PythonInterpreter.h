
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
 * A single python subinterpreter.  Multiple subinterpreters are independent
 * and may exist simultaneously.
 *
 * Each new subinterpreter corresponds to a new call to Py_NewInterpreter().
 * The global routine Py_Initialize() and Py_Finalize() are called when the
 * first interpreter is created and the last interpreter is destroyed
 * respectively.
 */
@interface PythonInterpreter : NSObject

/**
 * Initialises a new subinterpreter, using the given output and error
 * streams.  The two stream may be the same.  Moreover, one or both streams
 * may be \c nil, in which case the corresponding output will be logged via
 * NSLog().
 */
- (id)initWithOut:(id<PythonOutputStream>)out err:(id<PythonOutputStream>)err;

/**
 * Interactions with the Python subinterpreter:
 */
- (bool)executeLine:(NSString*)command;
- (bool)importRegina;
- (bool)setVar:(NSString*)name value:(regina::Packet*)value;
- (bool)compileScript:(NSString*)code;
- (bool)runScript:(NSString*)code;
- (bool)runScript:(NSString*)code filename:(NSString*)filename;

@end
