
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

/*! \file subcomplex/layeredsolidtorus.h
 *  \brief Deals with layered solid tori in a triangulation.
 */

#ifndef __LAYEREDSOLIDTORUS_H
#ifndef __DOXYGEN
#define __LAYEREDSOLIDTORUS_H
#endif

#include "regina-core.h"
#include "subcomplex/standardtri.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a layered solid torus in a triangulation.
 * A layered solid torus must contain at least one tetrahedron.
 *
 * Note that this class \b only represents layered solid tori with a
 * (3,2,1) at their base.  Thus triangulations that begin with a
 * degenerate (2,1,1) mobius strip and layer over the mobius strip
 * boundary (including the minimal (1,1,0) triangulation) are not
 * described by this class.
 *
 * All optional StandardTriangulation routines are implemented for this
 * class.
 */
class REGINA_API LayeredSolidTorus : public StandardTriangulation {
    private:
        size_t nTetrahedra;
            /**< The number of tetrahedra in this torus. */

        Tetrahedron<3>* base_;
            /**< The tetrahedron that is glued to itself at the base of
                 this torus. */
        int baseEdge_[6];
            /**< The edges of the base tetrahedron that are identified as
                 a group of 1, 2 or 3 according to whether the index is
                 0, 1-2 or 3-5 respectively.  See baseEdge() for
                 further details. */
        int baseEdgeGroup_[6];
            /**< Classifies the edges of the base tetrahedron according
                 to whether they are identified in a group of 1, 2 or 3. */
        int baseFace_[2];
            /**< The two faces of the base tetrahedron that are glued to
                 each other. */

        Tetrahedron<3>* topLevel_;
            /**< The tetrahedron on the boundary of this torus. */
        int topEdge_[3][2];
            /**< Returns the edges of the top tetrahedron that the meridinal
                 disc cuts fewest, middle or most times according to whether
                 the first index is 0, 1 or 2 respectively.  See topEdge()
                 for further details. */
        unsigned long meridinalCuts_[3];
            /**< Returns the number of times the meridinal disc cuts each
                 boundary edge; this array is in non-decreasing order. */
        int topEdgeGroup_[6];
            /**< Classifies the edges of the boundary tetrahedron
                 according to whether the meridinal disc cuts them fewest,
                 middle or most times. */
        int topFace_[2];
            /**< The two faces of the boundary tetrahedron that form the
                 torus boundary. */

    public:
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        LayeredSolidTorus* clone() const;

        /**
         * Returns the number of tetrahedra in this layered solid torus.
         *
         * @return the number of tetrahedra.
         */
        size_t size() const;

        /**
         * Returns the tetrahedron that is glued to itself at the base of
         * this layered solid torus.
         *
         * @return the base tetrahedron.
         */
        Tetrahedron<3>* base() const;
        /**
         * Returns the requested edge of the base tetrahedron belonging
         * to the given group.  The layering identifies the six edges
         * of the base tetrahedron into a group of three, a group of two
         * and a single unidentified edge; these are referred to as
         * groups 3, 2 and 1 respectively.
         *
         * Note that <tt>baseEdgeGroup(baseEdge(group, index)) ==
         * group</tt> for all values of \c group and \c index.
         *
         * Edges <tt>baseEdge(2,0)</tt> and <tt>baseEdge(3,0)</tt>
         * will both belong to face <tt>baseFace(0)</tt>.
         * Edges <tt>baseEdge(2,1)</tt> and <tt>baseEdge(3,2)</tt>
         * will both belong to face <tt>baseFace(1)</tt>.
         *
         * @param group the group that the requested edge should belong
         * to; this must be 1, 2 or 3.
         * @param index the index within the given group of the requested
         * edge; this must be between 0 and <i>group</i>-1 inclusive.
         * Note that in group 3 the edge at index 1 is adjacent to both the
         * edges at indexes 0 and 2.
         * @return the edge number in the base tetrahedron of the
         * requested edge; this will be between 0 and 5 inclusive.
         */
        int baseEdge(int group, int index) const;
        /**
         * Returns the group that the given edge of the base tetrahedron
         * belongs to.  See baseEdge() for further details about
         * groups.
         *
         * Note that <tt>baseEdgeGroup(baseEdge(group, index)) ==
         * group</tt> for all values of \c group and \c index.
         *
         * @param edge the edge number in the base tetrahedron of the
         * given edge; this must be between 0 and 5 inclusive.
         * @return the group to which the given edge belongs; this will
         * be 1, 2 or 3.
         */
        int baseEdgeGroup(int edge) const;
        /**
         * Returns one of the two faces of the base tetrahedron that are
         * glued to each other.
         *
         * @param index specifies which of the two faces to return; this
         * must be 0 or 1.
         * @return the requested face number in the base tetrahedron;
         * this will be between 0 and 3 inclusive.
         */
        int baseFace(int index) const;

