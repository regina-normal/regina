
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

/*! \file ndoubledescriptor.h
 *  \brief Provides a modified double descriptor method for polytope
 *  vertex enumeration.
 */

#ifndef __NDOUBLEDESCRIPTOR_H
#ifndef __DOXYGEN
#define __NDOUBLEDESCRIPTOR_H
#endif

#include "enumerate/nenumerator.h"

namespace regina {

/**
 * \weakgroup enumerate
 * @{
 */

/**
 * Implements a modified double descriptor method for polytope vertex
 * enumeration.
 *
 * \ifaces Not present.
 */
class NDoubleDescriptor : public NVertexEnumerator {
    public:
        /**
         * Creates a new vertex enumeration engine.
         */
        NDoubleDescriptor();

        template <class OutputIterator, class RayIterator, class FaceIterator>
        void enumerateVertices(OutputIterator results,
            RayIterator oldRaysFirst, RayIterator oldRaysLast,
            FaceIterator facesFirst, FaceIterator facesLast,
            const NMatrixInt& subspace, const NCompConstraintSet* constraints)
            const;

    private:
        /**
         * Implements vertex enumeration for the simplified case in which
         * the given linear subspace is just a single hyperplane.
         *
         * In all other respects this routine behaves identically to
         * the full vertex enumeration routine
         * enumerateVertices(OutputIterator, RayIterator, RayIterator, FaceIterator, FaceIterator, const NMatrixInt&, const NCompConstraintSet*).
         *
         * All parameters not listed are identical to those for the full
         * vertex enumeration routine.
         *
         * @param hyperplane the hyperplane that forms the given linear
         * subspace; this hyperplane is represented by a vertex
         * perpendicular to it.
         */
        template <class OutputIterator, class RayIterator, class FaceIterator>
        void enumerateVertices(OutputIterator results,
            RayIterator oldRaysFirst, RayIterator oldRaysLast,
            FaceIterator facesFirst, FaceIterator facesLast,
            const NVector<NLargeInteger>& hyperplane,
            const NCompConstraintSet* constraints) const;
};

/*@}*/

// Inline functions for NDoubleDescriptor

inline NDoubleDescriptor::NDoubleDescriptor() {
}

} // namespace regina

// Template definitions

#include "enumerate/ndoubledescriptor.tcc"

#endif

