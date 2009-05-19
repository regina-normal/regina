
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

/*! \file surfaces/nsquadoct.h
 *  \brief Implements almost normal surface vectors using quad-oct coordinates.
 */

#ifndef __NSQUADOCT_H
#ifndef __DOXYGEN
#define __NSQUADOCT_H
#endif

#include "surfaces/nsmirrored.h"

namespace regina {

class NMatrixInt;

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * An almost normal surface vector using quad-oct coordinates.
 *
 * If there are \a t tetrahedra in the underlying
 * triangulation, there must be precisely 6<i>t</i> coordinates.
 * The first six coordinates will be for the first tetrahedron, the
 * next six for the second tetrahedron and so on.  For each
 * tetrahedron, the first three coordinates represent the number of
 * quadrilateral discs of type 0, 1 and 2 (see NNormalSurface::getQuadCoord()),
 * and the final three represent the number of octagonal discs of type
 * 0, 1 and 2 (see NNormalSurface::getOctCoord()).
 *
 * \ifacespython Not present.
 */
class NNormalSurfaceVectorQuadOct : public NNormalSurfaceVectorMirrored {
    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalSurfaceVectorQuadOct(unsigned length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalSurfaceVectorQuadOct(const NVector<NLargeInteger>& cloneMe);

        virtual NNormalSurfaceVector* makeMirror(NTriangulation* triang) const;

        virtual bool allowsAlmostNormal() const;
        virtual const NVertex* isVertexLink(NTriangulation* triang) const;

        virtual NVector<NLargeInteger>* clone() const;

        static NNormalSurfaceVector* makeZeroVector(
            const NTriangulation* triangulation);
        static NMatrixInt* makeMatchingEquations(NTriangulation* triangulation);
        static NEnumConstraintList* makeEmbeddedConstraints(
            NTriangulation* triangulation);
};

/*@}*/

// Inline functions for NNormalSurfaceVectorQuadOct

inline NNormalSurfaceVectorQuadOct::NNormalSurfaceVectorQuadOct(
        unsigned length) : NNormalSurfaceVectorMirrored(length) {
}
inline NNormalSurfaceVectorQuadOct::NNormalSurfaceVectorQuadOct(
        const NVector<NLargeInteger>& cloneMe) :
        NNormalSurfaceVectorMirrored(cloneMe) {
}

inline const NVertex* NNormalSurfaceVectorQuadOct::isVertexLink(
        NTriangulation*) const {
    // Quad-oct space does not contain vertex links at all.
    return 0;
}

} // namespace regina

#endif

