
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

#ifndef __DIM4COMPONENT_H
#ifndef __DOXYGEN
#define __DIM4COMPONENT_H
#endif

/*! \file dim4/dim4component.h
 *  \brief Deals with components of a 4-manifold triangulation.
 */

#include <vector>
#include "shareableobject.h"
#include "utilities/nmarkedvector.h"

namespace regina {

class Dim4Pentachoron;
class Dim4Tetrahedron;
class Dim4Face;
class Dim4Edge;
class Dim4Vertex;
class Dim4BoundaryComponent;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a component of a 4-manifold triangulation.
 * Components are highly temporary; once a triangulation changes, all
 * its component objects will be deleted and new ones will be created.
 */
class Dim4Component : public ShareableObject, public NMarkedElement {
    private:
        std::vector<Dim4Pentachoron*> pentachora_;
            /**< List of pentachora in the component. */
        std::vector<Dim4Tetrahedron*> tetrahedra_;
            /**< List of tetrahedra in the component. */
        std::vector<Dim4Face*> faces_;
            /**< List of faces in the component. */
        std::vector<Dim4Edge*> edges_;
            /**< List of edges in the component. */
        std::vector<Dim4Vertex*> vertices_;
            /**< List of vertices in the component. */
        std::vector<Dim4BoundaryComponent*> boundaryComponents_;
            /**< List of boundary components in the component. */

        bool ideal_;
            /**< Is the component ideal? */
        bool orientable_;
            /**< Is the component orientable? */

    public:
        /**
         * Default destructor.
         */
        virtual ~Dim4Component();

        /**
         * Returns the number of pentachora in this component.
         *
         * @return the number of pentachora.
         */
        unsigned long getNumberOfPentachora() const;

        /**
         * Returns the number of tetrahedra in this component.
         *
         * @return the number of tetrahedra.
         */
        unsigned long getNumberOfTetrahedra() const;

        /**
         * Returns the number of faces in this component.
         *
         * @return the number of faces.
         */
        unsigned long getNumberOfFaces() const;

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
         * Returns the requested pentachoron in this component.
         *
         * @param index the index of the requested pentachoron in the
         * component.  This should be between 0 and
         * getNumberOfPentachora()-1 inclusive.
         * Note that the index of a pentachoron in the component need
         * not be the index of the same pentachoron in the entire
         * triangulation.
         * @return the requested pentachoron.
         */
        Dim4Pentachoron* getPentachoron(unsigned long index) const;

        /**
         * Returns the requested tetrahedron in this component.
         *
         * @param index the index of the requested tetrahedron in the
         * component.  This should be between 0 and
         * getNumberOfTetrahedra()-1 inclusive.
         * Note that the index of a tetrahedron in the component need
         * not be the index of the same tetrahedron in the entire
         * triangulation.
         * @return the requested tetrahedron.
         */
        Dim4Tetrahedron* getTetrahedron(unsigned long index) const;

        /**
         * Returns the requested face in this component.
         *
         * @param index the index of the requested face in the
         * component.  This should be between 0 and
         * getNumberOfFaces()-1 inclusive.
         * Note that the index of a face in the component need
         * not be the index of the same face in the entire
         * triangulation.
         * @return the requested face.
         */
        Dim4Face* getFace(unsigned long index) const;

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
        Dim4Edge* getEdge(unsigned long index) const;

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
        Dim4Vertex* getVertex(unsigned long index) const;

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
        Dim4BoundaryComponent* getBoundaryComponent(unsigned long index) const;

        /**
         * Determines if this component is ideal.
         * This is the case if and only if it contains an ideal vertex
         * as described by Dim4Vertex::isIdeal().
         *
         * @return \c true if and only if this component is ideal.
         */
        bool isIdeal() const;

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
         * Note that ideal components are not closed.  Likewise,
         * components with invalid vertices are not closed.
         * See Dim4Vertex::isBoundary() for details.
         *
         * @return \c true if and only if this component is closed.
         */
        bool isClosed() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Default constructor.
         *
         * Marks the component as orientable and not ideal.
         */
        Dim4Component();

    friend class Dim4Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

// Inline functions for Dim4Component

inline Dim4Component::Dim4Component() : ideal_(false), orientable_(true) {
}

inline Dim4Component::~Dim4Component() {
}

inline unsigned long Dim4Component::getNumberOfPentachora() const {
    return pentachora_.size();
}

inline unsigned long Dim4Component::getNumberOfTetrahedra() const {
    return tetrahedra_.size();
}

inline unsigned long Dim4Component::getNumberOfFaces() const {
    return faces_.size();
}

inline unsigned long Dim4Component::getNumberOfEdges() const {
    return edges_.size();
}

inline unsigned long Dim4Component::getNumberOfVertices() const {
    return vertices_.size();
}

inline unsigned long Dim4Component::getNumberOfBoundaryComponents() const {
    return boundaryComponents_.size();
}

inline Dim4Pentachoron* Dim4Component::getPentachoron(unsigned long index)
        const {
    return pentachora_[index];
}

inline Dim4Tetrahedron* Dim4Component::getTetrahedron(unsigned long index)
        const {
    return tetrahedra_[index];
}

inline Dim4Face* Dim4Component::getFace(unsigned long index) const {
    return faces_[index];
}

inline Dim4Edge* Dim4Component::getEdge(unsigned long index) const {
    return edges_[index];
}

inline Dim4Vertex* Dim4Component::getVertex(unsigned long index) const {
    return vertices_[index];
}

inline Dim4BoundaryComponent* Dim4Component::getBoundaryComponent(
        unsigned long index) const {
    return boundaryComponents_[index];
}

inline bool Dim4Component::isIdeal() const {
    return ideal_;
}

inline bool Dim4Component::isOrientable() const {
    return orientable_;
}

inline bool Dim4Component::isClosed() const {
    return (boundaryComponents_.empty());
}

inline void Dim4Component::writeTextShort(std::ostream& out) const {
    out << "Component with " << getNumberOfPentachora() << " pentachora";
}

} // namespace regina

#endif

