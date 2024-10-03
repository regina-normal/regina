
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file triangulation/facetspec.h
 *  \brief Allows lightweight representation of individual facets of
 *  simplices.
 */

#ifndef __REGINA_FACETSPEC_H
#ifndef __DOXYGEN
#define __REGINA_FACETSPEC_H
#endif

#include "utilities/tightencoding.h"
#include <cstddef>
#include <iostream>
#include <sys/types.h> // for ssize_t

namespace regina {

/**
 * A lightweight class used to refer to a particular facet of a particular
 * top-dimensional simplex in a <i>dim</i>-dimensional triangulation.
 * Only the simplex index and the facet number are stored.
 *
 * Facilities are provided for iterating through simplex facets.
 * With this in mind, it is also possible to represent the overall
 * boundary, a past-the-end value and a before-the-start value.
 *
 * When iterating through the simplex facets, the facets will be
 * ordered first by simplex index and then by facet number.  The
 * overall boundary appears after all other simplex facets.
 *
 * If there are \a n simplices, the simplices will be numbered from 0
 * to <i>n</i>-1 inclusive.  The boundary will be represented as
 * simplex \a n, facet 0.  The past-the-end value will be represented
 * as simplex \a n, facet 1, and the before-the-start value will be
 * represented as simplex -1, facet \a dim.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \python Python does not support templates.  Instead this class
 * can be used by appending the dimension as a suffix (e.g., FacetSpec2
 * and FacetSpec3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \ingroup triangulation
 */
template <int dim>
struct FacetSpec : public TightEncodable<FacetSpec<dim>> {
    ssize_t simp;
        /**< The simplex referred to.  Simplex numbering begins
         *   at 0. */
    int facet;
        /**< The facet of the simplex referred to.  The facet number
         *   is between 0 and \a dim inclusive. */

    /**
     * Creates a new specifier with no initialisation.  This
     * specifier must be initialised before it is used.
     */
    FacetSpec() = default;
    /**
     * Creates a new specifier referring to the given facet of the given
     * simplex.
     *
     * \param newSimp the given simplex; see the class notes for
     * allowable values of this parameter.
     * \param newFacet the given facet; this should be between 0 and
     * \a dim inclusive.
     */
    FacetSpec(ssize_t newSimp, int newFacet);
    /**
     * Creates a new specifier referring to the same simplex facet as
     * the given specifier.
     *
     * \param cloneMe the specifier to clone.
     */
    FacetSpec(const FacetSpec<dim>& cloneMe) = default;

    /**
     * Determines if this specifier represents the overall boundary.
     *
     * \param nSimplices the number of simplices under consideration.
     * Note that the boundary is represented in this specifier as
     * simplex \a nSimplices, facet 0.
     * \return \c true if and only if this specifier represents the
     * overall boundary.
     */
    bool isBoundary(size_t nSimplices) const;
    /**
     * Determines if this specifier represents a before-the-start value.
     *
     * \return \c true if and only if this specifier is before-the-start.
     */
    bool isBeforeStart() const;
    /**
     * Determines if this specifier represents a past-the-end value.
     * You can optionally declare the overall boundary to be
     * past-the-end as well as the already predefined past-the-end value.
     *
     * \param nSimplices the number of simplices under consideration.
     * Note that past-the-end is represented in this specifier as
     * simplex \a nSimplices, facet 1.
     * \param boundaryAlso \c true if the overall boundary should be
     * considered past-the-end in addition to the predefined past-the-end
     * value.
     * \return \c true if and only if this specifier is past-the-end.
     */
    bool isPastEnd(size_t nSimplices, bool boundaryAlso) const;

    /**
     * Sets this specifier to the first facet of the first simplex.
     */
    void setFirst();
    /**
     * Sets this specifier to the overall boundary.
     *
     * \param nSimplices the number of simplices under consideration.
     * Note that the boundary is represented in this specifier as
     * simplex \a nSimplices, facet 0.
     */
    void setBoundary(size_t nSimplices);
    /**
     * Sets this specifier to before-the-start.
     */
    void setBeforeStart();
    /**
     * Sets this specifier to past-the-end.
     *
     * \param nSimplices the number of simplices under consideration.
     * Note that past-the-end is represented in this specifier as
     * simplex \a nSimplices, facet 1.
     */
    void setPastEnd(size_t nSimplices);

