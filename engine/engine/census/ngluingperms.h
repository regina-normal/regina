
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

/*! \file ngluingperms.h
 *  \brief Deals with selecting gluing permutations to complement a
 *  particular pairing of tetrahedron faces.
 */

#ifndef __NGLUINGPERMS_H
#ifndef __DOXYGEN
#define __NGLUINGPERMS_H
#endif

#include "census/nfacepairing.h"

namespace regina {

class NGluingPerms;
class NTriangulation;

/**
 * \weakgroup census
 * @{
 */

/**
 * A routine used to do arbitrary processing upon a particular set of
 * gluing permutations.  Such routines are used to process permutation
 * sets found when running NGluingPerms::findAllPerms().
 *
 * The first parameter passed should be a set of gluing permutations
 * (this should not be deallocated by this routine).
 * The second parameter may contain arbitrary data as passed to
 * NGluingPerms::findAllPerms().
 *
 * Note that the first parameter passed might be \c null to signal that
 * gluing permutation generation has finished.
 */
typedef void (*UseGluingPerms)(const NGluingPerms*, void*);

/**
 * Represents a specific set of gluing permutations to complement a
 * particular pairwise matching of tetrahedron faces.  Given a
 * pairwise matching of faces (as described by class NFacePairing), each
 * face that is matched with some other face will have an associated
 * permutation of four elements (as described by class NPerm).
 *
 * If a face is matched with some other face, the two corresponding
 * associated permutations will be inverses.  If a face is left
 * deliberately unmatched, it will have no associated permutation.
 *
 * Such a set of permutations models part of the structure of a
 * triangulation, in which each tetrahedron face that is glued to another
 * face has a corresponding gluing permutation (and the matched face has
 * the inverse gluing permutation).
 *
 * \ifaces Not present.
 */
class NGluingPerms {
    private:
        static const int PURGE_NON_MINIMAL;
            /**< Indicates that non-minimal triangulations may be ignored. */
        static const int PURGE_NON_PRIME;
            /**< Indicates that non-prime triangulations may be ignored. */
        static const int PURGE_NON_MINIMAL_PRIME;
            /**< Indicates that triangulations that are not both minimal
                 and prime may be ignored. */

    private:
        const NFacePairing* pairing;
            /**< The face pairing complemented by this permutation set. */
        int* orientation;
            /**< The orientation of each tetrahedron.  Orientation is
                 +/-1, or 0 if unknown. */
        int* permIndices;
            /**< The index into array \a allPermsS3 describing how each
                 tetrahedron face is glued to its partner.  Note that this
                 is not a gluing permutation as such but rather a permutation
                 of 0, 1 and 2 only. */

    public:
        /**
         * Creates a new set of gluing permutations that is a clone of
         * the given permutation set.
         *
         * @param cloneMe the gluing permutations to clone.
         */
        NGluingPerms(const NGluingPerms& cloneMe);

        /**
         * Deallocates any memory used by this structure.
         */
        virtual ~NGluingPerms();

        /**
         * Returns the total number of tetrahedra under consideration.
         *
         * @return the number of tetrahedra under consideration.
         */
        unsigned getNumberOfTetrahedra() const;

        /**
         * Returns the specific pairing of tetrahedron faces that this
         * set of gluing permutations complements.
         *
         * @return the corresponding tetrahedron face pairing.
         */
        const NFacePairing* getFacePairing() const;

        /**
         * Returns the gluing permutation associated with the given
         * tetrahedron face.
         *
         * \pre The given face is actually paired with some other face in
         * the underlying pairwise matching (see routine getFacePairing()).
         * \pre The given face is a real tetrahedron
         * face (not boundary, before-the-start or past-the-end).
         *
         * @param source the tetrahedron face under investigation.
         * @return the associated gluing permutation.
         */
        NPerm gluingPerm(const NTetFace& source) const;

        /**
         * Returns the gluing permutation associated with the given
         * tetrahedron face.
         *
         * \pre The given face is actually paired with some other face in
         * the underlying pairwise matching (see routine getFacePairing()).
         *
         * @param tet the tetrahedron under investigation (this must be
         * strictly less than the total number of tetrahedra under
         * consideration).
         * @param face the face of the given tetrahedron under
         * investigation (between 0 and 3 inclusive).
         * @return the associated gluing permutation.
         */
        NPerm gluingPerm(unsigned tet, unsigned face) const;

