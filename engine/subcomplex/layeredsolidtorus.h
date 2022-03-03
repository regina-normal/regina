
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

/*! \file subcomplex/layeredsolidtorus.h
 *  \brief Deals with layered solid tori in a triangulation.
 */

#ifndef __REGINA_LAYEREDSOLIDTORUS_H
#ifndef __DOXYGEN
#define __REGINA_LAYEREDSOLIDTORUS_H
#endif

#include "regina-core.h"
#include "maths/perm.h"
#include "subcomplex/standardtri.h"
#include "triangulation/facepair.h"

namespace regina {

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
class LayeredSolidTorus : public StandardTriangulation {
    private:
        size_t size_;
            /**< The number of tetrahedra in this torus. */

        const Tetrahedron<3>* base_;
            /**< The tetrahedron that is glued to itself at the base of
                 this torus. */
        const Tetrahedron<3>* top_;
            /**< The tetrahedron on the boundary of this torus. */

        Perm<6> baseEdge_;
            /**< Edges baseEdge_[0..5] of the base tetrahedron are identified
                 as a group of 1, 2, 2, 3, 3, 3 respectively.
                 See baseEdge() for further details. */
        Perm<6> topEdge_;
            /**< Edges topEdge_[0,1], topEdge_[2,3] and topEdge_[4,5] are the
                 boundary edges of the top tetrahedron that the meridinal disc
                 fewest, middle and most times respectively.  As an exception,
                 one of the edges is *not* on the boundary; this will
                 be put in the group with only one edge, and will correspond to
                 index 1, 3 or 5.  See topEdge() for further details. */

        FacePair baseFace_;
            /**< The two faces of the base tetrahedron that are glued to
                 each other. */
        FacePair topFace_;
            /**< The two faces of the boundary tetrahedron that form the
                 torus boundary. */

        unsigned long meridinalCuts_[3];
            /**< Returns the number of times the meridinal disc cuts each
                 boundary edge; this array is in non-decreasing order. */

    public:
        /**
         * Creates a new copy of this structure.
         */
        LayeredSolidTorus(const LayeredSolidTorus&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * @return a reference to this structure.
         */
        LayeredSolidTorus& operator = (const LayeredSolidTorus&) = default;

        /**
         * Deprecated routine that returns a new copy of this structure.
         *
         * \deprecated Just use the copy constructor instead.
         *
         * @return a newly created clone.
         */
        [[deprecated]] LayeredSolidTorus* clone() const;

        /**
         * Swaps the contents of this and the given structure.
         *
         * @param other the structure whose contents should be swapped
         * with this.
         */
        void swap(LayeredSolidTorus& other) noexcept;

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
        const Tetrahedron<3>* base() const;
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
        const Tetrahedron<3>* topLevel() const;
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
         * Determines whether this and the given object represent the same
         * type of layered solid torus.
         *
         * Specifically, two layered solid tori will compare as equal if
         * and only if each has the same ordered triple of integer
         * parameters (describing how many times the three top-level edge
         * groups cut the meridinal disc).
         *
         * Note that it is possible for two non-isomorphic layered solid tori
         * to compare as equal, since these integer parameters do not
         * detect the presence of redundant layerings (i.e., consecutive
         * layerings that topologically cancel each other out).
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass, as noted above, is weaker than combinatorial isomorphism).
         *
         * @param other the layered solid torus to compare with this.
         * @return \c true if and only if this and the given object
         * represent the same type of layered solid torus.
         */
        bool operator == (const LayeredSolidTorus& other) const;

