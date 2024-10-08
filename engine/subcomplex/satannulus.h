
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

/*! \file subcomplex/satannulus.h
 *  \brief Deals with saturated two-triangle annuli within a
 *  Seifert fibred space.
 */

#ifndef __REGINA_SATANNULUS_H
#ifndef __DOXYGEN
#define __REGINA_SATANNULUS_H
#endif

#include "regina-core.h"
#include "maths/perm.h"
#include "triangulation/dim3.h"
#include <tuple>

namespace regina {

class Matrix2;

/**
 * Represents an annulus formed from a pair of triangles in a Seifert fibred
 * space.  This annulus is saturated, i.e., a union of fibres.  More than
 * that, the fibres run parallel to the two boundary edges of the annulus.
 *
 * The annulus is described from one side only.  The description
 * includes an array of indices \a tet[] describing which two tetrahedra
 * provide the triangles of the annulus, as well as an array of permutations
 * \a roles[] detailing how the annulus matches up with the individual
 * tetrahedron vertices.
 *
 * The annulus can be drawn as follows, with the upper edge identified
 * with the lower:
 *
 * <pre>
 *            *--->---*
 *            |0  2 / |
 *    First   |    / 1|  Second
 *   triangle |   /   | triangle
 *            |1 /    |
 *            | / 2  0|
 *            *--->---*
 * </pre>
 *
 * Suppose that \a tet[0] and \a tet[1] are the tetrahedra providing the
 * first and second triangles respectively.  Then the markings 0..2 on the
 * first triangle above correspond to vertices \a roles[0][0..2] of tetrahedron
 * \a tet[0], and likewise the markings 0..2 on the second triangle above
 * correspond to vertices \a roles[1][0..2] of tetrahedron \a tet[1].
 *
 * Note that the diagram above can also be drawn as follows.
 *
 * <pre>
 *            *--->---*
 *            | \ 2  1|
 *    First   |0 \    |  Second
 *   triangle |   \   | triangle
 *            |    \ 0|
 *            |1  2 \ |
 *            *--->---*
 * </pre>
 *
 * Note also that the labelling of the tetrahedra and their vertices
 * establishes an orientation on the vertical fibres, as well as a
 * left-to-right direction across the annulus.
 *
 * For convenience we refer to edges \a roles[][0-1] as _vertical_,
 * edges \a roles[][0-2] as _horizontal_, and edge \a roles[][1-2] as
 * _diagonal_.  This is illustrated in the following diagrams.
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
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \python The member arrays \a tet and \a roles are accessed for
 * reading through functions \a tet() and \a roles() respectively.  For
 * instance, the first triangle tetrahedron for the saturated annulus \a a can
 * be accessed as `a.tet(0)`.  These same member arrays are
 * accessed for writing through functions \a setTet() and \a setRoles(),
 * so for instance the second triangle vertex roles for the saturated annulus
 * \a a can be modified by calling `a.setRoles(1, newRoles)`.
 *
 * \ingroup subcomplex
 */
struct SatAnnulus {
    const Tetrahedron<3>* tet[2];
        /**< Describes which tetrahedra provide the first and second
             triangles.  See the class notes for details. */
    Perm<4> roles[2];
        /**< Describes how the first and second triangles match up with
             individual tetrahedron vertices.  See the class notes for
             details. */

