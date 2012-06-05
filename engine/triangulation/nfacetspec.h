
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

/*! \file triangulation/nfacetspec.h
 *  \brief Allows lightweight representation of individual facets of
 *  simplices.
 */

#ifndef __NFACETSPEC_H
#ifndef __DOXYGEN
#define __NFACETSPEC_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * A lightweight class used to refer to a particular facet of a
 * particular simplex in a triangulation.  Only the simplex index
 * and the facet number are stored.
 *
 * The template parameter gives the dimension of the triangulation
 * (so for dimension three, this class describes a face of a tetrahedron,
 * and for dimension four it describes a facet of a pentachoron).
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
 * \ifacespython The generic template NFacetSpec is not available to
 * Python users, although the special 3-dimensional case NTetFace is.
 * All Python notes in this class refer to the special case
 * NTetFace only.
 */
template <int dim>
struct NFacetSpec {
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
    NFacetSpec();
    /**
     * Creates a new specifier referring to the given facet of the given
     * simplex.
     *
     * @param newSimp the given simplex; see the class notes for
     * allowable values of this parameter.
     * @param newFacet the given facet; this should be between 0 and
     * \a dim inclusive.
     */
    NFacetSpec(int newSimp, int newFacet);
    /**
     * Creates a new specifier referring to the same simplex facet as
     * the given specifier.
     *
     * @param cloneMe the specifier to clone.
     */
    NFacetSpec(const NFacetSpec<dim>& cloneMe);

    /**
     * Determines if this specifier represents the overall boundary.
     *
     * @param nSimplices the number of simplices under consideration.
     * Note that the boundary is represented in this specifier as
     * simplex \a nSimplices, facet 0.
     * @return \c true if and only if this specifier represents the
     * overall boundary.
     */
    bool isBoundary(unsigned nSimplices) const;
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
    bool isPastEnd(unsigned nSimplices, bool boundaryAlso) const;

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
    void setBoundary(unsigned nSimplices);
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
    void setPastEnd(unsigned nSimplices);

    /**
     * Sets this specifier to the value of the given specifier.
     *
     * @param other the given specifier.
     * @return a reference to this specifier.
     */
    NFacetSpec& operator = (const NFacetSpec<dim>& other);
    /**
     * Increments this specifier.  It will be changed to point to the
     * next simplex facet.
     *
     * Faces are ordered first by simplex index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not past-the-end.
     *
     * \ifacespython Not present, although the preincrement operator is
     * present in python as the member function inc().
     *
     * @return A copy of this specifier after it has been incremented.
     */
    NFacetSpec operator ++ ();
    /**
     * Increments this specifier.  It will be changed to point to the
     * next simplex facet.
     *
     * Faces are ordered first by simplex index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not past-the-end.
     *
     * \ifacespython This routine is named inc() since python does not
     * support the increment operator.
     *
     * @return A copy of this specifier before it was incremented.
     */
    NFacetSpec operator ++ (int);
    /**
     * Decrements this specifier.  It will be changed to point to the
     * previous simplex facet.
     *
     * Faces are ordered first by simplex index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not before-the-start.
     *
     * \ifacespython Not present, although the predecrement operator is
     * present in python as the member function dec().
     *
     * @return A copy of this specifier after it has been decremented.
     */
    NFacetSpec operator -- ();
    /**
     * Decrements this specifier.  It will be changed to point to the
     * previous simplex facet.
     *
     * Faces are ordered first by simplex index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not before-the-start.
     *
     * \ifacespython This routine is named dec() since python does not
     * support the decrement operator.
     *
     * @return A copy of this specifier before it was decremented.
     */
    NFacetSpec operator -- (int);

    /**
     * Determines if this and the given specifier are identical.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this and the given specifier are
     * equal.
     */
    bool operator == (const NFacetSpec<dim>& other) const;
    /**
     * Determines if this is less than the given specifier.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this is less than the given
     * specifier.
     */
    bool operator < (const NFacetSpec<dim>& other) const;
    /**
     * Determines if this is less than or equal to the given specifier.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this is less than or equal to
     * the given specifier.
     */
    bool operator <= (const NFacetSpec<dim>& other) const;
};

