
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#ifndef __NBOUNDARYCOMPONENT_H
#ifndef __DOXYGEN
#define __NBOUNDARYCOMPONENT_H
#endif

/*! \file triangulation/nboundarycomponent.h
 *  \brief Deals with components of the boundary of a triangulation.
 */

#include <vector>
#include "regina-core.h"
#include "output.h"
#include "utilities/nmarkedvector.h"
#include <boost/noncopyable.hpp>
// NOTE: More #includes follow after the class declarations.

namespace regina {

class NComponent;
class NEdge;
class NTriangle;
class NTetrahedron;
class NVertex;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Represents a component of the boundary of a triangulation.
 * Note that an ideal vertex constitutes a boundary component of its
 * own.
 *
 * We can run into some interesting cases with invalid triangulations.
 * Suppose some vertex link is a multiply punctured surface (which makes
 * the vertex and hence the entire triangulation invalid).  This means
 * that different parts of the 3-manifold boundary are effectively
 * "pinched" together.  If this happens, the different parts of the
 * boundary that are pinched might or might not be listed as part of the
 * same boundary component; if not then the offending vertex will be
 * included in all of these boundary components.  Nevertheless, only one
 * of these can be considered the "official" boundary component of the
 * vertex as returned by NVertex::getBoundaryComponent().  This is all a
 * bit of a mess, but then again the entire triangulation is invalid and
 * so you almost certainly have bigger problems to deal with.
 *
 * Boundary components are highly temporary; once a triangulation
 * changes, all its boundary component objects will be deleted and new
 * ones will be created.
 */
class REGINA_API NBoundaryComponent :
        public Output<NBoundaryComponent>,
        public NMarkedElement {
    private:
        std::vector<NTriangle*> triangles_;
            /**< List of triangles in the component. */
        std::vector<NEdge*> edges_;
            /**< List of edges in the component. */
        std::vector<NVertex*> vertices_;
            /**< List of vertices in the component. */

        bool orientable_;
            /**< Is this boundary component orientable? */

    public:

        /**
         * Returns the index of this boundary component in the underlying
         * triangulation.  This is identical to calling
         * <tt>getTriangulation()->boundaryComponentIndex(this)</tt>.
         *
         * @return the index of this boundary component vertex.
         */
        unsigned long index() const;

        /**
         * Returns the number of triangles in this boundary component.
         *
         * @return the number of triangles.
         */
        unsigned long getNumberOfTriangles() const;
        /**
         * A deprecated alias for getNumberOfTriangles().
         *
         * This routine returns the number of triangular faces in this
         * boundary component.  See getNumberOfTriangles() for further details.
         *
         * \deprecated This routine will be removed in a future version
         * of Regina.  Please use getNumberOfTriangles() instead.
         *
         * @return the number of triangles.
         */
        unsigned long getNumberOfFaces() const;

        /**
         * Returns the number of edges in this boundary component.
         *
         * @return the number of edges.
         */
        unsigned long getNumberOfEdges() const;

        /**
         * Returns the number of vertices in this boundary component.
         *
         * @return the number of vertices.
         */
        unsigned long getNumberOfVertices() const;

        /**
         * Returns the requested triangle in this boundary component.
         *
         * For an ideal boundary component (which consists of a single
         * vertex), there are no real triangles in the boundary component
         * and this routine cannot be used.
         *
         * @param index the index of the requested triangle in the boundary
         * component.  This should be between 0 and getNumberOfTriangles()-1
         * inclusive.
         * Note that the index of a triangle in the boundary component need
         * not be the index of the same triangle in the entire
         * triangulation.
         * @return the requested triangle.
         */
        NTriangle* getTriangle(unsigned long index) const;
        /**
         * A deprecated alias for getTriangle().
         *
         * This routine returns the requested triangular face in this
         * boundary component.  See getTriangle() for further details.
         *
         * \deprecated This routine will be removed in a future version
         * of Regina.  Please use getTriangle() instead.
         *
         * @param index the index of the requested triangle in the boundary
         * component.  This should be between 0 and getNumberOfTriangles()-1
         * inclusive.
         * Note that the index of a triangle in the boundary component need
         * not be the index of the same triangle in the entire
         * triangulation.
         * @return the requested triangle.
         */
        NTriangle* getFace(unsigned long index) const;

        /**
         * Returns the requested edge in this boundary component.
         *
         * For an ideal boundary component (which consists of a single
         * vertex), there are no real edges in the boundary component
         * and this routine cannot be used.
         *
         * @param index the index of the requested edge in the boundary
         * component.  This should be between 0 and getNumberOfEdges()-1
         * inclusive.
         * Note that the index of a edge in the boundary component need
         * not be the index of the same edge in the entire
         * triangulation.
         * @return the requested edge.
         */
        NEdge* getEdge(unsigned long index) const;

        /**
         * Returns the requested vertex in this boundary component.
         *
         * @param index the index of the requested vertex in the boundary
         * component.  This should be between 0 and getNumberOfVertices()-1
         * inclusive.
         * Note that the index of a vertex in the boundary component need
         * not be the index of the same vertex in the entire
         * triangulation.
         * @return the requested vertex.
         */
        NVertex* getVertex(unsigned long index) const;

        /**
         * Returns the component of the triangulation to which this
         * boundary component belongs.
         *
         * @return the component containing this boundary component.
         */
        NComponent* getComponent() const;

        /**
         * Returns the Euler characteristic of this boundary component.
         * If this boundary component is ideal, the Euler characteristic
         * of the link of the corresponding ideal vertex is returned.
         *
         * @return the Euler characteristic.
         */
        long getEulerChar() const;

        /**
         * A deprecated alias for getEulerChar().
         *
         * Returns the Euler characteristic of this boundary component.
         * If this boundary component is ideal, the Euler characteristic
         * of the link of the corresponding ideal vertex is returned.
         *
         * \deprecated This routine will be removed in a future version of
         * Regina.  Please use the identical routine getEulerChar() instead.
         *
         * @return the Euler characteristic.
         */
        long getEulerCharacteristic() const;

        /**
         * Determines if this boundary component is ideal.
         * This is the case if and only if it consists of a single
         * (ideal) vertex and no triangles.
         *
         * @return \c true if and only if this boundary component is
         * ideal.
         */
        bool isIdeal() const;

        /**
         * Determines if this boundary component is orientable.
         * If the boundary component is ideal, the orientability
         * of the link of the corresponding ideal vertex is returned.
         *
         * @return \c true if and only if this boundary component is
         * orientable.
         */
        bool isOrientable() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Default constructor.
         */
        NBoundaryComponent();

        /**
         * Creates a new boundary component consisting only of the given
         * ideal vertex.
         *
         * \pre The given vertex is ideal as returned by NVertex::isIdeal().
         *
         * @param idealVertex the vertex to place in the new boundary
         * component.
         */
        NBoundaryComponent(NVertex* idealVertex);

    friend class NTriangulation;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/nvertex.h"
namespace regina {

// Inline functions for NBoundaryComponent

inline NBoundaryComponent::NBoundaryComponent() {
}

inline NBoundaryComponent::NBoundaryComponent(NVertex* idealVertex) {
    vertices_.push_back(idealVertex);
}

inline unsigned long NBoundaryComponent::index() const {
    return markedIndex();
}

inline unsigned long NBoundaryComponent::getNumberOfTriangles() const {
    return triangles_.size();
}

inline unsigned long NBoundaryComponent::getNumberOfFaces() const {
    return triangles_.size();
}

inline unsigned long NBoundaryComponent::getNumberOfEdges() const {
    return edges_.size();
}

inline unsigned long NBoundaryComponent::getNumberOfVertices() const {
    return vertices_.size();
}

inline NTriangle* NBoundaryComponent::getTriangle(unsigned long index) const {
    return triangles_[index];
}

inline NTriangle* NBoundaryComponent::getFace(unsigned long index) const {
    return triangles_[index];
}

inline NEdge* NBoundaryComponent::getEdge(unsigned long index) const {
    return edges_[index];
}

inline NVertex* NBoundaryComponent::getVertex(unsigned long index) const {
    return vertices_[index];
}

inline NComponent* NBoundaryComponent::getComponent() const {
    // There may be no triangles, but there is always a vertex.
    return vertices_.front()->getComponent();
}

inline long NBoundaryComponent::getEulerChar() const {
    return (isIdeal() ?
        vertices_.front()->getLinkEulerChar() :
        long(vertices_.size()) - long(edges_.size()) + long(triangles_.size()));
}

inline long NBoundaryComponent::getEulerCharacteristic() const {
    return getEulerChar();
}

inline bool NBoundaryComponent::isIdeal() const {
    return triangles_.empty();
}

inline bool NBoundaryComponent::isOrientable() const {
    return orientable_;
}

inline void NBoundaryComponent::writeTextShort(std::ostream& out) const {
    out << (isIdeal() ? "Ideal " : "Finite ") << "boundary component";
}

} // namespace regina

#endif

