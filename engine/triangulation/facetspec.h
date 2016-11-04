
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#ifndef __NFACETSPEC_H
#ifndef __DOXYGEN
#define __NFACETSPEC_H
#endif

#include "regina-core.h"
#include <cstddef>

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

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
 * \ifacespython Python does not support templates.  Instead this class
 * can be used by appending the dimension as a suffix (e.g., FacetSpec2
 * and FacetSpec3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
struct FacetSpec {
    int simp;
        /**< The simplex referred to.  Simplex numbering begins
         *   at 0. */
    int facet;
        /**< The facet of the simplex referred to.  The facet number
         *   is between 0 and \a dim inclusive. */

    /**
     * Creates a new specifier with no initialisation.  This
     * specifier must be initialised before it is used.
     */
    FacetSpec();
    /**
     * Creates a new specifier referring to the given facet of the given
     * simplex.
     *
     * @param newSimp the given simplex; see the class notes for
     * allowable values of this parameter.
     * @param newFacet the given facet; this should be between 0 and
     * \a dim inclusive.
     */
    FacetSpec(int newSimp, int newFacet);
    /**
     * Creates a new specifier referring to the same simplex facet as
     * the given specifier.
     *
     * @param cloneMe the specifier to clone.
     */
    FacetSpec(const FacetSpec<dim>& cloneMe);

    /**
     * Determines if this specifier represents the overall boundary.
     *
     * @param nSimplices the number of simplices under consideration.
     * Note that the boundary is represented in this specifier as
     * simplex \a nSimplices, facet 0.
     * @return \c true if and only if this specifier represents the
     * overall boundary.
     */
    bool isBoundary(size_t nSimplices) const;
    /**
     * Determines if this specifier represents a before-the-start value.
     *
     * @return \c true if and only if this specifier is before-the-start.
     */
    bool isBeforeStart() const;
    /**
     * Determines if this specifier represents a past-the-end value.
     * You can optionally declare the overall boundary to be
     * past-the-end as well as the already predefined past-the-end value.
     *
     * @param nSimplices the number of simplices under consideration.
     * Note that past-the-end is represented in this specifier as
     * simplex \a nSimplices, facet 1.
     * @param boundaryAlso \c true if the overall boundary should be
     * considered past-the-end in addition to the predefined past-the-end
     * value.
     * @return \c true if and only if this specifier is past-the-end.
     */
    bool isPastEnd(size_t nSimplices, bool boundaryAlso) const;

    /**
     * Sets this specifier to the first facet of the first simplex.
     */
    void setFirst();
    /**
     * Sets this specifier to the overall boundary.
     *
     * @param nSimplices the number of simplices under consideration.
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
     * @param nSimplices the number of simplices under consideration.
     * Note that past-the-end is represented in this specifier as
     * simplex \a nSimplices, facet 1.
     */
    void setPastEnd(size_t nSimplices);

    /**
     * Sets this specifier to the value of the given specifier.
     *
     * @param other the given specifier.
     * @return a reference to this specifier.
     */
    FacetSpec& operator = (const FacetSpec<dim>& other);
    /**
     * Increments this specifier.  It will be changed to point to the
     * next simplex facet.
     *
     * Facets are ordered first by simplex index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not past-the-end.
     *
     * \ifacespython Not present, although the preincrement operator is
     * present in python as the member function inc().
     *
     * @return A copy of this specifier after it has been incremented.
     */
    FacetSpec operator ++ ();
    /**
     * Increments this specifier.  It will be changed to point to the
     * next simplex facet.
     *
     * Facets are ordered first by simplex index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not past-the-end.
     *
     * \ifacespython This routine is named inc() since python does not
     * support the increment operator.
     *
     * @return A copy of this specifier before it was incremented.
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
     * \ifacespython Not present, although the predecrement operator is
     * present in python as the member function dec().
     *
     * @return A copy of this specifier after it has been decremented.
     */
    FacetSpec operator -- ();
    /**
     * Decrements this specifier.  It will be changed to point to the
     * previous simplex facet.
     *
     * Facets are ordered first by simplex index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not before-the-start.
     *
     * \ifacespython This routine is named dec() since python does not
     * support the decrement operator.
     *
     * @return A copy of this specifier before it was decremented.
     */
    FacetSpec operator -- (int);

    /**
     * Determines if this and the given specifier are identical.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this and the given specifier are
     * equal.
     */
    bool operator == (const FacetSpec<dim>& other) const;
    /**
     * Determines if this and the given specifier are not identical.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this and the given specifier are
     * not equal.
     */
    bool operator != (const FacetSpec<dim>& other) const;
    /**
     * Determines if this is less than the given specifier.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this is less than the given
     * specifier.
     */
    bool operator < (const FacetSpec<dim>& other) const;
    /**
     * Determines if this is less than or equal to the given specifier.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this is less than or equal to
     * the given specifier.
     */
    bool operator <= (const FacetSpec<dim>& other) const;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim2TriangleEdge, you should
 * use the real class name FacetSpec<2>.
 */
REGINA_DEPRECATED typedef FacetSpec<2> Dim2TriangleEdge;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef NTetFace, you should
 * use the real class name FacetSpec<3>.
 */
REGINA_DEPRECATED typedef FacetSpec<3> NTetFace;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4PentFacet, you should
 * use the real class name FacetSpec<4>.
 */
REGINA_DEPRECATED typedef FacetSpec<4> Dim4PentFacet;

/*@}*/

// Inline functions for FacetSpec

template <int dim>
inline FacetSpec<dim>::FacetSpec() {
}

template <int dim>
inline FacetSpec<dim>::FacetSpec(int newSimp, int newFacet) :
        simp(newSimp), facet(newFacet) {
}

template <int dim>
inline FacetSpec<dim>::FacetSpec(const FacetSpec& cloneMe) :
        simp(cloneMe.simp), facet(cloneMe.facet) {
}

template <int dim>
inline bool FacetSpec<dim>::isBoundary(size_t nSimplices) const {
    return (simp == static_cast<int>(nSimplices) && facet == 0);
}

template <int dim>
inline bool FacetSpec<dim>::isBeforeStart() const {
    return (simp < 0);
}

template <int dim>
inline bool FacetSpec<dim>::isPastEnd(size_t nSimplices, bool boundaryAlso)
        const {
    return (simp == static_cast<int>(nSimplices) &&
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
inline FacetSpec<dim>& FacetSpec<dim>::operator = (
        const FacetSpec<dim>& other) {
    simp = other.simp;
    facet = other.facet;
    return *this;
}

template <int dim>
inline FacetSpec<dim> FacetSpec<dim>::operator ++ () {
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
inline FacetSpec<dim> FacetSpec<dim>::operator -- () {
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
inline bool FacetSpec<dim>::operator == (const FacetSpec<dim>& other) const {
    return (simp == other.simp && facet == other.facet);
}

template <int dim>
inline bool FacetSpec<dim>::operator != (const FacetSpec<dim>& other) const {
    return (simp != other.simp || facet != other.facet);
}

template <int dim>
inline bool FacetSpec<dim>::operator < (const FacetSpec<dim>& other) const {
    return (simp < other.simp || (simp == other.simp && facet < other.facet));
}

template <int dim>
inline bool FacetSpec<dim>::operator <= (const FacetSpec<dim>& other) const {
    return (simp < other.simp || (simp == other.simp && facet <= other.facet));
}

} // namespace regina

#endif

