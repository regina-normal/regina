
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

#ifndef __DIM2BOUNDARYCOMPONENT_H
#ifndef __DOXYGEN
#define __DIM2BOUNDARYCOMPONENT_H
#endif

/*! \file dim2/dim2boundarycomponent.h
 *  \brief Deals with boundary components of a 2-manifold triangulation.
 */

#include <vector>
#include "regina-core.h"
#include "shareableobject.h"
#include "utilities/nmarkedvector.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim2Edge;
class Dim2Vertex;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a component of the boundary of a 2-manifold triangulation.
 *
 * Boundary components are highly temporary; once a triangulation
 * changes, all its boundary component objects will be deleted and new
 * ones will be created.
 */
class REGINA_API Dim2BoundaryComponent :
        public ShareableObject, public NMarkedElement {
    private:
        std::vector<Dim2Edge*> edges_;
            /**< List of edges in the component. */
        std::vector<Dim2Vertex*> vertices_;
            /**< List of vertices in the component. */

    public:
        /**
         * Default destructor.
         */
        virtual ~Dim2BoundaryComponent();

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
         * Returns the requested edge in this boundary component.
         *
         * The index of a Dim2Edge in the boundary component need
         * not be the index of the same edge in the entire
         * 2-manifold triangulation.
         *
         * @param index the index of the requested edge in the boundary
         * component.  This should be between 0 and getNumberOfEdges()-1
         * inclusive.
         * @return the requested edge.
         */
        Dim2Edge* getEdge(unsigned long index) const;

        /**
         * Returns the requested vertex in this boundary component.
         *
         * The index of a Dim2Vertex in the boundary component need
         * not be the index of the same vertex in the entire
         * 2-manifold triangulation.
         *
         * @param index the index of the requested vertex in the boundary
         * component.  This should be between 0 and getNumberOfVertices()-1
         * inclusive.
         * @return the requested vertex.
         */
        Dim2Vertex* getVertex(unsigned long index) const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Default constructor.
         */
        Dim2BoundaryComponent();

    friend class Dim2Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

// Inline functions for Dim2BoundaryComponent

inline Dim2BoundaryComponent::Dim2BoundaryComponent() {
}

inline Dim2BoundaryComponent::~Dim2BoundaryComponent() {
}

inline unsigned long Dim2BoundaryComponent::getNumberOfEdges() const {
    return edges_.size();
}

inline unsigned long Dim2BoundaryComponent::getNumberOfVertices() const {
    return vertices_.size();
}

inline Dim2Edge* Dim2BoundaryComponent::getEdge(unsigned long index) const {
    return edges_[index];
}

inline Dim2Vertex* Dim2BoundaryComponent::getVertex(unsigned long index) const {
    return vertices_[index];
}

inline void Dim2BoundaryComponent::writeTextShort(std::ostream& out) const {
    out << "Boundary component";
}

} // namespace regina

#endif

