
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

/*! \file nsnappea.h
 *  \brief Allows reading and writing SnapPea files.
 */

#ifndef __NSNAPPEA_H
#ifndef __DOXYGEN
#define __NSNAPPEA_H
#endif

#include <string>

namespace regina {

class NTriangulation;

/**
 * \addtogroup foreign Foreign File Formats
 * Reading and writing foreign file formats.
 * @{
 */

/**
 * Reads a triangulation from the given SnapPea file.  A newly allocated
 * triangulation will be returned; it is the user's responsibility to
 * deallocate this when it is finished with.
 *
 * The new triangulation will have a packet label corresponding to the
 * manifold name stored in the SnapPea file.
 *
 * If the file could not be read or if the data was not in the correct
 * format, 0 will be returned.
 *
 * \pre The first two lines of the SnapPea file each contain at most
 * 1000 characters.  The first line is the type of file which should
 * simply be ``<tt>% Triangulation</tt>''.  The second line is the name of
 * the manifold.
 *
 * @param filename the name of the SnapPea file from which to read.
 * @return a new triangulation containing the data read from the SnapPea
 * file, or 0 on error.
 */
NTriangulation* readSnapPea(const char *filename);

/**
 * Writes the given triangulation to the given file in SnapPea format.
 * All information aside from tetrahedron gluings will be flagged as
 * unknown for SnapPea to recalculate.
 * The manifold name written in the file will be derived from the packet
 * label.
 *
 * \pre The given triangulation is not invalid.
 *
 * @param filename the name of the SnapPea file to which to write.
 * @param tri the triangulation to write to the SnapPea file.
 * @return \c true if the export was successful, or \c false otherwise.
 */
bool writeSnapPea(const char* filename, NTriangulation& tri);

/**
 * Returns a token derived from the given string.
 * All whitespace characters in the given string will be replaced with
 * an underscore.
 *
 * @param str the string on which to base the token.
 * @return the corresponding token.
 */
std::string stringToToken(const char* str);

/**
 * Returns a token derived from the given string.
 * All whitespace characters in the given string will be replaced with
 * an underscore.
 *
 * @param str the string on which to base the token.
 * @return the corresponding token.
 */
std::string stringToToken(const std::string& str);

/*@}*/

} // namespace regina

#endif