        /**
         * Returns a newly created triangulation as modelled by this set
         * of gluing permutations and the associated tetrahedron face
         * pairing.
         *
         * Each matched pair of faces and their associated permutations
         * will be realised as two tetrahedron faces glued
         * together with the corresponding gluing permutation.  Each
         * unmatched face will be realised as a boundary tetrahedron face.
         *
         * It is the responsibility of the caller of this routine to
         * delete this triangulation once it is no longer required.
         *
         * @return a newly created triangulation modelled by this structure.
         */
        NTriangulation* triangulate() const;

        /**
         * Generates all possible gluing permutation sets that complement
         * the given tetrahedron face pairing.
         *
         * Each set of gluing permutations will be produced precisely
         * once up to equivalence, where equivalence is defined by a set
         * of automorphisms upon the given face pairing.
         *
         * For each permutation set that is generated, routine \a use (as
         * passed to this function) will be called with that permutation
         * set as an argument.
         *
         * Once the generation of permutation sets has finished, routine
         * \a use will be called once more, this time with \c null as its
         * first (permutation set) argument.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre Within any single tetrahedron in the given face pairing, the
         * face partners appear in increasing order for faces 0, 1, 2 and 3.
         * Partners are ordered first by tetrahedron number and then by
         * face number within that tetrahedron.  An unmatched face must
         * appear after all matched faces within any particular tetrahedron.
         * \pre In the given face pairing, each tetrahedron aside from
         * the first has some face paired with a face in an earlier
         * tetrahedron.
         *
         * \todo \feature Allow cancellation of permutation set generation.
         *
         * @param newPairing the specific pairing of tetrahedron faces
         * that the generated permutation sets will complement.
         * @param autos the collection of automorphisms defining equivalence
         * of permutation sets.  These must all be automorphisms upon the
         * given face pairing, and will generally be the set of all such
         * automorphisms.
         * @param orientableOnly \c true if only gluing permutations
         * corresponding to orientable triangulations should be
         * generated, or \c false if no such restriction should be imposed.
         * @param use the function to call upon each permutation set that
         * is found.  The first parameter passed to this function will be
         * a gluing permutation set.  The second parameter will be
         * parameter \a useArgs as was passed to this routine.
         * @param useArgs the pointer to pass as the final parameter for
         * the function \a use which will be called upon each permutation
         * set found.
         */
        static void findAllPerms(const NFacePairing* newPairing,
            const NFacePairingIsoList* autos, bool orientableOnly,
            UseGluingPerms use, void* useArgs = 0);

    private:
        /**
         * Creates a new permutation set.  All internal arrays will be
         * allocated but not initialised.
         *
         * @param newPairing the specific pairing of tetrahedron faces
         * that this permutation set will complement.
         */
        NGluingPerms(const NFacePairing* newPairing);

        /**
         * Returns the index into array \a allPermsS3 describing how the
         * the given face is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0, 1 and 2 only.  For a real face
         * gluing permutation, see routine gluingPerm().
         *
         * \pre The given face is a real tetrahedron
         * face (not boundary, before-the-start or past-the-end).
         *
         * @param source the tetrahedron face under investigation.
         * @return a reference to the corresponding array index.
         */
        int& permIndex(const NTetFace& source);

        /**
         * Returns the index into array \a allPermsS3 describing how the
         * the given face is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0, 1 and 2 only.  For a real face
         * gluing permutation, see routine gluingPerm().
         *
         * @param tet the tetrahedron under investigation (this must be
         * strictly less than the total number of tetrahedra under
         * consideration).
         * @param face the face of the given tetrahedron under
         * investigation (between 0 and 3 inclusive).
         * @return a reference to the corresponding array index.
         */
        int& permIndex(unsigned tet, unsigned face);

        /**
         * Returns the index into array \a allPermsS3 describing how the
         * the given face is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0, 1 and 2 only.  For a real face
         * gluing permutation, see routine gluingPerm().
         *
         * \pre The given face is a real tetrahedron
         * face (not boundary, before-the-start or past-the-end).
         *
         * @param source the tetrahedron face under investigation.
         * @return a reference to the corresponding array index.
         */
        const int& permIndex(const NTetFace& source) const;