        /**
         * Determines whether this and the given object do not represent the
         * same type of layered solid torus.
         *
         * Specifically, two layered solid tori will compare as equal if
         * and only if each has the same ordered triple of integer
         * parameters (describing how many times the three top-level edge
         * groups cut the meridinal disc).
         *
         * Note that it is possible for two non-isomorphic layered solid tori
         * to compare as equal, since these integer parameters do not
         * detect the presence of redundant layerings (i.e., consecutive
         * layerings that topologically cancel each other out).
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass, as noted above, is weaker than combinatorial isomorphism).
         *
         * @param other the layered solid torus to compare with this.
         * @return \c true if and only if this and the given object
         * do not represent the same type of layered solid torus.
         */
        bool operator != (const LayeredSolidTorus& other) const;

        /**
         * Flattens this layered solid torus to a Mobius band.
         * A new modified triangulation is returned; the original triangulation
         * that contains this layered solid torus will be left unchanged.
         *
         * Note that there are three different ways in which this layered
         * solid torus can be flattened, corresponding to the three
         * different edges of the boundary torus that could become the
         * boundary edge of the new Mobius band.
         *
         * @param mobiusBandBdry the edge group on the boundary of this
         * layered solid torus that will become the boundary of the new
         * Mobius band (the remaining edge groups will become internal
         * edges of the new Mobius band).  This must be 0, 1 or 2.
         * See topEdge() for further details about edge groups.
         * @return a new triangulation in which this layered solid torus has
         * been flattened to a Mobius band.
         */
        Triangulation<3> flatten(int mobiusBandBdry) const;

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
        void transform(const Triangulation<3>& originalTri,
                const Isomorphism<3>& iso, const Triangulation<3>& newTri);

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
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * @param tet the tetrahedron to examine as a potential base.
         * @return a structure containing details of the layered solid torus,
         * or \c null if the given tetrahedron is not the base of a
         * layered solid torus.
         */
        static std::unique_ptr<LayeredSolidTorus> recogniseFromBase(
            const Tetrahedron<3>* tet);
        /**
         * A deprecated alias to recognise if a tetrahedron forms the
         * base of a layered solid torus.
         *
         * \deprecated This function has been renamed to recogniseFromBase().
         * See recognise() for details on the parameters and return value.
         */
        [[deprecated]] static std::unique_ptr<LayeredSolidTorus>
            formsLayeredSolidTorusBase(const Tetrahedron<3>* tet);

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
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
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
         * @return a structure containing details of the layered solid torus,
         * or \c null if the given tetrahedron with its two faces do not form
         * the top level of a layered solid torus.
         */
        static std::unique_ptr<LayeredSolidTorus> recogniseFromTop(
            const Tetrahedron<3>* tet, unsigned topFace1, unsigned topFace2);
        /**
         * A deprecated alias to recognise if a tetrahedron forms the
         * top level of a layered solid torus.
         *
         * \deprecated This function has been renamed to recogniseFromTop().
         * See recognise() for details on the parameters and return value.
         */
        [[deprecated]] static std::unique_ptr<LayeredSolidTorus>
            formsLayeredSolidTorusTop(const Tetrahedron<3>* tet,
            unsigned topFace1, unsigned topFace2);

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
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * @param comp the triangulation component to examine.
         * @return a structure containing details of the layered solid torus,
         * or \c null if the given component is not a layered solid torus.
         */
        static std::unique_ptr<LayeredSolidTorus> recognise(
            Component<3>* comp);
        /**
         * A deprecated alias to recognise if a component forms a
         * layered solid torus.
         *
         * \deprecated This function has been renamed to recognise().
         * See recognise() for details on the parameters and return value.
         */
        [[deprecated]] static std::unique_ptr<LayeredSolidTorus>
            isLayeredSolidTorus(Component<3>* comp);

        std::unique_ptr<Manifold> manifold() const override;
        AbelianGroup homology() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextShort(std::ostream& out) const override;

