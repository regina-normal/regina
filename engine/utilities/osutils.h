
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file utilities/osutils.h
 *  \brief Provides miscellaneous helper routines that are specific to
 *  particular operating systems.
 */

#ifndef __OSUTILS_H
#ifndef __DOXYGEN
#define __OSUTILS_H
#endif

#include <iostream>
#include "regina-core.h"

namespace regina {

/**
 * Writes time and memory usage for the current process to the given
 * output stream.  Information is gathered from the \c /proc filesystem,
 * which means that this routine will only write meaningful information
 * under a Linux system where \c /proc is mounted.
 *
 * Specifically, this routine reads information on the running process from
 * \c /proc/self/stat, and writes a short string to the given output
 * stream of the form:
 *
 * \code
 * utime=..., stime=..., vsize=...
 * \endcode
 *
 * The three ellipses in the example above will be replaced with integers,
 * representing:
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
 * The output will not contain a newline, and the given output stream
 * will not be flushed.
 *
 * If \c /proc/self/stat cannot be read, this routine will write a short
 * message to that effect to the given output stream (which means that
 * this utility is safe to call under non-Linux operating systems,
 * although it will of course be useless in such scenarios).
 *
 * If \c /proc/self/stat can be read but contains unexpected
 * information, the behaviour of this routine is undefined.
 *
 * \ifacespython This routine does not take any arguments; instead the
 * stream \a out is assumed to be standard output.
 *
 * \warning Currently this routine allows at most 255 characters for the
 * \e comm field in \c /proc/self/stat (which stores the executable
 * filename along with surrounding parentheses).  If the \e comm field is too
 * long (i.e., the executable filename is too long), then this routine
 * will not be able to parse \c /proc/self/stat, and will write a note to
 * this effect to the given output stream.  If you encounter this problem,
 * you should be able to fix it by renaming your executable to something
 * shorter.
 *
 * @param out the output stream to which usage information will be written.
 */
REGINA_API void writeResUsage(std::ostream& out);

} // namespace regina

#endif

