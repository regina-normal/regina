
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

/*! \file nlayeredsolidtorus.h
 *  \brief Deals with layered solid tori in a triangulation.
 */

#ifndef __NLAYEREDSOLIDTORUS_H
#ifndef __DOXYGEN
#define __NLAYEREDSOLIDTORUS_H
#endif

#include "subcomplex/nstandardtri.h"

namespace regina {

class NTetrahedron;

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
 * All optional NStandardTriangulation routines are implemented for this
 * class.
 */
class NLayeredSolidTorus : public NStandardTriangulation {
    private:
        unsigned long nTetrahedra;
            /**< The number of tetrahedra in this torus. */

        NTetrahedron* base;
            /**< The tetrahedron that is glued to itself at the base of
                 this torus. */
        int baseEdge[6];
            /**< The edges of the base tetrahedron that are identified as
                 a group of 1, 2 or 3 according to whether the index is
                 0, 1-2 or 3-5 respectively.  See getBaseEdge() for
                 further details. */
        int baseEdgeGroup[6];
            /**< Classifies the edges of the base tetrahedron according
                 to whether they are identified in a group of 1, 2 or 3. */
        int baseFace[2];
            /**< The two faces of the base tetrahedron that are glued to
                 each other. */

        NTetrahedron* topLevel;
            /**< The tetrahedron on the boundary of this torus. */
        int topEdge[3][2];
            /**< Returns the edges of the top tetrahedron that the meridinal
                 disc cuts fewest, middle or most times according to whether
                 the first index is 0, 1 or 2 respectively.  See getTopEdge()
                 for further details. */
        unsigned long meridinalCuts[3];
            /**< Returns the number of times the meridinal disc cuts each
                 boundary edge; this array is in non-decreasing order. */
        int topEdgeGroup[6];
            /**< Classifies the edges of the boundary tetrahedron
                 according to whether the meridinal disc cuts them fewest,
                 middle or most times. */
        int topFace[2];
            /**< The two faces of the boundary tetrahedron that form the
                 torus boundary. */

    public:
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        NLayeredSolidTorus* clone() const;

        /**
         * Returns the number of tetrahedra in this layered solid torus.
         *
         * @return the number of tetrahedra.
         */
        unsigned long getNumberOfTetrahedra() const;

        /**
         * Returns the tetrahedron that is glued to itself at the base of
         * this layered solid torus.
         *
         * @return the base tetrahedron.
         */
        NTetrahedron* getBase() const;
        /**
         * Returns the requested edge of the base tetrahedron belonging
         * to the given group.  The layering identifies the six edges
         * of the base tetrahedron into a group of three, a group of two
         * and a single unidentified edge; these are referred to as
         * groups 3, 2 and 1 respectively.
         *
         * Note that <tt>getBaseEdgeGroup(getBaseEdge(group, index)) ==
         * group</tt> for all values of \c group and \c index.
         *
         * Edges <tt>getBaseEdge(2,0)</tt> and <tt>getBaseEdge(3,0)</tt>
         * will both belong to face <tt>getBaseFace(0)</tt>.
         * Edges <tt>getBaseEdge(2,1)</tt> and <tt>getBaseEdge(3,2)</tt>
         * will both belong to face <tt>getBaseFace(1)</tt>.
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
        int getBaseEdge(int group, int index) const;
        /**
         * Returns the group that the given edge of the base tetrahedron
         * belongs to.  See getBaseEdge() for further details about
         * groups.
         *
         * Note that <tt>getBaseEdgeGroup(getBaseEdge(group, index)) ==
         * group</tt> for all values of \c group and \c index.
         *
         * @param edge the edge number in the base tetrahedron of the
         * given edge; this must be between 0 and 5 inclusive.
         * @return the group to which the given edge belongs; this will
         * be 1, 2 or 3.
         */
        int getBaseEdgeGroup(int edge) const;
        /**
         * Returns one of the two faces of the base tetrahedron that are
         * glued to each other.
         *
         * @param index specifies which of the two faces to return; this
         * must be 0 or 1.
         * @return the requested face number in the base tetrahedron;
         * this will be between 0 and 3 inclusive.
         */
        int getBaseFace(int index) const;

