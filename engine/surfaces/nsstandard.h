
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

/*! \file nsstandard.h
 *  \brief Implements normal surface vectors using standard
 *  triangle-quad coordinates.
 */

#ifndef __NSSTANDARD_H
#ifndef __DOXYGEN
#define __NSSTANDARD_H
#endif

#include "surfaces/nnormalsurface.h"

namespace regina {

class NMatrixInt;

/**
 * \weakgroup surfaces
 * @{
 */

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
 * 1 and 2 (see NNormalSurface::getQuadCoord()).
 *
 * \ifacespython Not present.
 */
class NNormalSurfaceVectorStandard : public NNormalSurfaceVector {
    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalSurfaceVectorStandard(unsigned length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalSurfaceVectorStandard(const NVector<NLargeInteger>& cloneMe);

        virtual bool allowsAlmostNormal() const;

        virtual NLargeInteger getTriangleCoord(unsigned long tetIndex,
            int vertex, NTriangulation* triang) const;
        virtual NLargeInteger getQuadCoord(unsigned long tetIndex,
            int quadType, NTriangulation* triang) const;
        virtual NLargeInteger getOctCoord(unsigned long tetIndex,
            int octType, NTriangulation* triang) const;
        virtual NLargeInteger getEdgeWeight(unsigned long edgeIndex,
            NTriangulation* triang) const;
        virtual NLargeInteger getFaceArcs(unsigned long faceIndex,
            int faceVertex, NTriangulation* triang) const;

        virtual NVector<NLargeInteger>* clone() const;

        template <class RayOutputIterator, class FaceOutputIterator>
        static void createNonNegativeCone(NTriangulation* triangulation,
            RayOutputIterator rays, FaceOutputIterator faces);
        static NMatrixInt* makeMatchingEquations(NTriangulation* triangulation);
        static NCompConstraintSet* makeEmbeddedConstraints(
            NTriangulation* triangulation);
};

/*@}*/

// Inline functions for NNormalSurfaceVectorStandard

inline NNormalSurfaceVectorStandard::NNormalSurfaceVectorStandard(
        unsigned length) : NNormalSurfaceVector(length) {
}
inline NNormalSurfaceVectorStandard::NNormalSurfaceVectorStandard(
        const NVector<NLargeInteger>& cloneMe) :
        NNormalSurfaceVector(cloneMe) {
}

inline NLargeInteger NNormalSurfaceVectorStandard::getTriangleCoord(
        unsigned long tetIndex, int vertex, NTriangulation*) const {
    return (*this)[7 * tetIndex + vertex];
}
inline NLargeInteger NNormalSurfaceVectorStandard::getQuadCoord(
        unsigned long tetIndex, int quadType, NTriangulation*) const {
    return (*this)[7 * tetIndex + 4 + quadType];
}
inline NLargeInteger NNormalSurfaceVectorStandard::getOctCoord(
        unsigned long, int, NTriangulation*) const {
    return zero;
}

} // namespace regina

// Template definitions

#include "surfaces/nsstandard.tcc"

#endif

