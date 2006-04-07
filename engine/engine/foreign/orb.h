
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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
 *  \brief Allows reading and writing SnapPea files.
 */

#ifndef __ORB_H
#ifndef __DOXYGEN
#define __ORB_H
#endif

#include <string>

namespace regina {

/**
 * Reads a triangulation from the given Orb file.  A newly allocated
 * triangulation will be returned; it is the user's responsibility to
 * deallocate this when it is finished with.
 *
 * The new triangulation will have a packet label corresponding to the
 * manifold name stored in the SnapPea file.
 *
 * If the file could not be read or if the data was not in the correct
 * format, 0 will be returned.
 *
 * \pre The first two lines of the Orb file each contain at most
 * 1000 characters.  The first line is the type of file which should
 * simply be ``<tt>% Triangulation</tt>''.  The second line is the name of
 * the manifold.
 *
 * @param filename the name of the Orb file from which to read.
 * @return a new triangulation containing the data read from the SnapPea
 * file, or 0 on error.
 */
NTriangulation* readOrb(const char *filename);

/*@}*/

} // namespace regina

#endif

