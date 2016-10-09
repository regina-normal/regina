
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

/*! \file surfaces/nsstandard.h
 *  \brief Implements normal surface vectors using standard
 *  triangle-quad coordinates.
 */

#ifndef __NSSTANDARD_H
#ifndef __DOXYGEN
#define __NSSTANDARD_H
#endif

#include "regina-core.h"
#include "surfaces/nnormalsurface.h"

namespace regina {

class NNormalSurfaceVectorStandard;

/**
 * \weakgroup surfaces
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct NormalInfo<NS_STANDARD> {
    typedef NNormalSurfaceVectorStandard Class;
    typedef NormalInfo<NS_STANDARD> Standard;
    typedef NormalInfo<NS_QUAD> Reduced;
    inline static const char* name() {
        return "Standard normal (tri-quad)";
    }
    enum {
        almostNormal = 0,
        spun = 0,
        oriented = 0
    };
};
#endif

/**
 * A normal surface vector using standard triangle-quad coordinates.
 *
 * If there are \a t tetrahedra in the underlying
 * triangulation, there must be precisely 7<i>t</i> coordinates.
 * The first seven coordinates will be for the first tetrahedron, the
 * next seven for the second tetrahedron and so on.  For each
 * tetrahedron, the first four represent the number of
 * triangular discs about vertex 0, 1, 2 and 3, and the next
 * three represent the number of quadrilateral discs of type 0,
 * 1 and 2 (see NNormalSurface::quads()).
 *
 * \ifacespython Not present.
 */
class REGINA_API NNormalSurfaceVectorStandard : public NNormalSurfaceVector {
    REGINA_NORMAL_SURFACE_FLAVOUR(NNormalSurfaceVectorStandard, NS_STANDARD)

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalSurfaceVectorStandard(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalSurfaceVectorStandard(const Vector<LargeInteger>& cloneMe);

        virtual LargeInteger triangles(size_t tetIndex,
            int vertex, const NTriangulation* triang) const;
        virtual LargeInteger quads(size_t tetIndex,
            int quadType, const NTriangulation* triang) const;
        virtual LargeInteger octs(size_t tetIndex,
            int octType, const NTriangulation* triang) const;
        virtual LargeInteger edgeWeight(size_t edgeIndex,
            const NTriangulation* triang) const;
        virtual LargeInteger arcs(size_t triIndex,
            int triVertex, const NTriangulation* triang) const;

        static NNormalSurfaceVector* makeZeroVector(
            const NTriangulation* triangulation);
        static MatrixInt* makeMatchingEquations(
            const NTriangulation* triangulation);
        static EnumConstraints* makeEmbeddedConstraints(
            const NTriangulation* triangulation);
};

/*@}*/

// Inline functions for NNormalSurfaceVectorStandard

inline NNormalSurfaceVectorStandard::NNormalSurfaceVectorStandard(
        size_t length) : NNormalSurfaceVector(length) {
}
inline NNormalSurfaceVectorStandard::NNormalSurfaceVectorStandard(
        const Vector<LargeInteger>& cloneMe) :
        NNormalSurfaceVector(cloneMe) {
}

inline LargeInteger NNormalSurfaceVectorStandard::triangles(
        size_t tetIndex, int vertex, const NTriangulation*) const {
    return coords_[7 * tetIndex + vertex];
}
inline LargeInteger NNormalSurfaceVectorStandard::quads(
        size_t tetIndex, int quadType, const NTriangulation*) const {
    return coords_[7 * tetIndex + 4 + quadType];
}
inline LargeInteger NNormalSurfaceVectorStandard::octs(
        size_t, int, const NTriangulation*) const {
    return Ray::zero;
}

} // namespace regina

#endif

