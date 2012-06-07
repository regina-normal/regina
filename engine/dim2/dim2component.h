
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#ifndef __DIM2COMPONENT_H
#ifndef __DOXYGEN
#define __DIM2COMPONENT_H
#endif

/*! \file dim2/dim2component.h
 *  \brief Deals with components of a 2-manifold triangulation.
 */

#include <vector>
#include "regina-core.h"
#include "shareableobject.h"
#include "utilities/nmarkedvector.h"

namespace regina {

class Dim2BoundaryComponent;
class Dim2Edge;
class Dim2Triangle;
class Dim2Vertex;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a component of a 2-manifold triangulation.
 * Components are highly temporary; once a triangulation changes, all
 * its component objects will be deleted and new ones will be created.
 */
class REGINA_API Dim2Component : public ShareableObject, public NMarkedElement {
    private:
        std::vector<Dim2Triangle*> triangles_;
            /**< List of triangles in the component. */
        std::vector<Dim2Edge*> edges_;
            /**< List of edges in the component. */
        std::vector<Dim2Vertex*> vertices_;
            /**< List of vertices in the component. */
        std::vector<Dim2BoundaryComponent*> boundaryComponents_;
            /**< List of boundary components in the component. */

        bool orientable_;
            /**< Is the component orientable? */

    public:
        /**
         * Default destructor.
         */
        virtual ~Dim2Component();

        /**
         * Returns the number of triangles in this component.
         *
         * @return the number of triangles.
         */
        unsigned long getNumberOfTriangles() const;

        /**
         * Returns the number of edges in this component.
         *
         * @return the number of edges.
         */
        unsigned long getNumberOfEdges() const;

        /**
         * Returns the number of vertices in this component.
         *
         * @return the number of vertices.
         */
        unsigned long getNumberOfVertices() const;

        /**
         * Returns the number of boundary components in this component.
         *
         * @return the number of boundary components.
         */
        unsigned long getNumberOfBoundaryComponents() const;

        /**
         * Returns the requested triangle in this component.
         *
         * @param index the index of the requested triangle in the
         * component.  This should be between 0 and
         * getNumberOfTriangles()-1 inclusive.
         * Note that the index of a triangle in the component need
         * not be the index of the same triangle in the entire
         * triangulation.
         * @return the requested triangle.
         */
        Dim2Triangle* getTriangle(unsigned long index) const;

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
        Dim2Edge* getEdge(unsigned long index) const;

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
        Dim2Vertex* getVertex(unsigned long index) const;

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
        Dim2BoundaryComponent* getBoundaryComponent(unsigned long index) const;

        /**
         * Determines if this component is orientable.
         * 
         * @return \c true if and only if this component is orientable.
         */
        bool isOrientable() const;

        /**
         * Determines if this component is closed.
         * This is the case if and only if it has no boundary.
         *
         * @return \c true if and only if this component is closed.
         */
        bool isClosed() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Default constructor.
         *
         * Marks the component as orientable.
         */
        Dim2Component();

    friend class Dim2Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

// Inline functions for Dim2Component

inline Dim2Component::Dim2Component() : orientable_(true) {
}

inline Dim2Component::~Dim2Component() {
}

inline unsigned long Dim2Component::getNumberOfTriangles() const {
    return triangles_.size();
}

inline unsigned long Dim2Component::getNumberOfEdges() const {
    return edges_.size();
}

inline unsigned long Dim2Component::getNumberOfVertices() const {
    return vertices_.size();
}

inline unsigned long Dim2Component::getNumberOfBoundaryComponents() const {
    return boundaryComponents_.size();
}

inline Dim2Triangle* Dim2Component::getTriangle(unsigned long index) const {
    return triangles_[index];
}

inline Dim2Edge* Dim2Component::getEdge(unsigned long index) const {
    return edges_[index];
}

inline Dim2Vertex* Dim2Component::getVertex(unsigned long index) const {
    return vertices_[index];
}

inline Dim2BoundaryComponent* Dim2Component::getBoundaryComponent(
        unsigned long index) const {
    return boundaryComponents_[index];
}

inline bool Dim2Component::isOrientable() const {
    return orientable_;
}

inline bool Dim2Component::isClosed() const {
    return (boundaryComponents_.empty());
}

inline void Dim2Component::writeTextShort(std::ostream& out) const {
    out << "Component with " << getNumberOfTriangles() << " triangles";
}

} // namespace regina

#endif