/**
 * A lightweight class used to refer to a particular edge of a
 * particular triangle in a 2-manifold triangulation.  This is a
 * convenience typedef for the template instance NFacetSpec<2>.
 *
 * \ifacespython The specific class Dim2TriangleEdge is available through
 * Python, even though the generic template NFacetSpec is not.
 */
typedef NFacetSpec<2> Dim2TriangleEdge;

/**
 * A lightweight class used to refer to a particular face of a
 * particular tetrahedron in a 3-manifold triangulation.  This is a
 * convenience typedef for the template instance NFacetSpec<3>.
 *
 * \ifacespython The specific class NTetFace is available through Python,
 * even though the generic template NFacetSpec is not.  Both the old field
 * names (\a tet and \a face) and the new field names (\a simp and \a facet)
 * are provided, though the old names are deprecated and will be removed
 * in a future version of Regina.
 *
 * \deprecated For the 3-dimensional class NTetFace, the old field names
 * \a tet and \a face are deprecated.  Please use the new (generic) names
 * \a simp and \a facet instead.  The old names are no longer supported
 * in C++, but will continue to be supported in Python until Regina 5.0.
 */
typedef NFacetSpec<3> NTetFace;

/*@}*/

// Inline functions for NFacetSpec

template <int dim>
inline NFacetSpec<dim>::NFacetSpec() {
}

template <int dim>
inline NFacetSpec<dim>::NFacetSpec(int newSimp, int newFacet) :
        simp(newSimp), facet(newFacet) {
}

template <int dim>
inline NFacetSpec<dim>::NFacetSpec(const NFacetSpec& cloneMe) :
        simp(cloneMe.simp), facet(cloneMe.facet) {
}

template <int dim>
inline bool NFacetSpec<dim>::isBoundary(unsigned nSimplices) const {
    return (simp == static_cast<int>(nSimplices) && facet == 0);
}

template <int dim>
inline bool NFacetSpec<dim>::isBeforeStart() const {
    return (simp < 0);
}

template <int dim>
inline bool NFacetSpec<dim>::isPastEnd(unsigned nSimplices, bool boundaryAlso)
        const {
    return (simp == static_cast<int>(nSimplices) &&
        (boundaryAlso || facet > 0));
}

template <int dim>
inline void NFacetSpec<dim>::setFirst() {
    simp = facet = 0;
}

template <int dim>
inline void NFacetSpec<dim>::setBoundary(unsigned nSimplices) {
    simp = nSimplices;
    facet = 0;
}

template <int dim>
inline void NFacetSpec<dim>::setBeforeStart() {
    simp = -1;
    facet = dim;
}

template <int dim>
inline void NFacetSpec<dim>::setPastEnd(unsigned nSimplices) {
    simp = nSimplices;
    facet = 1;
}

template <int dim>
inline NFacetSpec<dim>& NFacetSpec<dim>::operator = (
        const NFacetSpec<dim>& other) {
    simp = other.simp;
    facet = other.facet;
    return *this;
}

template <int dim>
inline NFacetSpec<dim> NFacetSpec<dim>::operator ++ () {
    if (++facet > dim) {
        facet = 0;
        ++simp;
    }
    return *this;
}

template <int dim>
inline NFacetSpec<dim> NFacetSpec<dim>::operator ++ (int) {
    NFacetSpec<dim> ans(*this);
    if (++facet > dim) {
        facet = 0;
        ++simp;
    }
    return ans;
}

template <int dim>
inline NFacetSpec<dim> NFacetSpec<dim>::operator -- () {
    if (--facet < 0) {
        facet = dim;
        --simp;
    }
    return *this;
}

template <int dim>
inline NFacetSpec<dim> NFacetSpec<dim>::operator -- (int) {
    NFacetSpec<dim> ans(*this);
    if (--facet < 0) {
        facet = dim;
        --simp;
    }
    return ans;
}

template <int dim>
inline bool NFacetSpec<dim>::operator == (const NFacetSpec<dim>& other) const {
    return (simp == other.simp && facet == other.facet);
}

template <int dim>
inline bool NFacetSpec<dim>::operator < (const NFacetSpec<dim>& other) const {
    return (simp < other.simp || (simp == other.simp && facet < other.facet));
}

template <int dim>
inline bool NFacetSpec<dim>::operator <= (const NFacetSpec<dim>& other) const {
    return (simp < other.simp || (simp == other.simp && facet <= other.facet));
}

} // namespace regina

#endif

