
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file nsquad.h
 *  \brief Implements normal surface vectors using quad coordinates.
 */

#ifndef __NSQUAD_H
#ifndef __DOXYGEN
#define __NSQUAD_H
#endif

#include "surfaces/nsmirrored.h"

namespace regina {

class NMatrixInt;

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * A normal surface vector using quad coordinates.
 *
 * If there are \a t tetrahedra in the underlying
 * triangulation, there must be precisely 3<i>t</i> coordinates.
 * The first three coordinates will be for the first tetrahedron, the
 * next three for the second tetrahedron and so on.  For each
 * tetrahedron, the three individual coordinates represent the
 * number of quadrilateral discs of type 0, 1 and 2
 * (see NNormalSurface::getQuadCoord()).
 *
 * \ifacespython Not present.
 */
class NNormalSurfaceVectorQuad : public NNormalSurfaceVectorMirrored {
    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalSurfaceVectorQuad(unsigned length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalSurfaceVectorQuad(const NVector<NLargeInteger>& cloneMe);

        virtual NNormalSurfaceVector* makeMirror(NTriangulation* triang) const;

        virtual bool allowsAlmostNormal() const;
        virtual const NVertex* isVertexLink(NTriangulation* triang) const;

        virtual NLargeInteger getOctCoord(unsigned long tetIndex,
            int octType, NTriangulation* triang) const;

        virtual NVector<NLargeInteger>* clone() const;

        template <class RayOutputIterator, class FaceOutputIterator>
        static void createNonNegativeCone(NTriangulation* triangulation,
            RayOutputIterator rays, FaceOutputIterator faces);
        static NMatrixInt* makeMatchingEquations(NTriangulation* triangulation);
        static NCompConstraintSet* makeEmbeddedConstraints(
            NTriangulation* triangulation);
};

/*@}*/

// Inline functions for NNormalSurfaceVectorQuad

inline NNormalSurfaceVectorQuad::NNormalSurfaceVectorQuad(
        unsigned length) : NNormalSurfaceVectorMirrored(length) {
}
inline NNormalSurfaceVectorQuad::NNormalSurfaceVectorQuad(
        const NVector<NLargeInteger>& cloneMe) :
        NNormalSurfaceVectorMirrored(cloneMe) {
}

inline const NVertex* NNormalSurfaceVectorQuad::isVertexLink(
        NTriangulation*) const {
    // Quad space does not contain vertex links at all.
    return 0;
}

inline NLargeInteger NNormalSurfaceVectorQuad::getOctCoord(
        unsigned long, int, NTriangulation*) const {
    return zero;
}

} // namespace regina

// Template definitions

#include "surfaces/nsquad.tcc"

#endif

