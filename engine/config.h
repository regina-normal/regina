
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

/*! \file config.h
 *  \brief Platform-dependent global definition file.
 *  Items may differ depending upon platform and compiler.
 *  Edit this file as appropriate for your system.
 *
 *  Tested platforms and compilers include:
 *  - GNU g++ (Linux)
 *  - Borland C++ Builder (PC); testing out of date
 *  - CodeWarrior (Mac, PC); testing out of date
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*! \def __NO_INCLUDE_PATHS
 *  Uncomment this if paths to include files will not be specified in
 *  the corresponding C++ source files.
 *
 *  Recommended for CodeWarrior.
 */
//#ifndef __DOXYGEN
//#define __NO_INCLUDE_PATHS
//#endif

/*! \def __NO_IOS_NOCREATE
 *  Uncomment this if <tt>ios::nocreate</tt> is not a valid file opening
 *  flag.
 *
 *  Recommended for CodeWarrior and Borland C++ Builder.
 */
//#ifndef __DOXYGEN
//#define __NO_IOS_NOCREATE
//#endif

/*! \def __BINARY_IO
 *  Uncomment this if <tt>ios::binary</tt> should be used in file
 *  opening routines.  This will ensure binary output on systems with
 *  non-unix style ASCII output.
 *
 *  Recommended for all compilers.
 */
#ifndef __DOXYGEN
#define __BINARY_IO
#endif

/*! \def __MUTE_WARNINGS
 *  Uncomment this if <tt>#pragma</tt> directives should be used to mute
 *  a small set of known non-dangerous warnings over small portions of
 *  known correct code.
 *
 *  Certain portions of correct code may attract compiler warnings.
 *  This macro will insert directives of the form <tt>#pragma warn
 *  -xxx</tt> and <tt>#pragma warn .xxx</tt> to bypass these specific
 *  warnings over these specific regions of code.
 *
 *  If your compiler does not recognise these <tt>#pragma</tt>
 *  directives, you can safely comment out this macro and ignore the
 *  resulting warnings.
 *
 *  Recommended for Borland C++ Builder.
 */
//#ifndef __DOXYGEN
//#define __MUTE_WARNINGS
//#endif

/*! \def __NO_RAW_CASTING
 *  Uncomment this if your compiler will not cast <tt>(void*)</tt> to
 *  <tt>long</tt> and vice versa.
 *
 *  Recommended for CodeWarrior.
 */
//#ifndef __DOXYGEN
//#define __NO_RAW_CASTING
//#endif

#ifdef __DOXYGEN
    // Don't comment out anything from this list; the C++ compiler will
    // ignore this section anyway.  It exists purely so doxygen will not
    // get confused by missing macro definitions when generating engine
    // documentation.
    #define __NO_INCLUDE_PATHS
    #define __NO_IOS_NOCREATE
    #define __BINARY_IO
    #define __MUTE_WARNINGS
    #define __NO_RAW_CASTING
#endif

#endif

