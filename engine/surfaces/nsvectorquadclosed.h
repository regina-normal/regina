
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file surfaces/nsvectorquadclosed.h
 *  \brief Implements normal surface vectors for closed surfaces in
 *  quad coordinates.
 */

#ifndef __NSVECTORQUADCLOSED_H
#ifndef __DOXYGEN
#define __NSVECTORQUADCLOSED_H
#endif

#include "regina-core.h"
#include "surfaces/nsvectorquad.h"

namespace regina {

class NSVectorQuadClosed;

/**
 * \weakgroup surfaces
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct NormalInfo<NS_QUAD_CLOSED> {
    typedef NSVectorQuadClosed Class;
    typedef NormalInfo<NS_STANDARD> Standard;
    typedef NormalInfo<NS_QUAD_CLOSED> Reduced;
    inline static const char* name() {
        return "Closed quad";
    }
    enum {
        almostNormal = 0,
        spun = 0,
        oriented = 0
    };
};
#endif

/**
 * The vector for a normal surface in an ideal triangulation, expressed using
 * quad coordinates and enumerated to include closed surfaces only.
 *
 * The underlying coordinate system is identical to "plain" quad coordinates,
 * as represented by the parent class NSVectorQuad.  The difference with
 * NSVectorQuadClosed is that, when \e enumerating surfaces in this class,
 * it adds linear constraints to ensure that only closed normal surfaces
 * are obtained (as opposed to spun-normal surfaces).  The resulting
 * solution space is the space Q_0 as described in "Computing closed
 * essential surfaces in knot complements", by Burton, Coward and Tillmann,
 * in SCG ’13: Proceedings of the 29th Annual Symposium on Computational
 * Geometry, ACM, 2013, pp. 405–414.
 *
 * Note that, if a vertex surface in quad coordinates is closed, then it
 * will also be a vertex surface in this system of "closed quad coordinates".
 * However, the converse is not true: a vertex surface in closed quad
 * coordinates need not be a vertex in "plain" quad coordinates.
 *
 * Recall that, in quad coordinates, if there are \a t tetrahedra in the
 * underlying triangulation then there are precisely 3<i>t</i> coordinates.
 * The first three coordinates will be for the first tetrahedron, the
 * next three for the second tetrahedron and so on.  For each
 * tetrahedron, the three individual coordinates represent the
 * number of quadrilateral discs of type 0, 1 and 2
 * (see NormalSurface::quads()).
 *
 * \pre This coordinate system can only be used with a limited class of
 * triangulations, as documented in the coordinate constant NS_QUAD_CLOSED.
 * Currently, such triangulations \e must be oriented and ideal, with precisely
 * one torus cusp and no other boundary components or internal vertices.
 * Moreover, SnapPea must be able to work with them without retriangulating
 * (though this should follow from the other constraints).
 *
 * \ifacespython Not present.
 */
class REGINA_API NSVectorQuadClosed : public NSVectorQuad {
    REGINA_NORMAL_SURFACE_FLAVOUR(NSVectorQuadClosed, NS_QUAD_CLOSED,
        NSVectorQuad);

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NSVectorQuadClosed(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NSVectorQuadClosed(const Vector<LargeInteger>& cloneMe);

        static NormalSurfaceVector* makeZeroVector(
            const Triangulation<3>* triangulation);
        static MatrixInt* makeMatchingEquations(
            const Triangulation<3>* triangulation);
};

/*@}*/

// Inline functions for NSVectorQuadClosed

inline NSVectorQuadClosed::NSVectorQuadClosed(size_t length) :
        NSVectorQuad(length) {
}

inline NSVectorQuadClosed::NSVectorQuadClosed(
        const Vector<LargeInteger>& cloneMe) :
        NSVectorQuad(cloneMe) {
}

} // namespace regina

#endif

