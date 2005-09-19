
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

/*! \file nisomorphism.h
 *  \brief Deals with combinatorial isomorphisms of triangulations.
 */

#ifndef __NISOMORPHISM_H
#ifndef __DOXYGEN
#define __NISOMORPHISM_H
#endif

#include "shareableobject.h"
#include "triangulation/nperm.h"
#include "triangulation/ntetface.h"

namespace regina {

class NTriangulation;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Represents a combinatorial isomorphism from one triangulation into
 * another.
 *
 * A combinatorial isomorphism from triangulation T to triangulation U
 * is a one-to-one map f from the individual tetrahedron faces of T to
 * the individual tetrahedron faces of U for which the following
 * conditions hold:
 *
 *   - if faces x and y belong to the same tetrahedron of T then
 *     faces f(x) and f(y) belong to the same tetrahedron of U;
 *   - if faces x and y are identified in T then faces f(x) and f(y)
 *     are identified in U.
 *
 * Isomorphisms can be <i>boundary complete</i> or
 * <i>boundary incomplete</i>.  A boundary complete isomorphism
 * satisfies the additional condition:
 *
 *   - if face x is a boundary face of T then face f(x) is a boundary
 *     face of U.
 *
 * A boundary complete isomorphism thus indicates that a copy of
 * triangulation T is present as an entire component (or components) of U,
 * whereas a boundary incomplete isomorphism represents an embedding of a
 * copy of triangulation T as a subcomplex of some possibly larger component
 * (or components) of U.
 *
 * Note that in all cases triangulation U may contain more tetrahedra
 * than triangulation T.
 *
 * This class cannot be directly instantiated; the individual subclasses
 * correspond to different methods of storing the face permutations
 * associated with each tetrahedron.
 *
 * \todo \feature Composition of isomorphisms.
 */
class NIsomorphism : public ShareableObject {
    protected:
        unsigned nTetrahedra;
            /**< The number of tetrahedra in the source triangulation. */
        int* mTetImage;
            /**< The tetrahedron of the destination triangulation that
                 each tetrahedron of the source triangulation maps to. */

    public:
        /**
         * Destroys this isomorphism.
         */
        virtual ~NIsomorphism();

        /**
         * Returns the number of tetrahedra in the source triangulation
         * associated with this isomorphism.  Note that this is always
         * less than or equal to the number of tetrahedra in the
         * destination triangulation.
         *
         * @return the number of tetrahedra in the source triangulation.
         */
        unsigned getSourceTetrahedra() const;

        /**
         * Determines the image of the given source tetrahedron under
         * this isomorphism.
         *
         * @param sourceTet the index of the source tetrahedron; this must
         * be between 0 and <tt>getSourceTetrahedra()-1</tt> inclusive.
         * @return a reference to the index of the destination tetrahedron
         * that the source tetrahedron maps to.
         */
        int& tetImage(unsigned sourceTet);
        /**
         * Determines the image of the given source tetrahedron under
         * this isomorphism.
         *
         * @param sourceTet the index of the source tetrahedron; this must
         * be between 0 and <tt>getSourceTetrahedra()-1</tt> inclusive.
         * @return the index of the destination tetrahedron
         * that the source tetrahedron maps to.
         */
        int tetImage(unsigned sourceTet) const;
        /**
         * Determines the permutation that is applied to the four faces
         * of the given source tetrahedron under this isomorphism.
         * Face \a i of source tetrahedron \a sourceTet will be mapped to
         * face <tt>facePerm(sourceTet)[i]</tt> of tetrahedron
         * <tt>tetImage(sourceTet)</tt>.
         *
         * @param sourceTet the index of the source tetrahedron containing
         * the original four faces; this must be between 0 and
         * <tt>getSourceTetrahedra()-1</tt> inclusive.
         * @return the permutation applied to the four faces of the
         * source tetrahedron.
         */
        virtual NPerm facePerm(unsigned sourceTet) const = 0;
        /**
         * Determines the image of the given source tetrahedron face
         * under this isomorphism.  Note that a value only is returned; this
         * routine cannot be used to alter the isomorphism.
         *
         * @param source the given source tetrahedron face; this must
         * be one of the four faces of one of the getSourceTetrahedra()
         * tetrahedra in the source triangulation.
         * @return the image of the source tetrahedron face under this
         * isomorphism.
         */
        NTetFace operator [] (const NTetFace& source) const;

