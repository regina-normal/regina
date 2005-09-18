
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file engine.h
 *  \brief Provides global routines for interfacing with the Regina
 *  calculation engine.
 */

#ifndef __ENGINE_H
#ifndef __DOXYGEN
#define __ENGINE_H
#endif

/**
 * Contains the entire Regina calculation engine.
 *
 * \todo \prob Derive from regina::boost::noncopyable where appropriate.
 * \todo \featurelong Enhance the test suite for the calculation engine.
 */
namespace regina {

/**
 * \addtogroup engine Engine Structure
 * Overall structure of the calculation engine.
 * @{
 */

/**
 * Returns the full version number of this calculation engine.
 * For instance, version 2.3.1 would have full version <tt>"2.3.1"</tt>.
 *
 * @return the version of this calculation engine.
 */
const char* getVersionString();

/**
 * Returns the major version number of this calculation engine.
 * For instance, version 2.3.1 would have major version 2.
 *
 * @return the major version number of this calculation engine.
 */
int getVersionMajor();

/**
 * Returns the minor version number of this calculation engine.
 * For instance, version 2.3.1 would have minor version 3.
 *
 * @return the minor version number of this calculation engine.
 */
int getVersionMinor();

/**
 * Tests to see if an interface can successfully communicate with the
 * underlying C++ calculation engine.
 *
 * This routine simply uses the engine to return the same value that is
 * passed to it; it can be used to test whether communications between
 * the interface and the C++ engine are working properly.
 *
 * @param value any integer; this same integer will be returned.
 * @return the same integer that was passed as \a value.
 */
int testEngine(int value);

/*@}*/

} // namespace regina

#endif
