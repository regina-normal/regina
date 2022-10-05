
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file utilities/osutils.h
 *  \brief Provides miscellaneous helper routines that are specific to
 *  particular operating systems.
 */

#ifndef __REGINA_OSUTILS_H
#ifndef __DOXYGEN
#define __REGINA_OSUTILS_H
#endif

#include <iostream>
#include <tuple>
#include "regina-core.h"

namespace regina {

/**
 * Returns time and memory usage for the current process, for use on Linux
 * systems.  Information is gathered from the <tt>/proc</tt> filesystem; if
 * this is run on a non-Linux system (or a Linux system where <tt>/proc</tt>
 * is not mounted), then this routine will throw an exception.
 *
 * More precisely, this routine reads information on the running process from
 * <tt>/proc/self/stat</tt>, and returns a tuple (\a utime, \a stime, \a vsize).
 * These three fields reperesent:
 *
 * - the number jiffies that this process has been scheduled in user mode
 *   (the \e utime field);
 * - the number jiffies that this process has been scheduled in kernel mode
 *   (the \e stime field);
 * - the the virtual memory size in bytes (the \e vsize field).
 *
 * The description of these three fields is taken directly from the
 * \c proc(5) manpage.  Note that the length of a jiffy can differ
 * from system to system; see the \c time(7) manpage for details.
 *
 * \exception FileError Either <tt>/proc/self/stat</tt> cannot be read,
 * or it contains unexpected information.
 *
 * \warning Currently this routine allows at most 255 characters for the
 * \e comm field in <tt>/proc/self/stat</tt> (which stores the executable
 * filename along with surrounding parentheses).  If the \e comm field is too
 * long (i.e., the executable filename is too long), then this routine will
 * not be able to parse <tt>/proc/self/stat</tt>, and will throw an exception.
 * If you encounter this problem, you should be able to fix it by renaming
 * your executable to something shorter.
 *
 * \ingroup utilities
 */
std::tuple<unsigned long, unsigned long, unsigned long> resUsage();

/**
 * Writes time and memory usage for the current process to the given
 * output stream, for use on Linux systems.
 *
 * This simply writes the information gathered from resUsage() to the given
 * output stream in a human-readable format.  See resUsage() for full details
 * on what this information means and on what systems it can be accessed.
 *
 * The output will be written in the following format, with no final newline,
 * and without flushing the output stream:
 *
 * \code
 * utime=..., stime=..., vsize=...
 * \endcode
 *
 * Unline resUsage(), if an error occurs then this routine will not throw an
 * exception.  Instead it will write an appropriate message to the output
 * stream.
 *
 * \nopython Instead use the variant resUsage() that takes no arguments and
 * returns a tuple.
 *
 * @param out the output stream to which to write.
 */
void writeResUsage(std::ostream& out);

} // namespace regina

#endif

