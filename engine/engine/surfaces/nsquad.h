
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nsmirrored.h"
    #include "ndoublelist.h"
#else
    #include "engine/surfaces/nsmirrored.h"
    #include "engine/utilities/ndoublelist.h"
#endif

class NMatrixInt;

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
 * \ifaces Not present.
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
        virtual bool isCompatibleWith(const NNormalSurfaceVector& other) const;

        virtual NLargeInteger getOctCoord(unsigned long tetIndex,
            int octType, NTriangulation* triang) const;

        virtual NVector<NLargeInteger>* clone() const;

        static NDoubleList<NNormalSurfaceVector*>* createNonNegativeCone(
            NTriangulation* triang);
        static NMatrixInt* makeMatchingEquations(NTriangulation* triang);
};

// Inline functions for NNormalSurfaceVectorQuad

inline NNormalSurfaceVectorQuad::NNormalSurfaceVectorQuad(
        unsigned length) : NNormalSurfaceVectorMirrored(length) {
}
inline NNormalSurfaceVectorQuad::NNormalSurfaceVectorQuad(
        const NVector<NLargeInteger>& cloneMe) :
        NNormalSurfaceVectorMirrored(cloneMe) {
}

inline NLargeInteger NNormalSurfaceVectorQuad::getOctCoord(
        unsigned long, int, NTriangulation*) const {
    return zero;
}

#endif

