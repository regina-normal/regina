
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file nlayering.h
 *  \brief Assists with the analysis of layerings upon a torus boundary.
 */

#ifndef __NLAYERING_H
#ifndef __DOXYGEN
#define __NLAYERING_H
#endif

#include "triangulation/nperm.h"
#include "utilities/boostutils.h"
#include "utilities/nmatrix2.h"

namespace regina {

class NTetrahedron;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a layering of zero or more tetrahedra upon a torus
 * boundary.
 *
 * A \e layering involves laying a new tetrahedron flat upon two
 * adjacent boundary faces in order to change the boundary curves.  Many
 * tetrahedra may be layered upon a boundary in succession in order to
 * change the boundary curves more dramatically.
 *
 * A torus boundary is specified by two tetrahedra (which may be the same)
 * and two permutations.  Each permutation maps (0,1,2) in the diagram
 * below to the corresponding vertex numbers in each tetrahedron (and
 * therefore maps 3 to the corresponding face number).
 *
 * <pre>
 *     *--->>--*
 *     |0  1 / |
 *     |    / 2|
 *     v   /   v
 *     |2 /    |
 *     | / 1  0|
 *     *--->>--*
 * </pre>
 *
 * In particular, if the two tetrahedra are \a t1 and \a t2 and the two
 * corresponding permutations are \a p1 and \a p2, then:
 * - the torus boundary is formed from faces \a p1[3] and \a p2[3] of
 *   tetrahedra \a t1 and \a t2 respectively;
 * - edges \a p1[0]-\a p1[1] and \a p2[1]-\a p2[0] of tetrahedra
 *   \a t1 and \a t2 respectively are identified;
 * - edges \a p1[1]-\a p1[2] and \a p2[2]-\a p2[1] of tetrahedra
 *   \a t1 and \a t2 respectively are identified;
 * - edges \a p1[2]-\a p1[0] and \a p2[0]-\a p2[2] of tetrahedra
 *   \a t1 and \a t2 respectively are identified.
 *
 * Note that we do not actually require these faces to form a torus, and
 * this is never verifed by any of the routines in this class.  What
 * these routines do is use the diagram above to define the rules of
 * what forms a valid layering (and in fact the layering itself will
 * often be the cause of these edge identifications).  This allows the
 * NLayering class a little more versatility in degenerate and boundary cases.
 *
 * This class keeps track of an \e old boundary, which is the original
 * pair of faces upon which the first tetrahedron is layered, and a
 * \e new boundary, which is formed by the last layered tetrahedron and
 * contains the modified boundary curves.  If no tetrahedra are layered
 * at all then the old and new boundaries will be identical.
 *
 * To search for a layering upon a particular pair of faces within a
 * triangulation, construct a trivial (zero-tetrahedron) layering upon
 * these faces using the NLayering constructor and then call extend().
 */
class NLayering : public boost::noncopyable {
    private:
        unsigned long size;
            /**< The number of tetrahedra that have been layered. */

        NTetrahedron* oldBdryTet[2];
            /**< The two tetrahedra of the old boundary (these may be
                 the same).  See the class notes for details. */
        NPerm oldBdryRoles[2];
            /**< The corresponding two permutations of the old boundary.
                 See the class notes for details. */

        NTetrahedron* newBdryTet[2];
            /**< The two tetrahedra of the new boundary (these may be
                 the same).  See the class notes for details. */
        NPerm newBdryRoles[2];
            /**< The corresponding two permutations of the new boundary.
                 See the class notes for details. */

        NMatrix2 reln;
            /**< A matrix that expresses the new boundary curves in terms
                 of the old, assuming that the old boundary is in fact a
                 torus as described in the class notes.  The first row of
                 \a reln expresses the new \a roles[0-1] curve in terms of
                 the old \a roles[0-1] and \a roles[0-2] curves, and the
                 second row expresses the new \a roles[0-2] curve in a
                 similar fashion.  It is guaranteed that the determinant
                 of this matrix is 1. */

    public:
        NLayering(NTetrahedron* bdry0, NPerm roles0, NTetrahedron* bdry1,
            NPerm roles1);

        unsigned long getSize() const;

        NTetrahedron* getOldBoundaryTet(unsigned which) const;
        NPerm getOldBoundaryRoles(unsigned which) const;
        NTetrahedron* getNewBoundaryTet(unsigned which) const;
        NPerm getNewBoundaryRoles(unsigned which) const;

        bool extendOne();
        /**
         * Some stuff.
         */
        unsigned long extend();

        bool matchesTop(NTetrahedron* upperBdry0, NPerm upperRoles0,
            NTetrahedron* upperBdry1, NPerm upperRoles1,
            NMatrix2& upperReln) const;
};

/*@}*/

// Inline functions for NLayering

inline unsigned long NLayering::getSize() const {
    return size;
}

inline NTetrahedron* NLayering::getOldBoundaryTet(unsigned which) const {
    return oldBdryTet[which];
}

inline NPerm NLayering::getOldBoundaryRoles(unsigned which) const {
    return oldBdryRoles[which];
}

inline NTetrahedron* NLayering::getNewBoundaryTet(unsigned which) const {
    return newBdryTet[which];
}

inline NPerm NLayering::getNewBoundaryRoles(unsigned which) const {
    return newBdryRoles[which];
}

} // namespace regina

#endif

