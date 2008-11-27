
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file dim4pentfacet.h
 *  \brief Allows lightweight representation of individual pentachoron
 *  facets.
 */

#ifndef __DIM4PENTFACET_H
#ifndef __DOXYGEN
#define __DIM4PENTFACET_H
#endif

namespace regina {

/**
 * \weakgroup dim4
 * @{
 */

/**
 * A lightweight class used to refer to a particular tetrahedral facet of a
 * particular pentachoron.  Only the pentachoron index and facet number
 * are stored.
 *
 * Facilities are provided for iterating through pentachoron facets.
 * With this in mind, it is also possible to represent the overall
 * boundary, a past-the-end value and a before-the-start value.
 *
 * When iterating through the pentachoron facets, the facets will be
 * ordered first by pentachoron index and then by facet number.  The
 * overall boundary appears after all other pentachoron facets.
 *
 * If there are \a n pentachora, the pentachora will be numbered from 0
 * to <i>n</i>-1 inclusive.  The boundary will be represented as
 * pentachoron \a n, facet 0.  The past-the-end value will be represented
 * as pentachoron \a n, facet 1, and the before-the-start value will be
 * represented as pentachoron -1, facet 4.
 */
struct Dim4PentFacet {
    int pent;
        /**< The pentachoron referred to.  Pentachoron numbering begins at 0. */
    int facet;
        /**< The facet of the pentachoron referred to.  The facet number
             is between 0 and 4 inclusive. */

    /**
     * Creates a new specifier with no initialisation.  This
     * specifier must be initialised before it is used.
     */
    Dim4PentFacet();
    /**
     * Creates a new specifier referring to the given facet of the given
     * pentachoron.
     *
     * @param newPent the given pentachoron; see the class notes for
     * allowable values of this parameter.
     * @param newFacet the given facet number; this should be between 0 and 4
     * inclusive.
     */
    Dim4PentFacet(int newPent, int newFacet);
    /**
     * Creates a new specifier referring to the same pentachoron facet as
     * the given specifier.
     *
     * @param cloneMe the specifier to clone.
     */
    Dim4PentFacet(const Dim4PentFacet& cloneMe);

    /**
     * Determines if this specifier represents the overall boundary.
     *
     * @param nPentachora the number of pentachora under consideration.
     * Note that the boundary is represented in this specifier as
     * pentachoron \a nPentachora, facet 0.
     * @return \c true if and only if this specifier represents the
     * overall boundary.
     */
    bool isBoundary(unsigned nPentachora) const;
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
     * @param nPentachora the number of pentachora under consideration.
     * Note that past-the-end is represented in this specifier as
     * pentachoron \a nPentachora, facet 1.
     * @param boundaryAlso \c true if the overall boundary should be
     * considered past-the-end in addition to the predefined past-the-end
     * value.
     * @return \c true if and only if this specifier is past-the-end.
     */
    bool isPastEnd(unsigned nPentachora, bool boundaryAlso) const;

    /**
     * Sets this specifier to the first facet of the first pentachoron.
     */
    void setFirst();
    /**
     * Sets this specifier to the overall boundary.
     *
     * @param nPentachora the number of pentachora under consideration.
     * Note that the boundary is represented in this specifier as
     * pentachoron \a nPentachora, facet 0.
     */
    void setBoundary(unsigned nPentachora);
    /**
     * Sets this specifier to before-the-start.
     */
    void setBeforeStart();
    /**
     * Sets this specifier to past-the-end.
     *
     * @param nPentachora the number of pentachora under consideration.
     * Note that past-the-end is represented in this specifier as
     * pentachoron \a nPentachora, facet 1.
     */
    void setPastEnd(unsigned nPentachora);