        /**
         * Determines whether or not this is an identity isomorphism.
         *
         * In an identity isomorphism, each tetrahedron image is itself,
         * and within each tetrahedron the face/vertex permutation is
         * the identity on (0,1,2,3).
         *
         * @return \c true if this is an identity isomorphism, or
         * \c false otherwise.
         */
        bool isIdentity() const;

        /**
         * Applies this isomorphism to the given triangulation.
         *
         * The given triangulation (call this T) is not modified in any way.
         * A new triangulation (call this S) is returned, so that this
         * isomorphism represents a one-to-one, onto and boundary complete
         * isomorphism from T to S.  That is, T and S are combinatorially
         * identical triangulations, and this isomorphism describes the
         * corresponding mapping between tetrahedra and tetrahedron faces.
         *
         * The resulting triangulation S is newly created, and must be
         * destroyed by the caller of this routine.
         *
         * There are several preconditions to this routine.  This
         * routine does a small amount of sanity checking (and returns 0
         * if an error is detected), but it certainly does not check the
         * entire set of preconditions.  It is up to the caller of this
         * routine to verify that all of the following preconditions are
         * met.
         *
         * \pre The number of tetrahedra in the given triangulation is
         * precisely the number returned by getSourceTetrahedra() for
         * this isomorphism.
         * \pre This is a valid isomorphism (i.e., it has been properly
         * initialised, so that all tetrahedron images are non-negative
         * and distinct, and all face permutations are real permutations
         * of (0,1,2,3).
         * \pre Each tetrahedron image for this isomorphism lies
         * between 0 and <tt>getSourceTetrahedra()-1</tt> inclusive
         * (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * @param original the triangulation to which this isomorphism
         * should be applied.
         * @return the resulting new triangulation, or 0 if a problem
         * was encountered (i.e., an unmet precondition was noticed).
         */
        NTriangulation* apply(const NTriangulation* original) const;

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        /**
         * Returns a random isomorphism for the given number of
         * tetrahedra.  This isomorphism will reorder tetrahedra
         * 0 to <tt>nTetrahedra-1</tt> in a random fashion, and for
         * each tetrahedron a random permutation of its four vertices
         * will be selected.
         *
         * The isomorphism returned is newly constructed, and must be
         * destroyed by the caller of this routine.
         *
         * Note that both the STL random number generator and the
         * standard C function rand() are used in this routine.  All
         * possible isomorphisms for the given number of tetrahedra are
         * equally likely.
         *
         * @param nTetrahedra the number of tetrahedra that the new
         * isomorphism should operate upon.
         * @return the newly constructed random isomorphism.
         */
        static NIsomorphism* random(unsigned nTetrahedra);

    protected:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * @param newSourceTetrahedra the number of tetrahedra in the source
         * triangulation; this may be zero.
         */
        NIsomorphism(unsigned newSourceTetrahedra);
};

/**
 * An isomorphism in which face permutations are stored directly.
 * There is direct write-access to the individual face permutations, but
 * no simple method of iterating through possible face permutations.
 *
 * See the NIsomorphism class notes for further details on the types of
 * isomorphism that can be represented.
 *
 * \ifacespython Not present.
 */
class NIsomorphismDirect : public NIsomorphism {
    private:
        NPerm* mFacePerm;
            /**< The permutation applied to the four faces of each
                 source tetrahedron. */

    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * @param newSourceTetrahedra the number of tetrahedra in the source
         * triangulation associated with this isomorphism; this may be
         * zero.
         */
        NIsomorphismDirect(unsigned newSourceTetrahedra);
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
         * Returns a read-write reference to the permutation that is
         * applied to the four faces of the given source tetrahedron
         * under this isomorphism.
         * Face \a i of source tetrahedron \a sourceTet will be mapped to
         * face <tt>facePerm(sourceTet)[i]</tt> of tetrahedron
         * <tt>tetImage(sourceTet)</tt>.
         *
         * @param sourceTet the index of the source tetrahedron containing
         * the original four faces; this must be between 0 and
         * <tt>getSourceTetrahedra()-1</tt> inclusive.
         * @return a read-write reference to the permutation applied to the
         * four faces of the source tetrahedron.
         */
        NPerm& facePerm(unsigned sourceTet);

