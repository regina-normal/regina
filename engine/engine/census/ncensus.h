
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

/*! \file ncensus.h
 *  \brief Deals with forming a census of all triangulations of a given
 *  size.
 */

#ifndef __NCENSUS_H
#ifndef __DOXYGEN
#define __NCENSUS_H
#endif

#include <list>
#include "utilities/nbooleans.h"
#include "utilities/nthread.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nisomorphism.h"

namespace regina {

class NProgressManager;
class NProgressMessage;

/**
 * \addtogroup census Census of Triangulations
 * Census building for 3-manifold triangulations.
 * @{
 */

/**
 * Fills the given packet with all triangulations in the census with the
 * requested parameters.  Each triangulation in the census will appear as
 * a child of the given packet.
 *
 * This routine will conduct a census of all valid triangulations containing a
 * given number
 * of tetrahedra.  All such triangulations are included in the census up
 * to combinatorial isomorphism; given any isomorphism class, exactly
 * one representative will appear in the census.
 * The census can also be restricted to only include triangulations
 * satisfying given extra conditions; see the parameter descriptions for
 * further details.
 *
 * Only valid triangulations will be produced; see
 * NTriangulation::isValid() for further details.
 *
 * Note that this routine should only be used if the census
 * contains a small enough total number of triangulations to
 * avoid any memory disasters.
 *
 * If a progress manager is passed, the calculation will run in a new
 * thread and this routine will return immediately.
 *
 * \todo \featurelong Allow passing an arbitrary criterion function that
 * will determine if a triangulation is interesting enough to include in
 * a custom census.
 *
 * @param parent the packet beneath which members of the
 * census will be placed.
 * @param nTetrahedra the number of tetrahedra in each
 * triangulation in the census.
 * @param finiteness determines whether to include finite and/or ideal
 * triangulations.  The set should contain \c true if finite (non-ideal)
 * triangulations are to be included, and should contain \c false if
 * ideal triangulations are to be included.
 * @param orientability determines whether to include orientable
 * and/or non-orientable triangulations.  The set should contain \c true
 * if orientable triangulations are to be included, and should contain
 * \c false if non-orientable triangulations are to be included.
 * @param boundary determines whether to include triangulations with
 * and/or without boundary faces.  The set should contain \c true
 * if triangulations with boundary faces are to be included, and should contain
 * \c false if triangulations with only internal faces are to be included.
 * @param nBdryFaces specifies the precise number of boundary faces
 * that should be present in the triangulations produced.  If this parameter
 * is negative, it is ignored and no restriction is imposed.
 * If \a boundary does not contain \c true, this parameter is ignored.
 * If \a boundary contains \c true and this parameter is at least zero,
 * only triangulations with this many
 * boundary faces will be produced.  In particular, if this parameter is
 * positive then triangulations with only internal faces will not be produced,
 * irrespective of whether \c false is contained in \a boundary.
 * Note that, in order to produce any
 * triangulations at all, this parameter must be even and at most
 * 2<i>T</i>+2, where <i>T</i> is the number of tetrahedra requested.
 * @param manager a progress manager via which progess will be reported,
 * or 0 if no progress reporting is required.  If non-zero,
 * \a manager must point to a progress manager for which
 * NProgressManager::isStarted() is still \c false.
 * @return the number of triangulations produced in the census, or 0 if
 * a progress manager was passed.
 */
unsigned long formCensus(NPacket* parent, unsigned nTetrahedra,
    NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
    int nBdryFaces = -1, NProgressManager* manager = 0);

/**
 * A utility class used by formCensus().  Other routines should never
 * refer to this class directly.  It is used to store temporary
 * information when forming the census.
 *
 * \todo \optlong When generating face pairings, do some checking to eliminate
 * cases in which tetrahedron (<i>k</i> > 0) can be swapped with tetrahedron 0
 * to produce a smaller representation of the same pairing.
 * \todo \optlong When generating automorphism groups, produce their generating
 * complete permutation sets instead of the entire automorphism groups.
 * \todo \optlong Change <i>isCanonicalInternal()</i> to a non-recursive
 * routine.
 *
 * \ifaces Not present.
 */
class NCensus : public NThread {
    private:
        NPacket* parent;
            /**< The argument passed to formCensus(). */
        unsigned nTetrahedra;
            /**< The argument passed to formCensus(). */
        NBoolSet finiteness;
            /**< The argument passed to formCensus(). */
        NBoolSet orientability;
            /**< The argument passed to formCensus(). */
        NBoolSet boundary;
            /**< The argument passed to formCensus(). */
        int nBdryFaces;
            /**< The argument passed to formCensus(). */

