
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

#ifndef __REGINA_FACETPAIRING_H_GENERIC
#ifndef __DOXYGEN
#define __REGINA_FACETPAIRING_H_GENERIC
#endif

/*! \file triangulation/generic/facetpairing.h
 *  \brief Internal header for dual graphs of triangulations of
 *  arbitrary dimension.
 *
 *  This file is automatically included from triangulation/facetpairing.h;
 *  there is no need for end users to include this header explicitly.
 */

#include "triangulation/detail/facetpairing.h"

namespace regina {

/**
 * Represents the dual graph of a <i>dim</i>-manifold triangulation;
 * that is, the pairwise matching of facets of <i>dim</i>-dimensional simplices.
 *
 * Given a fixed number of <i>dim</i>-dimensional simplices,
 * each facet of each simplex is either paired with some other simplex facet
 * (which is in turn paired with it) or remains unmatched.
 * A simplex facet cannot be paired with itself.
 *
 * Such a matching models part of the structure of a <i>dim</i>-manifold
 * triangulation, in which each simplex facet is either glued to some
 * other simplex facet (which is in turn glued to it) or is an unglued
 * boundary facet.
 *
 * Note that if this pairing is used to construct an actual
 * triangulation, the individual gluing permutations will still need to
 * be specified; they are not a part of this structure.
 *
 * For dimension 3, this template is specialised and offers more functionality.
 * In order to use this specialised class, you will need to include the
 * corresponding header triangulation/facetpairing3.h.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ifacespython Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., FacetPairing2 and FacetPairing3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \headerfile triangulation/generic.h
 *
 * \ingroup generic
 */
template <int dim>
class FacetPairing : public detail::FacetPairingBase<dim> {
    static_assert(dim != 3,
        "The generic implementation of FacetPairing<dim> "
        "should not be used for dimension 3.");

    public:
        /**
         * Creates a new copy of the given facet pairing.
         *
         * @param src the facet pairing to clone.
         */
        FacetPairing(const FacetPairing& src) = default;

        /**
         * Moves the given facet pairing into this facet pairing.
         * This is a fast (constant time) operation.
         *
         * The facet pairing that is passed (\a src) will no longer be usable.
         *
         * @param src the facet pairing to move.
         */
        FacetPairing(FacetPairing&& src) noexcept = default;

        /**
         * Creates the dual graph of the given triangulation.
         * This is the facet pairing that describes how the facets of
         * simplices in the given triangulation are joined together, as
         * described in the class notes.
         *
         * \pre The given triangulation is not empty.
         *
         * @param tri the triangulation whose facet pairing should
         * be constructed.
         */
        FacetPairing(const Triangulation<dim>& tri);

        /**
         * Reads a new facet pairing from the given input stream.  This
         * routine reads data in the format written by toTextRep().
         *
         * This routine will skip any initial whitespace in the given input
         * stream.  Once it finds its first non-whitespace character,
         * it will read the \e entire line from the input stream and expect
         * that line to containin the text representation of a facet pairing.
         *
         * \exception InvalidInput the data found in the input stream is
         * invalid, incomplete, or incorrectly formatted.
         *
         * \ifacespython Not present; instead you can use fromTextRep(), which
         * reads this same text format in string form.  The main differences
         * between this constructor and fromTextRep() are: (i) fromTextRep()
         * does not skip over initial whitespace; and (ii) fromTextRep()
         * throws InvalidArgument exceptions on error (not InvalidInput).
         *
         * @param in the input stream from which to read.
         */
        FacetPairing(std::istream& in);

        /**
         * Copies the given facet pairing into this facet pairing.
         *
         * It does not matter if this and the given facet pairing use
         * different numbers of top-dimensional simpilices; if they do
         * then this facet pairing will be resized accordingly.
         *
         * This operator induces a deep copy of \a src.
         *
         * @param src the facet pairing to copy.
         * @return a reference to this facet pairing.
         */
        FacetPairing& operator = (const FacetPairing& src) = default;

        /**
         * Moves the given facet pairing into this facet pairing.
         * This is a fast (constant time) operation.
         *
         * It does not matter if this and the given facet pairing use
         * different numbers of top-dimensional simpilices; if they do
         * then this facet pairing will be resized accordingly.
         *
         * The facet pairing that is passed (\a src) will no longer be usable.
         *
         * @param src the facet pairing to move.
         * @return a reference to this facet pairing.
         */
        FacetPairing& operator = (FacetPairing&& src) noexcept = default;

    private:
        /**
         * Creates a new facet pairing.
         * All internal arrays will be allocated but not initialised.
         *
         * \pre \a size is at least 1.
         *
         * @param size the number of top-dimensional simplicies under
         * consideration in this new facet pairing.
         */
        FacetPairing(size_t size);

    // Make sure the parent class can call the private constructor.
    friend class detail::FacetPairingBase<dim>;
};

/**
 * Swaps the contents of the given facet pairings.
 *
 * This global routine simply calls FacetPairing<dim>::swap(); it is provided
 * so that FacetPairing<dim> meets the C++ Swappable requirements.
 *
 * @param a the first facet pairing whose contents should be swapped.
 * @param b the second facet pairing whose contents should be swapped.
 *
 * \ingroup generic
 */
template <int dim>
void swap(FacetPairing<dim>& a, FacetPairing<dim>& b) noexcept;

// Inline functions for FacetPairing

template <int dim>
inline FacetPairing<dim>::FacetPairing(const Triangulation<dim>& tri) :
        detail::FacetPairingBase<dim>(tri) {
}

template <int dim>
inline FacetPairing<dim>::FacetPairing(std::istream& in) :
        detail::FacetPairingBase<dim>(in) {
}

template <int dim>
inline FacetPairing<dim>::FacetPairing(size_t size) :
        detail::FacetPairingBase<dim>(size) {
}

template <int dim>
inline void swap(FacetPairing<dim>& a, FacetPairing<dim>& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

