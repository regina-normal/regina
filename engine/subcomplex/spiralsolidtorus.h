
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

/*! \file subcomplex/spiralsolidtorus.h
 *  \brief Deals with spiralled solid tori in a triangulation.
 */

#ifndef __SPIRALSOLIDTORUS_H
#ifndef __DOXYGEN
#define __SPIRALSOLIDTORUS_H
#endif

#include "regina-core.h"
#include "maths/perm.h"
#include "subcomplex/standardtri.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a spiralled solid torus in a triangulation.
 *
 * A spiralled solid torus is created by placing tetrahedra one upon
 * another in a spiralling fashion to form a giant loop.
 *
 * For each tetrahedron, label the vertices A, B, C and D.  Draw the
 * tetrahedron so that the vertices form an upward spiral in the order
 * A-B-C-D, with D directly above A.  Face BCD is on the top, face ABC
 * is on the bottom and faces ABD and ACD are both vertical.
 *
 * When joining two tetrahedra, face BCD of the lower tetrahedron will
 * be joined to face ABC of the upper tetrahedron.  In this way the
 * tetrahedra are placed one upon another to form a giant loop (which is
 * closed up by placing the bottommost tetrahedron above the topmost
 * tetrahedron in a similar fashion), forming a solid torus overall.
 *
 * In each tetrahedron, directed edges AB, BC and CD are <i>major edges</i>,
 * directed edges AC and BD are <i>minor edges</i> and directed edge AD
 * is an <i>axis edge</i>.
 *
 * The major edges all combined form a single longitude of the solid
 * torus.  Using this directed longitude, using the directed meridinal curve
 * ACBA and assuming the spiralled solid torus contains \a n tetrahedra,
 * the minor edges all combined form a (2, <i>n</i>) curve and
 * the axis edges all combined form a (3, <i>n</i>) curve on the torus
 * boundary.
 *
 * Note that all tetrahedra in the spiralled solid torus must be distinct
 * and there must be at least one tetrahedron.
 *
 * Note also that class TriSolidTorus represents a spiralled solid
 * torus with precisely three tetrahedra.  A spiralled solid torus with
 * only one tetrahedron is in fact a (1,2,3) layered solid torus.
 *
 * All optional StandardTriangulation routines are implemented for this
 * class.
 */
class REGINA_API SpiralSolidTorus : public StandardTriangulation {
    private:
        size_t nTet;
            /**< The number of tetrahedra in this spiralled solid torus. */
        Tetrahedron<3>** tet;
            /**< The tetrahedra that make up this spiralled solid torus. */
        Perm<4>* vertexRoles_;
            /**< For tetrahedron \a i, <tt>vertexRoles[i]</tt> is a
                 permutation p chosen so that vertices A, B, C and D above
                 correspond to vertices p[0], p[1], p[2] and p[3]. */

    public:
        /**
         * Destroys this spiralled solid torus.
         */
        virtual ~SpiralSolidTorus();
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        SpiralSolidTorus* clone() const;

        /**
         * Returns the number of tetrahedra in this spiralled solid torus.
         *
         * @return the number of tetrahedra.
         */
        size_t size() const;

        /**
         * Returns the requested tetrahedron in this spiralled solid torus.
         * Tetrahedra are numbered from 0 to size()-1
         * inclusive, with tetrahedron <i>i</i>+1 being placed above
         * tetrahedron <i>i</i>.
         *
         * @param index specifies which tetrahedron to return; this must
         * be between 0 and size()-1 inclusive.
         * @return the requested tetrahedron.
         */
        Tetrahedron<3>* tetrahedron(size_t index) const;

        /**
         * Returns a permutation represeting the role that each vertex
         * of the requested tetrahedron plays in the solid torus.
         * The permutation returned (call this <tt>p</tt>) maps 0, 1, 2 and
         * 3 to the four vertices of tetrahedron \a index so that
         * vertices <tt>p[0]</tt>, <tt>p[1]</tt>, <tt>p[2]</tt> and
         * <tt>p[3]</tt> correspond to vertices A, B, C and D
         * respectively as described in the general class notes.
         *
         * In particular, the directed edge from vertex
         * <tt>p[0]</tt> to <tt>p[3]</tt> is an axis edge,
         * directed edges <tt>p[0]</tt> to <tt>p[2]</tt> and
         * <tt>p[1]</tt> to <tt>p[3]</tt> are minor edges and
         * the directed path from vertices <tt>p[0]</tt> to <tt>p[1]</tt>
         * to <tt>p[2]</tt> to <tt>p[3]</tt> follows the three
         * major edges.
         *
         * See the general class notes for further details.
         *
         * @param index specifies which tetrahedron in the solid torus
         * to examine; this must be between 0 and size()-1 inclusive.
         * @return a permutation representing the roles of the vertices
         * of the requested tetrahedron.
         */
        Perm<4> vertexRoles(size_t index) const;

