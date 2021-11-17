
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

/*! \file census/gluingperms.h
 *  \brief Deals with selecting gluing permutations to complement a
 *  particular pairing of facets of simplices in an n-manifold triangulation.
 */

#ifndef __REGINA_GLUINGPERMS_H
#ifndef __DOXYGEN
#define __REGINA_GLUINGPERMS_H
#endif

#include "regina-core.h"
#include "triangulation/facetspec.h"
#include "triangulation/forward.h"
#include "maths/perm.h"

namespace regina {

/**
 * Represents a specific set of gluing permutations to complement a
 * particular pairwise matching of simplex facets.  Given a pairwise
 * matching of facets (as described by class FacetPairing<dim>),
 * each facet that is matched with some other facet will have an associated
 * permutation of (\a dim + 1) elements.
 *
 * If a facet is matched with some other facet, the two associated
 * permutations in this set will be inverses.  If a facet is left
 * deliberately unmatched, it will have no associated permutation in this set.
 *
 * Such a set of permutations models part of the structure of a
 * triangulation, in which each simplex facet that is glued to another
 * facet has a corresponding gluing permutation (and the matched facet has
 * the inverse gluing permutation).
 *
 * The main purpose of this class is to provide a way of quickly describing
 * and manipulating the combinatorial structure of a triangulation without the
 * overhead of a full-blown Triangulation<dim> object.  In particular, this
 * class was designed for use in census enumeration, which involves rapid
 * construction and editing of these permutations.  To convert this into a
 * full Triangulation<dim> object, you can call triangulate().
 *
 * Internally, this class stores each permutation as an index into
 * Perm<dim+1>::Sn_1 (that is, an index into the permutation group \a S_dim).
 * Importantly, you can only \e set permutations using these indices,
 * via the non-const permIndex() function - access to the permutations
 * themselves is read-only.  You can use indexToGluing() and gluingToIndex()
 * to convert between indicex and permutations, or you can use the read-only
 * member function perm() which returns the permutation directly.  Note
 * that this correspondence is non-trivial, and depends on the details
 * of the underlying facet pairing.
 *
 * This class also supports a special "not yet chosen" permutation,
 * represented using the permutation index -1.  Attempting to convert this
 * to a real permutation using perm() or indexToGluing() is not allowed, and
 * will result in undefined behaviour.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \headers Parts of this template class are implemented in a separate header
 * (gluingperms-impl.h), which is not included automatically by
 * this file.  However, typical end users should never need this extra header,
 * since Regina's calculation engine already includes explicit instantiations
 * for all dimensions.
 *
 * \ifacespython Not available.
 *
 * \tparam dim the dimension of the underlying triangulation that is
 * being modelled.  This must be between 2 and 15 inclusive.
 *
 * \ingroup census
 */
template <int dim>
class GluingPerms {
    private:
        FacetPairing<dim> pairing_;
            /**< The facet pairing that this permutation set complements.
                 This is guaranteed to be the minimal representative of
                 its facet pairing isomorphism class. */
        int* permIndices_;
            /**< The index into array Perm<dim+1>::Sn_1 describing how each
                 simplex facet is glued to its partner.  Note that this
                 is not a gluing permutation as such but rather a permutation
                 of 0,...,\a dim-1 only (see the routines gluingToIndex() and
                 indexToGluing() for conversions).  If a permutation has not
                 yet been selected (e.g., if this permutation set is still
                 under construction) then this index is -1. */

    public:
        /**
         * Creates a new permutation set.  All internal permutations will be
         * marked as not yet chosen, and any attempt to query them before
         * setting them to explicit values will result in undefined behaviour.
         * The corresponding permutation indices will be set to -1.
         *
         * This constructor induces a deep copy of the given facet pairing.
         *
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any simplex from any other simplex via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by FacetPairing::isCanonical().  Note that all facet pairings
         * constructed by FacetPairing::findAllPairings() are of this form.
         *
         * @param pairing the specific pairing of simplex facets
         * that this permutation set will complement.
         */
        GluingPerms(const FacetPairing<dim>& pairing);