        /**
         * Returns the top level tetrahedron in this layered solid torus.
         * This is the tetrahedron that would be on the boundary of the
         * torus if the torus were the entire manifold.
         *
         * @return the top level tetrahedron.
         */
        Tetrahedron<3>* topLevel() const;
        /**
         * Returns the number of times the meridinal disc of the torus
         * cuts the top level tetrahedron edges in the given group.
         * See topEdge() for further details about groups.
         *
         * @param group the given edge group; this must be 0, 1 or 2.
         * @return the number of times the meridinal disc cuts the edges
         * in the given group.
         */
        unsigned long meridinalCuts(int group) const;
        /**
         * Returns the requested edge of the top level tetrahedron belonging
         * to the given group.  The layering reduces five of the top
         * level tetrahedron edges to three boundary edges of the solid
         * torus; this divides the five initial edges into groups of size
         * two, two and one.
         *
         * Group 0 represents the boundary edge that the meridinal disc
         * cuts fewest times.  Group 2 represents the boundary edge that
         * the meridinal disc cuts most times.  Group 1 is in the middle.
         *
         * Note that <tt>topEdgeGroup(topEdge(group, index)) ==
         * group</tt> for all values of \c group and \c index that
         * actually correspond to an edge.
         *
         * Edges <tt>topEdge(group, 0)</tt> will all belong to face
         * <tt>topFace(0)</tt>.  Edges <tt>topEdge(group, 1)</tt>
         * (if they exist) will all belong to face <tt>topFace(1)</tt>.
         *
         * @param group the group that the requested edge should belong
         * to; this must be 0, 1 or 2.
         * @param index the index within the given group of the requested
         * edge; this must be 0 or 1.  Note that one of the groups only
         * contains one tetrahedron edge, in which case this edge will be
         * stored at index 0.
         * @return the edge number in the top level tetrahedron of the
         * requested edge (between 0 and 5 inclusive), or -1 if there is
         * no such edge (only possible if the given group was the group
         * of size one and the given index was 1).
         */
        int topEdge(int group, int index) const;
        /**
         * Returns the group that the given edge of the top level
         * tetrahedron belongs to.  See topEdge() for further details
         * about groups.
         *
         * Note that <tt>topEdgeGroup(topEdge(group, index)) ==
         * group</tt> for all values of \c group and \c index that
         * actually correspond to an edge.
         *
         * @param edge the edge number in the top level tetrahedron of
         * the given edge; this must be between 0 and 5 inclusive.
         * @return the group to which the given edge belongs (0, 1 or 2),
         * or -1 if this edge does not belong to any group (only possible
         * if this is the unique edge in the top tetrahedron not on the
         * torus boundary).
         */
        int topEdgeGroup(int edge) const;
        /**
         * Returns one of the two faces of the top level tetrahedron that
         * form the boundary of this layered solid torus.
         *
         * @param index specifies which of the two faces to return; this
         * must be 0 or 1.
         * @return the requested face number in the top level tetrahedron;
         * this will be between 0 and 3 inclusive.
         */
        int topFace(int index) const;