        NProgressMessage* progress;
            /**< Reports the current state of progress of the census
             *   generation.  This will be 0 if progress reporting is
             *   not required. */

        unsigned long whichSoln;
            /**< The number of the solution we are up to. */
        NTriangulation working;
            /**< A working triangulation. */
        NTetrahedron** tet;
            /**< The tetrahedra in the working triangulation. */
        int* orientation;
            /**< The orientation of each tetrahedron.  Orientation is
             *   +/-1, or 0 if unknown. */
        NTetFace* joins;
            /**< The face to which each tetrahedron face is joined.
             *   If the destination for a particular face has not yet been
             *   decided, the face will be joined to itself. */
        int* joinPermIndices;
            /**< The index into array \a allPermsS3 representing the
             *   permutation with which each tetrahedron face is
             *   joined to its partner. */
        NIsomorphismIndexed automorphism;
            /**< The currently working automorphism of the face pairings
             *   described by \a joins. */
        int* autoPreImage;
            /**< An array containing the preimage of each tetrahedron
             *   under \a automorphism. */
        std::list<NIsomorphismIndexed*> allAutomorphisms;
            /**< A list of all found automorphisms of the face pairings
             *   described by \a joins. */

    private:
        /**
         * Creates a new structure to hold the given information.
         * All parameters not explained are taken directly from
         * formCensus().
         *
         * \pre \a newNTetrahedra is at least 1.
         *
         * @param newProgress the object with which to report progress
         * for the census generation, or 0 if progress reporting is not
         * required.
         */
        NCensus(NPacket* newParent, unsigned newNTetrahedra,
            const NBoolSet& newFiniteness,
            const NBoolSet& newOrientability,
            const NBoolSet& newBoundary, int newNBdryFaces,
            NProgressMessage* newProgress = 0);

        /**
         * Returns a reference to the face to which the given face is
         * joined.
         *
         * \pre The given face is a real tetrahedron
         * face (not boundary, before-the-start or past-the-end).
         *
         * @param source the face under investigation.
         * @return a reference to the face to which \a source is joined.
         */
        NTetFace& dest(const NTetFace& source);
        /**
         * Returns a reference to the face to which the given face is
         * joined.
         *
         * @param tet the tetrahedron under investigation (between 0 and
         * <i>nTetrahedra</i>-1 inclusive).
         * @param face the face of the given tetrahedron under
         * investigation (between 0 and 3 inclusive).
         */
        NTetFace& dest(int tet, int face);
        /**
         * Returns a reference to the index into array \a allPermsS3
         * representing the permutation with which the given face
         * is joined to its partner.
         *
         * \pre The given face is a real tetrahedron
         * face (not boundary, before-the-start or past-the-end).
         *
         * @param source the face under investigation.
         * @return a reference to the corresponding array index.
         */
        int& joinPermIndex(const NTetFace& source);
        /**
         * Determines if the destination join for the given face has not
         * yet been decided.
         *
         * \pre The given face is a real tetrahedron
         * face (not boundary, before-the-start or past-the-end).
         *
         * @param source the face under investigation.
         * @return \c true if and only if the destination for the given
         * face has not yet been decided.
         */
        bool noDest(const NTetFace& source);
        /**
         * Determines if the destination join for the given face has not
         * yet been decided.
         *
         * @param tet the tetrahedron under investigation (between 0 and
         * <i>nTetrahedra</i>-1 inclusive).
         * @param face the face of the given tetrahedron under
         * investigation (between 0 and 3 inclusive).
         * @return \c true if and only if the destination for the given
         * face has not yet been decided.
         */
        bool noDest(int tet, int face);

        /**
         * Runs through all combinatorially different ways of pairing
         * off faces (note that faces may also be paired with the
         * boundary).  This is the outermost layer of the census
         * generation routine.
         */
        void matchFacePairs();
        /**
         * Determines if the current set of face pairings is in
         * canonical (smallest lexicographical) form.
         * If so, \a allAutomorphisms will be filled with the set of all
         * combinatorial automorphisms of the set of face pairings.
         * If not, \a allAutomorphisms will be left empty.
         *
         * \pre \a allAutomorphisms is empty.
         *
         * @return \c true if and only if the current set of face
         * pairings is in canonical form.
         */
        bool isCanonical();
        /**
         * Runs through all permutations that can be used to actually
         * glue together the faces that have already been paired off.
         * Solutions that are equivalent under automorphisms in the list
         * \a allAutomorphisms will not be counted more than once.
         * This is the innermost layer of the census routine.
         */
        void selectGluingPerms();
        /**
         * Called when a triangulation has been found.
         * Builds the triangulation from the blueprint, checks any
         * required properties and places it in the final census if
         * appropriate.
         */
        void trySolution();

