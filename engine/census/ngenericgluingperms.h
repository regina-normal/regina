
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

/*! \file census/ngenericgluingperms.h
 *  \brief Deals with selecting gluing permutations to complement a
 *  particular pairing of facets of simplices in an n-manifold triangulation.
 */

#ifndef __NGENERICGLUINGPERMS_H
#ifndef __DOXYGEN
#define __NGENERICGLUINGPERMS_H
#endif

#include "regina-core.h"
#include "triangulation/dimtraits.h"

namespace regina {

/**
 * \weakgroup census
 * @{
 */

/**
 * A dimension-agnostic base class that represents a specific set
 * of gluing permutations to complement a particular pairwise matching of
 * simplex facets.  Each dimension that Regina works with (2, 3 and 4)
 * offers its own subclass, in some cases with richer functionality.
 * Users should not need to work with this template base class directly.
 *
 * Given a pairwise
 * matching of facets (as described by class NGenericFacetPairing<dim>),
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
 * \pre The dimension argument \a dim is either 2, 3 or 4.
 *
 * \ifacespython Not present.
 */
template <int dim>
class NGenericGluingPerms {
    public:
        typedef typename DimTraits<dim>::FacetPairing FacetPairing;
        typedef typename DimTraits<dim>::Perm Perm;
        typedef typename DimTraits<dim>::Simplex Simplex;
        typedef typename DimTraits<dim>::Triangulation Triangulation;
    protected:
        const FacetPairing* pairing_;
            /**< The facet pairing that this permutation set complements.
                 This is guaranteed to be the minimal representative of
                 its facet pairing isomorphism class. */
        int* permIndices_;
            /**< The index into array Perm::Sn_1 describing how each
                 simplex facet is glued to its partner.  Note that this
                 is not a gluing permutation as such but rather a permutation
                 of 0,...,\a dim-1 only (see the routines gluingToIndex() and
                 indexToGluing() for conversions).  If a permutation has not
                 yet been selected (e.g., if this permutation set is still
                 under construction) then this index is -1. */

        bool inputError_;
            /**< Has an error occurred during construction from an
                 input stream? */

    public:
        /**
         * Creates a new set of gluing permutations that is a clone of
         * the given permutation set.
         *
         * @param cloneMe the gluing permutations to clone.
         */
        NGenericGluingPerms(const NGenericGluingPerms<dim>& cloneMe);

        /**
         * Reads a new set of gluing permutations from the given input
         * stream.  This routine reads data in the format written by
         * dumpData().
         *
         * If the data found in the input stream is invalid or
         * incorrectly formatted, the routine inputError() will return
         * \c true but the contents of this object will be otherwise
         * undefined.
         *
         * \warning The data format is liable to change between
         * Regina releases.  Data in this format should be used on a
         * short-term temporary basis only.
         *
         * @param in the input stream from which to read.
         */
        NGenericGluingPerms(std::istream& in);

        /**
         * Deallocates any memory used by this structure.
         */
        virtual ~NGenericGluingPerms();

        /**
         * Was an error found during construction from an input stream?
         *
         * This routine returns \c true if an input stream constructor was
         * used to create this object but the data in the input stream
         * was invalid or incorrectly formatted.
         *
         * If a different constructor was called (i.e., no input stream
         * was used), then this routine will always return \c false.
         *
         * @return \c true if an error occurred during construction from
         * an input stream, or \c false otherwise.
         */
        bool inputError() const;

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
        const FacetPairing* getFacetPairing() const;

        /**
         * Returns the gluing permutation associated with the given
         * simplex facet.
         *
         * \pre The given facet is actually paired with some other facet in
         * the underlying pairwise matching (see routine getFacetPairing()).
         * \pre The given facet is a real simplex
         * facet (not boundary, before-the-start or past-the-end).
         *
         * @param source the simplex facet under investigation.
         * @return the associated gluing permutation.
         */
        Perm gluingPerm(const NFacetSpec<dim>& source) const;

