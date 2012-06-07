
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

/*! \file dim4/dim4isomorphism.h
 *  \brief Deals with combinatorial isomorphisms of 4-manifold triangulations.
 */

#ifndef __DIM4ISOMORPHISM_H
#ifndef __DOXYGEN
#define __DIM4ISOMORPHISM_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "maths/nperm5.h"
#include "triangulation/nfacetspec.h"
#include "triangulation/ngeneralisomorphism.h"

namespace regina {

class Dim4Triangulation;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a combinatorial isomorphism from one 4-manifold triangulation
 * into another.
 *
 * In essence, a combinatorial isomorphism from triangulation T to
 * triangulation U is a one-to-one map from the pentachora of T to the
 * pentachora of U that allows relabelling of both the pentachora and
 * their facets (or equivalently, their vertices), and that preserves
 * gluings across adjacent pentachora.
 *
 * More precisely:  An isomorphism consists of (i) a one-to-one map f
 * from the pentachora of T to the pentachora of U, and (ii) for each
 * pentachoron S of T, a permutation f_S of the facets (0,1,2,3,4) of S,
 * for which the following condition holds:
 *
 *   - If facet k of pentachoron S and facet k' of pentachoron S'
 *     are identified in T, then facet f_S(k) of f(S) and facet f_S'(k')
 *     of f(S') are identified in U.  Moreover, their gluing is consistent
 *     with the facet/vertex permutations; that is, there is a commutative
 *     square involving the gluing maps in T and U and the permutations
 *     f_S and f_S'.
 *
 * Isomorphisms can be <i>boundary complete</i> or
 * <i>boundary incomplete</i>.  A boundary complete isomorphism
 * satisfies the additional condition:
 *
 *   - If facet x is a boundary facet of T then facet f(x) is a boundary
 *     facet of U.
 *
 * A boundary complete isomorphism thus indicates that a copy of
 * triangulation T is present as an entire component (or components) of U,
 * whereas a boundary incomplete isomorphism represents an embedding of a
 * copy of triangulation T as a subcomplex of some possibly larger component
 * (or components) of U.
 *
 * Note that in all cases triangulation U may contain more pentachora
 * than triangulation T.
 */
class REGINA_API Dim4Isomorphism : public NGeneralIsomorphism<4> {
    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * \ifacespython Not present.
         *
         * @param sourcePentachora the number of pentachora in the source
         * triangulation associated with this isomorphism; this may be zero.
         */
        Dim4Isomorphism(unsigned sourcePentachora);
        /**
         * Creates a new isomorphism identical to the given isomorphism.
         *
         * @param cloneMe the isomorphism upon which to base the new
         * isomorphism.
         */
        Dim4Isomorphism(const Dim4Isomorphism& cloneMe);

        /**
         * Returns the number of pentachora in the source triangulation
         * associated with this isomorphism.  Note that this is always
         * less than or equal to the number of pentachora in the
         * destination triangulation.
         *
         * This is a convenience routine specific to four dimensions, and is
         * identical to the dimension-agnostic routine getSourceSimplices().
         *
         * @return the number of pentachora in the source triangulation.
         */
        unsigned getSourcePentachora() const;

        /**
         * Determines the image of the given source pentachoron under
         * this isomorphism.
         *
         * This is a convenience routine specific to four dimensions, and is
         * identical to the dimension-agnostic routine simpImage().
         *
         * \ifacespython Not present, though the read-only version of
         * this routine is.
         *
         * @param sourcePent the index of the source pentachoron; this must
         * be between 0 and <tt>getSourceSimplices()-1</tt> inclusive.
         * @return a reference to the index of the destination pentachoron
         * that the source pentachoron maps to.
         */
        int& pentImage(unsigned sourcePent);

        /**
         * Determines the image of the given source pentachoron under
         * this isomorphism.
         *
         * This is a convenience routine specific to four dimensions, and is
         * identical to the dimension-agnostic routine simpImage().
         *
         * @param sourcePent the index of the source pentachoron; this must
         * be between 0 and <tt>getSourceSimplices()-1</tt> inclusive.
         * @return the index of the destination pentachoron
         * that the source pentachoron maps to.
         */
        int pentImage(unsigned sourcePent) const;

        /**
         * Applies this isomorphism to the given triangulation and
         * returns the result as a new triangulation.
         *
         * The given triangulation (call this T) is not modified in any way.
         * A new triangulation (call this S) is returned, so that this
         * isomorphism represents a one-to-one, onto and boundary complete
         * isomorphism from T to S.  That is, T and S are combinatorially
         * identical triangulations, and this isomorphism describes the
         * corresponding mapping between pentachora and pentachoron
         * vertices/facets.
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
         * \pre The number of pentachora in the given triangulation is
         * precisely the number returned by getSourceSimplices() for
         * this isomorphism.
         * \pre This is a valid isomorphism (i.e., it has been properly
         * initialised, so that all pentachoron images are non-negative
         * and distinct, and all facet permutations are real permutations
         * of (0,1,2,3,4).
         * \pre Each pentachoron image for this isomorphism lies
         * between 0 and <tt>getSourceSimplices()-1</tt> inclusive
         * (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * @param original the triangulation to which this isomorphism
         * should be applied.
         * @return the resulting new triangulation, or 0 if a problem
         * was encountered (i.e., an unmet precondition was noticed).
         */
        Dim4Triangulation* apply(const Dim4Triangulation* original) const;

        /**
         * Applies this isomorphism to the given triangulation,
         * modifying the given triangulation directly.
         *
         * This is similar to apply(), except that instead of creating a
         * new triangulation, the pentachora and vertices of the given
         * triangulation are modified directly.
         *
         * See apply() for further details on how this operation is performed.
         *
         * As with apply(), there are several preconditions to this routine.
         * This routine does a small amount of sanity checking (and returns
         * without changes if an error is detected), but it certainly does
         * not check the entire set of preconditions.  It is up to the
         * caller of this routine to verify that all of the following
         * preconditions are met.
         *
         * \pre The number of pentachora in the given triangulation is
         * precisely the number returned by getSourceSimplices() for
         * this isomorphism.
         * \pre This is a valid isomorphism (i.e., it has been properly
         * initialised, so that all pentachoron images are non-negative
         * and distinct, and all facet permutations are real permutations
         * of (0,1,2,3,4).
         * \pre Each pentachoron image for this isomorphism lies
         * between 0 and <tt>getSourceSimplices()-1</tt> inclusive
         * (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * @param tri the triangulation to which this isomorphism
         * should be applied.
         */
        void applyInPlace(Dim4Triangulation* tri) const;

        /**
         * Returns a random isomorphism for the given number of
         * pentachora.  This isomorphism will reorder pentachora
         * 0 to <tt>nPentachora-1</tt> in a random fashion, and for
         * each pentachoron a random permutation of its five vertices
         * will be selected.
         *
         * The isomorphism returned will be newly constructed, and must be
         * destroyed by the caller of this routine.
         *
         * Note that both the STL random number generator and the
         * standard C function rand() are used in this routine.  All
         * possible isomorphisms for the given number of pentachora are
         * equally likely.
         *
         * @param nPentachora the number of pentachora that the new
         * isomorphism should operate upon.
         * @return the newly constructed random isomorphism.
         */
        static Dim4Isomorphism* random(unsigned nPentachora);
};

/*@}*/

// Inline functions for Dim4Isomorphism

inline Dim4Isomorphism::Dim4Isomorphism(unsigned sourcePentachora) :
        NGeneralIsomorphism<4>(sourcePentachora) {
}

inline Dim4Isomorphism::Dim4Isomorphism(const Dim4Isomorphism& cloneMe) :
        NGeneralIsomorphism<4>(cloneMe) {
}

inline unsigned Dim4Isomorphism::getSourcePentachora() const {
    return nSimplices_;
}

inline int& Dim4Isomorphism::pentImage(unsigned sourcePent) {
    return simpImage_[sourcePent];
}

inline int Dim4Isomorphism::pentImage(unsigned sourcePent) const {
    return simpImage_[sourcePent];
}

inline Dim4Isomorphism* Dim4Isomorphism::random(unsigned nPentachora) {
    return randomInternal<Dim4Isomorphism>(nPentachora);
}

} // namespace regina

#endif

