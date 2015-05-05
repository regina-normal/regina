
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

/*! \file generic/isomorphism.h
 *  \brief Deals with combinatorial isomorphisms between triangulations.
 */

#ifndef __ISOMORPHISM_H
#ifndef __DOXYGEN
#define __ISOMORPHISM_H
#endif

#include "regina-core.h"
#include "output.h"
#include "generic/dimtraits.h"
#include "generic/nfacetspec.h"
#include "generic/policies.h"
#include "maths/nperm.h"
#include <algorithm>
#include <boost/noncopyable.hpp>

namespace regina {

template <int dim> class Isomorphism;
template <int dim> class Triangulation;

/**
 * \weakgroup generic
 * @{
 */

/**
 * Provides core functionality for combinatorial isomorphisms between
 * <i>dim</i>-manifold triangulations.
 *
 * Such an isomorphism is represented by the class Isomorphism<dim>,
 * which uses this as a base class.  End users should not need to refer
 * to IsomorphismBase directly.
 *
 * See the Isomorphism class notes for further information.
 *
 * \ifacespython This base class is not present, but the "end user" class
 * Isomorphism<dim> is.
 *
 * \tparam dim the dimension of the triangulations that this isomorphism
 * class works with.  This must be at least 2.
 */
template <int dim>
class REGINA_API IsomorphismBase :
        public Output<IsomorphismBase<dim>>,
        public boost::noncopyable {
    static_assert(dim >= 2, "Isomorphism requires dimension >= 2.");
    protected:
        unsigned nSimplices_;
            /**< The number of simplices in the source triangulation. */
        int* simpImage_;
            /**< Stores the simplex of the destination triangulation that
                 each simplex of the source triangulation maps to.
                 This array has size nSimplices_. */
        NPerm<dim+1>* facetPerm_;
            /**< The permutation applied to the facets of each
                 source simplex.  This array has size nSimplices_. */

    public:
        /**
         * Creates a new isomorphism with no initialisation.
         * The images of the simplices and their vertices must be
         * explicitly set using simpImage() and facetPerm().
         *
         * @param nSimplices the number of simplices in the source
         * triangulation associated with this isomorphism.
         * This is allowed to be zero.
         */
        IsomorphismBase(unsigned nSimplices);
        /**
         * Creates a new copy of the given isomorphism.
         *
         * @param copy the isomorphism to copy.
         */
        IsomorphismBase(const IsomorphismBase<dim>& copy);
        /**
         * Destroys this isomorphism.
         */
        ~IsomorphismBase();

        /**
         * Returns the number of simplices in the source triangulation
         * associated with this isomorphism.  Note that this is always
         * less than or equal to the number of simplices in the
         * destination triangulation.
         *
         * @return the number of simplices in the source triangulation.
         */
        unsigned size() const;

        /**
         * A deprecated alias for size(), which returns the number of
         * simplices in the source triangulation associated with this
         * isomorphism.
         *
         * \deprecated Simply call size() instead.
         *
         * @return the number of simplices in the source triangulation.
         */
        unsigned getSourceSimplices() const;

        /**
         * Determines the image of the given source simplex under
         * this isomorphism.
         *
         * \ifacespython This is not available for Python users.
         * However, the read-only version of this routine is.
         *
         * @param sourceSimp the index of the source simplex; this must
         * be between 0 and <tt>size()-1</tt> inclusive.
         * @return a reference to the index of the destination simplex
         * that the source simplex maps to.
         */
        int& simpImage(unsigned sourceSimp);
        /**
         * Determines the image of the given source simplex under
         * this isomorphism.
         *
         * @param sourceSimp the index of the source simplex; this must
         * be between 0 and <tt>size()-1</tt> inclusive.
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
         * \ifacespython This is not available for Python users.
         * However, the read-only version of this routine is.
         *
         * @param sourceSimp the index of the source simplex containing
         * the original (\a dim + 1) facets; this must be between 0 and
         * <tt>size()-1</tt> inclusive.
         * @return a read-write reference to the permutation applied to the
         * facets of the source simplex.
         */
        NPerm<dim+1>& facetPerm(unsigned sourceSimp);
        /**
         * Determines the permutation that is applied to the (\a dim + 1)
         * facets of the given source simplex under this isomorphism.
         * Facet \a i of source simplex \a sourceSimp will be mapped to
         * face <tt>facetPerm(sourceSimp)[i]</tt> of simplex
         * <tt>simpImage(sourceSimp)</tt>.
         *
         * @param sourceSimp the index of the source simplex containing
         * the original (\a dim + 1) facets; this must be between 0 and
         * <tt>size()-1</tt> inclusive.
         * @return the permutation applied to the facets of the
         * source simplex.
         */
        NPerm<dim+1> facetPerm(unsigned sourceSimp) const;
        /**
         * Determines the image of the given source simplex facet
         * under this isomorphism.  This operator returns by value:
         * it cannot be used to alter the isomorphism.
         *
         * @param source the given source simplex facet; this must
         * be one of the (\a dim + 1) facets of one of the size()
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
         * Applies this isomorphism to the given triangulation, and
         * returns the result as a new triangulation.
         *
         * An isomorphism represents a combinatorial map from a triangulation
         * \a T to a triangulation \a U.  This routine treats the given
         * triangulation as the domain \a T, and returns the corresponding
         * range \a U.  The given triangulation \a T is not modified in any way.
         *
         * In more detail: A new triangulation \a U is returned, so that this
         * isomorphism represents a one-to-one, onto and boundary complete
         * isomorphism from \a T to \a U.  That is, \a T and \a U will be
         * combinatorially identical triangulations, and this isomorphism
         * describes the mapping from the simplices of \a T and their facets
         * to the simplices of \a U and their facets.
         *
         * The resulting triangulation \a U is newly created, and must be
         * destroyed by the caller of this routine.
         *
         * There are several preconditions to this routine.  This
         * routine does a small amount of sanity checking (and returns 0
         * if an error is detected), but it certainly does not check the
         * full set of preconditions.  It is up to the caller of this
         * routine to verify that all of the following preconditions are met.
         *
         * \pre The number of simplices in the given triangulation is
         * precisely the number returned by size() for this isomorphism.
         * \pre The simplex images are precisely 0,1,...,size()-1 in some
         * order (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * @param original the triangulation to which this isomorphism
         * should be applied.
         * @return the new isomorphic triangulation, or 0 if a problem
         * was encountered (i.e., an unmet precondition was noticed).
         */
        Triangulation<dim>* apply(const Triangulation<dim>* original) const;

        /**
         * Applies this isomorphism to the given triangulation,
         * modifying the given triangulation directly.
         *
         * This is similar to apply(), except that instead of creating a
         * new triangulation, the simplices and vertices of the given
         * triangulation are modified in-place.
         *
         * See apply() for further details on how this operation is performed.
         *
         * As with apply(), there are several preconditions to this routine.
         * This routine does a small amount of sanity checking (and returns
         * without changes if an error is detected), but it certainly does
         * not check the full set of preconditions.  It is up to the
         * caller of this routine to verify that all of the following
         * preconditions are met.
         *
         * \pre The number of simplices in the given triangulation is
         * precisely the number returned by size() for this isomorphism.
         * \pre The simplex images are precisely 0,1,...,size()-1 in some
         * order (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * @param tri the triangulation to which this isomorphism
         * should be applied.
         */
        void applyInPlace(Triangulation<dim>* tri) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /**
         * Returns the identity isomorphism for the given number of simplices.
         * This isomorphism sends every simplex and every vertex to itself.
         *
         * The isomorphism will be newly constructed, and must be
         * destroyed by the caller of this routine.
         *
         * @param nSimplices the number of simplices that the new
         * isomorphism should operate upon.
         * @return the newly constructed identity isomorphism.
         */
        static Isomorphism<dim>* identity(unsigned nSimplices);

        /**
         * Returns a random isomorphism for the given number of simplices.
         * This isomorphism will reorder simplices
         * 0 to <tt>nSimplices-1</tt> in a random fashion, and for
         * each simplex a random permutation of its (\a dim + 1) vertices
         * will be selected.
         *
         * The isomorphism will be newly constructed, and must be
         * destroyed by the caller of this routine.
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
        static Isomorphism<dim>* random(unsigned nSimplices);
};

/**
 * Represents a combinatorial isomorphism from one <i>dim</i>-manifold
 * triangulation into another.
 *
 * In essence, a combinatorial isomorphism from triangulation \a T to
 * triangulation \a U is a one-to-one map from the simplices of \a T to the
 * simplices of \a U that allows relabelling of both the simplices and
 * their facets (or equivalently, their vertices), and that preserves
 * gluings across adjacent simplices.
 *
 * More precisely:  An isomorphism consists of (i) a one-to-one map \a f
 * from the simplices of \a T to the simplices of \a U, and (ii) for each
 * simplex \a S of \a T, a permutation \a f<sub>S</sub> of the facets
 * (0,...,\a dim) of \a S, for which the following condition holds:
 *
 *   - If facet \a k of simplex \a S and facet \a k' of simplex \a S'
 *     are identified in \a T, then facet \a f<sub>S</sub>(\a k) of \a f(S)
 *     and facet \a f<sub>S'</sub>(\a k') of \a f(S') are identified in \a U.
 *     Moreover, their gluing is consistent with the facet/vertex permutations;
 *     that is, there is a commutative square involving the gluing maps in
 *     \a T and \a U and the permutations \a f<sub>S</sub> and
 *     \a f<sub>S'</sub>.
 *
 * Isomorphisms can be <i>boundary complete</i> or
 * <i>boundary incomplete</i>.  A boundary complete isomorphism
 * satisfies the additional condition:
 *
 *   - If facet \a x is a boundary facet of \a T then facet \a f(x) is a
 *     boundary facet of \a U.
 *
 * A boundary complete isomorphism thus indicates that a copy of
 * triangulation \a T is present as an entire component (or components) of \a U,
 * whereas a boundary incomplete isomorphism represents an embedding of a
 * copy of triangulation \a T as a subcomplex of some possibly larger component
 * (or components) of \a U.
 *
 * Note that for all types of isomorphism, triangulation \a U is allowed
 * to contain more simplices than triangulation \a T.
 *
 * For dimensions 2 and 3, this template is specialised and offers
 * some extra functionality.  In order to use these specialised classes,
 * you will need to include the corresponding headers
 * (dim2/dim2isomorphism.h for \a dim = 2, or
 * triangulation/nisomorphism.h for \a dim = 3).  For convenience, there
 * are typedefs available for these specialised classes (Dim2Isomorphism and
 * NIsomorphism respectively).
 *
 * \ifacespython Python does not support templates.  For \a dim = 2 and 3,
 * this class is available in Python under the names Isomorphism2 and
 * Isomorphism3 respectively (as well as the typedefs mentioned above).
 * Higher-dimensional classes are not available in Python for the time being.
 *
 * \tparam dim The dimension of the underlying triangulation.
 * This must be at least 2.
 */
template <int dim>
class REGINA_API Isomorphism : public IsomorphismBase<dim> {
    public:
        /**
         * Creates a new isomorphism with no initialisation.
         * The images of the simplices and their vertices must be
         * explicitly set using simpImage() and facetPerm().
         *
         * \ifacespython Not present.
         *
         * @param nSimplices the number of simplices in the source
         * triangulation associated with this isomorphism.
         * This is allowed to be zero.
         */
        Isomorphism(unsigned nSimplices);
        /**
         * Creates a copy of the given isomorphism.
         *
         * @param copy the isomorphism to copy.
         */
        Isomorphism(const Isomorphism& copy);
};

// Note that some of our isomorphism classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class Isomorphism<2>;
template <> class Isomorphism<3>;

/*@}*/

// Inline functions for IsomorphismBase

template <int dim>
inline IsomorphismBase<dim>::IsomorphismBase(unsigned nSimplices) :
        nSimplices_(nSimplices),
        simpImage_(new int[nSimplices]),
        facetPerm_(new NPerm<dim+1>[nSimplices]) {
}

template <int dim>
inline IsomorphismBase<dim>::IsomorphismBase(const IsomorphismBase<dim>& copy) :
        nSimplices_(copy.nSimplices_),
        simpImage_(new int[copy.nSimplices_]),
        facetPerm_(new NPerm<dim+1>[copy.nSimplices_]) {
    std::copy(copy.simpImage_, copy.simpImage_ + nSimplices_, simpImage_);
    std::copy(copy.facetPerm_, copy.facetPerm_ + nSimplices_, facetPerm_);
}

template <int dim>
inline IsomorphismBase<dim>::~IsomorphismBase() {
    delete[] simpImage_;
    delete[] facetPerm_;
}

template <int dim>
inline unsigned IsomorphismBase<dim>::size() const {
    return nSimplices_;
}

template <int dim>
inline unsigned IsomorphismBase<dim>::getSourceSimplices() const {
    return nSimplices_;
}

template <int dim>
inline int& IsomorphismBase<dim>::simpImage(unsigned sourceSimp) {
    return simpImage_[sourceSimp];
}

template <int dim>
inline int IsomorphismBase<dim>::simpImage(unsigned sourceSimp) const {
    return simpImage_[sourceSimp];
}

template <int dim>
inline NPerm<dim+1>& IsomorphismBase<dim>::facetPerm(unsigned sourceSimp) {
    return facetPerm_[sourceSimp];
}

template <int dim>
inline NPerm<dim+1> IsomorphismBase<dim>::facetPerm(unsigned sourceSimp) const {
    return facetPerm_[sourceSimp];
}

template <int dim>
inline NFacetSpec<dim> IsomorphismBase<dim>::operator [] (
        const NFacetSpec<dim>& source) const {
    return NFacetSpec<dim>(simpImage_[source.simp],
        facetPerm_[source.simp][source.facet]);
}

template <int dim>
bool IsomorphismBase<dim>::isIdentity() const {
    for (unsigned p = 0; p < nSimplices_; ++p) {
        if (simpImage_[p] != p)
            return false;
        if (! facetPerm_[p].isIdentity())
            return false;
    }
    return true;
}

template <int dim>
Triangulation<dim>* IsomorphismBase<dim>::apply(
        const Triangulation<dim>* original) const {
    if (original->getNumberOfSimplices() != nSimplices_)
        return 0;

    if (nSimplices_ == 0)
        return new Triangulation<dim>();

    Triangulation<dim>* ans = new Triangulation<dim>();
    Simplex<dim>** tet = new Simplex<dim>*[nSimplices_];
    unsigned long t;
    int f;

    ChangeEventSpan<Triangulation<dim>> span(ans);
    for (t = 0; t < nSimplices_; t++)
        tet[t] = ans->newSimplex();

    for (t = 0; t < nSimplices_; t++)
        tet[simpImage_[t]]->setDescription(
            original->getSimplex(t)->getDescription());

    const Simplex<dim> *myTet, *adjTet;
    unsigned long adjTetIndex;
    NPerm<dim+1> gluingPerm;
    for (t = 0; t < nSimplices_; t++) {
        myTet = original->getSimplex(t);
        for (f = 0; f <= dim; f++)
            if ((adjTet = myTet->adjacentSimplex(f))) {
                // We have an adjacent simplex.
                adjTetIndex = original->simplexIndex(adjTet);
                gluingPerm = myTet->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjTetIndex > t || (adjTetIndex == t &&
                        gluingPerm[f] > f))
                    tet[simpImage_[t]]->joinTo(facetPerm_[t][f],
                        tet[simpImage_[adjTetIndex]],
                        facetPerm_[adjTetIndex] * gluingPerm *
                            facetPerm_[t].inverse());
            }
    }

    delete[] tet;
    return ans;
}

template <int dim>
void IsomorphismBase<dim>::applyInPlace(Triangulation<dim>* tri) const {
    if (tri->getNumberOfSimplices() != nSimplices_)
        return;

    if (nSimplices_ == 0)
        return;

    Triangulation<dim>* staging = apply(tri);
    tri->swapContents(*staging);
    delete staging;
}

template <int dim>
inline void IsomorphismBase<dim>::writeTextShort(std::ostream& out) const {
    out << "Isomorphism between " << dim << "-manifold triangulations";
}

template <int dim>
inline void IsomorphismBase<dim>::writeTextLong(std::ostream& out) const {
    for (unsigned i = 0; i < nSimplices_; ++i)
        out << i << " -> " << simpImage_[i] << " (" << facetPerm_[i] << ")\n";
}

template <int dim>
inline Isomorphism<dim>* IsomorphismBase<dim>::identity(unsigned nSimplices) {
    Isomorphism<dim>* id = new Isomorphism<dim>(nSimplices);
    for (unsigned i = 0; i < nSimplices; ++i)
        id->simpImage_[i] = i;
    return id;
}

template <int dim>
Isomorphism<dim>* IsomorphismBase<dim>::random(unsigned nSimplices) {
    Isomorphism<dim>* ans = new Isomorphism<dim>(nSimplices);

    // Randomly choose the destination simplices.
    unsigned i;
    for (i = 0; i < nSimplices; i++)
        ans->simpImage_[i] = i;
    std::random_shuffle(ans->simpImage_, ans->simpImage_ + nSimplices);

    // Randomly choose the individual permutations.
    for (i = 0; i < nSimplices; i++)
        ans->facetPerm_[i] = NPerm<dim+1>::rand();

    return ans;
}

// Inline functions for Isomorphism

template <int dim>
inline Isomorphism<dim>::Isomorphism(unsigned nSimplices) :
        IsomorphismBase<dim>(nSimplices) {
}

template <int dim>
inline Isomorphism<dim>::Isomorphism(const Isomorphism& copy) :
        IsomorphismBase<dim>(copy) {
}

} // namespace regina

#endif

