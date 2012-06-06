
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
 *
 * TODO: Fix the class notes.
 *
 * A combinatorial isomorphism from triangulation T to triangulation U
 * is a one-to-one map f from the individual simplex facets of T to
 * the individual simplex facets of U for which the following
 * conditions hold:
 *
 *   - if facets x and y belong to the same simplex of T then
 *     facets f(x) and f(y) belong to the same simplex of U;
 *   - if facets x and y are identified in T then facets f(x) and f(y)
 *     are identified in U.
 *
 * Isomorphisms can be <i>boundary complete</i> or
 * <i>boundary incomplete</i>.  A boundary complete isomorphism
 * satisfies the additional condition:
 *
 *   - if facet x is a boundary facet of T then facet f(x) is a boundary
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
class REGINA_API NGeneralIsomorphism : public ShareableObject {
    public:
        typedef typename DimTraits<dim>::Perm Perm;
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
         * \ifacespython Not present.
         *
         * @param nSimplices the number of simplices in the source
         * triangulation associated with this isomorphism; this may be zero.
         */
        NGeneralIsomorphism(unsigned nSimplices);
        /**
         * Creates a new isomorphism identical to the given isomorphism.
         *
         * @param cloneMe the isomorphism upon which to base the new
         * isomorphism.
         */
        NGeneralIsomorphism(const NGeneralIsomorphism& cloneMe);
        /**
         * Destroys this isomorphism.
         */
        ~NGeneralIsomorphism();

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
         * \ifacespython Not present, though the read-only version of this
         * routine is.
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
         * \ifacespython Not present, though the read-only version of this
         * routine is.
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

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
};

/*@}*/

// Inline functions for NGeneralIsomorphism

template <int dim>
inline NGeneralIsomorphism<dim>::NGeneralIsomorphism(unsigned nSimplices) :
        nSimplices_(nSimplices),
        simpImage_(nSimplices > 0 ? new int[nSimplices] : 0),
        facetPerm_(nSimplices > 0 ? new Perm[nSimplices] : 0) {
}

template <int dim>
inline NGeneralIsomorphism<dim>::~NGeneralIsomorphism() {
    // Always safe to delete null.
    delete[] simpImage_;
    delete[] facetPerm_;
}

template <int dim>
inline unsigned NGeneralIsomorphism<dim>::getSourceSimplices() const {
    return nSimplices_;
}

template <int dim>
inline int& NGeneralIsomorphism<dim>::simpImage(unsigned sourceSimp) {
    return simpImage_[sourceSimp];
}

template <int dim>
inline int NGeneralIsomorphism<dim>::simpImage(unsigned sourceSimp) const {
    return simpImage_[sourceSimp];
}

template <int dim>
inline typename NGeneralIsomorphism<dim>::Perm&
        NGeneralIsomorphism<dim>::facetPerm(
        unsigned sourceSimp) {
    return facetPerm_[sourceSimp];
}

template <int dim>
inline typename NGeneralIsomorphism<dim>::Perm
        NGeneralIsomorphism<dim>::facetPerm(
        unsigned sourceSimp) const {
    return facetPerm_[sourceSimp];
}

template <int dim>
inline NFacetSpec<dim> NGeneralIsomorphism<dim>::operator [] (
        const NFacetSpec<dim>& source) const {
    return NFacetSpec<dim>(simpImage_[source.simp],
        facetPerm_[source.simp][source.facet]);
}

} // namespace regina

#endif