        /**
         * Flattens this layered solid torus to a Mobius band.
         * A newly created modified triangulation is returned; the
         * original triangulation is unchanged.
         *
         * Note that there are three different ways in which this layered
         * solid torus can be flattened, corresponding to the three
         * different edges of the boundary torus that could become the
         * boundary edge of the new Mobius band.
         *
         * @param original the triangulation containing this layered
         * solid torus; this triangulation will not be changed.
         * @param mobiusBandBdry the edge group on the boundary of this
         * layered solid torus that will become the boundary of the new
         * Mobius band (the remaining edge groups will become internal
         * edges of the new Mobius band).  This must be 0, 1 or 2.
         * See topEdge() for further details about edge groups.
         * @return a newly created triangulation in which this layered
         * solid torus has been flattened to a Mobius band.
         */
        Triangulation<3>* flatten(const Triangulation<3>* original,
                int mobiusBandBdry) const;

        /**
         * Adjusts the details of this layered solid torus according to
         * the given isomorphism between triangulations.
         *
         * The given isomorphism must describe a mapping from \a originalTri
         * to \a newTri, and this layered solid torus must currently
         * refer to tetrahedra in \a originalTri.  After this routine is
         * called this structure will instead refer to the corresponding
         * tetrahedra in \a newTri (with changes in vertex/face
         * numbering also accounted for).
         *
         * \pre This layered solid torus currently refers to tetrahedra
         * in \a originalTri, and \a iso describes a mapping from
         * \a originalTri to \a newTri.
         *
         * @param originalTri the triangulation currently referenced by this
         * layered solid torus.
         * @param iso the mapping from \a originalTri to \a newTri.
         * @param newTri the triangulation to be referenced by the updated
         * layered solid torus.
         */
        void transform(const Triangulation<3>* originalTri,
                const Isomorphism<3>* iso, Triangulation<3>* newTri);

        /**
         * Determines if the given tetrahedron forms the base of a
         * layered solid torus within a triangulation.  The torus need
         * not be the entire triangulation; the top level tetrahedron of
         * the torus may be glued to something else (or to itself).
         *
         * Note that the base tetrahedron of a layered solid torus is the
         * tetrahedron furthest from the boundary of the torus, i.e. the
         * tetrahedron glued to itself with a twist.
         *
         * @param tet the tetrahedron to examine as a potential base.
         * @return a newly created structure containing details of the
         * layered solid torus, or \c null if the given tetrahedron is
         * not the base of a layered solid torus.
         */
        static LayeredSolidTorus* formsLayeredSolidTorusBase(
            Tetrahedron<3>* tet);

        /**
         * Determines if the given tetrahedron forms the top level
         * tetrahedron of a layered solid torus, with the two given
         * faces of this tetrahedron representing the boundary of the
         * layered solid torus.
         *
         * Note that the two given faces need not be boundary triangles in the
         * overall triangulation.  That is, the layered solid torus may be
         * a subcomplex of some larger triangulation.  For example, the
         * two given faces may be joined to some other tetrahedra outside
         * the layered solid torus or they may be joined to each other.
         * In fact, they may even extend this smaller layered solid torus
         * to a larger layered solid torus.
         *
         * @param tet the tetrahedron to examine as a potential top
         * level of a layered solid torus.
         * @param topFace1 the face number of the given tetrahedron that
         * should represent the first boundary triangle of the layered solid
         * torus.  This should be between 0 and 3 inclusive.
         * @param topFace2 the face number of the given tetrahedron that
         * should represent the second boundary triangle of the layered solid
         * torus.  This should be between 0 and 3 inclusive, and should
         * not be equal to \a topFace1.
         * @return a newly created structure containing details of the
         * layered solid torus, or \c null if the given tetrahedron with
         * its two faces do not form the top level of a layered solid torus.
         */
        static LayeredSolidTorus* formsLayeredSolidTorusTop(
            Tetrahedron<3>* tet, unsigned topFace1, unsigned topFace2);

