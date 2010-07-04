
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#ifndef __DIM4BOUNDARYCOMPONENT_H
#ifndef __DOXYGEN
#define __DIM4BOUNDARYCOMPONENT_H
#endif

/*! \file dim4/dim4boundarycomponent.h
 *  \brief Deals with components of the boundary of a 4-manifold triangulation.
 */

#include <vector>
#include "dim4/dim4vertex.h"

namespace regina {

class Dim4Tetrahedron;
class Dim4Face;
class Dim4Edge;
class Dim4Vertex;
class NTriangulation;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a component of the boundary of a 4-manifold triangulation.
 *
 * Note that an ideal vertex constitutes a boundary component of its
 * own.  Likewise, an invalid vertex is placed in its own boundary
 * component if it does not already belong to some larger boundary
 * component.  See Dim4Vertex::isBoundary() for details.
 *
 * We can encounter some interesting cases with invalid triangulations.
 * Consider some vertex in a 4-manifold triangulation.  If the link of
 * this vertex is itself a 3-manifold with more than one boundary component
 * (which makes the vertex invalid), then different parts of the 4-manifold
 * boundary are effectively "pinched" together.  If this happens, the
 * different parts of the boundary that are pinched might or might not be
 * listed as part of the same boundary component; if not then the
 * offending vertex will be included in all of these boundary components.
 * Nevertheless, only one of these can be considered the "official" boundary
 * component of the vertex as returned by Dim4Vertex::getBoundaryComponent().
 * This is all a bit of a mess (and even more so when you consider that
 * the same thing can happen with invalid edges), but then again the
 * entire 4-manifold triangulation is invalid and so you almost certainly
 * have bigger problems to deal with.
 *
 * Boundary components are highly temporary; once a triangulation
 * changes, all its boundary component objects will be deleted and new
 * ones will be created.
 */
class Dim4BoundaryComponent : public ShareableObject, public NMarkedElement {
    private:
        std::vector<Dim4Tetrahedron*> tetrahedra_;
            /**< List of tetrahedra in the component. */
        std::vector<Dim4Face*> faces_;
            /**< List of faces in the component. */
        std::vector<Dim4Edge*> edges_;
            /**< List of edges in the component. */
        std::vector<Dim4Vertex*> vertices_;
            /**< List of vertices in the component. */

        NTriangulation* boundary_;
            /**< A full triangulation of the boundary component formed
                 by joining together individual boundary tetrahedra.
                 If this boundary component does not contain any tetrahedra
                 (such as an ideal boundary component) then this will be
                 the null pointer. */

    public:
        /**
         * Default destructor.
         */
        virtual ~Dim4BoundaryComponent();

        /**
         * Returns the number of tetrahedra in this boundary component.
         *
         * @return the number of tetrahedra.
         */
        unsigned long getNumberOfTetrahedra() const;

        /**
         * Returns the number of faces in this boundary component.
         *
         * @return the number of faces.
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
         * Returns the requested tetrahedron in this boundary component.
         *
         * The index of a Dim4Tetrahedron in the boundary component need
         * not be the index of the same tetrahedron in the entire
         * 4-manifold triangulation.  However, if this boundary component
         * is built from one or more tetrahedra (i.e., it is not ideal),
         * then the index of each Dim4Tetrahedron in this boundary component
         * matches the index of the corresponding NTetrahedron in the
         * 3-manifold triangulation returned by getTriangulation().
         *
         * @param index the index of the requested tetrahedron in the boundary
         * component.  This should be between 0 and getNumberOfTetrahedra()-1
         * inclusive.
         * @return the requested tetrahedron.
         */
        Dim4Tetrahedron* getTetrahedron(unsigned long index) const;

        /**
         * Returns the requested face in this boundary component.
         *
         * The index of a Dim4Face in the boundary component need
         * not be the index of the same face in the entire
         * 4-manifold triangulation.  However, if this boundary component
         * is built from one or more tetrahedra (i.e., it is not ideal),
         * then the index of each Dim4Face in this boundary component
         * matches the index of the corresponding NFace in the
         * 3-manifold triangulation returned by getTriangulation().
         *
         * @param index the index of the requested face in the boundary
         * component.  This should be between 0 and getNumberOfFaces()-1
         * inclusive.
         * @return the requested face.
         */
        Dim4Face* getFace(unsigned long index) const;

        /**
         * Returns the requested edge in this boundary component.
         *
         * The index of a Dim4Edge in the boundary component need
         * not be the index of the same edge in the entire
         * 4-manifold triangulation.  However, if this boundary component
         * is built from one or more tetrahedra (i.e., it is not ideal),
         * then the index of each Dim4Edge in this boundary component
         * matches the index of the corresponding NEdge in the
         * 3-manifold triangulation returned by getTriangulation().
         *
         * @param index the index of the requested edge in the boundary
         * component.  This should be between 0 and getNumberOfEdges()-1
         * inclusive.
         * @return the requested edge.
         */
        Dim4Edge* getEdge(unsigned long index) const;