        /**
         * Reverses this spiralled solid torus.
         * Tetrahedra 0, 1, 2, ..., size()-1 will become tetrahedra
         * size()-1, ..., 2, 1, 0 respectively.  Note that this operation
         * will change the vertex roles as well.
         *
         * The underlying triangulation is not changed; all that changes
         * is how this spiralled solid torus is represented.
         */
        void reverse();

        /**
         * Cycles this spiralled solid torus by the given number of
         * tetrahedra.
         * Tetrahedra <i>k</i>, <i>k</i>+1, <i>k</i>+2 and so on
         * will become tetrahedra 0, 1, 2 and so on respectively.
         * Note that this operation will not change the vertex roles.
         *
         * The underlying triangulation is not changed; all that changes
         * is how this spiralled solid torus is represented.
         *
         * @param k the number of tetrahedra through which we should cycle.
         */
        void cycle(size_t k);

        /**
         * Converts this spiralled solid torus into its canonical
         * representation.  The canonical representation of a spiralled
         * solid torus is unique in a given triangulation.
         *
         * Tetrahedron 0 in the spiralled solid torus will be the
         * tetrahedron with the lowest index in the triangulation, and
         * under permutation <tt>vertexRoles(0)</tt> the image of 0
         * will be less than the image of 3.
         *
         * @param tri the triangulation in which this solid torus lives.
         * @return \c true if and only if the representation of this
         * spiralled solid torus was actually changed.
         */
        bool makeCanonical(const Triangulation<3>* tri);

        /**
         * Determines whether this spiralled solid torus is in canonical
         * form.  Canonical form is described in detail in the
         * description for makeCanonical().
         *
         * @param tri the triangulation in which this solid torus lives.
         * @return \c true if and only if this spiralled solid torus is
         * in canonical form.
         */
        bool isCanonical(const Triangulation<3>* tri) const;

        /**
         * Determines if the given tetrahedron forms part of a
         * spiralled solid torus with its vertices
         * playing the given roles in the solid torus.
         *
         * Note that the boundary triangles of the spiralled solid
         * torus need not be boundary triangles within the overall
         * triangulation, i.e., they may be identified with each other
         * or with triangles of other tetrahedra.
         *
         * @param tet the tetrahedron to examine.
         * @param useVertexRoles a permutation describing the role each
         * tetrahedron vertex must play in the solid torus; this must be
         * in the same format as the permutation returned by vertexRoles().
         *
         * @return a newly created structure containing details of the
         * solid torus with the given tetrahedron as tetrahedron 0, or
         * \c null if the given tetrahedron is not part of a spiralled
         * solid torus with the given vertex roles.
         */
        static SpiralSolidTorus* formsSpiralSolidTorus(Tetrahedron<3>* tet,
                Perm<4> useVertexRoles);

        Manifold* manifold() const;
        AbelianGroup* homology() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new partially initialised structure.
         * Member \a nTet will be initialised and dynamic arrays
         * \a tet and \a vertexRoles will be created.
         *
         * @param newNTet the number of tetrahedra in this spiralled
         * solid torus; this must be strictly positive.
         */
        SpiralSolidTorus(size_t newNTet);
};

/*@}*/

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSpiralSolidTorus has now been renamed to
 * SpiralSolidTorus.
 */
REGINA_DEPRECATED typedef SpiralSolidTorus NSpiralSolidTorus;

// Inline functions for SpiralSolidTorus

inline SpiralSolidTorus::SpiralSolidTorus(size_t newNTet) :
        nTet(newNTet), tet(new Tetrahedron<3>*[newNTet]),
        vertexRoles_(new Perm<4>[newNTet]) {
}

inline SpiralSolidTorus::~SpiralSolidTorus() {
    delete[] tet;
    delete[] vertexRoles_;
}

inline size_t SpiralSolidTorus::size() const {
    return nTet;
}

inline Tetrahedron<3>* SpiralSolidTorus::tetrahedron(size_t index) const {
    return tet[index];
}
inline Perm<4> SpiralSolidTorus::vertexRoles(size_t index) const {
    return vertexRoles_[index];
}

inline std::ostream& SpiralSolidTorus::writeName(std::ostream& out) const {
    return out << "Spiral(" << nTet << ')';
}
inline std::ostream& SpiralSolidTorus::writeTeXName(std::ostream& out) const {
    return out << "\\mathit{Spiral}(" << nTet << ')';
}
inline void SpiralSolidTorus::writeTextLong(std::ostream& out) const {
    out << nTet << "-tetrahedron spiralled solid torus";
}

} // namespace regina

#endif