    private:
        /**
         * Create a new uninitialised structure.
         */
        LayeredSolidTorus() = default;
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls LayeredSolidTorus::swap(); it is provided
 * so that LayeredSolidTorus meets the C++ Swappable requirements.
 *
 * @param a the first structure whose contents should be swapped.
 * @param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(LayeredSolidTorus& a, LayeredSolidTorus& b) noexcept;

// Inline functions for LayeredSolidTorus

inline LayeredSolidTorus* LayeredSolidTorus::clone() const {
    return new LayeredSolidTorus(*this);
}

inline void LayeredSolidTorus::swap(LayeredSolidTorus& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(base_, other.base_);
    std::swap(top_, other.top_);
    std::swap(baseEdge_, other.baseEdge_);
    std::swap(topEdge_, other.topEdge_);
    std::swap(baseFace_, other.baseFace_);
    std::swap(topFace_, other.topFace_);
    std::swap_ranges(meridinalCuts_, meridinalCuts_ + 3, other.meridinalCuts_);
}

inline size_t LayeredSolidTorus::size() const {
    return size_;
}

inline const Tetrahedron<3>* LayeredSolidTorus::base() const {
    return base_;
}
inline int LayeredSolidTorus::baseEdge(int group, int index) const {
    return group == 1 ? baseEdge_[index] :
        group == 2 ? baseEdge_[1 + index] : baseEdge_[3 + index];
}
inline int LayeredSolidTorus::baseEdgeGroup(int edge) const {
    int pre = baseEdge_.pre(edge);
    return (pre == 0 ? 1 : pre < 3 ? 2 : 3);
}
inline int LayeredSolidTorus::baseFace(int index) const {
    return (index == 0 ? baseFace_.lower() : baseFace_.upper());
}

inline const Tetrahedron<3>* LayeredSolidTorus::topLevel() const {
    return top_;
}
inline unsigned long LayeredSolidTorus::meridinalCuts(int group) const {
    return meridinalCuts_[group];
}
inline int LayeredSolidTorus::topEdge(int group, int index) const {
    int ans = topEdge_[2 * group + index];
    return (ans == topFace_.oppositeEdge() ? -1 : ans);
}
inline int LayeredSolidTorus::topEdgeGroup(int edge) const {
    return (edge == topFace_.oppositeEdge() ? -1 : (topEdge_.pre(edge) / 2));
}
inline int LayeredSolidTorus::topFace(int index) const {
    return (index == 0 ? topFace_.lower() : topFace_.upper());
}

inline bool LayeredSolidTorus::operator == (const LayeredSolidTorus& other)
        const {
    return std::equal(meridinalCuts_, meridinalCuts_ + 3,
        other.meridinalCuts_);
}

inline bool LayeredSolidTorus::operator != (const LayeredSolidTorus& other)
        const {
    return ! std::equal(meridinalCuts_, meridinalCuts_ + 3,
        other.meridinalCuts_);
}

inline std::ostream& LayeredSolidTorus::writeName(std::ostream& out) const {
    return out << "LST(" << meridinalCuts_[0] << ',' << meridinalCuts_[1] << ','
        << meridinalCuts_[2] << ')';
}
inline std::ostream& LayeredSolidTorus::writeTeXName(std::ostream& out) const {
    return out << "\\mathop{\\rm LST}(" << meridinalCuts_[0] << ','
        << meridinalCuts_[1] << ',' << meridinalCuts_[2] << ')';
}

inline std::unique_ptr<LayeredSolidTorus>
        LayeredSolidTorus::formsLayeredSolidTorusBase(
        const Tetrahedron<3>* tet) {
    return recogniseFromBase(tet);
}

inline std::unique_ptr<LayeredSolidTorus>
        LayeredSolidTorus::formsLayeredSolidTorusTop(
        const Tetrahedron<3>* tet, unsigned topFace1, unsigned topFace2) {
    return recogniseFromTop(tet, topFace1, topFace2);
}

inline std::unique_ptr<LayeredSolidTorus>
        LayeredSolidTorus::isLayeredSolidTorus(Component<3>* comp) {
    return recognise(comp);
}

inline void swap(LayeredSolidTorus& a, LayeredSolidTorus& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