        /**
         * Returns the gluing permutation associated with the given
         * simplex facet.
         *
         * \pre The given facet is actually paired with some other facet in
         * the underlying pairwise matching (see routine getFacetPairing()).
         *
         * @param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * @param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * @return the associated gluing permutation.
         */
        Perm gluingPerm(unsigned simp, unsigned facet) const;

        /**
         * Returns a newly created triangulation as modelled by this set
         * of gluing permutations and the associated simplex facet
         * pairing.
         *
         * Each matched pair of facets and their associated permutations
         * will be realised as two simplex facets in the triangulation glued
         * together with the corresponding gluing permutation.  Each
         * unmatched facet will be realised as a boundary facet in the
         * triangulation.
         *
         * It is the responsibility of the caller of this routine to
         * delete this triangulation once it is no longer required.
         *
         * @return a newly created triangulation modelled by this structure.
         */
        Triangulation* triangulate() const;

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
         * \warning The data format is liable to change between
         * Regina releases.  Data in this format should be used on a
         * short-term temporary basis only.
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
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any simplex from any other simplex via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by NGenericFacetPairing::isCanonical().  Note that all facet pairings
         * constructed by NGenericFacetPairing::findAllPairings() are of this
         * form.
         *
         * @param pairing the specific pairing of simplex facets
         * that this permutation set will complement.
         */
        NGenericGluingPerms(const FacetPairing* pairing);

        /**
         * Returns the index into array Perm::Sn_1 describing how the
         * the given facet is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0,...,\a dim-1 only.  For a real facet
         * gluing permutation, see routine gluingPerm().
         *
         * \pre The given facet is a real simplex
         * facet (not boundary, before-the-start or past-the-end).
         *
         * @param source the simplex facet under investigation.
         * @return a reference to the corresponding array index.
         */
        int& permIndex(const NFacetSpec<dim>& source);

        /**
         * Returns the index into array Perm::Sn_1 describing how the
         * the given facet is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0,...,\a dim-1 only.  For a real facet
         * gluing permutation, see routine gluingPerm().
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
         * Returns the index into array Perm::Sn_1 describing how the
         * the given facet is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0,...,\a dim-1 only.  For a real facet
         * gluing permutation, see routine gluingPerm().
         *
         * \pre The given facet is a real simplex
         * facet (not boundary, before-the-start or past-the-end).
         *
         * @param source the simplex facet under investigation.
         * @return a reference to the corresponding array index.
         */
        const int& permIndex(const NFacetSpec<dim>& source) const;

        /**
         * Returns the index into array Perm::Sn_1 describing how the
         * the given facet is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0,...,\a dim-1 only.  For a real facet
         * gluing permutation, see routine gluingPerm().
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
         * Returns the index into array Perm::Sn_1 corresponding to
         * the given gluing permutation from the given facet to its
         * partner.  This need not be the index into Perm::Sn_1 that
         * is currently stored for the given facet.
         *
         * Indices into array Perm::Sn_1 are stored internally in the
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
         * @return the index into Perm::Sn_1 corresponding to the
         * given gluing permutation; this will be between 0 and \a dim!-1
         * inclusive.
         */
        int gluingToIndex(const NFacetSpec<dim>& source, const Perm& gluing)
            const;

        /**
         * Returns the index into array Perm::Sn_1 corresponding to
         * the given gluing permutation from the given facet to its
         * partner.  This need not be the index into Perm::Sn_1 that
         * is currently stored for the given facet.
         *
         * Indices into array Perm::Sn_1 are stored internally in the
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
         * @return the index into Perm::Sn_1 corresponding to the
         * given gluing permutation; this will be between 0 and \a dim!-1
         * inclusive.
         */
        int gluingToIndex(unsigned simp, unsigned facet, const Perm& gluing)
            const;

