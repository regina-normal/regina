
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

#ifndef __DIM2COMPONENT_H
#ifndef __DOXYGEN
#define __DIM2COMPONENT_H
#endif

/*! \file dim2/dim2component.h
 *  \brief Deals with connected components of a 2-manifold triangulation.
 */

#include "regina-core.h"
#include "generic/component.h"

namespace regina {

class Dim2BoundaryComponent;
class Dim2Edge;
class Dim2Vertex;

template <int> class Simplex;
template <int> class Triangulation;
typedef Simplex<2> Dim2Triangle;
typedef Triangulation<2> Dim2Triangulation;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a connected component of a 2-manifold triangulation.
 *
 * This is a specialisation of the generic Component class template; see
 * the Component documentation for an overview of how this class works.
 *
 * This 2-dimensional specialisation contains some extra functionality.
 * In particular, each 2-dimensional component also stores details on
 * lower-dimensional faces (i.e., vertices and edges) and boundary components.
 */
template <>
class REGINA_API Component<2> : public ComponentBase<2> {
    private:
        std::vector<Dim2Edge*> edges_;
            /**< List of edges in the component. */
        std::vector<Dim2Vertex*> vertices_;
            /**< List of vertices in the component. */
        std::vector<Dim2BoundaryComponent*> boundaryComponents_;
            /**< List of boundary components in the component. */

    public:
        /**
         * A dimension-specific alias for size().
         *
         * See size() for further information.
         */
        size_t getNumberOfTriangles() const;

        /**
         * Returns the number of edges in this component.
         *
         * @return the number of edges.
         */
        size_t getNumberOfEdges() const;

        /**
         * Returns the number of vertices in this component.
         *
         * @return the number of vertices.
         */
        size_t getNumberOfVertices() const;

        /**
         * Returns the number of boundary components in this component.
         *
         * @return the number of boundary components.
         */
        size_t getNumberOfBoundaryComponents() const;

        /**
         * A dimension-specific alias for simplices().
         *
         * See simplices() for further information.
         */
        const std::vector<Dim2Triangle*>& getTriangles() const;

        /**
         * Returns all edges in the component.
         *
         * The reference returned will remain valid for as long as this
         * component object exists, always reflecting the edges currently 
         * in the component.
         *
         * \ifacespython This routine returns a python list.
         */
        const std::vector<Dim2Edge*>& getEdges() const;

        /**
         * Returns all vertices in the component.
         *
         * The reference returned will remain valid for as long as this
         * component object exists, always reflecting the vertices currently 
         * in the component.
         *
         * \ifacespython This routine returns a python list.
         */
        const std::vector<Dim2Vertex*>& getVertices() const;

        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        Dim2Triangle* getTriangle(size_t index) const;

        /**
         * Returns the requested edge in this component.
         *
         * @param index the index of the requested edge in the
         * component.  This should be between 0 and
         * getNumberOfEdges()-1 inclusive.
         * Note that the index of an edge in the component need
         * not be the index of the same edge in the entire
         * triangulation.
         * @return the requested edge.
         */
        Dim2Edge* getEdge(size_t index) const;

        /**
         * Returns the requested vertex in this component.
         *
         * @param index the index of the requested vertex in the
         * component.  This should be between 0 and
         * getNumberOfVertices()-1 inclusive.
         * Note that the index of a vertex in the component need
         * not be the index of the same vertex in the entire
         * triangulation.
         * @return the requested vertex.
         */
        Dim2Vertex* getVertex(size_t index) const;

        /**
         * Returns the requested boundary component in this component.
         *
         * @param index the index of the requested boundary component in
         * this component.  This should be between 0 and
         * getNumberOfBoundaryComponents()-1 inclusive.
         * Note that the index of a boundary component in the component
         * need not be the index of the same boundary component in the
         * entire triangulation.
         * @return the requested boundary component.
         */
        Dim2BoundaryComponent* getBoundaryComponent(size_t index) const;

        /**
         * Determines if this component is closed.
         * This is the case if and only if it has no boundary.
         *
         * @return \c true if and only if this component is closed.
         */
        bool isClosed() const;

        /**
         * A dimension-specific alias for getNumberOfBoundaryFacets().
         *
         * See getNumberOfBoundaryFacets() for further information.
         */
        size_t getNumberOfBoundaryEdges() const;

    private:
        /**
         * Default constructor.
         *
         * Marks the component as orientable, with no boundary facets.
         */
        Component();

    friend class Triangulation<2>;
    friend class TriangulationBase<2>;
};

/**
 * A convenience typedef for Component<2>.
 */
typedef Component<2> Dim2Component;

/*@}*/

// Inline functions for Component<2>

inline Component<2>::Component() : ComponentBase<2>() {
}

inline size_t Component<2>::getNumberOfTriangles() const {
    return size();
}

inline size_t Component<2>::getNumberOfEdges() const {
    return edges_.size();
}

inline size_t Component<2>::getNumberOfVertices() const {
    return vertices_.size();
}

inline size_t Component<2>::getNumberOfBoundaryComponents() const {
    return boundaryComponents_.size();
}

inline const std::vector<Dim2Triangle*>& Component<2>::getTriangles() const {
    return simplices();
}

inline const std::vector<Dim2Edge*>& Component<2>::getEdges() const {
    return edges_;
}

inline const std::vector<Dim2Vertex*>& Component<2>::getVertices() const {
    return vertices_;
}

inline Dim2Triangle* Component<2>::getTriangle(size_t index) const {
    return simplex(index);
}

inline Dim2Edge* Component<2>::getEdge(size_t index) const {
    return edges_[index];
}

inline Dim2Vertex* Component<2>::getVertex(size_t index) const {
    return vertices_[index];
}

inline Dim2BoundaryComponent* Component<2>::getBoundaryComponent(size_t index)
        const {
    return boundaryComponents_[index];
}

inline bool Component<2>::isClosed() const {
    return (boundaryComponents_.empty());
}

inline size_t Component<2>::getNumberOfBoundaryEdges() const {
    return getNumberOfBoundaryFacets();
}

} // namespace regina

#endif

