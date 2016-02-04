
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file foreign/recogniser.h
 *  \brief Deprecated header that allows exports to Matveev's 3-manifold
 *  recogniser.
 *
 *  \deprecated All global functions that were once declared in this file have
 *  now been renamed to member functions of NTriangulation.
 */

#ifndef __RECOGNISER_H
#ifndef __DOXYGEN
#define __RECOGNISER_H
#endif

#include <string>
#include "regina-core.h"
#include "triangulation/ntriangulation.h"

namespace regina {

template <int> class Triangulation;
typedef Triangulation<3> NTriangulation;

/**
 * \weakgroup foreign
 * @{
 */

/**
 * Deprecated function that writes the given triangulation to the given file
 * in Matveev's 3-manifold recogniser format.
 *
 * \deprecated This routine has been renamed as
 * NTriangulation::saveRecogniser().  This old routine behaves exactly as in
 * the past, but the new NTriangulation::saveRecogniser() has a slight change
 * of behaviour: unlike the old routine, it now requires a non-empty filename.
 * To send data to standard output (as in the old routine), you may call
 * NTriangulation::recogniser() and simply print the resulting string.
 * See the NTriangulation::saveRecogniser() documentation for further details.
 */
REGINA_API bool writeRecogniser(const char* filename, NTriangulation& tri);

/**
 * Deprecated function that writes the given triangulation to the given file
 * in Matveev's 3-manifold recogniser format.
 *
 * \deprecated This routine has been renamed as
 * NTriangulation::saveRecogniser().  This old routine behaves exactly as in
 * the past, but the new NTriangulation::saveRecogniser() has a slight change
 * of behaviour: unlike the old routine, it now requires a non-empty filename.
 * To send data to standard output (as in the old routine), you may call
 * NTriangulation::recogniser() and simply print the resulting string.
 * See the NTriangulation::saveRecogniser() documentation for further details.
 */
REGINA_API bool writeRecognizer(const char* filename, NTriangulation& tri);

/*@}*/

// Inline functions:

inline bool writeRecogniser(const char* filename, NTriangulation& tri) {
    return tri.saveRecogniser(filename);
}

inline bool writeRecognizer(const char* filename, NTriangulation& tri) {
    return tri.saveRecogniser(filename);
}

} // namespace regina

#endif