        /**
         * Creates a new permutation set.  All internal permutations will be
         * marked as not yet chosen, and any attempt to query them before
         * setting them to explicit values will result in undefined behaviour.
         * The corresponding permutation indices will be set to -1.
         *
         * This constructor moves the contents of the given facet pairing into
         * this object.  After it returns, \a pairing will no longer be usable.
         *
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any simplex from any other simplex via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by FacetPairing::isCanonical().  Note that all facet pairings
         * constructed by FacetPairing::findAllPairings() are of this form.
         *
         * @param pairing the specific pairing of simplex facets
         * that this permutation set will complement.
         */
        GluingPerms(FacetPairing<dim>&& pairing);

        /**
         * Creates a new copy of the given gluing permutation set.
         *
         * @param src the gluing permutations to copy.
         */
        GluingPerms(const GluingPerms<dim>& src);

        /**
         * Moves the contents of the given gluing permutation set into
         * this new gluing permutation set.
         * This is a fast (constant time) operation.
         *
         * The permutation set that is passed (\a src) will no longer be usable.
         *
         * @param src the gluing permutations to move.
         */
        GluingPerms(GluingPerms<dim>&& src) noexcept;

        /**
         * Reads a new set of gluing permutations from the given input
         * stream.  This routine reads data in the format written by
         * dumpData().
         *
         * \warning The data format is liable to change between
         * Regina releases.  Data in this format should be used on a
         * short-term temporary basis only.
         *
         * \exception InvalidInput the data found in the input stream is
         * invalid, incomplete, or incorrectly formatted.
         *
         * @param in the input stream from which to read.
         */
        GluingPerms(std::istream& in);

        /**
         * Deallocates any memory used by this structure.
         */
        ~GluingPerms();

        /**
         * Sets this to be a copy of the given gluing permutation set.
         *
         * @param src the gluing permutations to copy.
         * @return a reference to this gluing permutation set.
         */
        GluingPerms& operator = (const GluingPerms& src);

        /**
         * Moves the contents of the given gluing permutation set into
         * this new gluing permutation set.
         * This is a fast (constant time) operation.
         *
         * The permutation set that is passed (\a src) will no longer be usable.
         *
         * @param src the gluing permutations to move.
         * @return a reference to this gluing permutation set.
         */
        GluingPerms& operator = (GluingPerms&& src) noexcept;

        /**
         * Swaps the contents of this and the given gluing permutation set.
         *
         * @param other the set whose contents should be swapped with this.
         */
        void swap(GluingPerms& other) noexcept;

        /**
         * Returns the total number of simplices under consideration.
         *
         * @return the number of simplices under consideration.
         */
        unsigned size() const;

        /**
         * Returns the specific pairing of simplex facets that this
         * set of gluing permutations complements.
         *
         * @return the corresponding simplex facet pairing.
         */
        const FacetPairing<dim>& pairing() const;
        /**
         * Deprecated routine to return the facet pairing that this gluing
         * permutation set complements.
         *
         * \deprecated This routine has been renamed to pairing().
         *
         * @return the corresponding simplex facet pairing.
         */
        [[deprecated]] const FacetPairing<dim>& facetPairing() const;

        /**
         * Returns the gluing permutation associated with the given
         * simplex facet.
         *
         * \pre The given facet is actually paired with some other facet in
         * the underlying pairwise matching (see routine facetPairing()).
         * \pre The given facet is a real simplex
         * facet (not boundary, before-the-start or past-the-end).
         * \pre The requested permutation is not the special "not yet chosen"
         * value (i.e., the internal index that is stored for it is not -1).
         *
         * @param source the simplex facet under investigation.
         * @return the associated gluing permutation.
         */
        Perm<dim+1> perm(const FacetSpec<dim>& source) const;
        /**
         * Deprecated routine to return the gluing permutation
         * associated with the given simplex facet.
         *
         * \deprecated This routine has been renamed to perm().  See perm()
         * for details on the preconditions, parameters and return value.
         */
        [[deprecated]] Perm<dim+1> gluingPerm(const FacetSpec<dim>& source)
            const;