        /**
         * Returns the index into array \a allPermsS3 describing how the
         * the given face is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0, 1 and 2 only.  For a real face
         * gluing permutation, see routine gluingPerm().
         *
         * @param tet the tetrahedron under investigation (this must be
         * strictly less than the total number of tetrahedra under
         * consideration).
         * @param face the face of the given tetrahedron under
         * investigation (between 0 and 3 inclusive).
         * @return a reference to the corresponding array index.
         */
        const int& permIndex(unsigned tet, unsigned face) const;

        /**
         * Compares the current set of gluing permutations with its
         * preimage under the given automorphism of face pairings, in order
         * to see which is closer to canonical form.
         *
         * @param pairing the pairing of faces to which the given
         * automorphism should be applied.
         * @param automorph the given automorphism.
         * @return -1 if this set is closer to canonical form, 0 if this set
         * equals its preimage and 1 if its preimage is closer to canonical
         * form.
         */
        int cmpPermsWithPreImage(const NFacePairing* pairing,
            const NIsomorphism& automorph);

        /**
         * Internal to findAllPerms().
         *
         * This routine in fact performs the same processing as
         * findAllPerms() but is non-static.  Routine findAllPerms() does
         * nothing but call this routine upon a newly created (and
         * uninitialised) gluing permutation set.
         *
         * All parameters are as described for findAllPerms().
         *
         * \pre As described for findAllPerms().
         */
        void findAllPermsInternal(const NFacePairing* newPairing,
            const NFacePairingIsoList* autos, bool orientableOnly,
            UseGluingPerms use, void* useArgs = 0);

        /**
         * Determines whether the permutations under construction are
         * doomed to model a triangulation that can be purged from the
         * census.  The conditions under which a triangulation may be
         * purged are specified by parameter \a whichPurge.
         *
         * Note that this routine will not identify all triangulations
         * that satisfy the given conditions; however, whenever this
         * routine \e does return \c true it is guaranteed that the
         * permutations under construction will only lead to
         * triangulations that do meet the given conditions.
         *
         * This routine must \e only be called from within
         * findAllPermsInternal() since it relies on specific details of
         * how these permutations are generated.
         *
         * Tests that do not refer to the gluing permutation for the
         * given face will not be run.
         *
         * \pre All gluing permutations for faces up to and including the
         * given face have already been selected.
         *
         * @param face the specific tetrahedron face upon which tests
         * will be based.
         * @param whichPurge specifies the conditions under which a
         * triangulation may be purged from the census; this should be a
         * bitwise OR of purge constants defined in this class, or 0 if
         * no triangulations may be purged.
         * @param orientableOnly \c true if only gluing permutations
         * corresponding to orientable triangulations are being
         * constructed, or \c false otherwise.
         * @return \c true if the permutations under construction will only
         * lead to triangulations that may be purged, or \c false if the
         * results are inconclusive.
         */
        bool mayPurge(const NTetFace& face, int whichPurge,
                bool orientableOnly);
};

/*@}*/

// Inline functions for NGluingPerms

inline NGluingPerms::NGluingPerms(const NFacePairing* newPairing) :
        pairing(newPairing),
        orientation(new int[newPairing->getNumberOfTetrahedra()]),
        permIndices(new int[newPairing->getNumberOfTetrahedra() * 4]) {
}

inline NGluingPerms::~NGluingPerms() {
    delete[] orientation;
    delete[] permIndices;
}

inline unsigned NGluingPerms::getNumberOfTetrahedra() const {
    return pairing->getNumberOfTetrahedra();
}

inline const NFacePairing* NGluingPerms::getFacePairing() const {
    return pairing;
}

inline NPerm NGluingPerms::gluingPerm(const NTetFace& source) const {
    return NPerm(pairing->dest(source).face, 3) *
        allPermsS3[permIndex(source)] * NPerm(source.face, 3);
}

inline NPerm NGluingPerms::gluingPerm(unsigned tet, unsigned face) const {
    return NPerm(pairing->dest(tet, face).face, 3) *
        allPermsS3[permIndex(tet, face)] * NPerm(face, 3);
}

inline int& NGluingPerms::permIndex(const NTetFace& source) {
    return permIndices[4 * source.tet + source.face];
}

inline int& NGluingPerms::permIndex(unsigned tet, unsigned face) {
    return permIndices[4 * tet + face];
}

inline const int& NGluingPerms::permIndex(const NTetFace& source) const {
    return permIndices[4 * source.tet + source.face];
}

inline const int& NGluingPerms::permIndex(unsigned tet, unsigned face) const {
    return permIndices[4 * tet + face];
}

} // namespace regina

#endif

