
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file subcomplex/trisolidtorus.h
 *  \brief Deals with triangular solid tori in a triangulation.
 */

#ifndef __REGINA_TRISOLIDTORUS_H
#ifndef __DOXYGEN
#define __REGINA_TRISOLIDTORUS_H
#endif

#include <optional>
#include "regina-core.h"
#include "maths/perm.h"
#include "subcomplex/standardtri.h"
#include "triangulation/dim3.h"

namespace regina {

/**
 * Represents a three-tetrahedron triangular solid torus in a triangulation.
 * A three-tetrahedron triangular solid torus is a three-tetrahedron
 * triangular prism with its two ends identified.
 *
 * The resulting triangular solid torus will have all edges as boundary
 * edges.  Three of these will be <i>axis edges</i> (parallel to the
 * axis of the solid torus).  Between the axis edges will be three
 * annuli, each with two internal edges.  One of these internal edges will
 * meet all three tetrahedra (the \a major edge) and one of these internal
 * edges will only meet two of the tetrahedra (the \a minor edge).
 *
 * Assume the axis of the layered solid torus is oriented.  The three
 * major edges together form a loop on the boundary torus.  This loop can
 * be oriented to run around the solid torus in the same direction as
 * the axis; this then induces an orientation on the boundary of a
 * meridinal disc.  Thus, using an axis edge as longitude, the three major
 * edges will together form a (1,1) curve on the boundary torus.
 *
 * We can now orient the minor edges so they also run around the solid torus
 * in the same direction as the axis, together forming a
 * (2, -1) curve on the boundary torus.
 *
 * Finally, the three tetrahedra can be numbered 0, 1 and 2 in an order
 * that follows the axis, and the annuli can be numbered 0, 1 and 2 in
 * an order that follows the meridinal disc boundary so that annulus
 * \a i does not use any faces from tetrahedron \a i.
 *
 * Note that all three tetrahedra in the triangular solid torus must be
 * distinct.
 *
 * All optional StandardTriangulation routines are implemented for this class.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the other StandardTriangulation
 * subclasses.  Note that the only way to create these objects (aside from
 * copying or moving) is via the static member function recognise().
 *
 * \ingroup subcomplex
 */
class TriSolidTorus : public StandardTriangulation {
    private:
        Tetrahedron<3>* tet_[3];
            /**< The tetrahedra that make up this solid torus. */
        Perm<4> vertexRoles_[3];
            /**< For tetrahedron \a i, <tt>vertexRoles[i]</tt> is a
                 permutation p chosen so that the axis edge for that
                 tetrahedron runs from vertex p[0] to p[3] and the
                 major edge opposite that axis edge runs from vertex p[1]
                 to p[2]. */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        TriSolidTorus(const TriSolidTorus&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * @return a reference to this structure.
         */
        TriSolidTorus& operator = (const TriSolidTorus&) = default;

        /**
         * Swaps the contents of this and the given structure.
         *
         * @param other the structure whose contents should be swapped
         * with this.
         */
        void swap(TriSolidTorus& other) noexcept;

        /**
         * Returns the requested tetrahedron in this solid torus.
         * See the general class notes for further details.
         *
         * @param index specifies which tetrahedron in the solid torus
         * to return; this must be 0, 1 or 2.
         * @return the requested tetrahedron.
         */
        Tetrahedron<3>* tetrahedron(int index) const;

        /**
         * Returns a permutation represeting the role that each vertex
         * of the requested tetrahedron plays in the solid torus.
         * The permutation returned (call this <tt>p</tt>) maps 0, 1, 2 and
         * 3 to the four vertices of tetrahedron \a index so that the edge
         * from <tt>p[0]</tt> to <tt>p[3]</tt> is an oriented axis
         * edge, and the path from vertices <tt>p[0]</tt> to <tt>p[1]</tt>
         * to <tt>p[2]</tt> to <tt>p[3]</tt> follows the three oriented
         * major edges.  In particular, the major edge for annulus
         * \a index will run from vertices <tt>p[1]</tt> to <tt>p[2]</tt>.
         * Edges <tt>p[0]</tt> to <tt>p[2]</tt> and <tt>p[1]</tt> to
         * <tt>p[3]</tt> will both be oriented minor edges.
         *
         * Note that annulus <tt>index+1</tt> uses face <tt>p[1]</tt> of
         * the requested tetrahedron and annulus <tt>index+2</tt> uses
         * face <tt>p[2]</tt> of the requested tetrahedron.  Both annuli
         * use the axis edge <tt>p[0]</tt> to <tt>p[3]</tt>, and each
         * annulus uses one other major edge and one other minor edge so
         * that (according to homology) the axis edge equals the major
         * edge plus the minor edge.
         *
         * See the general class notes for further details.
         *
         * @param index specifies which tetrahedron in the solid torus
         * to examine; this must be 0, 1 or 2.
         * @return a permutation representing the roles of the vertices
         * of the requested tetrahedron.
         */
        Perm<4> vertexRoles(int index) const;

        /**
         * Determines whether the two triangles of the requested annulus are
         * glued to each other.
         *
         * If the two triangles are glued, this routine will return a
         * permutation describing how the vertex roles are glued to each other.
         * This will describe directly how axis edges, major edges and minor
         * edges map to each other without having to worry about the specific
         * assignment of tetrahedron vertex numbers.  For a discussion of
         * vertex roles, see vertexRoles().
         *
         * Note that annulus <tt>index</tt> uses faces
         * from tetrahedra <tt>index+1</tt> and <tt>index+2</tt>.
         * The gluing permutation that maps vertices
         * of tetrahedron <tt>index+1</tt> to vertices of tetrahedron
         * <tt>index+2</tt> will be <tt>vertexRoles(index+2) * roleMap *
         * vertexRoles(index+1).inverse()</tt>.
         *
         * @param index specifies which annulus on the solid torus
         * boundary to examine; this must be 0, 1 or 2.
         * @return a permutation that describes the gluing of vertex roles,
         * or \c nullopt if the two triangles of the requested annulus are
         * not glued together.
         */
        std::optional<Perm<4>> isAnnulusSelfIdentified(int index) const;

        /**
         * Determines whether the two given annuli are linked in a
         * particular fashion by a layered chain.
         *
         * In this scenario, both of the given annuli meet one face of
         * the top tetrahedron and one face of the bottom tetrahedron of
         * the layered chain.
         *
         * To be identified by this routine, the layered chain
         * (described by LayeredChain) must be attached as follows.
         * The two directed major edges of the two annuli should
         * correspond to the two hinge edges of the layered chain (with
         * both hinge edges pointing in the same direction around the
         * solid torus formed by the layered chain).  The two directed
         * diagonals of the layered chain (between the two top faces and
         * between the two bottom faces, each pointing in the opposite
         * direction to the hinge edges around the solid torus formed by
         * the layered chain) should be identified and must correspond
         * to the (identified) two directed minor edges of the two
         * annuli.  The remaining boundary edges of the layered chain
         * should correspond to the axis edges of the triangular solid
         * torus (this correspondence is determined by the previous
         * identifications).
         *
         * @param otherAnnulus the annulus on the solid torus boundary
         * \a not to be examined; this must be 0, 1 or 2.
         * @return the number of tetrahedra in the layered chain if the
         * two annuli are linked as described, or 0 otherwise.
         */
        size_t areAnnuliLinkedMajor(int otherAnnulus) const;

        /**
         * Determines whether the two given annuli are linked in a
         * particular fashion by a layered chain.
         *
         * In this scenario, one of the given annuli meets both faces of
         * the top tetrahedron and the other annulus meets both faces of the
         * bottom tetrahedron of the layered chain.
         *
         * To be identified by this routine, the layered chain
         * (described by LayeredChain) must be attached as follows.
         * We shall refer to the two hinge edges of the layered chain as
         * \e first and \e second.
         *
         * The two diagonals of the layered chain (between the two top
         * faces and between the two bottom faces) should correspond to
         * the two directed major edges of the two annuli, with the major
         * edges both pointing from top hinge edge to bottom hinge edge.
         * The other boundary edges of the layered chain that are not
         * hinge edges should correspond to the two directed minor edges
         * of the two annuli, with the minor edges both pointing from
         * bottom hinge edge to top hinge edge.  The hinge edges
         * themselves should correspond to the axis edges of the
         * triangular solid torus (this correspondence is determined by
         * the previous identifications; the axis edge between the two
         * annuli will be identified to both of the others in reverse).
         *
         * @param otherAnnulus the annulus on the solid torus boundary
         * \a not to be examined; this must be 0, 1 or 2.
         * @return the number of tetrahedra in the layered chain if the
         * two annuli are linked as described, or 0 otherwise.
         */
        size_t areAnnuliLinkedAxis(int otherAnnulus) const;

        /**
         * Determines whether this and the given object represent the same
         * specific presentation of a triangular solid torus.
         *
         * Unlike the parameterised subclasses of StandardTriangulation,
         * this TriSolidTorus subclass represents a fixed structure, and
         * so its comparisons test not for the \e structure but the precise
         * \e location of this structure within the enclosing triangulation.
         *
         * Specifically, two triangular solid tori will compare as equal if
         * and only if each uses the same three numbered tetrahedra, in
         * the same order, and with the same vertex roles.  That is, the
         * corresponding permutations returned by vertexRoles() must be equal,
         * and the corresponding tetrahedra returned by tetrahedron() must
         * have equal indices within the triangulation.  In particular, it is
         * still meaningful to compare triangular solid tori within different
         * triangulations.
         *
         * @param other the triangular solid torus to compare with this.
         * @return \c true if and only if this and the given object represent
         * the same specific presentation of a triangular solid torus.
         */
        bool operator == (const TriSolidTorus& other) const;

        /**
         * Determines whether this and the given object represent
         * different specific presentations of a triangular solid torus.
         *
         * Unlike the parameterised subclasses of StandardTriangulation,
         * this TriSolidTorus subclass represents a fixed structure, and
         * so its comparisons test not for the \e structure but the precise
         * \e location of this structure within the enclosing triangulation.
         *
         * Specifically, two triangular solid tori will compare as equal if
         * and only if each uses the same three numbered tetrahedra, in
         * the same order, and with the same vertex roles.  That is, the
         * corresponding permutations returned by vertexRoles() must be equal,
         * and the corresponding tetrahedra returned by tetrahedron() must
         * have equal indices within the triangulation.  In particular, it is
         * still meaningful to compare triangular solid tori within different
         * triangulations.
         *
         * @param other the triangular solid torus to compare with this.
         * @return \c true if and only if this and the given object represent
         * different specific presentations of a triangular solid torus.
         */
        bool operator != (const TriSolidTorus& other) const;

        /**
         * Determines if the given tetrahedron forms part of a
         * three-tetrahedron triangular solid torus with its vertices
         * playing the given roles in the solid torus.
         *
         * Note that the six boundary triangles of the triangular solid
         * torus need not be boundary triangles within the overall
         * triangulation, i.e., they may be identified with each other
         * or with faces of other tetrahedra.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * @param tet the tetrahedron to examine.
         * @param useVertexRoles a permutation describing the role each
         * tetrahedron vertex must play in the solid torus; this must be
         * in the same format as the permutation returned by
         * vertexRoles().
         * @return a structure containing details of the solid torus with the
         * given tetrahedron as tetrahedron 0, or \c null if the given
         * tetrahedron is not part of a triangular solid torus with the given
         * vertex roles.
         */
        static std::unique_ptr<TriSolidTorus> recognise(Tetrahedron<3>* tet,
                Perm<4> useVertexRoles);

        std::unique_ptr<Manifold> manifold() const override;
        AbelianGroup homology() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextShort(std::ostream& out) const override;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        TriSolidTorus() = default;
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls TriSolidTorus::swap(); it is provided
 * so that TriSolidTorus meets the C++ Swappable requirements.
 *
 * @param a the first structure whose contents should be swapped.
 * @param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(TriSolidTorus& a, TriSolidTorus& b) noexcept;

// Inline functions for TriSolidTorus

inline void TriSolidTorus::swap(TriSolidTorus& other) noexcept {
    std::swap_ranges(tet_, tet_ + 3, other.tet_);
    std::swap_ranges(vertexRoles_, vertexRoles_ + 3, other.vertexRoles_);
}

inline Tetrahedron<3>* TriSolidTorus::tetrahedron(int index) const {
    return tet_[index];
}
inline Perm<4> TriSolidTorus::vertexRoles(int index) const {
    return vertexRoles_[index];
}

inline bool TriSolidTorus::operator == (const TriSolidTorus& other) const {
    return
        tet_[0]->index() == other.tet_[0]->index() &&
        tet_[1]->index() == other.tet_[1]->index() &&
        tet_[2]->index() == other.tet_[2]->index() &&
        std::equal(vertexRoles_, vertexRoles_ + 3, other.vertexRoles_);
}

inline bool TriSolidTorus::operator != (const TriSolidTorus& other) const {
    return ! ((*this) == other);
}

inline std::ostream& TriSolidTorus::writeName(std::ostream& out) const {
    return out << "TST";
}
inline std::ostream& TriSolidTorus::writeTeXName(std::ostream& out) const {
    return out << "\\mathop{\\rm TST}";
}

inline void swap(TriSolidTorus& a, TriSolidTorus& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

