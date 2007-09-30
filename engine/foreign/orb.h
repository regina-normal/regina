
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

/*! \file orb.h
 *  \brief Allows reading Orb / Casson triangulation files.
 */

#ifndef __ORB_H
#ifndef __DOXYGEN
#define __ORB_H
#endif

#include <string>

namespace regina {

class NTriangulation;

/**
 * \weakgroup foreign
 * @{
 */

/**
 * Reads a triangulation from the given Orb / Casson file.  A newly
 * allocated triangulation will be returned; it is the user's
 * responsibility to deallocate this when it is finished with.
 *
 * The packet label of the new triangulation will be the manifold name
 * read from the second line of the Orb / Casson file.  The first line
 * of the Orb / Casson file must simply be ``<tt>% orb</tt>''.
 *
 * If the file could not be read or if the data was not in the correct
 * format, 0 will be returned.
 *
 * @param filename the name of the Orb / Casson file from which to read.
 * @return a new triangulation containing the data read from the Orb / Casson
 * file, or 0 on error.
 *
 * @author Ryan Budney, also with code from Damien Heard
 */
NTriangulation* readOrb(const char *filename);

/*@}*/

} // namespace regina

#endif