        /**
         * Determines if the given triangulation component forms a
         * layered solid torus in its entirity.
         *
         * Note that, unlike formsLayeredSolidTorusBase(), this routine
         * tests for a component that is a layered solid torus with no
         * additional tetrahedra or gluings.  That is, the two boundary
         * triangles of the layered solid torus must in fact be boundary
         * triangles of the component.
         *
         * @param comp the triangulation component to examine.
         * @return a newly created structure containing details of the
         * layered solid torus, or \c null if the given component is not
         * a layered solid torus.
         */
        static LayeredSolidTorus* isLayeredSolidTorus(Component<3>* comp);

        Manifold* manifold() const;
        AbelianGroup* homology() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Create a new uninitialised structure.
         */
        LayeredSolidTorus();

        /**
         * Fills <tt>topEdge[destGroup]</tt> with the edges produced by
         * following the edges in group \c sourceGroup from the current
         * top level tetrahedron up to the next layered tetrahedron.
         *
         * Note that which edge is placed in <tt>topEdge[][0]</tt> and
         * which edge is placed in <tt>topEdge[][1]</tt> will be an
         * arbitrary decision; these may need to be switched later on.
         *
         * \pre There is a next layered tetrahedron.
         * \pre Member variables \a topLevel and \a topFace have not yet been
         * changed to reflect the next layered tetrahedron.
         * \pre The edges in group \a destGroup in the next layered
         * tetrahedron are actually layered onto the edges in group \a
         * sourceGroup in the current top level tetrahedron.
         *
         * @param sourceGroup the group in the current top level
         * tetrahedron to which the edges belong.
         * @param destGroup the group in the next layered tetrahedron to
         * which the same edges belong.
         */
        void followEdge(int destGroup, int sourceGroup);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NLayeredSolidTorus has now been renamed to
 * LayeredSolidTorus.
 */
REGINA_DEPRECATED typedef LayeredSolidTorus NLayeredSolidTorus;

/*@}*/

// Inline functions for LayeredSolidTorus

inline LayeredSolidTorus::LayeredSolidTorus() {
}

inline size_t LayeredSolidTorus::size() const {
    return nTetrahedra;
}

inline Tetrahedron<3>* LayeredSolidTorus::base() const {
    return base_;
}
inline int LayeredSolidTorus::baseEdge(int group, int index) const {
    return group == 1 ? baseEdge_[index] :
        group == 2 ? baseEdge_[1 + index] : baseEdge_[3 + index];
}
inline int LayeredSolidTorus::baseEdgeGroup(int edge) const {
    return baseEdgeGroup_[edge];
}
inline int LayeredSolidTorus::baseFace(int index) const {
    return baseFace_[index];
}

inline Tetrahedron<3>* LayeredSolidTorus::topLevel() const {
    return topLevel_;
}
inline unsigned long LayeredSolidTorus::meridinalCuts(int group) const {
    return meridinalCuts_[group];
}
inline int LayeredSolidTorus::topEdge(int group, int index) const {
    return topEdge_[group][index];
}
inline int LayeredSolidTorus::topEdgeGroup(int edge) const {
    return topEdgeGroup_[edge];
}
inline int LayeredSolidTorus::topFace(int index) const {
    return topFace_[index];
}

inline std::ostream& LayeredSolidTorus::writeName(std::ostream& out) const {
    return out << "LST(" << meridinalCuts_[0] << ',' << meridinalCuts_[1] << ','
        << meridinalCuts_[2] << ')';
}
inline std::ostream& LayeredSolidTorus::writeTeXName(std::ostream& out) const {
    return out << "\\mathop{\\rm LST}(" << meridinalCuts_[0] << ','
        << meridinalCuts_[1] << ',' << meridinalCuts_[2] << ')';
}
inline void LayeredSolidTorus::writeTextLong(std::ostream& out) const {
    out << "( " << meridinalCuts_[0] << ", " << meridinalCuts_[1] << ", "
        << meridinalCuts_[2] << " ) layered solid torus";
}

} // namespace regina

#endif