    /**
     * Creates a new uninitialised structure.  Both tetrahedra will be
     * set to null pointers.
     */
    SatAnnulus();
    /**
     * Creates a new copy of the given structure.
     */
    SatAnnulus(const SatAnnulus&) = default;
    /**
     * Creates a new structure initialised to the given values.  See the
     * class notes for what the various tetrahedra and permutations mean.
     *
     * \param t0 the tetrahedron to assign to \a tet[0].
     * \param r0 the permutation to assign to \a roles[0].
     * \param t1 the tetrahedron to assign to \a tet[1].
     * \param r1 the permutation to assign to \a roles[1].
     */
    SatAnnulus(const Tetrahedron<3>* t0, Perm<4> r0,
        const Tetrahedron<3>* t1, Perm<4> r1);
    /**
     * Sets this to be a copy of the given structure.
     *
     * \return a reference to this structure.
     */
    SatAnnulus& operator = (const SatAnnulus&) = default;

#ifdef __APIDOCS
    /**
     * A Python-only routine that allows you to query the \a tet field.
     *
     * The \a tet field describes which two tetrahedra provide the two
     * triangles that make up this annulus.  See the class notes for details.
     *
     * \nocpp This is only for Python users, who cannot access the
     * \a tet and \a roles fields directly.
     *
     * \param which identifies whether we are querying information for
     * the first or second triangle of this annulus.  This argument
     * must be 0 or 1 respectively.
     * \return the tetrahedron that provides the given triangle.
     */
    const Tetrahedron<3>* tet(int which) const;
    /**
     * A Python-only routine that allows you to query the \a roles field.
     *
     * The \a roles field describes how the two triangles that make up this
     * match up with the individual vertices of their corresponding tetrahedra.
     * See the class notes for details.
     *
     * \nocpp This is only for Python users, who cannot access the
     * \a tet and \a roles fields directly.
     *
     * \param which identifies whether we are querying information for
     * the first or second triangle of this annulus.  This argument
     * must be 0 or 1 respectively.
     * \return the permutation that describes how the given triangle matches
     * up with the individual vertices of its corresponding tetrahedron.
     */
    Perm<4> roles(int which) const;
    /**
     * A Python-only routine that allows you to set the \a tet field.
     *
     * The \a tet field describes which two tetrahedra provide the two
     * triangles that make up this annulus.  See the class notes for details.
     *
     * \nocpp This is only for Python users, who cannot access the
     * \a tet and \a roles fields directly.
     *
     * \param which identifies whether we are setting information for
     * the first or second triangle of this annulus.  This argument
     * must be 0 or 1 respectively.
     * \param value the tetrahedron that provides the given triangle.
     */
    void setTet(int which, const Tetrahedron<3>* value);
    /**
     * A Python-only routine that allows you to set the \a roles field.
     *
     * The \a roles field describes how the two triangles that make up this
     * match up with the individual vertices of their corresponding tetrahedra.
     * See the class notes for details.
     *
     * \nocpp This is only for Python users, who cannot access the
     * \a tet and \a roles fields directly.
     *
     * \param which identifies whether we are setting information for
     * the first or second triangle of this annulus.  This argument
     * must be 0 or 1 respectively.
     * \param value the permutation that describes how the given triangle
     * matches up with the individual vertices of its corresponding tetrahedron.
     */
    void setRoles(int which, Perm<4> value);
#endif // __APIDOCS

    /**
     * Determines whether or not this and the given structure describe
     * the same specific presentation of a saturated annulus.
     *
     * Specifically, in order to compare as equal, two saturated annuli
     * must use the same two numbered tetrahedra, presented in the same
     * order, and with the same \a roles permutations.
     *
     * Because this operation compares tetrahedron _numbers_ and not
     * the underlying Tetrahedron objects, it is meaningful to compare
     * saturated annuli from different triangulations.
     *
     * \param other the structure to compare with this.
     * \return \c true if and only if both structures describe the same
     * specific presentation of a saturated annulus.
     */
    bool operator == (const SatAnnulus& other) const;

    /**
     * Determines how many triangles of this annulus lie on the boundary
     * of the triangulation.
     *
     * Note that this routine can also be used as a boolean function
     * to determine whether any triangles of the annulus lie on the
     * triangulation boundary.
     *
     * \return the number of triangles of this annulus that lie on the boundary
     * of the triangulation; this will be 0, 1 or 2.
     */
    int meetsBoundary() const;