        virtual NPerm facePerm(unsigned sourceTet) const;
};

/**
 * An isomorphism in which face permutations are stored as indices into
 * the ::allPermsS4 array.
 * It is easy to iterate through possible face permutations, but there
 * is no direct write-access to the permutations themselves.
 *
 * See the NIsomorphism class notes for further details on the types of
 * isomorphism that can be represented.
 *
 * \ifacespython Not present.
 */
class NIsomorphismIndexed : public NIsomorphism {
    private:
        int* mIndex;
            /**< The index into the ::allPermsS4 array representing the
                 permutation applied to the four faces of each source
                 tetrahedron. */

    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * @param newSourceTetrahedra the number of tetrahedra in the
         * source triangulation associated with this isomorphism; this
         * may be zero.
         */
        NIsomorphismIndexed(unsigned newSourceTetrahedra);
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

        virtual NPerm facePerm(unsigned sourceTet) const;

        /**
         * Returns a read-write reference to the index into
         * array ::allPermsS4 that points to
         * the permutation that is applied to the four faces of the
         * given source tetrahedron under this isomorphism.
         * Face \a i of tetrahedron \a sourceTet will be mapped to face
         * <tt>allPermsS4[facePermIndex(sourceTet)][i]</tt> of tetrahedron
         * <tt>tetImage(sourceTet)</tt>.
         *
         * @param sourceTet the index of the source tetrahedron containing
         * the original four faces; this must be between 0 and
         * <tt>getSourceTetrahedra()-1</tt> inclusive.
         * @return a read-write reference to the corresponding array index.
         */
        int& facePermIndex(unsigned sourceTet);
        /**
         * Returns the index into the array ::allPermsS4 that points to
         * the permutation that is applied to the four faces of the
         * given source tetrahedron under this isomorphism.
         * Face \a i of tetrahedron \a sourceTet will be mapped to face
         * <tt>allPermsS4[facePermIndex(sourceTet)][i]</tt> of tetrahedron
         * <tt>tetImage(sourceTet)</tt>.
         *
         * @param sourceTet the index of the source tetrahedron containing
         * the original four faces; this must be between 0 and
         * <tt>getSourceTetrahedra()-1</tt> inclusive.
         * @return the corresponding array index.
         */
        int facePermIndex(unsigned sourceTet) const;
};

/*@}*/

// Inline functions for NIsomorphism

inline NIsomorphism::NIsomorphism(unsigned newSourceTetrahedra) :
        nTetrahedra(newSourceTetrahedra),
        mTetImage(newSourceTetrahedra > 0 ?
            new int[newSourceTetrahedra] : 0) {
}
inline NIsomorphism::~NIsomorphism() {
    if (mTetImage)
        delete[] mTetImage;
}

inline unsigned NIsomorphism::getSourceTetrahedra() const {
    return nTetrahedra;
}

inline int& NIsomorphism::tetImage(unsigned sourceTet) {
    return mTetImage[sourceTet];
}
inline int NIsomorphism::tetImage(unsigned sourceTet) const {
    return mTetImage[sourceTet];
}
inline NTetFace NIsomorphism::operator [] (const NTetFace& source) const {
    return NTetFace(mTetImage[source.tet], facePerm(source.tet)[source.face]);
}

// Inline functions for NIsomorphismDirect

inline NIsomorphismDirect::NIsomorphismDirect(unsigned newSourceTetrahedra) :
        NIsomorphism(newSourceTetrahedra),
        mFacePerm(newSourceTetrahedra > 0 ?
            new NPerm[newSourceTetrahedra] : 0) {
}
inline NIsomorphismDirect::~NIsomorphismDirect() {
    if (mFacePerm)
        delete[] mFacePerm;
}

inline NPerm& NIsomorphismDirect::facePerm(unsigned sourceTet) {
    return mFacePerm[sourceTet];
}
inline NPerm NIsomorphismDirect::facePerm(unsigned sourceTet) const {
    return mFacePerm[sourceTet];
}

// Inline functions for NIsomorphismIndexed

inline NIsomorphismIndexed::NIsomorphismIndexed(unsigned newSourceTetrahedra) :
        NIsomorphism(newSourceTetrahedra),
        mIndex(newSourceTetrahedra > 0 ?
            new int[newSourceTetrahedra] : 0) {
}
inline NIsomorphismIndexed::~NIsomorphismIndexed() {
    if (mIndex)
        delete[] mIndex;
}

inline NPerm NIsomorphismIndexed::facePerm(unsigned sourceTet) const {
    return allPermsS4[mIndex[sourceTet]];
}
inline int& NIsomorphismIndexed::facePermIndex(unsigned sourceTet) {
    return mIndex[sourceTet];
}
inline int NIsomorphismIndexed::facePermIndex(unsigned sourceTet) const {
    return mIndex[sourceTet];
}

} // namespace regina

#endif