    /**
     * Sets this specifier to the value of the given specifier.
     *
     * \param other the given specifier.
     * \return a reference to this specifier.
     */
    FacetSpec& operator = (const FacetSpec<dim>& other) = default;
    /**
     * Increments this specifier.  It will be changed to point to the
     * next simplex facet.
     *
     * Facets are ordered first by simplex index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not past-the-end.
     *
     * \nopython The postincrement operator is present in Python as the
     * member function inc().
     *
     * \return A reference to this specifier.
     */
    FacetSpec& operator ++ ();
    /**
     * Increments this specifier.  It will be changed to point to the
     * next simplex facet.
     *
     * Facets are ordered first by simplex index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not past-the-end.
     *
     * \python This routine is named inc() since python does not
     * support the increment operator.
     *
     * \return A copy of this specifier before it was incremented.
     */
    FacetSpec operator ++ (int);
    /**
     * Decrements this specifier.  It will be changed to point to the
     * previous simplex facet.
     *
     * Facets are ordered first by simplex index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not before-the-start.
     *
     * \nopython The postdecrement operator is present in Python as the
     * member function dec().
     *
     * \return A reference to this specifier.
     */
    FacetSpec& operator -- ();
    /**
     * Decrements this specifier.  It will be changed to point to the
     * previous simplex facet.
     *
     * Facets are ordered first by simplex index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not before-the-start.
     *
     * \python This routine is named dec() since python does not
     * support the decrement operator.
     *
     * \return A copy of this specifier before it was decremented.
     */
    FacetSpec operator -- (int);

    /**
     * Determines if this and the given specifier are identical.
     *
     * \param rhs the specifier to compare with this.
     * \return \c true if and only if this and the given specifier are
     * equal.
     */
    bool operator == (const FacetSpec<dim>& rhs) const;
    /**
     * Compares two specifiers, following the same ordering that is used for
     * iteration.
     *
     * Specifically: specifiers are compared first by simplex number and then
     * by facet number.  The overall boundary is considered greater than any
     * "real" simplex facet, the before-the-start value is considered smaller
     * than anything else, and the past-the-end value is considered greater
     * than anything else.
     *
     * This generates all of the usual comparison operators, including
     * `<`, `<=`, `>`, and `>=`.
     *
     * \python This spaceship operator `x <=> y` is not available, but the
     * other comparison operators that it generates _are_ available.
     *
     * \param rhs the specifier to compare with this.
     * \return The result of the comparison between this and the given
     * specifier.
     */
    std::strong_ordering operator <=> (const FacetSpec<dim>& rhs) const;

    /**
     * Writes the tight encoding of this specifier to the given output
     * stream.  See the page on \ref tight "tight encodings" for details.
     *
     * Before-the-start, past-the-end and boundary specifiers can all be
     * safely encoded.
     *
     * \nopython Use tightEncoding() instead, which returns a string.
     *
     * \param out the output stream to which the encoded string will be written.
     */
    void tightEncode(std::ostream& out) const;

