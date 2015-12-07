
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

/*! \file dim2/dim2vertex.h
 *  \brief Deals with vertices in a 2-manifold triangulation.
 */

#ifndef __DIM2VERTEX_H
#ifndef __DOXYGEN
#define __DIM2VERTEX_H
#endif

#include <deque>
#include "regina-core.h"
#include "output.h"
#include "generic/face.h"
#include "maths/nperm3.h"
#include "utilities/nmarkedvector.h"
#include <boost/noncopyable.hpp>
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim2BoundaryComponent;

template <int> class Component;
template <int> class Simplex;
template <int> class Triangulation;
typedef Component<2> Dim2Component;
typedef Simplex<2> Dim2Triangle;
typedef Triangulation<2> Dim2Triangulation;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Details how a vertex of a 2-manifold triangulation appears within each
 * triangle.
 *
 * This is a specialisation of the generic FaceEmbedding class template;
 * see the documentation for FaceEmbedding (and also Face) for a general
 * overview of how these face-related classes work.
 *
 * This 2-dimensional specialisation of FaceEmbedding offers additional
 * dimension-specific aliases of some member functions.
 */
template <>
class REGINA_API FaceEmbedding<2, 0> : public FaceEmbeddingBase<2, 0> {
    public:
        /**
         * Default constructor.  This object is unusable until it has
         * some data assigned to it using <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        FaceEmbedding();

        /**
         * Creates a new object containing the given data.
         *
         * @param tri the triangle in which the underlying vertex
         * of the triangulation is contained.
         * @param vertex the corresponding vertex number of \a tri.
         * This must be between 0 and 2 inclusive.
         */
        FaceEmbedding(Dim2Triangle* tri, int vertex);

        /**
         * Creates a new copy of the given object.
         *
         * @param cloneMe the object to copy.
         */
        FaceEmbedding(const FaceEmbedding& cloneMe);

        /**
         * A dimension-specific alias for getSimplex().
         *
         * See getSimplex() for further information.
         */
        Dim2Triangle* getTriangle() const;

        /**
         * A dimension-specific alias for getFace().
         *
         * See getFace() for further information.
         */
        int getVertex() const;
};

/**
 * A convenience typedef for FaceEmbedding<2, 0>.
 */
typedef FaceEmbedding<2, 0> Dim2VertexEmbedding;

/**
 * Represents a vertex in the skeleton of a 2-manifold triangulation.
 * Vertices are highly temporary; once a triangulation changes, all its
 * vertex objects will be deleted and new ones will be created.
 */
class REGINA_API Dim2Vertex :
        public Output<Dim2Vertex>,
        public boost::noncopyable,
        public NMarkedElement {
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
         * Returns the index of this vertex in the underlying
         * triangulation.  This is identical to calling
         * <tt>getTriangulation()->vertexIndex(this)</tt>.
         *
         * @return the index of this vertex.
         */
        unsigned long index() const;

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
         * Returns the triangulation to which this vertex belongs.
         *
         * @return the triangulation containing this vertex.
         */
        Dim2Triangulation* getTriangulation() const;

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
         * Creates a new vertex and marks it as belonging to the
         * given triangulation component.
         *
         * @param component the triangulation component to which this
         * vertex belongs.
         */
        Dim2Vertex(Dim2Component* component);

    friend class Triangulation<2>;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim2/dim2triangle.h"
namespace regina {

// Inline functions for Dim2VertexEmbedding

inline FaceEmbedding<2, 0>::FaceEmbedding() :
        FaceEmbeddingBase<2, 0>() {
}

inline FaceEmbedding<2, 0>::FaceEmbedding(Dim2Triangle* tri, int vertex) :
        FaceEmbeddingBase<2, 0>(tri, vertex) {
}

inline FaceEmbedding<2, 0>::FaceEmbedding(
        const Dim2VertexEmbedding& cloneMe) :
        FaceEmbeddingBase<2, 0>(cloneMe) {
}

inline Dim2Triangle* FaceEmbedding<2, 0>::getTriangle() const {
    return getSimplex();
}

inline int FaceEmbedding<2, 0>::getVertex() const {
    return getFace();
}

// Inline functions for Dim2Vertex

inline Dim2Vertex::Dim2Vertex(Dim2Component* component) :
        component_(component), boundaryComponent_(0) {
}

inline unsigned long Dim2Vertex::index() const {
    return markedIndex();
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

inline Dim2Triangulation* Dim2Vertex::getTriangulation() const {
    return emb_.front().getTriangle()->getTriangulation();
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

