
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file nsatannulus.h
 *  \brief Deals with saturated two-face annuli within a Seifert fibred space.
 */

#ifndef __NSATANNULUS_H
#ifndef __DOXYGEN
#define __NSATANNULUS_H
#endif

#include "triangulation/nperm.h"

namespace regina {

class NIsomorphism;
class NTetrahedron;
class NTriangulation;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents an annulus formed from a pair of faces in a Seifert fibred
 * space.  This annulus is saturated, i.e., a union of fibres.  More than
 * that, the fibres run parallel to the two boundary edges of the annulus.
 *
 * The annulus is described from one side only.  The description
 * includes an array of indices \a tet[] describing which two tetrahedra
 * provide the faces of the annulus, as well as an array of permutations
 * \a roles[] detailing how the annulus matches up with the individual
 * tetrahedron vertices.
 *
 * The annulus can be drawn as follows, with the upper edge identified
 * with the lower:
 *
 * <pre>
 *            *--->---*
 *            |0  2 / |
 *     First  |    / 1|  Second
 *     face   |   /   |   face
 *            |1 /    |
 *            | / 2  0|
 *            *--->---*
 * </pre>
 *
 * Suppose that \a tet[0] and \a tet[1] are the tetrahedra providing the
 * first and second faces respectively.  Then the markings 0..2 on the
 * first face above correspond to vertices \a roles[0][0..2] of tetrahedron
 * \a tet[0], and likewise the markings 0..2 on the second face above
 * correspond to vertices \a roles[1][0..2] of tetrahedron \a tet[1].
 *
 * Note that the diagram above can also be drawn as follows.
 *
 * <pre>
 *            *--->---*
 *            | \ 2  1|
 *     First  |0 \    |  Second
 *     face   |   \   |   face
 *            |    \ 0|
 *            |1  2 \ |
 *            *--->---*
 * </pre>
 *
 * Note also that the labelling of the tetrahedra and their vertices
 * establishes an orientation on the vertical fibres, as well as a
 * left-to-right direction across the annulus.
 *
 * For convenience we refer to edges \a roles[][0-1] as \e vertical,
 * edges \a roles[][0-2] as \e horizontal, and edge \a roles[][1-2] as
 * \e diagonal.  This is illustrated in the following diagrams.
 *
 * <pre>
 *         V  Horizontal       V   Diagonal
 *         e  *--->---*        e  *--->---*
 *         r  |   g / |        r  |H\ 2  1|
 *         t  |  a / 1|        t  | o\    |
 *         i  | i /   |        i  |  r\   |
 *         c  |D /    |        c  |   i\ 0|
 *         a  | / 2  0|        a  |    z\ |
 *         l  *--->---*        l  *--->---*
 * </pre>
 */
struct NSatAnnulus {
    NTetrahedron* tet[2];
        /**< Describes which tetrahedra provide the first and second
             faces.  See the class notes for details. */
    NPerm roles[2];
        /**< Describes how the first and second faces match up with
             individual tetrahedron vertices.  See the class notes for
             details. */

    /**
     * Creates a new uninitialised structure.  Both tetrahedra will be
     * set to null pointers.
     */
    NSatAnnulus();
    /**
     * Creates a clone of the given structure.
     *
     * @param cloneMe the structure to clone.
     */
    NSatAnnulus(const NSatAnnulus& cloneMe);
    /**
     * Creates a new structure initialised to the given values.  See the
     * class notes for what the various tetrahedra and permutations mean.
     *
     * @param t0 the tetrahedron to assign to \a tet[0].
     * @param r0 the permutation to assign to \a roles[0].
     * @param t1 the tetrahedron to assign to \a tet[1].
     * @param r1 the permutation to assign to \a roles[1].
     */
    NSatAnnulus(NTetrahedron* t0, NPerm r0, NTetrahedron* t1, NPerm r1);
    /**
     * Makes this equal to a clone of the given structure.
     *
     * @param cloneMe the structure to clone.
     * @return a reference to this structure.
     */
    NSatAnnulus& operator = (const NSatAnnulus& cloneMe);

    /**
     * Determines whether or not this and the given structure describe
     * the same annulus with the same representation.  This requires
     * both structures to have identical \a tet[] and \a roles[] arrays.
     *
     * @param other the structure to compare with this.
     * @return \c true if the structures describe the same annulus with
     * the same representation, or \c false if they do not.
     */
    bool operator == (const NSatAnnulus& other) const;
    /**
     * Determines whether or not this and the given structure describe
     * the same annulus with the same representation.  This requires
     * both structures to have identical \a tet[] and \a roles[] arrays.
     *
     * @param other the structure to compare with this.
     * @return \c true if the structures do not describe the same annulus
     * with the same representation, or \c false if they do.
     */
    bool operator != (const NSatAnnulus& other) const;

    /**
     * Determines whether some face of this annulus lies on the boundary
     * of the triangulations.
     *
     * @return \c true if one or both faces of this annulus are boundary
     * faces of the triangulation, or \c false if both faces are internal.
     */
    bool meetsBoundary() const;

    /**
     * Converts this into a representation of the same annulus from the
     * other side.  The first and second faces and their 0..2 markings
     * (as described in the class notes) remain unchanged.  However, the
     * two tetrahedra that are used to describe the annulus will be
     * replaced by their counterparts on the other side of the annulus
     * (i.e., the two new tetrahedra that meet the two original tetrahedra
     * along the annulus itself).
     *
     * \pre Neither face of this annulus is a boundary face of the
     * triangulations.  See meetsBoundary() for details.
     */
    void switchSides();
    /**
     * Returns a representation of the same annulus from the other side.
     * This structure will not be changed.  See switchSides() for further
     * details.
     *
     * \pre Neither face of this annulus is a boundary face of the
     * triangulations.  See meetsBoundary() for details.
     *
     * @return a new representation of this annulus from the other side.
     */
    NSatAnnulus otherSide() const;

