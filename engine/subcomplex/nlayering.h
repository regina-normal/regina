
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

/*! \file nlayering.h
 *  \brief Assists with the analysis of layerings upon a torus boundary.
 */

#ifndef __NLAYERING_H
#ifndef __DOXYGEN
#define __NLAYERING_H
#endif

#include "maths/nmatrix2.h"
#include "triangulation/nperm.h"
#include "utilities/boostutils.h"

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
 *     |0  2 / |
 *     |    / 1|
 *     v   /   v
 *     |1 /    |
 *     | / 2  0|
 *     *--->>--*
 * </pre>
 *
 * In particular, if the two tetrahedra are \a t0 and \a t1 and the two
 * corresponding permutations are \a p0 and \a p1, then:
 * - the torus boundary is formed from faces \a p0[3] and \a p1[3] of
 *   tetrahedra \a t0 and \a t1 respectively;
 * - edges \a p0[0]-\a p0[1] and \a p1[1]-\a p1[0] of tetrahedra
 *   \a t0 and \a t1 respectively are identified;
 * - edges \a p0[1]-\a p0[2] and \a p1[2]-\a p1[1] of tetrahedra
 *   \a t0 and \a t1 respectively are identified;
 * - edges \a p0[2]-\a p0[0] and \a p1[0]-\a p1[2] of tetrahedra
 *   \a t0 and \a t1 respectively are identified.
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
 * This class is used to search for layerings as follows.  The
 * constructor is called with a particular pair of faces that will form
 * the old boundary (note that these are generally \e not boundary faces
 * in the triangulation, since we are searching for layerings that have
 * been placed upon them).  This forms a trivial (zero-tetrahedron)
 * layering.  The routines extend() or extendOne() are then called to see
 * how many additional tetrahedra have been layered upon this pair of faces
 * according to the rules above.
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
                 similar fashion (here we always talk in terms of the
                 first tetrahedron for each boundary).  It is guaranteed
                 that the determinant of this matrix is 1. */

    public:
        /**
         * Creates a new trivial (zero-tetrahedron) layering upon the
         * given boundary.
         *
         * The boundary is described by two tetrahedra and two
         * permutations as explained in the class notes.  Note that the
         * given tetrahedra need not be boundary faces in the triangulation
         * (and if search routines such as extend() are called then they
         * almost certainly should not be).
         *
         * @param bdry0 the tetrahedron providing the first face of the
         * boundary.
         * @param roles0 the permutation describing how this first face is
         * formed from three vertices of tetrahedron \a bdry0, as
         * described in the class notes.
         * @param bdry1 the tetrahedron providing the second face of the
         * boundary.
         * @param roles1 the permutation describing how this second face is
         * formed from three vertices of tetrahedron \a bdry1.
         */
        NLayering(NTetrahedron* bdry0, NPerm roles0, NTetrahedron* bdry1,
            NPerm roles1);

        /**
         * Returns the number of individual tetrahedra that have been
         * layered onto the original boundary, according to the data
         * stored in this structure.
         *
         * This begins at zero when the class constructor is called, and
         * it increases if the routines extend() or extendOne() find that
         * additional layerings have taken place.
         *
         * @return the number of layered tetrahedra.
         */
        unsigned long getSize() const;

        /**
         * Returns the tetrahedra that provide the old boundary faces.
         * These belong to the original boundary before any layerings
         * take place.
         *
         * See the NLayering class notes for details on how a torus
         * boundary is formed from two tetrahedra and two permutations.
         *
         * @param which specifies which tetrahedron to return; this must
         * be either 0 or 1.
         * @return the requested tetrahedron of the old boundary.
         */
        NTetrahedron* getOldBoundaryTet(unsigned which) const;
        /**
         * Returns the permutations that describe the old boundary faces.
         * These refer to the original boundary before any layerings
         * take place.
         *
         * See the NLayering class notes for details on how a torus
         * boundary is formed from two tetrahedra and two permutations.
         *
         * @param which specifies which permutation to return; this must
         * be either 0 or 1.
         * @return the requested permutation describing the old boundary.
         */
        NPerm getOldBoundaryRoles(unsigned which) const;
        /**
         * Returns the tetrahedra that provide the new boundary faces.
         * These belong to the final boundary after layerings have been
         * performed.
         *
         * See the NLayering class notes for details on how a torus
         * boundary is formed from two tetrahedra and two permutations.
         *
         * @param which specifies which tetrahedron to return; this must
         * be either 0 or 1.
         * @return the requested tetrahedron of the new boundary.
         */
        NTetrahedron* getNewBoundaryTet(unsigned which) const;
        /**
         * Returns the permutations that describe the new boundary faces.
         * These refer to the final boundary after layerings have been
         * performed.
         *
         * See the NLayering class notes for details on how a torus
         * boundary is formed from two tetrahedra and two permutations.
         *
         * @param which specifies which permutation to return; this must
         * be either 0 or 1.
         * @return the requested permutation describing the new boundary.
         */
        NPerm getNewBoundaryRoles(unsigned which) const;

        /**
         * Returns a 2-by-2 matrix describing the relationship between
         * curves on the old and new boundary tori.  Note that this
         * relationship will often be non-trivial, since one of the
         * key reasons for layering is to modify boundary curves.
         *
         * Let \a t and \a p be the first tetrahedron and
         * permutation of the old boundary (as returned by
         * getOldBoundaryTet(0) and getOldBoundaryRoles(0)), and let
         * \a old_x and \a old_y be the directed edges \a p[0]-\a p[1]
         * and \a p[0]-\a p[2] respectively of tetrahedron \a t (these
         * are the leftmost and uppermost edges of the diagram below).
         * Likewise, let \a s and \a q be the first tetrahedron and
         * permutation of the new boundary (as returned by
         * getNewBoundaryTet(0) and getNewBoundaryRoles(0)), and let
         * \a new_x and \a new_y be the directed edges \a q[0]-\a q[1]
         * and \a q[0]-\a q[2] respectively of tetrahedron \a s.
         *
         * <pre>
         *     *--->>--*
         *     |0  2 / |
         *     |    / 1|
         *     v   /   v
         *     |1 /    |
         *     | / 2  0|
         *     *--->>--*
         * </pre>
         *
         * Assuming both boundaries are tori, edges \a old_x and \a old_y are
         * generators of the old boundary torus and edges \a new_x and
         * \a new_y are generators of the new boundary torus.  Suppose
         * that this routine returns the matrix \a M.  This signifies
         * that, using additive notation:
         *
         * <pre>
         *     [new_x]         [old_x]
         *     [     ]  =  M * [     ] .
         *     [new_y]         [old_y]
         * </pre>
         *
         * In other words, the matrix that is returned expresses the
         * generator curves of the new boundary in terms of the
         * generator curves of the old boundary.
         *
         * Note that the determinant of this matrix will always be 1.
         *
         * @return the matrix relating the old and new boundary curves.
         */
        const NMatrix2& boundaryReln() const;

        /**
         * Examines whether a single additional tetrahedron has been
         * layered upon the current new boundary.
         *
         * The new boundary faces are assumed to form a torus as
         * described in the class notes (this is not verified, and there
         * are degenerate cases where this will likely be false).  This
         * defines three possible ways in which an additional tetrahedron
         * may be layered (over the three boundary edges respectively).
         *
         * If it is found that an additional tetrahedron does exist and
         * has been joined to the new boundary in one of these three
         * possible ways, this structure is extended to incorporate the
         * additional tetrahedron.  The size will grow by one, and the
         * new boundary will become the remaining two faces of this
         * additional tetrahedron.
         * 
         * @return \c true if a tetrahedron was found as described above
         * and this structure was extended accordingly, or \c false otherwise.
         */
        bool extendOne();
        /**
         * Examines whether one or more additional tetrahedra have been
         * layered upon the current new boundary.
         *
         * Specifically, this routine calls extendOne() as many times as
         * possible.  If \a k additional layerings are discovered as a
         * result, the size of this structure will have grown by \a k
         * and the new boundary will be changed to describe the
         * remaining two faces of the \a kth layered tetrahedron.
         *
         * It is guaranteed that, once this routine is finished, the new
         * boundary will not have any additional tetrahedron layered
         * upon it.  That is, if extendOne() were called again then it
         * would return \c false.
         *
         * @return the number of additional layered tetrahedra that were
         * discovered.
         */
        unsigned long extend();

        /**
         * Determines whether the new torus boundary of this structure
         * is identified with the given torus boundary.  In other words,
         * this routine determines whether the new torus boundary of
         * this structure and the given torus boundary represent
         * opposite sides of the same two faces.
         *
         * The two boundaries must be identified according to some
         * homeomorphism of the torus.  Note that there are 12 different
         * ways in which this can be done (two choices for which
         * tetrahedron face joins with which, and then six possible
         * rotations and reflections).
         *
         * As with the other routines in this class, this routine does
         * not verify that either boundary in fact forms a torus.
         * Instead, it uses this assumption to define the rules of what
         * identifications are allowable.
         *
         * If there is a match, the given matrix \a upperReln will be
         * modified to describe how the edges of the given boundary
         * relate to the edges of the old boundary torus.  Note that
         * this relationship depends on how the intermediate tetrahedra
         * are layered (and in fact the purpose of a layering is often to
         * produce such a non-trivial relationship).
         *
         * Specifically, let \a t0 and \a p0 be the first tetrahedron and
         * permutation of the old boundary (as returned by
         * getOldBoundaryTet(0) and getOldBoundaryRoles(0)), and let
         * \a x and \a y be the directed edges \a p0[0]-\a p0[1] and
         * \a p0[0]-\a p0[2] of tetrahedron \a t0 respectively (these
         * are the leftmost and uppermost edges of the diagram below).
         * Likewise, let \a u and \a q be the first tetrahedron and
         * permutation of the given boundary (as passed by parameters
         * \a upperBdry0 and \a upperRoles0), and let
         * \a a and \a b be the directed edges \a q[0]-\a q[1] and
         * \a q[0]-\a q[2] of tetrahedron \a u respectively.
         *
         * <pre>
         *     *--->>--*
         *     |0  2 / |
         *     |    / 1|
         *     v   /   v
         *     |1 /    |
         *     | / 2  0|
         *     *--->>--*
         * </pre>
         *
         * Assuming both boundaries are tori, edges \a x and \a y are
         * generators of the original torus boundary and edges \a a and
         * \a b are generators of the given torus boundary.  Using
         * additive notation, the matrix \a upperReln is modified so
         * that
         *
         * <pre>
         *     [a]                 [x]
         *     [ ]  =  upperReln * [ ] .
         *     [b]                 [y]
         * </pre>
         *
         * In other words, the modified \a upperReln matrix expresses
         * the generator curves of the given boundary in terms of the
         * generator curves of the old boundary.
         *
         * If no match is found, the matrix \a upperReln is not touched.
         *
         * @param upperBdry0 the tetrahedron providing the first face of
         * the given boundary.
         * @param upperRoles0 the permutation describing how this
         * first face is formed from three vertices of tetrahedron
         * upperBdry0, as described in the class notes.
         * @param upperBdry1 the tetrahedron providing the second face of
         * the given boundary.
         * @param upperRoles1 the permutation describing how this second
         * face is formed from three vertices of tetrahedron upperBdry1.
         * @param upperReln the matrix that is changed to reflect the
         * relationship between the old boundary of this structure and
         * the given boundary.
         * @return \c true if the given boundary is found to matche the
         * new boundary of this structure, or \c false otherwise.
         */
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

inline const NMatrix2& NLayering::boundaryReln() const {
    return reln;
}

} // namespace regina

#endif

