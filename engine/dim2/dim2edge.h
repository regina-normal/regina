
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

/*! \file dim2/dim2edge.h
 *  \brief Deals with edges in the 1-skeleton of a 2-manifold triangulation.
 */

#ifndef __DIM2EDGE_H
#ifndef __DOXYGEN
#define __DIM2EDGE_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "maths/nperm3.h"
#include "utilities/nmarkedvector.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim2BoundaryComponent;
class Dim2Component;
class Dim2Triangle;
class Dim2Vertex;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Details how an edge in the 1-skeleton of a 2-manifold triangulation
 * forms part of an individual triangle.
 */
class REGINA_API Dim2EdgeEmbedding {
    private:
        Dim2Triangle* triangle_;
            /**< The triangle in which this edge is contained. */
        int edge_;
            /**< The edge number of the triangle that is this edge. */

    public:
        /**
         * Default constructor.  The embedding descriptor created is
         * unusable until it has some data assigned to it using
         * <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        Dim2EdgeEmbedding();

        /**
         * Creates an embedding descriptor containing the given data.
         *
         * @param tri the triangle in which this edge is contained.
         * @param edge the edge number of \a tri that is this edge.
         */
        Dim2EdgeEmbedding(Dim2Triangle* tri, int edge);

        /**
         * Creates an embedding descriptor containing the same data as
         * the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim2EdgeEmbedding(const Dim2EdgeEmbedding& cloneMe);

        /**
         * Assigns to this embedding descriptor the same data as is
         * contained in the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim2EdgeEmbedding& operator = (const Dim2EdgeEmbedding& cloneMe);

        /**
         * Returns the triangle in which this edge is contained.
         *
         * @return the triangle.
         */
        Dim2Triangle* getTriangle() const;

        /**
         * Returns the edge number within getTriangle() that is this edge.
         *
         * @return the edge number that is this edge.
         */
        int getEdge() const;

        /**
         * Returns a mapping from vertices (0,1) of this edge to the
         * corresponding vertex numbers in getTriangle(), as described
         * in Dim2Triangle::getEdgeMapping().
         *
         * @return a mapping from the vertices of this edge to the
         * corresponding vertices of getTriangle().
         */
        NPerm3 getVertices() const;
};

/**
 * Represents an edge in the 1-skeleton of a 2-manifold triangulation.
 * Edges are highly temporary; once a triangulation changes, all its
 * edge objects will be deleted and new ones will be created.
 */
class REGINA_API Dim2Edge : public ShareableObject, public NMarkedElement {
    public:
        /**
         * An array that maps edge numbers within a triangle to the canonical
         * ordering of the individual triangle vertices that form each edge.
         *
         * This means that the vertices of edge \a i in a triangle
         * are, in canonical order, <tt>ordering[i][0,1]</tt>.  As an
         * immediate consequence, we obtain <tt>ordering[i][2] == i</tt>.
         *
         * This table does \e not describe the mapping from specific
         * edges within a triangulation into individual triangles
         * (for that, see Dim2Triangle::getEdgeMapping() instead).
         * This table merely provides a neat and consistent way of
         * listing the vertices of any given edge of a triangle.
         */
        static const NPerm3 ordering[3];

    private:
        Dim2EdgeEmbedding emb_[2];
            /**< A list of descriptors telling how this edge forms a
                 part of each individual triangle that it belongs to. */
        unsigned nEmb_;
            /**< The number of descriptors stored in the list \a emb_.
                 This will never exceed two. */
        Dim2Component* component_;
            /**< The component that this edge is a part of. */
        Dim2BoundaryComponent* boundaryComponent_;
            /**< The boundary component that this edge is a part of,
                 or 0 if this edge is internal. */

    public:
        /**
         * Default destructor.
         */
        ~Dim2Edge();