        /**
         * Returns the top level tetrahedron in this layered solid torus.
         * This is the tetrahedron that would be on the boundary of the
         * torus if the torus were the entire manifold.
         *
         * @return the top level tetrahedron.
         */
        NTetrahedron* getTopLevel() const;
        /**
         * Returns the number of times the meridinal disc of the torus
         * cuts the top level tetrahedron edges in the given group.
         * See getTopEdge() for further details about groups.
         *
         * @param group the given edge group; this must be 0, 1 or 2.
         * @return the number of times the meridinal disc cuts the edges
         * in the given group.
         */
        unsigned long getMeridinalCuts(int group) const;
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
         * Note that <tt>getTopEdgeGroup(getTopEdge(group, index)) ==
         * group</tt> for all values of \c group and \c index that
         * actually correspond to an edge.
         *
         * Edges <tt>getTopEdge(group, 0)</tt> will all belong to face
         * <tt>getTopFace(0)</tt>.  Edges <tt>getTopEdge(group, 1)</tt>
         * (if they exist) will all belong to face <tt>getTopFace(1)</tt>.
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
        int getTopEdge(int group, int index) const;
        /**
         * Returns the group that the given edge of the top level
         * tetrahedron belongs to.  See getTopEdge() for further details
         * about groups.
         *
         * Note that <tt>getTopEdgeGroup(getTopEdge(group, index)) ==
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
        int getTopEdgeGroup(int edge) const;
        /**
         * Returns one of the two faces of the top level tetrahedron that
         * form the boundary of this layered solid torus.
         *
         * @param index specifies which of the two faces to return; this
         * must be 0 or 1.
         * @return the requested face number in the top level tetrahedron;
         * this will be between 0 and 3 inclusive.
         */
        int getTopFace(int index) const;

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
         * See getTopEdge() for further details about edge groups.
         * @return a newly created triangulation in which this layered
         * solid torus has been flattened to a Mobius band.
         */
        NTriangulation* flatten(const NTriangulation* original,
                int mobiusBandBdry) const;

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
        static NLayeredSolidTorus* formsLayeredSolidTorusBase(
            NTetrahedron* tet);

        NManifold* getManifold() const;
        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Create a new uninitialised structure.
         */
        NLayeredSolidTorus();

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

/*@}*/

// Inline functions for NLayeredSolidTorus

inline NLayeredSolidTorus::NLayeredSolidTorus() {
}

inline unsigned long NLayeredSolidTorus::getNumberOfTetrahedra() const {
    return nTetrahedra;
}

inline NTetrahedron* NLayeredSolidTorus::getBase() const {
    return base;
}
inline int NLayeredSolidTorus::getBaseEdge(int group, int index) const {
    return group == 1 ? baseEdge[index] :
        group == 2 ? baseEdge[1 + index] : baseEdge[3 + index];
}
inline int NLayeredSolidTorus::getBaseEdgeGroup(int edge) const {
    return baseEdgeGroup[edge];
}
inline int NLayeredSolidTorus::getBaseFace(int index) const {
    return baseFace[index];
}

inline NTetrahedron* NLayeredSolidTorus::getTopLevel() const {
    return topLevel;
}
inline unsigned long NLayeredSolidTorus::getMeridinalCuts(int group) const {
    return meridinalCuts[group];
}
inline int NLayeredSolidTorus::getTopEdge(int group, int index) const {
    return topEdge[group][index];
}
inline int NLayeredSolidTorus::getTopEdgeGroup(int edge) const {
    return topEdgeGroup[edge];
}
inline int NLayeredSolidTorus::getTopFace(int index) const {
    return topFace[index];
}

inline std::ostream& NLayeredSolidTorus::writeName(std::ostream& out) const {
    return out << "LST(" << meridinalCuts[0] << ',' << meridinalCuts[1] << ','
        << meridinalCuts[2] << ')';
}
inline std::ostream& NLayeredSolidTorus::writeTeXName(std::ostream& out) const {
    return out << "$\\mathop{\\rm LST}(" << meridinalCuts[0] << ','
        << meridinalCuts[1] << ',' << meridinalCuts[2] << ")$";
}
inline void NLayeredSolidTorus::writeTextLong(std::ostream& out) const {
    out << "( " << meridinalCuts[0] << ", " << meridinalCuts[1] << ", "
        << meridinalCuts[2] << " ) layered solid torus";
}

} // namespace regina

#endif

