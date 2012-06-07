
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

/*! \file dim2/dim2vertex.h
 *  \brief Deals with vertices in a 2-manifold triangulation.
 */

#ifndef __DIM2VERTEX_H
#ifndef __DOXYGEN
#define __DIM2VERTEX_H
#endif

#include <deque>
#include "regina-core.h"
#include "shareableobject.h"
#include "maths/nperm3.h"
#include "utilities/nmarkedvector.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim2BoundaryComponent;
class Dim2Component;
class Dim2Triangle;
class Dim2Triangulation;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Details how a vertex in the skeleton of a 2-manifold triangulation forms
 * part of an individual triangle.
 */
class REGINA_API Dim2VertexEmbedding {
    private:
        Dim2Triangle* triangle_;
            /**< The triangle in which this vertex is contained. */
        int vertex_;
            /**< The vertex number of the triangle that is this vertex. */

    public:
        /**
         * Default constructor.  The embedding descriptor created is
         * unusable until it has some data assigned to it using
         * <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        Dim2VertexEmbedding();

        /**
         * Creates an embedding descriptor containing the given data.
         *
         * @param tri the triangle in which this vertex is contained.
         * @param vertex the vertex number of \a tri that is this vertex.
         */
        Dim2VertexEmbedding(Dim2Triangle* tri, int vertex);

        /**
         * Creates an embedding descriptor containing the same data as
         * the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim2VertexEmbedding(const Dim2VertexEmbedding& cloneMe);

        /**
         * Assigns to this embedding descriptor the same data as is
         * contained in the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim2VertexEmbedding& operator =(const Dim2VertexEmbedding& cloneMe);

        /**
         * Returns the triangle in which this vertex is contained.
         *
         * @return the triangle.
         */
        Dim2Triangle* getTriangle() const;

        /**
         * Returns the vertex number within getTriangle() that is this vertex.
         *
         * @return the vertex number that is this vertex.
         */
        int getVertex() const;

        /**
         * Returns a permutation that maps 0 to the vertex number within
         * getTriangle() that is this vertex.  This permutation also maps
         * (1,2) to the two remaining triangle vertices in a manner that
         * preserves orientation as you walk around the vertex.
         * See Dim2Triangle::getVertexMapping() for details.
         *
         * @return a permutation that maps 0 to the corresponding
         * vertex number of getTriangle().
         */
        NPerm3 getVertices() const;
};

/**
 * Represents a vertex in the skeleton of a 2-manifold triangulation.
 * Vertices are highly temporary; once a triangulation changes, all its
 * vertex objects will be deleted and new ones will be created.
 */
class REGINA_API Dim2Vertex : public ShareableObject, public NMarkedElement {
    private:
        std::deque<Dim2VertexEmbedding> emb_;
            /**< A list of descriptors telling how this vertex forms a part of
                 each individual triangle that it belongs to. */
        Dim2Component* component_;
            /**< The component that this vertex is a part of. */
        Dim2BoundaryComponent* boundaryComponent_;
            /**< The boundary component that this vertex is a part of,
                 or 0 if this vertex is internal. */

    public:
        /**
         * Returns the list of descriptors detailing how this vertex forms
         * a part of various triangles in the triangulation.
         * Note that if this vertex represents multiple vertices of a
         * particular triangle, then there will be multiple embedding
         * descriptors in the list regarding that triangle.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of embedding descriptors.
         * @see Dim2VertexEmbedding
         */
        const std::deque<Dim2VertexEmbedding>& getEmbeddings() const;

        /**
         * Returns the number of descriptors in the list returned by
         * getEmbeddings().  Note that this is identical to getDegree().
         *
         * @return the number of embedding descriptors.
         */
        unsigned long getNumberOfEmbeddings() const;

        /**
         * Returns the requested descriptor from the list returned by
         * getEmbeddings().
         *
         * @param index the index of the requested descriptor.  This
         * should be between 0 and getNumberOfEmbeddings()-1 inclusive.
         * @return the requested embedding descriptor.
         */
        const Dim2VertexEmbedding& getEmbedding(unsigned long index) const;

        /**
         * Returns the component of the triangulation to which this
         * vertex belongs.
         *
         * @return the component containing this vertex.
         */
        Dim2Component* getComponent() const;

        /**
         * Returns the boundary component of the triangulation to which
         * this vertex belongs.
         *
         * @return the boundary component containing this vertex,
         * or 0 if this vertex is not on the boundary of the triangulation.
         */
        Dim2BoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the degree of this vertex.  Note that this is
         * identical to getNumberOfEmbeddings().
         *
         * @return the degree of this vertex.
         */
        unsigned long getDegree() const;

        /**
         * Determines if this vertex lies on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this vertex lies on the boundary.
         */
        bool isBoundary() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new vertex and marks it as belonging to the
         * given triangulation component.
         *
         * @param component the triangulation component to which this
         * vertex belongs.
         */
        Dim2Vertex(Dim2Component* component);

    friend class Dim2Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim2/dim2triangle.h"
namespace regina {

// Inline functions for Dim2VertexEmbedding

inline Dim2VertexEmbedding::Dim2VertexEmbedding() : triangle_(0) {
}

inline Dim2VertexEmbedding::Dim2VertexEmbedding(Dim2Triangle* tri,
        int vertex) :
        triangle_(tri), vertex_(vertex) {
}

inline Dim2VertexEmbedding::Dim2VertexEmbedding(
        const Dim2VertexEmbedding& cloneMe) :
        triangle_(cloneMe.triangle_), vertex_(cloneMe.vertex_) {
}

inline Dim2VertexEmbedding& Dim2VertexEmbedding::operator =
        (const Dim2VertexEmbedding& cloneMe) {
    triangle_ = cloneMe.triangle_;
    vertex_ = cloneMe.vertex_;
    return *this;
}

inline Dim2Triangle* Dim2VertexEmbedding::getTriangle() const {
    return triangle_;
}

inline int Dim2VertexEmbedding::getVertex() const {
    return vertex_;
}

// Inline functions for Dim2Vertex

inline Dim2Vertex::Dim2Vertex(Dim2Component* component) :
        component_(component), boundaryComponent_(0) {
}

inline const std::deque<Dim2VertexEmbedding>& Dim2Vertex::getEmbeddings()
        const {
    return emb_;
}

inline unsigned long Dim2Vertex::getNumberOfEmbeddings() const {
    return emb_.size();
}

inline const Dim2VertexEmbedding& Dim2Vertex::getEmbedding(unsigned long index)
        const {
    return emb_[index];
}

inline NPerm3 Dim2VertexEmbedding::getVertices() const {
    return triangle_->getVertexMapping(vertex_);
}

inline Dim2Component* Dim2Vertex::getComponent() const {
    return component_;
}

inline Dim2BoundaryComponent* Dim2Vertex::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline unsigned long Dim2Vertex::getDegree() const {
    return emb_.size();
}

inline bool Dim2Vertex::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline void Dim2Vertex::writeTextShort(std::ostream& out) const {
    out << (boundaryComponent_ ? "Boundary " : "Internal ")
        << "vertex of degree " << emb_.size();
}

} // namespace regina

#endif