    /**
     * Converts this into a representation of the same annulus from the
     * other side.  The first and second triangles and their 0..2 markings
     * (as described in the class notes) remain unchanged.  However, the
     * two tetrahedra that are used to describe the annulus will be
     * replaced by their counterparts on the other side of the annulus
     * (i.e., the two new tetrahedra that meet the two original tetrahedra
     * along the annulus itself).
     *
     * \pre Neither triangle of this annulus is a boundary triangle of the
     * triangulation.
     */
    void switchSides();
    /**
     * Returns a representation of the same annulus from the other side.
     * This structure will not be changed.  See switchSides() for further
     * details.
     *
     * \pre Neither triangle of this annulus is a boundary triangle of the
     * triangulation.
     *
     * \return a new representation of this annulus from the other side.
     */
    SatAnnulus otherSide() const;

    /**
     * Reverses the direction of the vertical fibres in this annulus
     * representation.  The first and second triangles (as described in the
     * class notes) will remain unchanged, but the markings 0 and 1 on
     * each triangle will be switched.
     */
    void reflectVertical();
    /**
     * Returns a representation of this annulus in which the vertical
     * direction of the fibres has been reversed.  This structure will
     * not be changed.  See reflectVertical() for further details.
     *
     * \return a new representation of this annulus in which fibres have
     * been reversed.
     */
    SatAnnulus verticalReflection() const;

    /**
     * Performs a left-to-right reflection of this annulus
     * representation.  The vertical direction of the fibres will remain
     * unchanged, but the first and second triangles will be switched (and
     * the 0..2 markings changed to compensate).
     */
    void reflectHorizontal();
    /**
     * Returns a left-to-right reflected representation of this annulus.
     * This structure will not be changed.  See reflectHorizontal() for
     * further details.
     *
     * \return a new left-to-right reflection of this annulus.
     */
    SatAnnulus horizontalReflection() const;

    /**
     * Rotates the representation of this annulus by 180 degrees.
     * This has the effect of switching the first and second triangles and
     * also reversing the direction of the vertical fibres.
     *
     * Calling this routine is equivalent to calling reflectVertical() and
     * then reflectHorizontal().
     */
    void rotateHalfTurn();
    /**
     * Returns a 180 degree rotated representation of this annulus.
     * This structure will not be changed.  See rotateHalfTurn() for
     * further details.
     *
     * \return a new 180 degree rotation of this annulus.
     */
    SatAnnulus halfTurnRotation() const;

    /**
     * Determines whether this and the given annulus are adjacent,
     * possibly modulo vertical or horizontal reflections.  That is,
     * this routine determines whether this and the given structure
     * represent opposite sides of the same saturated annulus, where the
     * fibres for both structures are consistent (though possibly reversed).
     * See switchSides() for details on what "opposite sides" means in
     * this context, and see reflectVertical() and reflectHorizontal()
     * for descriptions of the various types of reflection.
     *
     * Information regarding reflections is returned via the second and
     * third elements of the returned tuple (call these \a refVert and
     * \a refHoriz).  If the two annuli are identically opposite each other
     * as described by switchSides(), both booleans will be \c false.  If the
     * two annuli are identically opposite after one undergoes a vertical
     * and/or horizontal reflection, then the booleans \a refVert and/or
     * \a refHoriz will be \c true accordingly.
     *
     * The critical difference between this routine and isJoined() is
     * that this routine insists that the fibres on each annulus be
     * consistent.  This routine is thus suitable for examining joins
     * between different sections of the same Seifert fibred space,
     * for example.
     *
     * \param other the annulus to compare with this.
     * \return a tuple of booleans (\a adj, \a refVert, \a refHoriz), where:
     * \a adj is \c true iff some adjacency was found (either with or
     * without reflections); \a refVert is \c true iff a vertical reflection
     * is required; and \a refHoriz is \c true iff a horizontal reflection is
     * required.  If no adjacency was found at all, then both \a refVert and
     * \a refHoriz will be \c false.
     */
    std::tuple<bool, bool, bool> isAdjacent(const SatAnnulus& other) const;