        /**
         * Internal to isCanonical(); runs through all possible images
         * of the faces of the given tetrahedron.
         *
         * \pre The current set of face pairings and its
         * preimage under \a automorphism are known to be
         * lexicographically equal for all faces strictly less than
         * \a equalUpTo.
         *
         * @param whichTet the first tetrahedron whose face images under
         * \a automorphism have not yet been decided.
         * @param equalUpTo the first face for which it is \a not known
         * that the current set of face pairings and its preimage under
         * \a automorphism are equal.  This parameter can
         * be used to reduce the execution time of this routine.
         * @return \c false if it is already known that the current set
         * of face pairings is \a not in canonical form; \c true
         * if we are still undecided.
         */
        bool isCanonicalInternal(int whichTet, const NTetFace& equalUpTo);
        /**
         * Lexicographically compares the current set of face pairings
         * with its preimage under \a automorphism, where the
         * automorphism may not yet be completely determined.
         *
         * Only the gluings of the faces of the first \a compareTets
         * tetrahedra will be compared.  If the first lexicographical
         * discrepancy occurs where the gluing of some face in the
         * preimage is still undetermined, the pairings will be
         * considered equal.
         *
         * \pre The preimages themselves of the faces of
         * the first \a compareTets tetrahedra have been determined.
         * \pre The two pairing sets are already known to be
         * lexicographically equal for all faces strictly less than
         * \a equalUpTo.
         *
         * \post The two pairing sets are now known to
         * be lexicographically equal for all faces strictly less than
         * the returned value of \a equalUpTo.
         *
         * @param compareTets the number of tetrahedra to compare.
         * @param equalUpTo the first face for which it is \a not known
         * that the two sets of pairings are equal.  This parameter can
         * be used to reduce the execution time of this routine.
         * If nothing is known about the two sets of pairings, simply
         * pass <tt>NTetFace(0, 0)</tt>.  This parameter will be changed
         * during the routine; see the postcondition above for details.
         * @return -1 if this set is less than its preimage, 0 if this
         * set equals its preimage and 1 if this set is greater than its
         * preimage.
         */
        int cmpWithPreImage(unsigned compareTets, NTetFace& equalUpTo);
        /**
         * Compares the current set of gluing permutations with its
         * preimage under the given automorphism of face pairings, in order
         * to see which is closer to canonical form.
         *
         * @param automorph the given automorphism.
         * @return -1 if this set is closer to canonical form, 0 if this set
         * equals its preimage and 1 if its preimage is closer to canonical
         * form.
         */
        int cmpPermsWithPreImage(const NIsomorphism& automorph);
    
    public:
        /**
         * Deallocates any memory used specifically by this
         * structure.
         */
        virtual ~NCensus();

        /**
         * Runs a complete census generation.  At most one copy of this
         * routine should be running at any given time for a particular
         * NCensus.
         *
         * @param param this parameter is ignored.
         * @return 0.
         */
        void* run(void* param);
    
    friend unsigned long formCensus(NPacket* parent, unsigned nTetrahedra,
        NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
        int nBdryFaces, NProgressManager* manager);
};

/*@}*/

// Inline functions for NCensus

inline NTetFace& NCensus::dest(const NTetFace& source) {
    return joins[4 * source.tet + source.face];
}
inline NTetFace& NCensus::dest(int tet, int face) {
    return joins[4 * tet + face];
}

inline int& NCensus::joinPermIndex(const NTetFace& source) {
    return joinPermIndices[4 * source.tet + source.face];
}

inline bool NCensus::noDest(const NTetFace& source) {
    return dest(source) == source;
}
inline bool NCensus::noDest(int tet, int face) {
    NTetFace& f = joins[4 * tet + face];
    return (f.tet == tet && f.face == face);
}

inline bool NCensus::isCanonical() {
    return isCanonicalInternal(0, NTetFace(0, 0));
}

} // namespace regina

#endif

