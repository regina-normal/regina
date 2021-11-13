
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file triangulation/generic/isomorphism.h
 *  \brief Internal header for combinatorial isomorphisms between
 *  triangulations of arbitrary dimension.
 *
 *  This file is automatically included from triangulation/generic.h;
 *  there is no need for end users to include this header explicitly.
 */

#ifndef __REGINA_ISOMORPHISM_H
#ifndef __DOXYGEN
#define __REGINA_ISOMORPHISM_H
#endif

#include "regina-core.h"
#include "core/output.h"
#include "triangulation/facetspec.h"
#include "triangulation/alias/isomorphism.h"
#include "triangulation/forward.h"
#include "maths/perm.h"
#include "utilities/exception.h"
#include "utilities/randutils.h"
#include <algorithm>

namespace regina {

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
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ifacespython Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., Isomorphism2 and Isomorphism3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the triangulations that this isomorphism
 * class works with.  This must be between 2 and 15 inclusive.
 *
 * \headerfile triangulation/generic.h
 *
 * \ingroup generic
 */
template <int dim>
class Isomorphism :
        public Output<Isomorphism<dim>>,
        public alias::IsomorphismImage<Isomorphism<dim>, dim> {
    static_assert(dim >= 2, "Isomorphism requires dimension >= 2.");

    protected:
        unsigned nSimplices_;
            /**< The number of simplices in the source triangulation. */
        int* simpImage_;
            /**< Stores the simplex of the destination triangulation that
                 each simplex of the source triangulation maps to.
                 This array has size nSimplices_. */
        Perm<dim+1>* facetPerm_;
            /**< The permutation applied to the facets of each
                 source simplex.  This array has size nSimplices_. */

    public:
        /**
         * Creates a new isomorphism with no initialisation.
         * The images of the simplices and their vertices must be
         * explicitly set using simpImage() and facetPerm().
         *
         * \ifacespython For Python users, the images of the simplices and
         * their vertices must be set using setSimpImage() and setFacetPerm()
         * instead.
         *
         * @param nSimplices the number of simplices in the source
         * triangulation associated with this isomorphism.
         * This is allowed to be zero.
         */
        Isomorphism(unsigned nSimplices);
        /**
         * Creates a copy of the given isomorphism.
         *
         * @param src the isomorphism to copy.
         */
        Isomorphism(const Isomorphism& src);
        /**
         * Moves the given isomorphism into this new isomorphism.
         * This is a fast (constant time) operation.
         *
         * The isomorphism that is passed (\a src) will no longer be usable.
         *
         * @param src the isomorphism to move.
         */
        Isomorphism(Isomorphism&& src) noexcept;
        /**
         * Destroys this isomorphism.
         */
        ~Isomorphism();
        /**
         * Copies the given isomorphism into this isomorphism.
         *
         * It does not matter if this and the given isomorphism use different
         * numbers of simplices; if they do then this isomorphism will be
         * resized as a result.
         *
         * This operator induces a deep copy of \a src.
         *
         * @param src the isomorphism to copy.
         * @return a reference to this isomorphism.
         */
        Isomorphism& operator = (const Isomorphism& src);
        /**
         * Moves the given isomorphism into this isomorphism.
         * This is a fast (constant time) operation.
         *
         * It does not matter if this and the given isomorphism use different
         * numbers of simplices; if they do then this isomorphism will be
         * resized as a result.
         *
         * The isomorphism that is passed (\a src) will no longer be usable.
         *
         * @param src the isomorphism to move.
         * @return a reference to this isomorphism.
         */
        Isomorphism& operator = (Isomorphism&& src) noexcept;

        /**
         * Swaps the contents of this and the given isomorphism.
         *
         * It does not matter if this and the given isomorphism use different
         * numbers of simplices; if so then they will be adjusted accordingly.
         *
         * @param other the isomorphism whose contents are to be swapped with
         * this.
         */
        void swap(Isomorphism& other) noexcept;

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
         * Determines the image of the given source simplex under
         * this isomorphism.
         *
         * \ifacespython Python users can only access the read-only version
         * of this function that returns by value: you cannot use simpImage()
         * to edit the isomorphism.  As an alternative however, Python users
         * can call <tt>setSimpImage(sourceSimp, image)</tt> instead.
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
         * \ifacespython Python users can only access the read-only version
         * of this function that returns by value: you cannot use facetPerm()
         * to edit the isomorphism.  As an alternative however, Python users
         * can call <tt>setFacetPerm(sourceSimp, perm)</tt> instead.
         *
         * @param sourceSimp the index of the source simplex containing
         * the original (\a dim + 1) facets; this must be between 0 and
         * <tt>size()-1</tt> inclusive.
         * @return a read-write reference to the permutation applied to the
         * facets of the source simplex.
         */
        Perm<dim+1>& facetPerm(unsigned sourceSimp);
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
        Perm<dim+1> facetPerm(unsigned sourceSimp) const;
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
        FacetSpec<dim> operator [] (const FacetSpec<dim>& source) const;

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
         * \pre The simplex images are precisely 0,1,...,size()-1 in some
         * order (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * \todo Lock the topological properties of the underlying manifold,
         * to avoid recomputing them after the isomorphism is applied.
         *
         * \exception InvalidArgument the number of simplices in the given
         * triangulation is not equal to size() for this isomorphism.
         *
         * @param original the triangulation to which this isomorphism
         * should be applied.
         * @return the new isomorphic triangulation.
         */
        Triangulation<dim> apply(const Triangulation<dim>& original) const;

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
         * \pre The simplex images are precisely 0,1,...,size()-1 in some
         * order (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * \todo Lock the topological properties of the underlying manifold,
         * to avoid recomputing them after the isomorphism is applied.
         *
         * \exception InvalidArgument the number of simplices in the given
         * triangulation is not equal to size() for this isomorphism.
         *
         * @param tri the triangulation to which this isomorphism
         * should be applied.
         */
        void applyInPlace(Triangulation<dim>& tri) const;

        /**
         * Returns the composition of this isomorphism with the given
         * isomorphism.
         *
         * This follows the same order convention as Regina's permutation
         * classes: the composition <tt>a * b</tt> first applies the right-hand
         * isomorphism \a b, and then the left-hand isomorphism \a a.
         *
         * \pre The source triangulation for this isomorphism (the left-hand
         * side) is at least as large as the destination triangulation
         * for \a rhs (the right-hand side).  In other words, the maximum
         * value of <tt>rhs.simpImage(i)</tt> over all \a i must be less than
         * <tt>this->size()</tt>.
         *
         * @return the composition of both isomorphisms.
         */
        Isomorphism operator * (const Isomorphism& rhs) const;

        /**
         * Returns the inverse of this isomorphism.
         *
         * \pre The destination triangulation has precisely the same
         * number of simplices as the source triangulation.  In other words,
         * there are no "gaps" in the simplex images: the values
         * <tt>simpImage(0)</tt>, ..., <tt>simpImage(size()-1)</tt> must be
         * a permutation of 0, ..., <tt>size()-1</tt>.
         *
         * @return the inverse isomorphism.
         */
        Isomorphism inverse() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present; use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present; use detail() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /**
         * Returns the identity isomorphism for the given number of simplices.
         * This isomorphism sends every simplex and every vertex to itself.
         *
         * @param nSimplices the number of simplices that the new
         * isomorphism should operate upon.
         * @return the identity isomorphism.
         */
        static Isomorphism<dim> identity(unsigned nSimplices);

        /**
         * Returns a random isomorphism for the given number of simplices.
         * This isomorphism will reorder simplices
         * 0 to <tt>nSimplices-1</tt> in a random fashion, and for
         * each simplex a random permutation of its (\a dim + 1) vertices
         * will be selected.
         *
         * All possible isomorphisms for the given number of simplices are
         * equally likely.
         *
         * This routine is thread-safe, and uses RandomEngine for its
         * random number generation.
         *
         * @param nSimplices the number of simplices that the new
         * isomorphism should operate upon.
         * @param even if \c true, then every simplex will have its
         * vertices permuted with an even permutation.  This means that,
         * if the random isomorphism is applied to an oriented triangulation,
         * it will preserve the orientation.
         * @return the new random isomorphism.
         */
        static Isomorphism<dim> random(unsigned nSimplices, bool even = false);
};

/**
 * Swaps the contents of the given isomorphisms.
 *
 * This global routine simply calls Isomorphism<dim>::swap(); it is provided
 * so that Isomorphism<dim> meets the C++ Swappable requirements.
 *
 * @param a the first isomorphism whose contents should be swapped.
 * @param b the second isomorphism whose contents should be swapped.
 *
 * \ingroup generic
 */
template <int dim>
void swap(Isomorphism<dim>& a, Isomorphism<dim>& b) noexcept;

// Inline functions for Isomorphism

template <int dim>
inline Isomorphism<dim>::Isomorphism(unsigned nSimplices) :
        nSimplices_(nSimplices),
        simpImage_(new int[nSimplices]),
        facetPerm_(new Perm<dim+1>[nSimplices]) {
}

template <int dim>
inline Isomorphism<dim>::Isomorphism(const Isomorphism<dim>& src) :
        nSimplices_(src.nSimplices_),
        simpImage_(new int[src.nSimplices_]),
        facetPerm_(new Perm<dim+1>[src.nSimplices_]) {
    std::copy(src.simpImage_, src.simpImage_ + nSimplices_, simpImage_);
    std::copy(src.facetPerm_, src.facetPerm_ + nSimplices_, facetPerm_);
}

template <int dim>
inline Isomorphism<dim>::Isomorphism(Isomorphism<dim>&& src) noexcept:
        nSimplices_(src.nSimplices_),
        simpImage_(src.simpImage_),
        facetPerm_(src.facetPerm_) {
    src.simpImage_ = nullptr;
    src.facetPerm_ = nullptr;
}

template <int dim>
inline Isomorphism<dim>::~Isomorphism() {
    delete[] simpImage_;
    delete[] facetPerm_;
}

template <int dim>
Isomorphism<dim>& Isomorphism<dim>::operator = (const Isomorphism<dim>& src) {
    // std::copy() exhibits undefined behaviour in the case of self-assignment.
    if (std::addressof(src) == this)
        return *this;

    if (nSimplices_ != src.nSimplices_) {
        delete[] simpImage_;
        delete[] facetPerm_;

        nSimplices_ = src.nSimplices_;

        simpImage_ = new int[nSimplices_];
        facetPerm_ = new Perm<dim+1>[nSimplices_];
    }

    std::copy(src.simpImage_, src.simpImage_ + nSimplices_, simpImage_);
    std::copy(src.facetPerm_, src.facetPerm_ + nSimplices_, facetPerm_);
    return *this;
}

template <int dim>
Isomorphism<dim>& Isomorphism<dim>::operator = (Isomorphism<dim>&& src)
        noexcept{
    nSimplices_ = src.nSimplices_;
    std::swap(simpImage_, src.simpImage_);
    std::swap(facetPerm_, src.facetPerm_);
    // Let src dispose of the original contents in its own destructor.
    return *this;
}

template <int dim>
void Isomorphism<dim>::swap(Isomorphism<dim>& other) noexcept {
    std::swap(nSimplices_, other.nSimplices_);
    std::swap(simpImage_, other.simpImage_);
    std::swap(facetPerm_, other.facetPerm_);
}

template <int dim>
inline unsigned Isomorphism<dim>::size() const {
    return nSimplices_;
}

template <int dim>
inline int& Isomorphism<dim>::simpImage(unsigned sourceSimp) {
    return simpImage_[sourceSimp];
}

template <int dim>
inline int Isomorphism<dim>::simpImage(unsigned sourceSimp) const {
    return simpImage_[sourceSimp];
}

template <int dim>
inline Perm<dim+1>& Isomorphism<dim>::facetPerm(unsigned sourceSimp) {
    return facetPerm_[sourceSimp];
}

template <int dim>
inline Perm<dim+1> Isomorphism<dim>::facetPerm(unsigned sourceSimp) const {
    return facetPerm_[sourceSimp];
}

template <int dim>
inline FacetSpec<dim> Isomorphism<dim>::operator [] (
        const FacetSpec<dim>& source) const {
    return FacetSpec<dim>(simpImage_[source.simp],
        facetPerm_[source.simp][source.facet]);
}

template <int dim>
bool Isomorphism<dim>::isIdentity() const {
    for (unsigned p = 0; p < nSimplices_; ++p) {
        if (simpImage_[p] != p)
            return false;
        if (! facetPerm_[p].isIdentity())
            return false;
    }
    return true;
}

template <int dim>
Triangulation<dim> Isomorphism<dim>::apply(
        const Triangulation<dim>& original) const {
    if (original.size() != nSimplices_)
        throw InvalidArgument("Isomorphism::apply() was given "
            "an input triangulation of the wrong size");

    if (nSimplices_ == 0)
        return Triangulation<dim>();

    Triangulation<dim> ans;
    auto* tet = new Simplex<dim>*[nSimplices_];
    unsigned long t;
    int f;

    // Ensure only one event pair is fired in this sequence of changes.
    typename Triangulation<dim>::ChangeEventSpan span(ans);
    for (t = 0; t < nSimplices_; t++)
        tet[t] = ans.newSimplex();

    for (t = 0; t < nSimplices_; t++)
        tet[simpImage_[t]]->setDescription(
            original.simplex(t)->description());

    const Simplex<dim> *myTet, *adjTet;
    unsigned long adjTetIndex;
    Perm<dim+1> gluingPerm;
    for (t = 0; t < nSimplices_; t++) {
        myTet = original.simplex(t);
        for (f = 0; f <= dim; f++)
            if ((adjTet = myTet->adjacentSimplex(f))) {
                // We have an adjacent simplex.
                adjTetIndex = adjTet->index();
                gluingPerm = myTet->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjTetIndex > t || (adjTetIndex == t &&
                        gluingPerm[f] > f))
                    tet[simpImage_[t]]->join(facetPerm_[t][f],
                        tet[simpImage_[adjTetIndex]],
                        facetPerm_[adjTetIndex] * gluingPerm *
                            facetPerm_[t].inverse());
            }
    }

    delete[] tet;
    return ans;
}

template <int dim>
void Isomorphism<dim>::applyInPlace(Triangulation<dim>& tri) const {
    Triangulation<dim> staging = apply(tri);
    tri.swap(staging);
}

template <int dim>
Isomorphism<dim> Isomorphism<dim>::operator * (const Isomorphism& rhs) const {
    Isomorphism<dim> ans(rhs.nSimplices_);
    for (unsigned i = 0; i < rhs.nSimplices_; ++i) {
        ans.simpImage_[i] = simpImage_[rhs.simpImage_[i]];
        ans.facetPerm_[i] = facetPerm_[rhs.simpImage_[i]] * rhs.facetPerm_[i];
    }
    return ans;
}

template <int dim>
Isomorphism<dim> Isomorphism<dim>::inverse() const {
    Isomorphism<dim> ans(nSimplices_);
    for (unsigned i = 0; i < nSimplices_; ++i) {
        ans.simpImage_[simpImage_[i]] = i;
        ans.facetPerm_[simpImage_[i]] = facetPerm_[i].inverse();
    }
    return ans;
}

template <int dim>
inline void Isomorphism<dim>::writeTextShort(std::ostream& out) const {
    out << "Isomorphism between " << dim << "-manifold triangulations";
}

template <int dim>
inline void Isomorphism<dim>::writeTextLong(std::ostream& out) const {
    for (unsigned i = 0; i < nSimplices_; ++i)
        out << i << " -> " << simpImage_[i] << " (" << facetPerm_[i] << ")\n";
}

template <int dim>
inline Isomorphism<dim> Isomorphism<dim>::identity(unsigned nSimplices) {
    Isomorphism<dim> id(nSimplices);
    for (unsigned i = 0; i < nSimplices; ++i)
        id.simpImage_[i] = i;
    return id;
}

template <int dim>
Isomorphism<dim> Isomorphism<dim>::random(unsigned nSimplices, bool even) {
    Isomorphism<dim> ans(nSimplices);

    // Prepare the destination simplices.
    unsigned i;
    for (i = 0; i < nSimplices; ++i)
        ans.simpImage_[i] = i;

    // Construct the RandomEngine as late as possible, because it grabs
    // a mutex lock.
    RandomEngine engine;

    // Randomly choose the destination simplices.
    std::shuffle(ans.simpImage_, ans.simpImage_ + nSimplices, engine.engine());

    // Randomly choose the individual permutations.
    for (i = 0; i < nSimplices; ++i)
        ans.facetPerm_[i] = Perm<dim+1>::rand(engine.engine(), even);

    return ans;
}

template <int dim>
inline void swap(Isomorphism<dim>& a, Isomorphism<dim>& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

