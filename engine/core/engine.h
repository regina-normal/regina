
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file core/engine.h
 *  \brief Provides global routines for interfacing with the Regina
 *  calculation engine.
 */

#ifndef __REGINA_ENGINE_H
#ifndef __DOXYGEN
#define __REGINA_ENGINE_H
#endif

#include "regina-core.h"

/**
 * Contains the entire Regina calculation engine.
 */
namespace regina {

/**
 * \defgroup engine Engine Structure
 * Overall structure of the calculation engine.
 */

/**
 * Returns the full version number of this calculation engine.
 * For instance, version 2.3.1 would have full version `"2.3.1"`.
 *
 * \return the version of this calculation engine.
 *
 * \ingroup engine
 */
const char* versionString();

/**
 * Returns the major version number of this calculation engine.
 * For instance, version 2.3.1 would have major version 2.
 *
 * \return the major version number of this calculation engine.
 *
 * \ingroup engine
 */
int versionMajor();

/**
 * Returns the minor version number of this calculation engine.
 * For instance, version 2.3.1 would have minor version 3.
 *
 * \return the minor version number of this calculation engine.
 *
 * \ingroup engine
 */
int versionMinor();

/**
 * Returns any additional information about this specific build of Regina.
 * For instance, an official download from the Regina website might return
 * "Official macOS build", and a package from the official Debian servers
 * might return "Debian package build".
 *
 * This information is supplied at build time by passing the option
 * `-DBUILD_INFO=...` to CMake.
 *
 * If no such information was supplied at build time, then this string
 * will be empty.
 *
 * \return any additional information supplied at build time.
 *
 * \ingroup engine
 */
const char* buildInfo();

/**
 * Did the given version of Regina consistently use UTF-8 in its data
 * files?
 *
 * In Regina versions 4.4 and below, no particular attention was paid to
 * character encodings.  As a result, the GUI typically stored data in
 * LATIN1 (the default at the time for the Qt libraries).
 *
 * As of Regina 4.5, all strings are now stored in UTF-8 where possible.
 *
 * This routine allows programs to determine which regime a particular
 * version of Regina belongs to.  This can be useful when working with
 * Regina data files on a low-level basis.
 *
 * Any whitespace in the version string will confuse the result, and the
 * return value will be undefined.
 *
 * As a special case, an empty string is treated as belonging to the
 * UTF-8 regime.
 *
 * \param version a version string from some release of Regina, such as "4.2".
 * This must not contain any whitespace padding.
 * \return \c true if the given version uses UTF-8 consistently, or
 * \c false if the given version is an older version that did not pay
 * attention to character encodings.
 *
 * \ingroup engine
 */
bool versionUsesUTF8(const char* version);

/**
 * Returns the version of SnapPy whose underlying SnapPea kernel is built
 * into Regina.
 *
 * The SnapPea kernel is now maintained by the larger \e SnapPy project,
 * and Regina sources its version of the SnapPea kernel from one of the
 * recent SnapPy releases.  See http://snappy.computop.org/ for details.
 *
 * In Regina, you can access the SnapPea kernel through the
 * SnapPeaTriangulation class.
 *
 * \return the version of SnapPy whose underlying SnapPea kernel is
 * built into Regina.
 *
 * \ingroup engine
 */
const char* versionSnapPy();

/**
 * An alias for versionSnapPy(), which returns the version of SnapPy whose
 * underlying SnapPea kernel is built into Regina.
 *
 * See versionSnapPy() for details.
 *
 * \return the version of SnapPy whose underlying SnapPea kernel is
 * built into Regina.
 *
 * \ingroup engine
 */
const char* versionSnapPea();

/**
 * Does this particular build of Regina support native 128-bit arithmetic?
 *
 * On those platforms that do support native 128-bit arithmetic, some
 * expensive algorithms (in particular for normal surface enumeration)
 * are optimised to run significantly faster in some cases.
 *
 * \return \c true if and only if this build supports native 128-bit arithmetic.
 *
 * \ingroup engine
 */
bool hasInt128();

/**
 * A suggestion for user interfaces as to how many threads should be
 * used for parallel computations.
 *
 * The current implementation returns approximately half the number of
 * cores available on the machine.
 *
 * \return the suggested number of threads to use.
 *
 * \ingroup engine
 */
int politeThreads();

/**
 * Tests to see if an interface can successfully communicate with the
 * underlying C++ calculation engine.
 *
 * This routine simply uses the engine to return the same value that is
 * passed to it; it can be used to test whether communications between
 * the interface and the C++ engine are working properly.
 *
 * \param value any integer; this same integer will be returned.
 * \return the same integer that was passed as \a value.
 *
 * \ingroup engine
 */
int testEngine(int value);

} // namespace regina

#endif