    /**
     * Sets this specifier to the value of the given specifier.
     *
     * @param other the given specifier.
     * @return a reference to this specifier.
     */
    Dim4PentFacet& operator = (const Dim4PentFacet& other);
    /**
     * Increments this specifier.  It will be changed to point to the
     * next pentachoron facet.
     *
     * Facets are ordered first by pentachoron index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not past-the-end.
     *
     * \ifacespython Not present, although the preincrement operator is
     * present in python as the member function inc().
     *
     * @return A copy of this specifier after it has been incremented.
     */
    Dim4PentFacet operator ++ ();
    /**
     * Increments this specifier.  It will be changed to point to the
     * next pentachoron facet.
     *
     * Facets are ordered first by pentachoron index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not past-the-end.
     *
     * \ifacespython This routine is named inc() since python does not
     * support the increment operator.
     *
     * @return A copy of this specifier before it was incremented.
     */
    Dim4PentFacet operator ++ (int);
    /**
     * Decrements this specifier.  It will be changed to point to the
     * previous pentachoron facet.
     *
     * Facets are ordered first by pentachoron index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not before-the-start.
     *
     * \ifacespython Not present, although the predecrement operator is
     * present in python as the member function dec().
     *
     * @return A copy of this specifier after it has been decremented.
     */
    Dim4PentFacet operator -- ();
    /**
     * Decrements this specifier.  It will be changed to point to the
     * previous pentachoron facet.
     *
     * Facets are ordered first by pentachoron index and then by facet
     * number.  The overall boundary appears after all other facets.
     *
     * \pre This specifier is not before-the-start.
     *
     * \ifacespython This routine is named dec() since python does not
     * support the decrement operator.
     *
     * @return A copy of this specifier before it was decremented.
     */
    Dim4PentFacet operator -- (int);

    /**
     * Determines if this and the given specifier are identical.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this and the given specifier are
     * equal.
     */
    bool operator == (const Dim4PentFacet& other) const;
    /**
     * Determines if this is less than the given specifier.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this is less than the given
     * specifier.
     */
    bool operator < (const Dim4PentFacet& other) const;
    /**
     * Determines if this is less than or equal to the given specifier.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this is less than or equal to
     * the given specifier.
     */
    bool operator <= (const Dim4PentFacet& other) const;
};

/*@}*/

// Inline functions for Dim4PentFacet

inline Dim4PentFacet::Dim4PentFacet() {
}
inline Dim4PentFacet::Dim4PentFacet(int newPent, int newFacet) :
        pent(newPent), facet(newFacet) {
}
inline Dim4PentFacet::Dim4PentFacet(const Dim4PentFacet& cloneMe) :
        pent(cloneMe.pent), facet(cloneMe.facet) {
}

inline bool Dim4PentFacet::isBoundary(unsigned nPentachora) const {
    return (pent == static_cast<int>(nPentachora) && facet == 0);
}
inline bool Dim4PentFacet::isBeforeStart() const {
    return (pent < 0);
}
inline bool Dim4PentFacet::isPastEnd(unsigned nPentachora, bool boundaryAlso)
        const {
    return (pent == static_cast<int>(nPentachora) &&
        (boundaryAlso || facet > 0));
}

inline void Dim4PentFacet::setFirst() {
    pent = facet = 0;
}
inline void Dim4PentFacet::setBoundary(unsigned nPentachora) {
    pent = nPentachora;
    facet = 0;
}
inline void Dim4PentFacet::setBeforeStart() {
    pent = -1;
    facet = 4;
}
inline void Dim4PentFacet::setPastEnd(unsigned nPentachora) {
    pent = nPentachora;
    facet = 1;
}

inline Dim4PentFacet& Dim4PentFacet::operator = (const Dim4PentFacet& other) {
    pent = other.pent;
    facet = other.facet;
    return *this;
}
inline Dim4PentFacet Dim4PentFacet::operator ++ () {
    if (++facet == 5) {
        facet = 0;
        ++pent;
    }
    return *this;
}
inline Dim4PentFacet Dim4PentFacet::operator ++ (int) {
    Dim4PentFacet ans(*this);
    if (++facet == 5) {
        facet = 0;
        ++pent;
    }
    return ans;
}
inline Dim4PentFacet Dim4PentFacet::operator -- () {
    if (--facet < 0) {
        facet = 4;
        --pent;
    }
    return *this;
}
inline Dim4PentFacet Dim4PentFacet::operator -- (int) {
    Dim4PentFacet ans(*this);
    if (--facet < 0) {
        facet = 4;
        --pent;
    }
    return ans;
}

inline bool Dim4PentFacet::operator == (const Dim4PentFacet& other) const {
    return (pent == other.pent && facet == other.facet);
}
inline bool Dim4PentFacet::operator < (const Dim4PentFacet& other) const {
    return (pent < other.pent ||
        (pent == other.pent && facet < other.facet));
}
inline bool Dim4PentFacet::operator <= (const Dim4PentFacet& other) const {
    return (pent < other.pent ||
        (pent == other.pent && facet <= other.facet));
}

} // namespace regina

#endif