        /**
         * Returns the gluing permutation associated with the given
         * simplex facet.
         *
         * \pre The given facet is actually paired with some other facet in
         * the underlying pairwise matching (see routine facetPairing()).
         * \pre The requested permutation is not the special "not yet chosen"
         * value (i.e., the internal index that is stored for it is not -1).
         *
         * @param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * @param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * @return the associated gluing permutation.
         */
        Perm<dim+1> perm(unsigned simp, unsigned facet) const;
        /**
         * Deprecated routine to return the gluing permutation
         * associated with the given simplex facet.
         *
         * \deprecated This routine has been renamed to perm().  See perm()
         * for details on the preconditions, parameters and return value.
         */
        [[deprecated]] Perm<dim+1> gluingPerm(unsigned simp, unsigned facet)
            const;

        /**
         * Returns the index into array Perm<dim+1>::Sn_1 describing how the
         * the given facet is joined to its partner.
         *
         * Note that this is not the \a S_n index of the gluing permutation
         * on (\a dim + 1) elements, but rather the index of a permutation
         * on just \a dim elements.  You can use indexToGluing() and
         * gluingToIndex() to convert between these indices and gluing
         * permutations, or you can call perm() to read the gluing
         * permutation directly.
         *
         * As described in the class notes, this index could be a real
         * permutation index between 0 and (dim!)-1 inclusive, or it
         * could be the special value -1 indicating that the permutation
         * has not yet been chosen.
         *
         * \pre The given facet is a real simplex
         * facet (not boundary, before-the-start or past-the-end).
         *
         * @param source the simplex facet under investigation.
         * @return a reference to the corresponding array index.
         */
        const int& permIndex(const FacetSpec<dim>& source) const;

        /**
         * Returns the index into array Perm<dim+1>::Sn_1 describing how the
         * the given facet is joined to its partner.
         *
         * Note that this is not the \a S_n index of the gluing permutation
         * on (\a dim + 1) elements, but rather the index of a permutation
         * on just \a dim elements.  You can use indexToGluing() and
         * gluingToIndex() to convert between these indices and gluing
         * permutations, or you can call perm() to read the gluing
         * permutation directly.
         *
         * As described in the class notes, this index could be a real
         * permutation index between 0 and (dim!)-1 inclusive, or it
         * could be the special value -1 indicating that the permutation
         * has not yet been chosen.
         *
         * @param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * @param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * @return a reference to the corresponding array index.
         */
        const int& permIndex(unsigned simp, unsigned facet) const;

        /**
         * Offers write access to the index into array Perm<dim+1>::Sn_1
         * describing how the the given facet is joined to its partner.
         *
         * Note that this is not the \a S_n index of the gluing permutation
         * on (\a dim + 1) elements, but rather the index of a permutation
         * on just \a dim elements.  You can use indexToGluing() and
         * gluingToIndex() to convert between these indices and gluing
         * permutations, or you can call perm() for read-only access to the
         * gluing permutation directly.
         *
         * As described in the class notes, this index can be a real
         * permutation index between 0 and (dim!)-1 inclusive, or it
         * may be the special value -1 indicating that the permutation
         * has not yet been chosen.
         *
         * \pre The given facet is a real simplex
         * facet (not boundary, before-the-start or past-the-end).
         *
         * @param source the simplex facet under investigation.
         * @return a reference to the corresponding array index.
         */
        int& permIndex(const FacetSpec<dim>& source);

        /**
         * Offers write access to the index into array Perm<dim+1>::Sn_1
         * describing how the the given facet is joined to its partner.
         *
         * Note that this is not the \a S_n index of the gluing permutation
         * on (\a dim + 1) elements, but rather the index of a permutation
         * on just \a dim elements.  You can use indexToGluing() and
         * gluingToIndex() to convert between these indices and gluing
         * permutations, or you can call perm() for read-only access to the
         * gluing permutation directly.
         *
         * As described in the class notes, this index can be a real
         * permutation index between 0 and (dim!)-1 inclusive, or it
         * may be the special value -1 indicating that the permutation
         * has not yet been chosen.
         *
         * @param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * @param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * @return a reference to the corresponding array index.
         */
        int& permIndex(unsigned simp, unsigned facet);