    /**
     * Reconstructs a specifier from its given tight encoding.
     * See the page on \ref tight "tight encodings" for details.
     *
     * The tight encoding will be read from the given input stream.
     * If the input stream contains leading whitespace then it will be
     * treated as an invalid encoding (i.e., this routine will throw an
     * exception).  The input stream _may_ contain further data: if this
     * routine is successful then the input stream will be left positioned
     * immediately after the encoding, without skipping any trailing
     * whitespace.
     *
     * Before-the-start, past-the-end and boundary specifiers can all be
     * safely reconstructed.
     *
     * \exception InvalidInput The given input stream does not begin with
     * a tight encoding of a <i>dim</i>-dimensional facet specifier.
     *
     * \nopython Use tightDecoding() instead, which takes a string as
     * its argument.
     *
     * \param input an input stream that begins with the tight encoding
     * for a <i>dim</i>-dimensional facet specifier.
     * \return the specifier represented by the given tight encoding.
     */
    static FacetSpec<dim> tightDecode(std::istream& input);
};

/**
 * Writes the given facet specifier to the given output stream.
 *
 * \param out the output stream to which to write.
 * \param spec the specifier to write.
 * \return a reference to \a out.
 *
 * \ingroup triangulation
 */
template <int dim>
std::ostream& operator << (std::ostream& out, const FacetSpec<dim>& spec);

// Inline functions for FacetSpec

template <int dim>
inline FacetSpec<dim>::FacetSpec(ssize_t newSimp, int newFacet) :
        simp(newSimp), facet(newFacet) {
}

template <int dim>
inline bool FacetSpec<dim>::isBoundary(size_t nSimplices) const {
    return (simp == static_cast<ssize_t>(nSimplices) && facet == 0);
}

template <int dim>
inline bool FacetSpec<dim>::isBeforeStart() const {
    return (simp < 0);
}

template <int dim>
inline bool FacetSpec<dim>::isPastEnd(size_t nSimplices, bool boundaryAlso)
        const {
    return (simp == static_cast<ssize_t>(nSimplices) &&
        (boundaryAlso || facet > 0));
}

template <int dim>
inline void FacetSpec<dim>::setFirst() {
    simp = facet = 0;
}

template <int dim>
inline void FacetSpec<dim>::setBoundary(size_t nSimplices) {
    simp = nSimplices;
    facet = 0;
}

template <int dim>
inline void FacetSpec<dim>::setBeforeStart() {
    simp = -1;
    facet = dim;
}

template <int dim>
inline void FacetSpec<dim>::setPastEnd(size_t nSimplices) {
    simp = nSimplices;
    facet = 1;
}

template <int dim>
inline FacetSpec<dim>& FacetSpec<dim>::operator ++ () {
    if (++facet > dim) {
        facet = 0;
        ++simp;
    }
    return *this;
}

template <int dim>
inline FacetSpec<dim> FacetSpec<dim>::operator ++ (int) {
    FacetSpec<dim> ans(*this);
    if (++facet > dim) {
        facet = 0;
        ++simp;
    }
    return ans;
}

template <int dim>
inline FacetSpec<dim>& FacetSpec<dim>::operator -- () {
    if (--facet < 0) {
        facet = dim;
        --simp;
    }
    return *this;
}

template <int dim>
inline FacetSpec<dim> FacetSpec<dim>::operator -- (int) {
    FacetSpec<dim> ans(*this);
    if (--facet < 0) {
        facet = dim;
        --simp;
    }
    return ans;
}

template <int dim>
inline bool FacetSpec<dim>::operator == (const FacetSpec& rhs) const {
    return (simp == rhs.simp && facet == rhs.facet);
}

template <int dim>
inline std::strong_ordering FacetSpec<dim>::operator <=> (const FacetSpec& rhs)
        const {
    if (simp < rhs.simp)
        return std::strong_ordering::less;
    else if (simp > rhs.simp)
        return std::strong_ordering::greater;
    else
        return facet <=> rhs.facet;
}

template <int dim>
inline std::ostream& operator << (std::ostream& out,
        const FacetSpec<dim>& spec) {
    return out << spec.simp << ':' << spec.facet;
}

template <int dim>
inline void FacetSpec<dim>::tightEncode(std::ostream& out) const {
    ssize_t enc = (simp < 0 ? -1 : simp * (dim + 1) + facet);
    regina::detail::tightEncodeIndex(out, enc);
}

template <int dim>
inline FacetSpec<dim> FacetSpec<dim>::tightDecode(std::istream& input) {
    auto enc = regina::detail::tightDecodeIndex<ssize_t>(input);
    if (enc < 0)
        return FacetSpec(-1, dim);
    else
        return FacetSpec(enc / (dim + 1), enc % (dim + 1));
}

} // namespace regina

#endif