        /**
         * Returns the requested vertex in this boundary component.
         *
         * The index of a Dim4Vertex in the boundary component need
         * not be the index of the same vertex in the entire
         * 4-manifold triangulation.  However, if this boundary component
         * is built from one or more tetrahedra (i.e., it is not ideal),
         * then the index of each Dim4Vertex in this boundary component
         * matches the index of the corresponding NVertex in the
         * 3-manifold triangulation returned by getTriangulation().
         *
         * @param index the index of the requested vertex in the boundary
         * component.  This should be between 0 and getNumberOfVertices()-1
         * inclusive.
         * @return the requested vertex.
         */
        Dim4Vertex* getVertex(unsigned long index) const;

        /**
         * Returns the full 3-manifold triangulation of this boundary
         * component.  Note that this triangulation is read-only (though
         * of course you can clone it and then operate upon the clone).
         *
         * If this boundary component contains one or more tetrahedra
         * (i.e., it is a regular boundary built from pentachoron facets),
         * then the triangulation of this boundary component is as follows.
         * Let \a i lie between 0 and getNumberOfTetrahedra()-1 inclusive.
         * Then tetrahedron \a i of the boundary 3-manifold triangulation is
         * a copy of tetrahedron <tt>getTetrahedron(i)</tt> of this 4-manifold
         * boundary component, and its vertices 0,1,2,3 are numbered in the
         * same way.  To relate tetrahedron vertex numbers to pentachoron
         * vertex numbers, see Dim4Pentachoron::getTetrahedronMapping().
         *
         * If this boundary component consists only of a single vertex
         * (which happens with ideal vertices and also some invalid vertices),
         * then this routine returns the triangulation of the corresponding
         * vertex link.  See Dim4Vertex::getLink() for details.
         *
         * It is guaranteed that the full skeleton of this 3-manifold
         * triangulation will have been generated already.
         *
         * @return the triangulation of this boundary component.
         */
        const NTriangulation* getTriangulation() const;

        /**
         * Determines if this boundary component is ideal.
         * This is the case if and only if it consists of a single
         * ideal vertex and no tetrahedra.
         *
         * Note that a boundary component formed from a single \e invalid
         * vertex is \e not considered to be ideal.  This means that, if a
         * boundary component contains no tetrahedra at all, then one and
         * only one of isIdeal() and isInvalidVertex() will return \c true.
         *
         * @return \c true if and only if this boundary component is ideal.
         */
        bool isIdeal() const;

        /**
         * Determines if this boundary component consists of a single invalid
         * vertex and nothing else.  In particular, such a boundary component
         * must contain no tetrahedra at all.
         *
         * An invalid vertex is only placed in its own boundary component if
         * it does not already belong to some larger boundary component.
         * This means that, for a boundary component consisting of one or
         * more tetrahedra, this routine will return \c false even if the
         * boundary tetrahedra include one or more invalid vertices.
         *
         * Note that, if a boundary component contains no tetrahedra at all,
         * then one and only one of isIdeal() and isInvalidVertex() will return
         * \c true.
         *
         * @return \c true if and only if this boundary component consists of a
         * single invalid vertex and nothing else.
         */
        bool isInvalidVertex() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Default constructor.
         */
        Dim4BoundaryComponent();

        /**
         * Creates a new boundary component consisting only of the given
         * vertex.
         *
         * \pre The given vertex is not part of a boundary tetrahedron,
         * but instead belongs in its own boundary component; an example
         * might be an ideal vertex.
         *
         * @param vertex the vertex to place in the new boundary component.
         */
        Dim4BoundaryComponent(Dim4Vertex* vertex);

    friend class Dim4Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

// Inline functions for Dim4BoundaryComponent

inline Dim4BoundaryComponent::Dim4BoundaryComponent() :
        boundary_(0) {
}

inline Dim4BoundaryComponent::Dim4BoundaryComponent(Dim4Vertex* vertex) :
        boundary_(0) {
    vertices_.push_back(vertex);
}

inline unsigned long Dim4BoundaryComponent::getNumberOfTetrahedra() const {
    return tetrahedra_.size();
}

inline unsigned long Dim4BoundaryComponent::getNumberOfFaces() const {
    return faces_.size();
}

inline unsigned long Dim4BoundaryComponent::getNumberOfEdges() const {
    return edges_.size();
}

inline unsigned long Dim4BoundaryComponent::getNumberOfVertices() const {
    return vertices_.size();
}

inline Dim4Tetrahedron* Dim4BoundaryComponent::getTetrahedron(
        unsigned long index) const {
    return tetrahedra_[index];
}

inline Dim4Face* Dim4BoundaryComponent::getFace(unsigned long index) const {
    return faces_[index];
}

inline Dim4Edge* Dim4BoundaryComponent::getEdge(unsigned long index) const {
    return edges_[index];
}

inline Dim4Vertex* Dim4BoundaryComponent::getVertex(unsigned long index) const {
    return vertices_[index];
}

inline const NTriangulation* Dim4BoundaryComponent::getTriangulation() const {
    return (boundary_ ? boundary_ : vertices_.front()->getLink());
}

inline void Dim4BoundaryComponent::writeTextShort(std::ostream& out) const {
    out << (isIdeal() ? "Ideal " : isInvalidVertex() ? "Invalid " : "Finite ")
        << "boundary component";
}

} // namespace regina

#endif

