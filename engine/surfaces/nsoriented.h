
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

/*! \file surfaces/nsoriented.h
 *  \brief Implements normal surface vectors using transversly oriented 
 *  normal surface coordinates.*/

#ifndef __NSORIENTED_H
#ifndef __DOXYGEN
#define __NSORIENTED_H
#endif


#include "regina-core.h"
#include "surfaces/nnormalsurface.h"

namespace regina {
class NMatrixInt;

/**
 * \weakgroup surfaces
 * @{
 */

/** TODO
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
class REGINA_API NNormalSurfaceVectorOriented : public NNormalSurfaceVector {
    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalSurfaceVectorOriented(unsigned length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalSurfaceVectorOriented(const NVector<NLargeInteger>& cloneMe);

        virtual bool allowsAlmostNormal() const;
        virtual bool allowsSpun() const;

        virtual NLargeInteger getTriangleCoord(unsigned long tetIndex,
            int vertex, NTriangulation* triang) const;
        virtual NLargeInteger getQuadCoord(unsigned long tetIndex,
            int quadType, NTriangulation* triang) const;
        virtual NLargeInteger getTriangleCoord(unsigned long tetIndex,
            int vertex, NTriangulation* triang, bool orientation) const;
        virtual NLargeInteger getQuadCoord(unsigned long tetIndex,
            int quadType, NTriangulation* triang, bool orientation) const;
        virtual NLargeInteger getOctCoord(unsigned long tetIndex,
            int octType, NTriangulation* triang) const;
        virtual NLargeInteger getEdgeWeight(unsigned long edgeIndex,
            NTriangulation* triang) const;
        virtual NLargeInteger getFaceArcs(unsigned long faceIndex,
            int faceVertex, NTriangulation* triang) const;

        virtual NNormalSurfaceVector* clone() const;

        static NNormalSurfaceVector* makeZeroVector(
            const NTriangulation* triangulation);
        static NMatrixInt* makeMatchingEquations(NTriangulation* triangulation);
        static NEnumConstraintList* makeEmbeddedConstraints(
            NTriangulation* triangulation);
    private:
        static const int noOfCoords = 14;
        static const int indexOfQuads = 8;
};

/*@}*/

// Inline functions for NNormalSurfaceVectorOriented

inline NNormalSurfaceVectorOriented::NNormalSurfaceVectorOriented(
        unsigned length) : NNormalSurfaceVector(length) {
}
inline NNormalSurfaceVectorOriented::NNormalSurfaceVectorOriented(
        const NVector<NLargeInteger>& cloneMe) :
        NNormalSurfaceVector(cloneMe) {
}

inline NLargeInteger NNormalSurfaceVectorOriented::getTriangleCoord(
        unsigned long tetIndex, int vertex, NTriangulation*) const {
    return (*this)[noOfCoords * tetIndex + vertex];
}
inline NLargeInteger NNormalSurfaceVectorOriented::getQuadCoord(
        unsigned long tetIndex, int quadType, NTriangulation*) const {
    return (*this)[noOfCoords * tetIndex + indexOfQuads + quadType];
}

inline NLargeInteger NNormalSurfaceVectorOriented::getTriangleCoord(
        unsigned long tetIndex, int vertex, NTriangulation*,
        bool orientation) const {
    return (*this)[noOfCoords * tetIndex + 2*vertex + orientation];
}
inline NLargeInteger NNormalSurfaceVectorOriented::getQuadCoord(
        unsigned long tetIndex, int quadType, NTriangulation*, 
        bool orientation) const {
    return (*this)[noOfCoords * tetIndex + indexOfQuads + 2*quadType + orientation];
}
inline NLargeInteger NNormalSurfaceVectorOriented::getOctCoord(
        unsigned long, int, NTriangulation*) const {
    return zero;
}

} // namespace regina

#endif