        /**
         * Returns the gluing permutation from the given facet to its
         * partner that corresponds to the given index into array
         * Perm::Sn_1.  This index into Perm::Sn_1 need not
         * be the index that is currently stored for the given facet.
         *
         * Indices into array Perm::Sn_1 are stored internally in the
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
         * @param index an index into Perm::Sn_1; this must be
         * between 0 and \a dim!-1 inclusive.
         * @return the gluing permutation corresponding to the given
         * index into Perm::Sn_1.
         */
        Perm indexToGluing(const NFacetSpec<dim>& source, int index) const;

        /**
         * Returns the gluing permutation from the given facet to its
         * partner that corresponds to the given index into array
         * Perm::Sn_1.  This index into Perm::Sn_1 need not
         * be the index that is currently stored for the given facet.
         *
         * Indices into array Perm::Sn_1 are stored internally in the
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
         * @param index an index into Perm::Sn_1; this must be
         * between 0 and \a dim!-1 inclusive.
         * @return the gluing permutation corresponding to the given
         * index into Perm::Sn_1.
         */
        Perm indexToGluing(unsigned simp, unsigned facet, int index) const;
};

/*@}*/

// Inline functions for NGenericGluingPerms

template <int dim>
inline NGenericGluingPerms<dim>::NGenericGluingPerms(
        const typename NGenericGluingPerms<dim>::FacetPairing* pairing) :
        pairing_(pairing),
        permIndices_(new int[pairing->size() * (dim + 1)]),
        inputError_(false) {
}

template <int dim>
inline NGenericGluingPerms<dim>::~NGenericGluingPerms() {
    delete[] permIndices_;
}

template <int dim>
inline bool NGenericGluingPerms<dim>::inputError() const {
    return inputError_;
}

template <int dim>
inline unsigned NGenericGluingPerms<dim>::size() const {
    return pairing_->size();
}

template <int dim>
inline const typename NGenericGluingPerms<dim>::FacetPairing*
        NGenericGluingPerms<dim>::getFacetPairing() const {
    return pairing_;
}

template <int dim>
inline typename NGenericGluingPerms<dim>::Perm
        NGenericGluingPerms<dim>::gluingPerm(const NFacetSpec<dim>& source)
        const {
    return indexToGluing(source, permIndex(source));
}

template <int dim>
inline typename NGenericGluingPerms<dim>::Perm
        NGenericGluingPerms<dim>::gluingPerm(
        unsigned simp, unsigned facet) const {
    return indexToGluing(simp, facet, permIndex(simp, facet));
}

template <int dim>
inline int& NGenericGluingPerms<dim>::permIndex(const NFacetSpec<dim>& source) {
    return permIndices_[(dim + 1) * source.simp + source.facet];
}

template <int dim>
inline int& NGenericGluingPerms<dim>::permIndex(unsigned simp, unsigned facet) {
    return permIndices_[(dim + 1) * simp + facet];
}

template <int dim>
inline const int& NGenericGluingPerms<dim>::permIndex(
        const NFacetSpec<dim>& source) const {
    return permIndices_[(dim + 1) * source.simp + source.facet];
}

template <int dim>
inline const int& NGenericGluingPerms<dim>::permIndex(
        unsigned simp, unsigned facet) const {
    return permIndices_[(dim + 1) * simp + facet];
}

template <int dim>
inline typename NGenericGluingPerms<dim>::Perm
        NGenericGluingPerms<dim>::indexToGluing(
        const NFacetSpec<dim>& source, int index) const {
    return Perm(pairing_->dest(source).facet, dim) *
        Perm::Sn_1[index] * Perm(source.facet, dim);
}

template <int dim>
inline typename NGenericGluingPerms<dim>::Perm
        NGenericGluingPerms<dim>::indexToGluing(
        unsigned simp, unsigned facet, int index) const {
    return Perm(pairing_->dest(simp, facet).facet, dim) *
        Perm::Sn_1[index] * Perm(facet, dim);
}

} // namespace regina

#endif

