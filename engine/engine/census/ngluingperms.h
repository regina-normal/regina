
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
 * \ifacespython Not present.
 */
class NGluingPerms {
    protected:
        const NFacePairing* pairing;
            /**< The face pairing complemented by this permutation set.
                 This is guaranteed to be the minimal representative of
                 its face pairing isomorphism class. */
        int* permIndices;
            /**< The index into array \a allPermsS3 describing how each
                 tetrahedron face is glued to its partner.  Note that this
                 is not a gluing permutation as such but rather a permutation
                 of 0, 1 and 2 only (see the routines gluingToIndex() and
                 indexToGluing() for conversions).  If a permutation has not
                 yet been selected (e.g., if this permutation set is still
                 under construction) then this index is -1. */

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
         * Dumps all internal data in a plain text format to the given
         * output stream.  This object can be recreated from this text
         * data by calling the input stream constructor for this class.
         *
         * This routine may be useful for transferring objects from
         * one processor to another.
         *
         * Note that subclass data is written after superclass data, so
         * it is safe to dump data from a subclass and then recreate a
         * new superclass object from that data (though subclass-specific
         * information will of course be lost).
         *
         * @param out the output stream to which the data should be
         * written.
         */
        virtual void dumpData(std::ostream& out) const;

    protected:
        /**
         * Creates a new permutation set.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by NFacePairing::isCanonical().  Note that all face pairings
         * constructed by NFacePairing::findAllPairings() are of this form.
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
         * Returns the index into array \a allPermsS3 corresponding to
         * the given gluing permutation from the given face to its
         * partner.  This need not be the index into \a allPermsS3 that
         * is currently stored for the given face.
         *
         * Indices into array \a allPermsS3 are stored internally in the
         * array \a permIndices.  Full gluing permutations on the other
         * hand are used in constructing triangulations.
         *
         * \pre The given tetrahedron face has a partner according to
         * the underlying face pairing, i.e., is not a boundary face.
         * \pre If the given tetrahedron face and its partner are faces
         * \a x and \a y of their respective tetrahedra, then the
         * given gluing permutation maps \a x to \a y.
         *
         * @param source the tetrahedron face under investigation.
         * @param gluing a possible gluing permutation from the given
         * tetrahedron face to its partner according to the underlying
         * face pairing.
         * @return the index into \a allPermsS3 corresponding to the
         * given gluing permutation; this will be between 0 and 5
         * inclusive.
         */
        int gluingToIndex(const NTetFace& source, const NPerm& gluing) const;

        /**
         * Returns the index into array \a allPermsS3 corresponding to
         * the given gluing permutation from the given face to its
         * partner.  This need not be the index into \a allPermsS3 that
         * is currently stored for the given face.
         *
         * Indices into array \a allPermsS3 are stored internally in the
         * array \a permIndices.  Full gluing permutations on the other
         * hand are used in constructing triangulations.
         *
         * \pre The given tetrahedron face has a partner according to
         * the underlying face pairing, i.e., is not a boundary face.
         * \pre If the given tetrahedron face and its partner are faces
         * \a x and \a y of their respective tetrahedra, then the
         * given gluing permutation maps \a x to \a y.
         *
         * @param tet the tetrahedron under investigation; this must be
         * strictly less than the total number of tetrahedra under
         * consideration.
         * @param face the face of the given tetrahedron under
         * investigation; this must be between 0 and 3 inclusive.
         * @param gluing a possible gluing permutation from the given
         * tetrahedron face to its partner according to the underlying
         * face pairing.
         * @return the index into \a allPermsS3 corresponding to the
         * given gluing permutation; this will be between 0 and 5
         * inclusive.
         */
        int gluingToIndex(unsigned tet, unsigned face, const NPerm& gluing)
            const;

        /**
         * Returns the gluing permutation from the given face to its
         * partner that corresponds to the given index into array
         * \a allPermsS3.  This index into \a allPermsS3 need not
         * be the index that is currently stored for the given face.
         *
         * Indices into array \a allPermsS3 are stored internally in the
         * array \a permIndices.  Full gluing permutations on the other
         * hand are used in constructing triangulations.
         *
         * If the given tetrahedron face and its partner according to
         * the underlying face pairing are faces \a x and \a y of their
         * respective tetrahedra, then the resulting gluing permutation
         * will map \a x to \a y.
         *
         * \pre The given tetrahedron face has a partner according to
         * the underlying face pairing, i.e., is not a boundary face.
         *
         * @param source the tetrahedron face under investigation.
         * @param index an index into \a allPermsS3; this must be
         * between 0 and 5 inclusive.
         * @return the gluing permutation corresponding to the given
         * index into \a allPermsS3.
         */
        NPerm indexToGluing(const NTetFace& source, int index) const;

        /**
         * Returns the gluing permutation from the given face to its
         * partner that corresponds to the given index into array
         * \a allPermsS3.  This index into \a allPermsS3 need not
         * be the index that is currently stored for the given face.
         *
         * Indices into array \a allPermsS3 are stored internally in the
         * array \a permIndices.  Full gluing permutations on the other
         * hand are used in constructing triangulations.
         *
         * If the given tetrahedron face and its partner according to
         * the underlying face pairing are faces \a x and \a y of their
         * respective tetrahedra, then the resulting gluing permutation
         * will map \a x to \a y.
         *
         * \pre The given tetrahedron face has a partner according to
         * the underlying face pairing, i.e., is not a boundary face.
         *
         * @param tet the tetrahedron under investigation; this must be
         * strictly less than the total number of tetrahedra under
         * consideration.
         * @param face the face of the given tetrahedron under
         * investigation; this must be between 0 and 3 inclusive.
         * @param index an index into \a allPermsS3; this must be
         * between 0 and 5 inclusive.
         * @return the gluing permutation corresponding to the given
         * index into \a allPermsS3.
         */
        NPerm indexToGluing(unsigned tet, unsigned face, int index) const;
};

/*@}*/

// Inline functions for NGluingPerms

inline NGluingPerms::NGluingPerms(const NFacePairing* newPairing) :
        pairing(newPairing),
        permIndices(new int[newPairing->getNumberOfTetrahedra() * 4]) {
}

inline NGluingPerms::~NGluingPerms() {
    delete[] permIndices;
}

inline unsigned NGluingPerms::getNumberOfTetrahedra() const {
    return pairing->getNumberOfTetrahedra();
}

inline const NFacePairing* NGluingPerms::getFacePairing() const {
    return pairing;
}

inline NPerm NGluingPerms::gluingPerm(const NTetFace& source) const {
    return indexToGluing(source, permIndex(source));
}

inline NPerm NGluingPerms::gluingPerm(unsigned tet, unsigned face) const {
    return indexToGluing(tet, face, permIndex(tet, face));
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

inline NPerm NGluingPerms::indexToGluing(const NTetFace& source, int index)
        const {
    return NPerm(pairing->dest(source).face, 3) *
        allPermsS3[index] * NPerm(source.face, 3);
}

inline NPerm NGluingPerms::indexToGluing(unsigned tet, unsigned face,
        int index) const {
    return NPerm(pairing->dest(tet, face).face, 3) *
        allPermsS3[index] * NPerm(face, 3);
}

} // namespace regina

#endif

