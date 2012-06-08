
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

/*! \file triangulation/ngenericisomorphism.h
 *  \brief Deals with combinatorial isomorphisms of \a n-manifold
 *  triangulations.
 */

#ifndef __NGENERALISOMORPHISM_H
#ifndef __DOXYGEN
#define __NGENERALISOMORPHISM_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "triangulation/dimtraits.h"
#include "triangulation/nfacetspec.h"

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * A dimension-agnostic base class that represents a combinatorial
 * isomorphism from one \a dim-manifold triangulation into another.
 * Each dimension that Regina works with (2 and 3) offers its own
 * subclass with richer functionality; users typically do not need to
 * work with this template base class directly.
 *
 * In essence, a combinatorial isomorphism from triangulation T to
 * triangulation U is a one-to-one map from the simplices of T to the
 * simplices of U that allows relabelling of both the simplices and
 * their facets (or equivalently, their vertices), and that preserves
 * gluings across adjacent simplices.
 *
 * More precisely:  An isomorphism consists of (i) a one-to-one map f
 * from the simplices of T to the simplices of U, and (ii) for each
 * simplex S of T, a permutation f_S of the facets (0,...,\a dim) of S,
 * for which the following condition holds:
 *
 *   - If facet k of simplex S and facet k' of simplex S'
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
 * Note that in all cases triangulation U may contain more simplices
 * than triangulation T.
 *
 * \pre The dimension argument \a dim is either 2 or 3.
 *
 * \ifacespython Not present, though the dimension-specific subclasses
 * (such as NIsomorphism and Dim4Isomorphism) are available for Python users.
 *
 * \testpart
 */
template <int dim>
class NGenericIsomorphism : public ShareableObject {
    public:
        typedef typename DimTraits<dim>::Isomorphism Isomorphism;
        typedef typename DimTraits<dim>::Perm Perm;
        typedef typename DimTraits<dim>::Simplex Simplex;
        typedef typename DimTraits<dim>::Triangulation Triangulation;
    protected:
        unsigned nSimplices_;
            /**< The number of simplices in the source triangulation. */
        int* simpImage_;
            /**< The simplex of the destination triangulation that
                 each simplex of the source triangulation maps to. */
        Perm* facetPerm_;
            /**< The permutation applied to the facets of each
                 source simplex. */

    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * @param nSimplices the number of simplices in the source
         * triangulation associated with this isomorphism; this may be zero.
         */
        NGenericIsomorphism(unsigned nSimplices);
        /**
         * Creates a new isomorphism identical to the given isomorphism.
         *
         * @param cloneMe the isomorphism upon which to base the new
         * isomorphism.
         */
        NGenericIsomorphism(const NGenericIsomorphism& cloneMe);
        /**
         * Destroys this isomorphism.
         */
        ~NGenericIsomorphism();

        /**
         * Returns the number of simplices in the source triangulation
         * associated with this isomorphism.  Note that this is always
         * less than or equal to the number of simplices in the
         * destination triangulation.
         *
         * @return the number of simplices in the source triangulation.
         */
        unsigned getSourceSimplices() const;

        /**
         * Determines the image of the given source simplex under
         * this isomorphism.
         *
         * \ifacespython This is not available for Python users, even in
         * the dimension-specific subclasses.  However, the read-only
         * version of this routine is.
         *
         * @param sourceSimp the index of the source simplex; this must
         * be between 0 and <tt>getSourceSimplices()-1</tt> inclusive.
         * @return a reference to the index of the destination simplex
         * that the source simplex maps to.
         */
        int& simpImage(unsigned sourceSimp);
        /**
         * Determines the image of the given source simplex under
         * this isomorphism.
         *
         * @param sourceSimp the index of the source simplex; this must
         * be between 0 and <tt>getSourceSimplices()-1</tt> inclusive.
         * @return the index of the destination simplex
         * that the source simplex maps to.
         */
        int simpImage(unsigned sourceSimp) const;
        /**
         * Returns a read-write reference to the permutation that is
         * applied to the (\a dim + 1) facets of the given source simplex
         * under this isomorphism.
         * Facet \a i of source simplex \a sourceSimp will be mapped to
         * facet <tt>facetPerm(sourceSimp)[i]</tt> of simplex
         * <tt>simpImage(sourceSimp)</tt>.
         *
         * \ifacespython This is not available for Python users, even in
         * the dimension-specific subclasses.  However, the read-only
         * version of this routine is.
         *
         * @param sourceSimp the index of the source simplex containing
         * the original (\a dim + 1) facets; this must be between 0 and
         * <tt>getSourceSimplices()-1</tt> inclusive.
         * @return a read-write reference to the permutation applied to the
         * facets of the source simplex.
         */
        Perm& facetPerm(unsigned sourceSimp);
        /**
         * Determines the permutation that is applied to the (\a dim + 1)
         * facets of the given source simplex under this isomorphism.
         * Facet \a i of source simplex \a sourceSimp will be mapped to
         * face <tt>facetPerm(sourceSimp)[i]</tt> of simplex
         * <tt>simpImage(sourceSimp)</tt>.
         *
         * @param sourceSimp the index of the source simplex containing
         * the original (\a dim + 1) facets; this must be between 0 and
         * <tt>getSourceSimplices()-1</tt> inclusive.
         * @return the permutation applied to the facets of the
         * source simplex.
         */
        Perm facetPerm(unsigned sourceSimp) const;
        /**
         * Determines the image of the given source simplex facet
         * under this isomorphism.  Note that a value only is returned; this
         * routine cannot be used to alter the isomorphism.
         *
         * @param source the given source simplex facet; this must
         * be one of the (\a dim + 1) facets of one of the getSourceSimplices()
         * simplices in the source triangulation.
         * @return the image of the source simplex facet under this
         * isomorphism.
         */
        NFacetSpec<dim> operator [] (const NFacetSpec<dim>& source) const;

