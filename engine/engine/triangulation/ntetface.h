
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file ntetface.h
 *  \brief Allows lightweight representation of individual tetrahedron
 *  faces.
 */

#ifndef __NTETFACE_H
#ifndef __DOXYGEN
#define __NTETFACE_H
#endif

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * A lightweight class used to refer to a particular face of a
 * particular tetrahedron.  Only the tetrahedron index and face number
 * are stored.
 *
 * Facilities are provided for iterating through tetrahedron faces.
 * With this in mind, it is also possible to represent the overall
 * boundary, a past-the-end value and a before-the-start value.
 *
 * When iterating through the tetrahedron faces, the faces will be
 * ordered first by tetrahedron index and then by face number.  The
 * overall boundary appears after all other tetrahedron faces.
 *
 * If there are \a n tetrahedra, the tetrahedra will be numbered from 0
 * to <i>n</i>-1 inclusive.  The boundary will be represented as
 * tetrahedron \a n, face 0.  The past-the-end value will be represented
 * as tetrahedron \a n, face 1, and the before-the-start value will be
 * represented as tetrahedron -1, face 3.
 *
 * \ifaces Not present.
 */
struct NTetFace {
    int tet;
        /**< The tetrahedron referred to.  Tetrahedron numbering begins
         *   at 0. */
    int face;
        /**< The face of the tetrahedron referred to.  The face number
         *   is between 0 and 3 inclusive. */

    /**
     * Creates a new specifier with no initialisation.  This
     * specifier must be initialised before it is used.
     */
    NTetFace();
    /**
     * Creates a new specifier referring to the given face of the given
     * tetrahedron.
     *
     * @param newTet the given tetrahedron; see the class notes for
     * allowable values of this parameter.
     * @param newFace the given face; this should be between 0 and 3
     * inclusive.
     */
    NTetFace(int newTet, int newFace);

    /**
     * Determines if this specifier represents the overall boundary.
     *
     * @param nTetrahedra the number of tetrahedra under consideration.
     * Note that the boundary is represented in this specifier as
     * tetrahedron \a nTetrahedra, face 0.
     * @return \c true if and only if this specifier represents the
     * overall boundary.
     */
    bool isBoundary(unsigned nTetrahedra) const;
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
     * @param nTetrahedra the number of tetrahedra under consideration.
     * Note that past-the-end is represented in this specifier as
     * tetrahedron \a nTetrahedra, face 1.
     * @param boundaryAlso \c true if the overall boundary should be
     * considered past-the-end in addition to the predefined past-the-end
     * value.
     * @return \c true if and only if this specifier is past-the-end.
     */
    bool isPastEnd(unsigned nTetrahedra, bool boundaryAlso) const;

    /**
     * Sets this specifier to the first face of the first tetrahedron.
     */
    void setFirst();
    /**
     * Sets this specifier to the overall boundary.
     *
     * @param nTetrahedra the number of tetrahedra under consideration.
     * Note that the boundary is represented in this specifier as
     * tetrahedron \a nTetrahedra, face 0.
     */
    void setBoundary(unsigned nTetrahedra);
    /**
     * Sets this specifier to before-the-start.
     */
    void setBeforeStart();
    /**
     * Sets this specifier to past-the-end.
     *
     * @param nTetrahedra the number of tetrahedra under consideration.
     * Note that past-the-end is represented in this specifier as
     * tetrahedron \a nTetrahedra, face 1.
     */
    void setPastEnd(unsigned nTetrahedra);

    /**
     * Sets this specifier to the value of the given specifier.
     *
     * @param other the given specifier.
     * @return a reference to this specifier.
     */
    NTetFace& operator = (const NTetFace& other);
    /**
     * Increments this specifier.  It will be changed to point to the
     * next tetrahedron face.
     *
     * Faces are ordered first by tetrahedron index and then by face
     * number.  The overall boundary appears after all other faces.
     *
     * \pre This specifier is not past-the-end.
     */
    void operator ++ (int);
    /**
     * Decrements this specifier.  It will be changed to point to the
     * previous tetrahedron face.
     *
     * Faces are ordered first by tetrahedron index and then by face
     * number.  The overall boundary appears after all other faces.
     *
     * \pre This specifier is not before-the-start.
     */
    void operator -- (int);

    /**
     * Determines if this and the given specifier are identical.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this and the given specifier are
     * equal.
     */
    bool operator == (const NTetFace& other) const;
    /**
     * Determines if this is less than the given specifier.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this is less than the given
     * specifier.
     */
    bool operator < (const NTetFace& other) const;
    /**
     * Determines if this is less than or equal to the given specifier.
     *
     * @param other the specifier to compare with this.
     * @return \c true if and only if this is less than or equal to
     * the given specifier.
     */
    bool operator <= (const NTetFace& other) const;
};

/*@}*/

// Inline functions for NTetFace

inline NTetFace::NTetFace() {
}
inline NTetFace::NTetFace(int newTet, int newFace) : tet(newTet),
        face(newFace) {
}

inline bool NTetFace::isBoundary(unsigned nTetrahedra) const {
    return (tet == (int)nTetrahedra && face == 0);
}
inline bool NTetFace::isBeforeStart() const {
    return (tet < 0);
}
inline bool NTetFace::isPastEnd(unsigned nTetrahedra, bool boundaryAlso)
        const {
    return (tet == (int)nTetrahedra && (boundaryAlso || face > 0));
}

inline void NTetFace::setFirst() {
    tet = face = 0;
}
inline void NTetFace::setBoundary(unsigned nTetrahedra) {
    tet = nTetrahedra;
    face = 0;
}
inline void NTetFace::setBeforeStart() {
    tet = -1;
    face = 3;
}
inline void NTetFace::setPastEnd(unsigned nTetrahedra) {
    tet = nTetrahedra;
    face = 1;
}

inline NTetFace& NTetFace::operator = (const NTetFace& other) {
    tet = other.tet;
    face = other.face;
    return *this;
}
inline void NTetFace::operator ++ (int) {
    face++;
    if (face == 4) {
        face = 0;
        tet++;
    }
}
inline void NTetFace::operator -- (int) {
    face--;
    if (face == -1) {
        face = 3;
        tet--;
    }
}

inline bool NTetFace::operator == (const NTetFace& other) const {
    return (tet == other.tet && face == other.face);
}
inline bool NTetFace::operator < (const NTetFace& other) const {
    return (tet < other.tet || (tet == other.tet && face < other.face));
}
inline bool NTetFace::operator <= (const NTetFace& other) const {
    return (tet < other.tet || (tet == other.tet && face <= other.face));
}

} // namespace regina

#endif

