
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

/*! \file ntrisolidtorus.h
 *  \brief Deals with triangular solid tori in a triangulation.
 */

#ifndef __NTRISOLIDTORUS_H
#ifndef __DOXYGEN
#define __NTRISOLIDTORUS_H
#endif

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "shareableobject.h"
    #include "nperm.h"
#else
    #include "engine/shareableobject.h"
    #include "engine/triangulation/nperm.h"
#endif

class NTetrahedron;

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
 * major axes together form a loop on the boundary torus.  This loop can
 * be oriented to run around the solid torus in the same direction as
 * the axis; this then induces an orientation on the boundary of a
 * meridinal disc.  Thus, using an axis edge as longitude, the three major
 * edges will together form a (1,1) curve on the boundary torus.
 *
 * We can now orient the minor axes so they run around the solid torus
 * in a direction \a opposite to that of the axis, together forming a
 * (-2, 1) curve on the boundary torus.
 *
 * Finally, the three tetrahedra can be numbered 0, 1 and 2 in an order
 * that follows the axis, and the annuli can be numbered 0, 1 and 2 in
 * an order that follows the meridinal disc boundary so that annulus
 * \a i does not use any faces from tetrahedron \a i.
 *
 * Note that all three tetrahedra in the triangular solid torus must be
 * distinct.
 */
class NTriSolidTorus : public ShareableObject {
    private:
        NTetrahedron* tet[3];
            /**< The tetrahedra that make up this solid torus. */
        NPerm vertexRoles[3];
            /**< For tetrahedron \a i, <tt>vertexRoles[i]</tt> is a
                 permutation p chosen so that the axis edge for that
                 tetrahedron runs from vertex p[0] to p[1] and the
                 major edge opposite that axis edge runs from vertex p[2]
                 to p[3]. */

    public:
        /**
         * Destroys this solid torus.
         */
        virtual ~NTriSolidTorus();
        /**
         * Returns a newly created clone of this structure.
         *
         * \ifaces This routine is named \a cloneMe.
         *
         * @return a newly created clone.
         */
        NTriSolidTorus* clone() const;

        /**
         * Returns the requested tetrahedron in this solid torus.
         * See the general class notes for further details.
         *
         * @param index specifies which tetrahedron in the solid torus
         * to return; this must be 0, 1 or 2.
         * @return the requested tetrahedron.
         */
        NTetrahedron* getTetrahedron(int index) const;

        /**
         * Returns a permutation represeting the role that each vertex
         * of the requested tetrahedron plays in the solid torus.
         * The permutation returned (call this <tt>p</tt>) maps 0, 1, 2 and
         * 3 to the four vertices of tetrahedron \a index so that the edge
         * from <tt>p[0]</tt> to <tt>p[1]</tt> is an oriented axis
         * edge, and the path from vertices <tt>p[0]</tt> to <tt>p[2]</tt>
         * to <tt>p[3]</tt> to <tt>p[1]</tt> follows the three oriented
         * major edges.  In particular, the major edge for annulus
         * \a index will run from vertices <tt>p[2]</tt> to <tt>p[3]</tt>.
         * Edges <tt>p[1]</tt> to <tt>p[2]</tt> and <tt>p[3]</tt> to
         * <tt>p[0]</tt> will both be oriented minor edges.
         *
         * Note that annulus <tt>index+1</tt> uses face <tt>p[2]</tt> of
         * the requested tetrahedron and annulus <tt>index+2</tt> uses
         * face <tt>p[3]</tt> of the requested tetrahedron.  Both annuli
         * use the axis edge <tt>p[0]</tt> to <tt>p[1]</tt>, and each
         * annulus uses one other major edge and one other minor edge so
         * that (according to homology) the axis edge equals the major
         * edge minus the minor edge.
         *
         * See the general class notes for further details.
         *
         * @param index specifies which tetrahedron in the solid torus
         * to examine; this must be 0, 1 or 2.
         * @return a permutation representing the roles of the vertices
         * of the requested tetrahedron.
         */
        NPerm getVertexRoles(int index) const;

        /**
         * Determines if the given tetrahedron forms part of a
         * three-tetrahedron triangular solid torus with its vertices
         * playing the given roles in the solid torus.
         *
         * \ifaces This routine is a member of class Engine.
         *
         * @param tet the tetrahedron to examine.
         * @param useVertexRoles a permutation describing the role each
         * tetrahedron vertex must play in the solid torus; this must be
         * in the same format as the permutation returned by
         * getVertexRoles().
         * @return a newly created structure containing details of the
         * solid torus with the given tetrahedron as tetrahedron 0, or
         * \c null if the given tetrahedron is not part of a triangular
         * solid torus with the given vertex roles.
         */
        static NTriSolidTorus* isTriSolidTorus(NTetrahedron* tet,
                NPerm useVertexRoles);

        void writeTextShort(ostream& out) const;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        NTriSolidTorus();
};

// Inline functions for NTriSolidTorus

inline NTriSolidTorus::NTriSolidTorus() {
}
inline NTriSolidTorus::~NTriSolidTorus() {
}

inline NTetrahedron* NTriSolidTorus::getTetrahedron(int index) const {
    return tet[index];
}
inline NPerm NTriSolidTorus::getVertexRoles(int index) const {
    return vertexRoles[index];
}

inline void NTriSolidTorus::writeTextShort(ostream& out) const {
    out << "3-tetrahedron triangular solid torus";
}

#endif