        /**
         * Returns the triangulation modelled by this set of gluing
         * permutations and the associated simplex facet pairing.
         *
         * Each matched pair of facets and their associated permutations
         * will be realised as two simplex facets in the triangulation glued
         * together with the corresponding gluing permutation.  Each
         * unmatched facet will be realised as a boundary facet in the
         * triangulation.
         *
         * \pre For every simplex facet in the underlying facet pairing
         * that is glued to a partner, the corresponding gluing
         * permutation in this set is not the special "not yet chosen"
         * value (i.e., does not have internal index -1).
         *
         * @return the triangulation modelled by this structure.
         */
        Triangulation<dim> triangulate() const;

        /**
         * Dumps all internal data in a plain text format to the given
         * output stream.  This object can be recreated from this text
         * data by calling the input stream constructor for this class.
         *
         * This routine may be useful for transferring objects from
         * one processor to another.
         *
         * \warning The data format is liable to change between
         * Regina releases.  Data in this format should be used on a
         * short-term temporary basis only.
         *
         * @param out the output stream to which the data should be
         * written.
         */
        void dumpData(std::ostream& out) const;

        /**
         * Returns the index into array Perm<dim+1>::Sn_1 corresponding to
         * the given gluing permutation from the given facet to its
         * partner.  This need not be the index into Perm<dim+1>::Sn_1 that
         * is currently stored for the given facet.
         *
         * Indices into array Perm<dim+1>::Sn_1 are stored internally in the
         * array \a permIndices_.  Full gluing permutations on the other
         * hand are used in constructing triangulations.
         *
         * \pre The given simplex facet has a partner according to
         * the underlying facet pairing, i.e., is not a boundary facet.
         * \pre If the given simplex facet and its partner are facets
         * \a x and \a y of their respective simplices, then the
         * given gluing permutation maps \a x to \a y.
         *
         * @param source the simplex facet under investigation.
         * @param gluing a possible gluing permutation from the given
         * simplex facet to its partner according to the underlying
         * facet pairing.
         * @return the index into Perm<dim+1>::Sn_1 corresponding to the
         * given gluing permutation; this will be between 0 and \a dim!-1
         * inclusive.
         */
        int gluingToIndex(const FacetSpec<dim>& source,
            const Perm<dim+1>& gluing) const;

        /**
         * Returns the index into array Perm<dim+1>::Sn_1 corresponding to
         * the given gluing permutation from the given facet to its
         * partner.  This need not be the index into Perm<dim+1>::Sn_1 that
         * is currently stored for the given facet.
         *
         * Indices into array Perm<dim+1>::Sn_1 are stored internally in the
         * array \a permIndices_.  Full gluing permutations on the other
         * hand are used in constructing triangulations.
         *
         * \pre The given simplex facet has a partner according to
         * the underlying facet pairing, i.e., is not a boundary facet.
         * \pre If the given simplex facet and its partner are facets
         * \a x and \a y of their respective simplices, then the
         * given gluing permutation maps \a x to \a y.
         *
         * @param simp the simplex under investigation; this must be
         * strictly less than the total number of simplices under
         * consideration.
         * @param facet the facet of the given simplex under
         * investigation; this must be between 0 and \a dim inclusive.
         * @param gluing a possible gluing permutation from the given
         * simplex facet to its partner according to the underlying
         * facet pairing.
         * @return the index into Perm<dim+1>::Sn_1 corresponding to the
         * given gluing permutation; this will be between 0 and \a dim!-1
         * inclusive.
         */
        int gluingToIndex(unsigned simp, unsigned facet,
            const Perm<dim+1>& gluing) const;