    /**
     * Reverses the direction of the vertical fibres in this annulus
     * representation.  The first and second faces (as described in the
     * class notes) will remain unchanged, but the markings 0 and 1 on
     * each face will be switched.
     */
    void reflectVertical();
    /**
     * Returns a representation of this annulus in which the vertical
     * direction of the fibres has been reversed.  This structure will
     * not be changed.  See reflectVertical() for further details.
     *
     * @return a new representation of this annulus in which fibres have
     * been reversed.
     */
    NSatAnnulus verticalReflection() const;

    /**
     * Performs a left-to-right reflection of this annulus
     * representation.  The vertical direction of the fibres will remain
     * unchanged, but the first and second faces will be switched (and
     * the 0..2 markings changed to compensate).
     */
    void reflectHorizontal();
    /**
     * Returns a left-to-right reflected representation of this annulus.
     * This structure will not be changed.  See reflectHorizontal() for
     * further details.
     *
     * @return a new left-to-right reflection of this annulus.
     */
    NSatAnnulus horizontalReflection() const;

    /**
     * TODO
     */
    bool isAdjacent(const NSatAnnulus& other, bool* refVert, bool* refHoriz)
        const;

    /**
     * Adjusts this annulus representation according to the given
     * isomorphism between triangulations.
     *
     * The given isomorphism must describe a mapping from \a originalTri
     * to \a newTri, and this annulus must refer to tetrahedra in
     * \a originalTri.  This routine will adjust this annulus according
     * to the given isomorphism, so that it refers to the corresponding
     * tetrahedra in \a newTri (with the \a roles permutations also
     * updated accordingly).
     *
     * \pre This annulus refers to tetrahedra in \a originalTri, and
     * \a iso describes a mapping from \a originalTri to \a newTri.
     *
     * @param originalTri the triangulation currently used by this
     * annulus representation.
     * @param iso the mapping from \a originalTri to \a newTri.
     * @param newTri the triangulation to be used by the updated annulus
     * representation.
     */
    void transform(const NTriangulation* originalTri,
            const NIsomorphism* iso, NTriangulation* newTri);
    /**
     * Returns the image of this annulus representation under the given
     * isomorphism between triangulations.  This annulus representation
     * will not be changed.  See transform() for further details.
     *
     * @param originalTri the triangulation currently used by this
     * annulus representation.
     * @param iso the mapping from \a originalTri to \a newTri.
     * @param newTri the triangulation to be used by the new annulus
     * representation.
     */
    NSatAnnulus image(const NTriangulation* originalTri,
            const NIsomorphism* iso, NTriangulation* newTri) const;
};

/*@}*/

// Inline functions for NSatAnnulus

inline NSatAnnulus::NSatAnnulus() {
    tet[0] = tet[1] = 0;
}

inline NSatAnnulus::NSatAnnulus(const NSatAnnulus& cloneMe) {
    tet[0] = cloneMe.tet[0]; tet[1] = cloneMe.tet[1];
    roles[0] = cloneMe.roles[0]; roles[1] = cloneMe.roles[1];
}

inline NSatAnnulus::NSatAnnulus(NTetrahedron* t0, NPerm r0,
        NTetrahedron* t1, NPerm r1) {
    tet[0] = t0; tet[1] = t1;
    roles[0] = r0; roles[1] = r1;
}

inline NSatAnnulus& NSatAnnulus::operator = (const NSatAnnulus& cloneMe) {
    tet[0] = cloneMe.tet[0]; tet[1] = cloneMe.tet[1];
    roles[0] = cloneMe.roles[0]; roles[1] = cloneMe.roles[1];
    return *this;
}

inline bool NSatAnnulus::operator == (const NSatAnnulus& other) const {
    return (tet[0] == other.tet[0] && tet[1] == other.tet[1] &&
            roles[0] == other.roles[0] && roles[1] == other.roles[1]);
}

inline bool NSatAnnulus::operator != (const NSatAnnulus& other) const {
    return (tet[0] != other.tet[0] || tet[1] != other.tet[1] ||
            roles[0] != other.roles[0] || roles[1] != other.roles[1]);
}

inline NSatAnnulus NSatAnnulus::otherSide() const {
    NSatAnnulus a(*this);
    a.switchSides();
    return a;
}

inline void NSatAnnulus::reflectVertical() {
    roles[0] = roles[0] * NPerm(0, 1);
    roles[1] = roles[1] * NPerm(0, 1);
}

inline NSatAnnulus NSatAnnulus::verticalReflection() const {
    return NSatAnnulus(tet[0], roles[0] * NPerm(0, 1),
                       tet[1], roles[1] * NPerm(0, 1));
}

inline void NSatAnnulus::reflectHorizontal() {
    NTetrahedron* t = tet[0];
    tet[0] = tet[1];
    tet[1] = t;

    NPerm r = roles[0];
    roles[0] = roles[1] * NPerm(0, 1);
    roles[1] = r * NPerm(0, 1);
}

inline NSatAnnulus NSatAnnulus::horizontalReflection() const {
    return NSatAnnulus(tet[1], roles[1] * NPerm(0, 1),
                       tet[0], roles[0] * NPerm(0, 1));
}

inline NSatAnnulus NSatAnnulus::image(const NTriangulation* originalTri,
        const NIsomorphism* iso, NTriangulation* newTri) const {
    NSatAnnulus a(*this);
    a.transform(originalTri, iso, newTri);
    return a;
}

} // namespace regina

#endif

