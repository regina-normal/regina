
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

/*! \file surfaces/nsorientedquad.h
 *  \brief Implements normal surface vectors using
 *  transversely oriented quad coordinates.
 */

#ifndef __NSORIENTEDQUAD_H
#ifndef __DOXYGEN
#define __NSORIENTEDQUAD_H
#endif
#include "regina-core.h"
#include "surfaces/nsmirrored.h"

namespace regina {

class NMatrixInt;
class NNormalSurfaceVectorOrientedQuad;

/**
 * \weakgroup surfaces
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct NormalInfo<NS_ORIENTED_QUAD> {
    typedef NNormalSurfaceVectorOrientedQuad Class;
    typedef NormalInfo<NS_ORIENTED> Standard;
    typedef NormalInfo<NS_ORIENTED_QUAD> Reduced;
    inline static const char* name() {
        return "Transversely oriented quad normal";
    }
    enum {
        almostNormal = 0,
        spun = 1,
        oriented = 1
    };
};
#endif

/**
 * A normal surface vector using transversely oriented quadrilateral
 * coordinates.
 *
 * If there are \a t tetrahedra in the underlying
 * triangulation, there must be precisely 6<i>t</i> coordinates.
 * For each \a i, coordinates 2<i>i</i> and 2<i>i</i>+1 represent
 * the \c true and \c false orientations for coordinate \a i in the
 * 3<i>t</i>-dimensional quadrilateral coordinate system.  See
 * NNormalSurfaceVectorQuad for further details.
 *
 * \warning Support for transversely oriented normal surfaces is still
 * experimental, and some features \b will break (e.g., testing
 * connectedness, disjointness or embeddedness).
 *
 * \ifacespython Not present.
 */
class REGINA_API NNormalSurfaceVectorOrientedQuad :
        public NNormalSurfaceVectorMirrored {
    REGINA_NORMAL_SURFACE_FLAVOUR(NNormalSurfaceVectorOrientedQuad, NS_ORIENTED_QUAD)

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalSurfaceVectorOrientedQuad(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalSurfaceVectorOrientedQuad(const NVector<NLargeInteger>& cloneMe);

        virtual NNormalSurfaceVector* makeMirror(const NTriangulation* triang)
            const;

        virtual const NVertex* isVertexLink(const NTriangulation* triang) const;

        virtual NLargeInteger quads(size_t tetIndex,
            int quadType, const NTriangulation* triang) const;
        virtual NLargeInteger getOrientedQuadCoord(size_t tetIndex,
            int quadType, const NTriangulation* triang, bool orientation) const;
        virtual NLargeInteger octs(size_t tetIndex,
            int octType, const NTriangulation* triang) const;

        static NNormalSurfaceVector* makeZeroVector(
            const NTriangulation* triangulation);
        static NMatrixInt* makeMatchingEquations(
            const NTriangulation* triangulation);
        static NEnumConstraintList* makeEmbeddedConstraints(
            const NTriangulation* triangulation);
};

/*@}*/

// Inline functions for NNormalSurfaceVectorOrientedQuad

inline NNormalSurfaceVectorOrientedQuad::NNormalSurfaceVectorOrientedQuad(
        size_t length) : NNormalSurfaceVectorMirrored(length) {
}
inline NNormalSurfaceVectorOrientedQuad::NNormalSurfaceVectorOrientedQuad(
        const NVector<NLargeInteger>& cloneMe) :
        NNormalSurfaceVectorMirrored(cloneMe) {
}

inline NLargeInteger NNormalSurfaceVectorOrientedQuad::quads(
        size_t tetIndex, int quadType, const NTriangulation* tri) const {
    return getOrientedQuadCoord(tetIndex, quadType, tri, true)
           + getOrientedQuadCoord(tetIndex, quadType, tri, false);
}

inline NLargeInteger NNormalSurfaceVectorOrientedQuad::getOrientedQuadCoord(
        size_t tetIndex, int quadType, const NTriangulation*, 
        bool orientation) const {
    return (*this)[6 * tetIndex + 2 * quadType +
        (orientation ? 0 : 1)];
}

inline const NVertex* NNormalSurfaceVectorOrientedQuad::isVertexLink(
        const NTriangulation*) const {
    // Quad space does not contain vertex links at all.
    return 0;
}

inline NLargeInteger NNormalSurfaceVectorOrientedQuad::octs(
        size_t, int, const NTriangulation*) const {
    return zero;
}

} // namespace regina

#endif