    /**
     * Determines whether this and the given annulus are joined in some
     * form, even if the fibres on each annulus are not consistent.
     *
     * This routine treats each annulus as though its boundaries are
     * identified to form a torus (though it does not actually test
     * whether this is true).  It then examines whether this and the
     * given annulus represent opposite sides of the same torus.
     * More specifically, it tests whether both annuli are formed from
     * the same pair of triangles, and whether the mapping of 0/1/2 markings
     * from one annulus to the other is the same for each triangle.  Note that
     * the triangles are allowed to be switched (i.e., the first triangle of one
     * annulus may be the second triangle of the other).
     *
     * The critical difference between this routine and isAdjacent() is
     * that this routine allows the fibres on each annulus to be
     * inconsistent.  This routine is thus suitable for examining joins
     * between different Seifert fibred blocks in a graph manifold, for
     * example.
     *
     * If the two annuli are joined, the precise relationship between
     * the curves on each annulus will be returned in the matrix
     * \a matching.  Specifically, let \a x and \a y be the oriented
     * curves running from markings 0-1 and 0-2 respectively on the
     * first triangle of this annulus.  Likewise, let \a x' and \a y' run
     * from markings 0-1 and 0-2 respectively on the first triangle of the
     * annulus \a other.  Then the joining between the two annuli can
     * be expressed as follows:
     *
     * <pre>
     *     [x ]                [x']
     *     [  ]  =  matching * [  ].
     *     [y ]                [y']
     * </pre>
     *
     * \param other the annulus to compare with this.
     * \param matching returns details on how the curves on each annulus
     * are related.  If the this and the given annulus are not joined,
     * then this matrix is not touched.
     * \return \c true if this and the given annulus are found to be
     * joined, or \c false if they are not.
     */
    bool isJoined(const SatAnnulus& other, Matrix2& matching) const;

    /**
     * Determines whether this annulus has its boundaries identified to
     * form an embedded two-sided torus within the surrounding triangulation.
     *
     * It will be verified that:
     * - the two triangles of this annulus are joined along all three pairs
     *   of edges to form a torus;
     * - the three edges of this torus remain distinct (i.e., different edges
     *   of the torus do not become identified within the larger triangulation);
     * - this torus is two-sided within the surrounding triangulation.
     *
     * \return \c true if this annulus forms an embedded two-sided torus as
     * described above, or \c false if it does not.
     */
    bool isTwoSidedTorus() const;

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
     * \param originalTri the triangulation currently used by this
     * annulus representation.
     * \param iso the mapping from \a originalTri to \a newTri.
     * \param newTri the triangulation to be used by the updated annulus
     * representation.
     */
    void transform(const Triangulation<3>& originalTri,
            const Isomorphism<3>& iso, const Triangulation<3>& newTri);
    /**
     * Returns the image of this annulus representation under the given
     * isomorphism between triangulations.  This annulus representation
     * will not be changed.  See transform() for further details.
     *
     * \param originalTri the triangulation currently used by this
     * annulus representation.
     * \param iso the mapping from \a originalTri to \a newTri.
     * \param newTri the triangulation to be used by the new annulus
     * representation.
     */
    SatAnnulus image(const Triangulation<3>& originalTri,
            const Isomorphism<3>& iso, const Triangulation<3>& newTri) const;

