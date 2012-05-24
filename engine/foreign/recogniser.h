
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2012, Ben Burton                                   *
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

/*! \file foreign/recogniser.h
 *  \brief Allows exports to Matveev's 3-manifold recogniser.
 */

#ifndef __RECOGNISER_H
#ifndef __DOXYGEN
#define __RECOGNISER_H
#endif

#include <string>
#include "regina-core.h"

namespace regina {

class NTriangulation;

/**
 * \weakgroup foreign
 * @{
 */

/**
 * Writes the given triangulation to the given file in Matveev's 3-manifold
 * recogniser format.
 *
 * \pre The given triangulation is not invalid, and does not contain any
 * boundary faces.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and
 * simply passes it through unchanged to low-level C/C++ file I/O routines.
 * The \e contents of the file will be written using UTF-8.
 *
 * @param filename the name of the Recogniser file to which to write.
 * This may be the null pointer (or the empty string), in which case the
 * data will be written to standard output instead.
 * @param tri the triangulation to write to the Recogniser file.
 * @return \c true if the export was successful, or \c false otherwise.
 */
REGINA_API bool writeRecogniser(const char* filename, NTriangulation& tri);

/**
 * A synonym for writeRecogniser().  See writeRecogniser() for details.
 */
REGINA_API bool writeRecognizer(const char* filename, NTriangulation& tri);

/*@}*/

// Inline functions:

inline bool writeRecognizer(const char* filename, NTriangulation& tri) {
    return writeRecogniser(filename, tri);
}

} // namespace regina

#endif