        /**
         * Returns the number of descriptors available through getEmbedding().
         * Note that this number will never be greater than two.
         *
         * @return the number of embedding descriptors.
         */
        unsigned getNumberOfEmbeddings() const;

        /**
         * Returns the requested descriptor detailing how this edge
         * forms a part of a particular triangle in the triangulation.
         * Note that if this edge represents multiple edges of a
         * particular triangle, then there will be multiple embedding
         * descriptors available regarding that triangle.
         *
         * @param index the index of the requested descriptor.  This
         * should be between 0 and getNumberOfEmbeddings()-1 inclusive.
         * @return the requested embedding descriptor.
         */
        const Dim2EdgeEmbedding& getEmbedding(unsigned index) const;

        /**
         * Returns the component of the triangulation to which this
         * edge belongs.
         *
         * @return the component containing this edge.
         */
        Dim2Component* getComponent() const;

        /**
         * Returns the boundary component of the triangulation to which
         * this edge belongs.
         *
         * @return the boundary component containing this edge, or 0
         * if this edge does not lie entirely within the boundary of
         * the triangulation.
         */
        Dim2BoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex of the 2-manifold triangulation corresponding
         * to the given vertex of this edge.
         *
         * @param vertex the vertex of this edge to examine.  This
         * should be either 0 or 1.
         * @return the corresponding vertex of the 2-manifold triangulation.
         */
        Dim2Vertex* getVertex(int vertex) const;

        /**
         * Determines if this edge lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this edge lies on the boundary.
         */
        bool isBoundary() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new edge and marks it as belonging to the
         * given triangulation component.
         *
         * @param component the triangulation component to which this
         * edge belongs.
         */
        Dim2Edge(Dim2Component* component);

    friend class Dim2Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim2/dim2triangle.h"
namespace regina {

// Inline functions for Dim2EdgeEmbedding

inline Dim2EdgeEmbedding::Dim2EdgeEmbedding() : triangle_(0) {
}

inline Dim2EdgeEmbedding::Dim2EdgeEmbedding(
        Dim2Triangle* tri, int edge) :
        triangle_(tri), edge_(edge) {
}

inline Dim2EdgeEmbedding::Dim2EdgeEmbedding(
        const Dim2EdgeEmbedding& cloneMe) :
        triangle_(cloneMe.triangle_), edge_(cloneMe.edge_) {
}

inline Dim2EdgeEmbedding& Dim2EdgeEmbedding::operator =
        (const Dim2EdgeEmbedding& cloneMe) {
    triangle_ = cloneMe.triangle_;
    edge_ = cloneMe.edge_;
    return *this;
}

inline Dim2Triangle* Dim2EdgeEmbedding::getTriangle() const {
    return triangle_;
}

inline int Dim2EdgeEmbedding::getEdge() const {
    return edge_;
}

inline NPerm3 Dim2EdgeEmbedding::getVertices() const {
    return triangle_->getEdgeMapping(edge_);
}

// Inline functions for Dim2Edge

inline Dim2Edge::Dim2Edge(Dim2Component* component) :
        nEmb_(0), component_(component), boundaryComponent_(0) {
}

inline Dim2Edge::~Dim2Edge() {
}

inline unsigned Dim2Edge::getNumberOfEmbeddings() const {
    return nEmb_;
}

inline const Dim2EdgeEmbedding& Dim2Edge::getEmbedding(
        unsigned index) const {
    return emb_[index];
}

inline Dim2Component* Dim2Edge::getComponent() const {
    return component_;
}

inline Dim2BoundaryComponent* Dim2Edge::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline Dim2Vertex* Dim2Edge::getVertex(int vertex) const {
    return emb_[0].getTriangle()->getVertex(emb_[0].getVertices()[vertex]);
}

inline bool Dim2Edge::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline void Dim2Edge::writeTextShort(std::ostream& out) const {
    out << (boundaryComponent_ ? "Boundary " : "Internal ") << "edge";
}

} // namespace regina

#endif