    /**
     * Attaches a layered solid torus to the given saturated annulus.
     * Instead of passing a SatAnnulus (which only offers const access
     * to the underlying triangluation), you must pass the individual
     * tetrahedra and permutations that describe it.
     *
     * The layered solid torus will be attached so that the
     * given values \a alpha and \a beta describe how the
     * meridinal disc cuts the vertical and horizontal edges of the
     * annulus respectively.
     *
     * The result will effectively insert an (\a alpha, \a beta)
     * exceptional fibre into the Seifert fibred space space, where
     * the vertical edges run parallel to the fibres and the horizontal
     * edges represent the base orbifold.  The sign of the fibre is
     * consistent with the fibre inserted by SatLST::adjustSFS()
     * (in particular, negating \a beta will negate the fibre).
     *
     * In the case of a (2,1) fibre, the layered solid torus will be
     * degenerate (i.e., the two triangles of the annulus will simply be
     * joined together).
     *
     * \pre The given value \a alpha is not zero.
     * \pre The given values \a alpha and \a beta are coprime.
     * \pre The two faces of the given saturated annulus (i.e., face
     * `r0[3]` of \a t0 and face `r1[3]` of \a t1) are boundary faces.
     *
     * \exception InvalidArgument At least one of the conditions above fails;
     * that is, either \a alpha is zero, or \a alpha and \a beta are not
     * coprime, or at least one of the two faces of the saturated annulus is
     * already joined to something.  Note that the operation may already be
     * partially complete by the time the exception is thrown (i.e., the
     * layered solid torus might be partially constructed and/or might be
     * already attached to one face of the annulus that was indeed boundary).
     *
     * \exception LockViolation At least one face of the given saturated
     * annulus (i.e., face `r0[3]` of \a t0 and/or face `r1[3]` of \a t1) is
     * locked.  See Simplex<3>::lockFacet() for further details on how facet
     * locks work and what their implications are.  Note that the operation
     * may already be partially complete by the time the exception is thrown
     * (i.e., the layered solid torus might be already constructed and/or
     * might be attached to one face of the annulus that was indeed unlocked).
     *
     * \param t0 the tetrahedron corresponding to SatAnnulus::tet[0].
     * \param r0 the permutation corresponding to SatAnnulus::roles[0].
     * \param t1 the tetrahedron corresponding to SatAnnulus::tet[1].
     * \param r1 the permutation corresponding to SatAnnulus::roles[1].
     * \param alpha describes how the meridinal disc of the torus should
     * cut the vertical edges.  This may be positive or negative.
     * \param beta describes how the meridinal disc of the torus should
     * cut the horizontal edges.  Again this may be positive or negative.
     */
    static void attachLST(Tetrahedron<3>* t0, Perm<4> r0,
        Tetrahedron<3>* t1, Perm<4> r1, long alpha, long beta);
};

// Inline functions for SatAnnulus

inline SatAnnulus::SatAnnulus() : tet { nullptr, nullptr } {
}

inline SatAnnulus::SatAnnulus(const Tetrahedron<3>* t0, Perm<4> r0,
        const Tetrahedron<3>* t1, Perm<4> r1) {
    tet[0] = t0; tet[1] = t1;
    roles[0] = r0; roles[1] = r1;
}

inline bool SatAnnulus::operator == (const SatAnnulus& other) const {
    return (tet[0]->index() == other.tet[0]->index() &&
            tet[1]->index() == other.tet[1]->index() &&
            roles[0] == other.roles[0] && roles[1] == other.roles[1]);
}

inline SatAnnulus SatAnnulus::otherSide() const {
    SatAnnulus a(*this);
    a.switchSides();
    return a;
}

inline void SatAnnulus::reflectVertical() {
    roles[0] = roles[0] * Perm<4>(0, 1);
    roles[1] = roles[1] * Perm<4>(0, 1);
}

inline SatAnnulus SatAnnulus::verticalReflection() const {
    return SatAnnulus(tet[0], roles[0] * Perm<4>(0, 1),
                       tet[1], roles[1] * Perm<4>(0, 1));
}

inline void SatAnnulus::reflectHorizontal() {
    std::swap(tet[0], tet[1]);

    Perm<4> r = roles[0];
    roles[0] = roles[1] * Perm<4>(0, 1);
    roles[1] = r * Perm<4>(0, 1);
}

inline SatAnnulus SatAnnulus::horizontalReflection() const {
    return SatAnnulus(tet[1], roles[1] * Perm<4>(0, 1),
                       tet[0], roles[0] * Perm<4>(0, 1));
}

inline void SatAnnulus::rotateHalfTurn() {
    std::swap(tet[0], tet[1]);

    Perm<4> r = roles[0];
    roles[0] = roles[1];
    roles[1] = r;
}

inline SatAnnulus SatAnnulus::halfTurnRotation() const {
    return SatAnnulus(tet[1], roles[1], tet[0], roles[0]);
}

inline SatAnnulus SatAnnulus::image(const Triangulation<3>& originalTri,
        const Isomorphism<3>& iso, const Triangulation<3>& newTri) const {
    SatAnnulus a(*this);
    a.transform(originalTri, iso, newTri);
    return a;
}

} // namespace regina

#endif

