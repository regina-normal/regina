
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file nisomorphism.h
 *  \brief Utilities for dealing with combinatorial isomorphisms of
 *  triangulations.
 */

#ifndef __NISOMORPHISM_H
#ifndef __DOXYGEN
#define __NISOMORPHISM_H
#endif

#include "triangulation/nperm.h"
#include "triangulation/ntetface.h"

/**
 * Represents a combinatorial isomorphism of a triangulation.
 * This class cannot be directly instantiated; the individual subclasses
 * correspond to different methods of storing the face permutation
 * associated with each tetrahedron.
 *
 * \ifaces Not present.
 */
class NIsomorphism {
    protected:
        unsigned nTetrahedra;
            /**< The number of tetrahedra in the associated triangulation. */
        int* mTetImage;
            /**< The tetrahedron that each tetrahedron maps to. */
    
    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * @param newNTetrahedra the number of tetrahedra in the associated
         * triangulation; this must be at least 1.
         */
        NIsomorphism(unsigned newNTetrahedra);
        /**
         * Destroys this isomorphism.
         */
        virtual ~NIsomorphism();

        /**
         * Returns the number of tetrahedra in the triangulation
         * associated with this isomorphism.
         *
         * @return the number of tetrahedra.
         */
        unsigned getNumberOfTetrahedra() const;

        /**
         * Determines the image of the given tetrahedron under the
         * isomorphism.
         *
         * @param tet the index of the given tetrahedron; this must be
         * between 0 and <tt>getNumberOfTetrahedra()-1</tt> inclusive.
         * @return a reference to the index of the tetrahedron that
         * the given tetrahedron maps to.
         */
        int& tetImage(unsigned tet);
        /**
         * Determines the image of the given tetrahedron under the
         * isomorphism.
         *
         * @param tet the index of the given tetrahedron; this must be
         * between 0 and <tt>getNumberOfTetrahedra()-1</tt> inclusive.
         * @return a reference to the index of the tetrahedron that
         * the given tetrahedron maps to.
         */
        const int& tetImage(unsigned tet) const;
        /**
         * Determines the permutation that is applied to the four faces
         * of the given tetrahedron under this isomorphism.
         * Face \a i of tetrahedron \a tet will be mapped to face
         * <tt>facePerm(tet)[i]</tt> of tetrahedron
         * <tt>tetImage(tet)</tt>.
         *
         * @param tet the index of the tetrahedron containing the
         * original four faces; this must be between 0 and
         * <tt>getNumberOfTetrahedra()-1</tt> inclusive.
         * @return a reference to the permutation applied to the
         * four faces of the given tetrahedron.
         */
        virtual const NPerm& facePerm(unsigned tet) const = 0;
        /**
         * Determines the image of the given specific tetrahedron face
         * under the isomorphism.  Note that a value only is returned; this
         * routine cannot be used to alter the isomorphism.
         *
         * @param source the given specific tetrahedron face; this must
         * be one of the four faces of one of the getNumberOfTetrahedra()
         * tetrahedra in the associated triangulation.
         * @return the image of the given tetrahedron face under the
         * isomorphism.
         */
        NTetFace operator [] (const NTetFace& source) const;
};

/**
 * An isomorphism in which face permutations are stored directly.
 * There is direct write-access to the individual face permutations, but
 * no simple method of iterating through possible face permutations.
 *
 * \ifaces Not present.
 */
class NIsomorphismDirect : public NIsomorphism {
    private:
        NPerm* mFacePerm;
            /**< The permutation applied to the four faces of each
             *   source tetrahedron. */
    
    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * @param newNTetrahedra the number of tetrahedra in the associated
         * triangulation; this must be at least 1.
         */
        NIsomorphismDirect(unsigned newNTetrahedra);
        /**
         * Creates a new isomorphism identical to the given isomorphism.
         *
         * @param cloneMe the isomorphism upon which to base the new
         * isomorphism.
         */
        NIsomorphismDirect(const NIsomorphism& cloneMe);
        /**
         * Destroys this isomorphism.
         */
        virtual ~NIsomorphismDirect();

        /**
         * Determines the permutation that is applied to the four faces
         * of the given tetrahedron under this isomorphism.
         * Face \a i of tetrahedron \a tet will be mapped to face
         * <tt>facePerm(tet)[i]</tt> of tetrahedron
         * <tt>tetImage(tet)</tt>.
         *
         * @param tet the index of the tetrahedron containing the
         * original four faces; this must be between 0 and
         * <tt>getNumberOfTetrahedra()-1</tt> inclusive.
         * @return a reference to the permutation applied to the
         * four faces of the given tetrahedron.
         */
        NPerm& facePerm(unsigned tet);
        /**
         * Determines the permutation that is applied to the four faces
         * of the given tetrahedron under this isomorphism.
         * Face \a i of tetrahedron \a tet will be mapped to face
         * <tt>facePerm(tet)[i]</tt> of tetrahedron
         * <tt>tetImage(tet)</tt>.
         *
         * @param tet the index of the tetrahedron containing the
         * original four faces; this must be between 0 and
         * <tt>getNumberOfTetrahedra()-1</tt> inclusive.
         * @return a reference to the permutation applied to the
         * four faces of the given tetrahedron.
         */
        virtual const NPerm& facePerm(unsigned tet) const;
};

