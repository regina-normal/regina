
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/**
 * \weakgroup surfaces
 * @{
 */

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
    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalSurfaceVectorOrientedQuad(unsigned length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalSurfaceVectorOrientedQuad(const NVector<NLargeInteger>& cloneMe);

        virtual NNormalSurfaceVector* makeMirror(NTriangulation* triang) const;

        virtual bool allowsAlmostNormal() const;
        virtual bool allowsSpun() const;
        virtual bool allowsOriented() const;
        virtual const NVertex* isVertexLink(NTriangulation* triang) const;

        virtual NLargeInteger getQuadCoord(unsigned long tetIndex,
            int quadType, NTriangulation* triang) const;
        virtual NLargeInteger getOrientedQuadCoord(unsigned long tetIndex,
            int quadType, NTriangulation* triang, bool orientation) const;
        virtual NLargeInteger getOctCoord(unsigned long tetIndex,
            int octType, NTriangulation* triang) const;

        virtual NNormalSurfaceVector* clone() const;

        static NNormalSurfaceVector* makeZeroVector(
            const NTriangulation* triangulation);
        static NMatrixInt* makeMatchingEquations(NTriangulation* triangulation);
        static NEnumConstraintList* makeEmbeddedConstraints(
            NTriangulation* triangulation);
};

/*@}*/

// Inline functions for NNormalSurfaceVectorOrientedQuad

inline NNormalSurfaceVectorOrientedQuad::NNormalSurfaceVectorOrientedQuad(
        unsigned length) : NNormalSurfaceVectorMirrored(length) {
}
inline NNormalSurfaceVectorOrientedQuad::NNormalSurfaceVectorOrientedQuad(
        const NVector<NLargeInteger>& cloneMe) :
        NNormalSurfaceVectorMirrored(cloneMe) {
}

inline NLargeInteger NNormalSurfaceVectorOrientedQuad::getQuadCoord(
        unsigned long tetIndex, int quadType, NTriangulation* tri) const {
    return getOrientedQuadCoord(tetIndex, quadType, tri, true)
           + getOrientedQuadCoord(tetIndex, quadType, tri, false);
}

inline NLargeInteger NNormalSurfaceVectorOrientedQuad::getOrientedQuadCoord(
        unsigned long tetIndex, int quadType, NTriangulation*, 
        bool orientation) const {
    return (*this)[6 * tetIndex + 2 * quadType +
        (orientation ? 0 : 1)];
}

inline const NVertex* NNormalSurfaceVectorOrientedQuad::isVertexLink(
        NTriangulation*) const {
    // Quad space does not contain vertex links at all.
    return 0;
}

inline NLargeInteger NNormalSurfaceVectorOrientedQuad::getOctCoord(
        unsigned long, int, NTriangulation*) const {
    return zero;
}

} // namespace regina

#endif