        /**
         * Determines whether or not this is an identity isomorphism.
         *
         * In an identity isomorphism, each simplex image is itself,
         * and within each simplex the facet/vertex permutation is
         * the identity permutation.
         *
         * @return \c true if this is an identity isomorphism, or
         * \c false otherwise.
         */
        bool isIdentity() const;

        /**
         * Applies this isomorphism to the given triangulation and
         * returns the result as a new triangulation.
         *
         * The given triangulation (call this T) is not modified in any way.
         * A new triangulation (call this S) is returned, so that this
         * isomorphism represents a one-to-one, onto and boundary complete
         * isomorphism from T to S.  That is, T and S are combinatorially
         * identical triangulations, and this isomorphism describes the
         * corresponding mapping between simplex and simplex facets.
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
         * \pre The number of simplices in the given triangulation is
         * precisely the number returned by getSourceSimplices() for
         * this isomorphism.
         * \pre This is a valid isomorphism (i.e., it has been properly
         * initialised, so that all simplex images are non-negative
         * and distinct, and all facet permutations are real permutations
         * of (0,...,\a dim).
         * \pre Each simplex image for this isomorphism lies
         * between 0 and <tt>getSourceSimplices()-1</tt> inclusive
         * (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * @param original the triangulation to which this isomorphism
         * should be applied.
         * @return the resulting new triangulation, or 0 if a problem
         * was encountered (i.e., an unmet precondition was noticed).
         */
        Triangulation* apply(const Triangulation* original) const;

        /**
         * Applies this isomorphism to the given triangulation,
         * modifying the given triangulation directly.
         *
         * This is similar to apply(), except that instead of creating a
         * new triangulation, the simplices and vertices of the given
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
         * \pre The number of simplices in the given triangulation is
         * precisely the number returned by getSourceSimplices() for
         * this isomorphism.
         * \pre This is a valid isomorphism (i.e., it has been properly
         * initialised, so that all simplex images are non-negative
         * and distinct, and all facet permutations are real permutations
         * of (0,...,\a dim).
         * \pre Each simplex image for this isomorphism lies
         * between 0 and <tt>getSourceSimplices()-1</tt> inclusive
         * (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * @param tri the triangulation to which this isomorphism
         * should be applied.
         */
        void applyInPlace(Triangulation* tri) const;

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        /**
         * Returns a random isomorphism for the given number of
         * simplices.  This isomorphism will reorder simplices
         * 0 to <tt>nSimplices-1</tt> in a random fashion, and for
         * each simplex a random permutation of its (\a dim + 1) vertices
         * will be selected.
         *
         * The isomorphism will be newly constructed, and must be
         * destroyed by the caller of this routine.  The new isomorphism
         * will be of the appropriate dimension-specific subclass
         * (e.g., NIsomorphism for \a dim=3, or Dim2Isomorphism for
         * \a dim=2).
         *
         * Note that both the STL random number generator and the
         * standard C function rand() are used in this routine.  All
         * possible isomorphisms for the given number of simplices are
         * equally likely.
         *
         * @param nSimplices the number of simplices that the new
         * isomorphism should operate upon.
         * @return the newly constructed random isomorphism.
         */
        static Isomorphism* random(unsigned nSimplices);
};

/*@}*/

// Inline functions for NGenericIsomorphism

template <int dim>
inline NGenericIsomorphism<dim>::NGenericIsomorphism(unsigned nSimplices) :
        nSimplices_(nSimplices),
        simpImage_(nSimplices > 0 ? new int[nSimplices] : 0),
        facetPerm_(nSimplices > 0 ? new Perm[nSimplices] : 0) {
}

template <int dim>
inline NGenericIsomorphism<dim>::~NGenericIsomorphism() {
    // Always safe to delete null.
    delete[] simpImage_;
    delete[] facetPerm_;
}

template <int dim>
inline unsigned NGenericIsomorphism<dim>::getSourceSimplices() const {
    return nSimplices_;
}

template <int dim>
inline int& NGenericIsomorphism<dim>::simpImage(unsigned sourceSimp) {
    return simpImage_[sourceSimp];
}

template <int dim>
inline int NGenericIsomorphism<dim>::simpImage(unsigned sourceSimp) const {
    return simpImage_[sourceSimp];
}

template <int dim>
inline typename NGenericIsomorphism<dim>::Perm&
        NGenericIsomorphism<dim>::facetPerm(
        unsigned sourceSimp) {
    return facetPerm_[sourceSimp];
}

template <int dim>
inline typename NGenericIsomorphism<dim>::Perm
        NGenericIsomorphism<dim>::facetPerm(
        unsigned sourceSimp) const {
    return facetPerm_[sourceSimp];
}

template <int dim>
inline NFacetSpec<dim> NGenericIsomorphism<dim>::operator [] (
        const NFacetSpec<dim>& source) const {
    return NFacetSpec<dim>(simpImage_[source.simp],
        facetPerm_[source.simp][source.facet]);
}

} // namespace regina

#endif

