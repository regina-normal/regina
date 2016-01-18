
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

/*! \file surfaces/nsanstandard.h
 *  \brief Implements almost normal surface vectors using standard
 *  triangle-quad-oct coordinates.
 */

#ifndef __NSANSTANDARD_H
#ifndef __DOXYGEN
#define __NSANSTANDARD_H
#endif

#include "regina-core.h"
#include "surfaces/nnormalsurface.h"

namespace regina {

class NMatrixInt;
class NNormalSurfaceVectorANStandard;

/**
 * \weakgroup surfaces
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct NormalInfo<NS_AN_STANDARD> {
    typedef NNormalSurfaceVectorANStandard Class;
    typedef NormalInfo<NS_AN_STANDARD> Standard;
    typedef NormalInfo<NS_AN_QUAD_OCT> Reduced;
    inline static const char* name() {
        return "Standard almost normal (tri-quad-oct)";
    }
    enum {
        almostNormal = 1,
        spun = 0,
        oriented = 0
    };
};
#endif

/**
 * An almost normal surface vector using standard triangle-quad-oct
 * coordinates.
 *
 * If there are \a t tetrahedra in the underlying
 * triangulation, there must be precisely 10<i>t</i> coordinates.
 * The first ten coordinates will be for the first tetrahedron, the
 * next ten for the second tetrahedron and so on.  For each
 * tetrahedron, the first four represent the number of
 * triangular discs about vertex 0, 1, 2 and 3, the next
 * three represent the number of quadrilateral discs of type 0,
 * 1 and 2 (see NNormalSurface::quads()) and the final three
 * represent the number of octagonal discs of type 0, 1 and 2 (see
 * NNormalSurface::octs()).
 *
 * \ifacespython Not present.
 */
class REGINA_API NNormalSurfaceVectorANStandard : public NNormalSurfaceVector {
    REGINA_NORMAL_SURFACE_FLAVOUR(NNormalSurfaceVectorANStandard, NS_AN_STANDARD)

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalSurfaceVectorANStandard(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalSurfaceVectorANStandard(const NVector<NLargeInteger>& cloneMe);

        virtual NLargeInteger triangles(size_t tetIndex,
            int vertex, const NTriangulation* triang) const;
        virtual NLargeInteger quads(size_t tetIndex,
            int quadType, const NTriangulation* triang) const;
        virtual NLargeInteger octs(size_t tetIndex,
            int octType, const NTriangulation* triang) const;
        virtual NLargeInteger edgeWeight(size_t edgeIndex,
            const NTriangulation* triang) const;
        virtual NLargeInteger arcs(size_t triIndex,
            int triVertex, const NTriangulation* triang) const;

        static NNormalSurfaceVector* makeZeroVector(
            const NTriangulation* triangulation);
        static NMatrixInt* makeMatchingEquations(
            const NTriangulation* triangulation);
        static NEnumConstraintList* makeEmbeddedConstraints(
            const NTriangulation* triangulation);
};

/*@}*/

// Inline functions for NNormalSurfaceVectorANStandard

inline NNormalSurfaceVectorANStandard::NNormalSurfaceVectorANStandard(
        size_t length) : NNormalSurfaceVector(length) {
}
inline NNormalSurfaceVectorANStandard::NNormalSurfaceVectorANStandard(
        const NVector<NLargeInteger>& cloneMe) :
        NNormalSurfaceVector(cloneMe) {
}

inline NLargeInteger NNormalSurfaceVectorANStandard::triangles(
        size_t tetIndex, int vertex, const NTriangulation*) const {
    return (*this)[10 * tetIndex + vertex];
}
inline NLargeInteger NNormalSurfaceVectorANStandard::quads(
        size_t tetIndex, int quadType, const NTriangulation*) const {
    return (*this)[10 * tetIndex + 4 + quadType];
}
inline NLargeInteger NNormalSurfaceVectorANStandard::octs(
        size_t tetIndex, int octType, const NTriangulation*) const {
    return (*this)[10 * tetIndex + 7 + octType];
}

} // namespace regina

#endif