        /**
         * Returns the gluing permutation from the given facet to its
         * partner that corresponds to the given index into array
         * Perm<dim+1>::Sn_1.  This index into Perm<dim+1>::Sn_1 need not
         * be the index that is currently stored for the given facet.
         *
         * Indices into array Perm<dim+1>::Sn_1 are stored internally in the
         * array \a permIndices_.  Full gluing permutations on the other
         * hand are used in constructing triangulations.
         *
         * If the given simplex facet and its partner according to
         * the underlying facet pairing are facets \a x and \a y of their
         * respective simplices, then the resulting gluing permutation
         * will map \a x to \a y.
         *
         * \pre The given simplex facet has a partner according to
         * the underlying facet pairing, i.e., is not a boundary facet.
         *
         * @param source the simplex facet under investigation.
         * @param index an index into Perm<dim+1>::Sn_1; this must be
         * between 0 and \a dim!-1 inclusive.
         * @return the gluing permutation corresponding to the given
         * index into Perm<dim+1>::Sn_1.
         */
        Perm<dim+1> indexToGluing(const FacetSpec<dim>& source, int index)
            const;

        /**
         * Returns the gluing permutation from the given facet to its
         * partner that corresponds to the given index into array
         * Perm<dim+1>::Sn_1.  This index into Perm<dim+1>::Sn_1 need not
         * be the index that is currently stored for the given facet.
         *
         * Indices into array Perm<dim+1>::Sn_1 are stored internally in the
         * array \a permIndices_.  Full gluing permutations on the other
         * hand are used in constructing triangulations.
         *
         * If the given simplex facet and its partner according to
         * the underlying facet pairing are facets \a x and \a y of their
         * respective simplices, then the resulting gluing permutation
         * will map \a x to \a y.
         *
         * \pre The given simplex facet has a partner according to
         * the underlying facet pairing, i.e., is not a boundary facet.
         *
         * @param simp the simplex under investigation; this must be
         * strictly less than the total number of simplices under
         * consideration.
         * @param facet the facet of the given simplex under
         * investigation; this must be between 0 and \a dim inclusive.
         * @param index an index into Perm<dim+1>::Sn_1; this must be
         * between 0 and \a dim!-1 inclusive.
         * @return the gluing permutation corresponding to the given
         * index into Perm<dim+1>::Sn_1.
         */
        Perm<dim+1> indexToGluing(unsigned simp, unsigned facet, int index)
            const;
};

/**
 * Swaps the contents of the given gluing permutation sets.
 *
 * This global routine simply calls GluingPerms<dim>::swap(); it is provided
 * so that GluingPerms<dim> meets the C++ Swappable requirements.
 *
 * @param a the first matrix whose contents should be swapped.
 * @param b the second matrix whose contents should be swapped.
 *
 * \ingroup census
 */
template <int dim>
void swap(GluingPerms<dim>& a, GluingPerms<dim>& b) noexcept;

// Inline functions for GluingPerms

template <int dim>
inline GluingPerms<dim>::GluingPerms(const FacetPairing<dim>& pairing) :
        pairing_(pairing), permIndices_(new int[pairing.size() * (dim + 1)]) {
    std::fill(permIndices_, permIndices_ + size() * (dim + 1), -1);
}

template <int dim>
inline GluingPerms<dim>::GluingPerms(FacetPairing<dim>&& pairing) :
        pairing_(std::move(pairing)), // note: pairing is now unusable
        permIndices_(new int[pairing_.size() * (dim + 1)]) {
    std::fill(permIndices_, permIndices_ + size() * (dim + 1), -1);
}

template <int dim>
inline GluingPerms<dim>::GluingPerms(const GluingPerms<dim>& src) :
        pairing_(src.pairing_), permIndices_(new int[src.size() * (dim + 1)]) {
    std::copy(src.permIndices_, src.permIndices_ + src.size() * (dim + 1),
        permIndices_);
}

template <int dim>
inline GluingPerms<dim>::GluingPerms(GluingPerms<dim>&& src) noexcept :
        pairing_(std::move(src.pairing_)), permIndices_(src.permIndices_) {
    src.permIndices_ = nullptr;
}

template <int dim>
inline GluingPerms<dim>& GluingPerms<dim>::operator = (const GluingPerms& src) {
    // std::copy() exhibits undefined behaviour in the case of self-assignment.
    if (std::addressof(src) == this)
        return *this;

    if (size() != src.size()) {
        delete[] permIndices_;
        permIndices_ = new int[src.size() * (dim + 1)];
    }
    pairing_ = src.pairing_;
    std::copy(src.permIndices_, src.permIndices_ + src.size() * (dim + 1),
        permIndices_);
    return *this;
}

template <int dim>
inline GluingPerms<dim>& GluingPerms<dim>::operator = (GluingPerms&& src)
        noexcept {
    pairing_ = std::move(src.pairing_);
    std::swap(permIndices_, src.permIndices_);
    // Let src dispose of the original permutations in its own destructor.
    return *this;
}

template <int dim>
inline GluingPerms<dim>::~GluingPerms() {
    delete[] permIndices_;
}

template <int dim>
inline void GluingPerms<dim>::swap(GluingPerms& other) noexcept {
    pairing_.swap(other.pairing_);
    std::swap(permIndices_, other.permIndices_);
}

template <int dim>
inline unsigned GluingPerms<dim>::size() const {
    return pairing_.size();
}

template <int dim>
inline const FacetPairing<dim>& GluingPerms<dim>::pairing() const {
    return pairing_;
}

template <int dim>
inline const FacetPairing<dim>& GluingPerms<dim>::facetPairing() const {
    return pairing_;
}

template <int dim>
inline Perm<dim+1> GluingPerms<dim>::perm(const FacetSpec<dim>& source) const {
    return indexToGluing(source, permIndex(source));
}

template <int dim>
inline Perm<dim+1> GluingPerms<dim>::gluingPerm(
        const FacetSpec<dim>& source) const {
    return indexToGluing(source, permIndex(source));
}

template <int dim>
inline Perm<dim+1> GluingPerms<dim>::perm(unsigned simp, unsigned facet) const {
    return indexToGluing(simp, facet, permIndex(simp, facet));
}

template <int dim>
inline Perm<dim+1> GluingPerms<dim>::gluingPerm(
        unsigned simp, unsigned facet) const {
    return indexToGluing(simp, facet, permIndex(simp, facet));
}

template <int dim>
inline int& GluingPerms<dim>::permIndex(const FacetSpec<dim>& source) {
    return permIndices_[(dim + 1) * source.simp + source.facet];
}

template <int dim>
inline int& GluingPerms<dim>::permIndex(unsigned simp, unsigned facet) {
    return permIndices_[(dim + 1) * simp + facet];
}

template <int dim>
inline const int& GluingPerms<dim>::permIndex(
        const FacetSpec<dim>& source) const {
    return permIndices_[(dim + 1) * source.simp + source.facet];
}

template <int dim>
inline const int& GluingPerms<dim>::permIndex(
        unsigned simp, unsigned facet) const {
    return permIndices_[(dim + 1) * simp + facet];
}

template <int dim>
inline Perm<dim+1> GluingPerms<dim>::indexToGluing(
        const FacetSpec<dim>& source, int index) const {
    if constexpr (standardDim(dim)) {
        return Perm<dim+1>(pairing_.dest(source).facet, dim) *
            Perm<dim+1>::Sn_1[index] * Perm<dim+1>(source.facet, dim);
    } else {
        // The fast Sn_1 lookup is only provided in standard dimensions.
        return Perm<dim+1>(pairing_.dest(source).facet, dim) *
            Perm<dim+1>::extend(Perm<dim>::Sn[index]) *
            Perm<dim+1>(source.facet, dim);
    }
}

template <int dim>
inline Perm<dim+1> GluingPerms<dim>::indexToGluing(
        unsigned simp, unsigned facet, int index) const {
    if constexpr (standardDim(dim)) {
        return Perm<dim+1>(pairing_.dest(simp, facet).facet, dim) *
            Perm<dim+1>::Sn_1[index] * Perm<dim+1>(facet, dim);
    } else {
        // The fast Sn_1 lookup is only provided in standard dimensions.
        return Perm<dim+1>(pairing_.dest(simp, facet).facet, dim) *
            Perm<dim+1>::extend(Perm<dim>::Sn[index]) *
            Perm<dim+1>(facet, dim);
    }
}

template <int dim>
inline void swap(GluingPerms<dim>& a, GluingPerms<dim>& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