/**
 * An isomorphism in which face permutations are stored as indices into
 * the ::allPermsS4 array.
 * It is easy to iterate through possible face permutations, but there
 * is no direct write-access to the permutations themselves.
 *
 * \ifaces Not present.
 */
class NIsomorphismIndexed : public NIsomorphism {
    private:
        int* mIndex;
            /**< The index into the ::allPermsS4 array representing the
             *   permutation applied to the four faces of each source
             *   tetrahedron. */

    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * @param newNTetrahedra the number of tetrahedra in the
         * associated triangulation; this must be at least 1.
         */
        NIsomorphismIndexed(unsigned newNTetrahedra);
        /**
         * Creates a new isomorphism identical to the given isomorphism.
         *
         * @param cloneMe the isomorphism upon which to base the new
         * isomorphism.
         */
        NIsomorphismIndexed(const NIsomorphismIndexed& cloneMe);
        /**
         * Destroys this isomorphism.
         */
        virtual ~NIsomorphismIndexed();

        /**
         * Determines the permutation that is applied to the four faces
         * of the given tetrahedron under this isomorphism.
         * Face \a i of tetrahedron \a tet will be mapped to face
         * <tt>facePerm(tet)[i]</tt> of tetrahedron
         * <tt>tetImage(tet)</tt>.
         *
         * @param tet the index of the tetrahedron containing the
         * original four faces; this must be between 0 and
         * <tt>getNumberOfTetrahedra()-1</tt> inclusive.
         * @return a reference to the permutation applied to the
         * four faces of the given tetrahedron.
         */
        virtual const NPerm& facePerm(unsigned tet) const;
        /**
         * Determines the index into the array ::allPermsS4 that points to
         * the permutation that is applied to the four faces of the
         * given tetrahedron under this isomorphism.
         * Face \a i of tetrahedron \a tet will be mapped to face
         * <tt>allPermsS4[facePermIndex(tet)][i]</tt> of tetrahedron
         * <tt>tetImage(tet)</tt>.
         *
         * @param tet the index of the tetrahedron containing the
         * original four faces; this must be between 0 and
         * <tt>getNumberOfTetrahedra()-1</tt> inclusive.
         * @return a reference to the corresponding array index.
         */
        int& facePermIndex(unsigned tet);
        /**
         * Determines the index into the array ::allPermsS4 that points to
         * the permutation that is applied to the four faces of the
         * given tetrahedron under this isomorphism.
         * Face \a i of tetrahedron \a tet will be mapped to face
         * <tt>allPermsS4[facePermIndex(tet)][i]</tt> of tetrahedron
         * <tt>tetImage(tet)</tt>.
         *
         * @param tet the index of the tetrahedron containing the
         * original four faces; this must be between 0 and
         * <tt>getNumberOfTetrahedra()-1</tt> inclusive.
         * @return a reference to the corresponding array index.
         */
        const int& facePermIndex(unsigned tet) const;
};

// Inline functions for NIsomorphism

inline NIsomorphism::NIsomorphism(unsigned newNTetrahedra) :
        nTetrahedra(newNTetrahedra),
        mTetImage(new int[newNTetrahedra]) {
}
inline NIsomorphism::~NIsomorphism() {
    delete[] mTetImage;
}

inline unsigned NIsomorphism::getNumberOfTetrahedra() const {
    return nTetrahedra;
}

inline int& NIsomorphism::tetImage(unsigned tet) {
    return mTetImage[tet];
}
inline const int& NIsomorphism::tetImage(unsigned tet) const {
    return mTetImage[tet];
}
inline NTetFace NIsomorphism::operator [] (const NTetFace& source) const {
    return NTetFace(mTetImage[source.tet], facePerm(source.tet)[source.face]);
}

// Inline functions for NIsomorphismDirect

inline NIsomorphismDirect::NIsomorphismDirect(unsigned newNTetrahedra) :
        NIsomorphism(newNTetrahedra), mFacePerm(new NPerm[newNTetrahedra]) {
}
inline NIsomorphismDirect::~NIsomorphismDirect() {
    delete[] mFacePerm;
}

inline NPerm& NIsomorphismDirect::facePerm(unsigned tet) {
    return mFacePerm[tet];
}
inline const NPerm& NIsomorphismDirect::facePerm(unsigned tet) const {
    return mFacePerm[tet];
}

// Inline functions for NIsomorphismIndexed

inline NIsomorphismIndexed::NIsomorphismIndexed(unsigned newNTetrahedra) :
        NIsomorphism(newNTetrahedra), mIndex(new int[newNTetrahedra]) {
}
inline NIsomorphismIndexed::~NIsomorphismIndexed() {
    delete[] mIndex;
}

inline const NPerm& NIsomorphismIndexed::facePerm(unsigned tet) const {
    return allPermsS4[mIndex[tet]];
}
inline int& NIsomorphismIndexed::facePermIndex(unsigned tet) {
    return mIndex[tet];
}
inline const int& NIsomorphismIndexed::facePermIndex(unsigned tet) const {
    return mIndex[tet];
}

#endif

