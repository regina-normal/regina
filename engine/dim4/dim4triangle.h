
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file dim4/dim4triangle.h
 *  \brief Deals with triangles in a 4-manifold triangulation.
 */

#ifndef __DIM4TRIANGLE_H
#ifndef __DOXYGEN
#define __DIM4TRIANGLE_H
#endif

#include <deque>
#include "regina-core.h"
#include "output.h"
#include "maths/nperm5.h"
#include "utilities/nmarkedvector.h"
#include <boost/noncopyable.hpp>
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim4BoundaryComponent;
class Dim4Edge;
class Dim4Vertex;

template <int> class Component;
template <int> class Simplex;
template <int> class Triangulation;
typedef Component<4> Dim4Component;
typedef Simplex<4> Dim4Pentachoron;
typedef Triangulation<4> Dim4Triangulation;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Details how a triangle in the skeleton of a 4-manifold triangulation forms
 * part of an individual pentachoron.
 */
class REGINA_API Dim4TriangleEmbedding {
    private:
        Dim4Pentachoron* pent_;
            /**< The pentachoron in which this triangle is contained. */
        int tri_;
            /**< The triangle number of the pentachoron that is this
                 triangle. */

    public:
        /**
         * Default constructor.  The embedding descriptor created is
         * unusable until it has some data assigned to it using
         * <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        Dim4TriangleEmbedding();

        /**
         * Creates an embedding descriptor containing the given data.
         *
         * @param pent the pentachoron in which this triangle is contained.
         * @param tri the triangle number of \a pent that is this triangle.
         */
        Dim4TriangleEmbedding(Dim4Pentachoron* pent, int tri);

        /**
         * Creates an embedding descriptor containing the same data as
         * the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim4TriangleEmbedding(const Dim4TriangleEmbedding& cloneMe);

        /**
         * Assigns to this embedding descriptor the same data as is
         * contained in the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim4TriangleEmbedding& operator =(const Dim4TriangleEmbedding& cloneMe);

        /**
         * Returns the pentachoron in which this triangle is contained.
         *
         * @return the pentachoron.
         */
        Dim4Pentachoron* getPentachoron() const;

        /**
         * Returns the triangle number within getPentachoron() that is this
         * triangle.
         *
         * @return the triangle number that is this triangle.
         */
        int getTriangle() const;

        /**
         * Returns a mapping from vertices (0,1,2) of this triangle to the
         * corresponding vertex numbers in getPentachoron().  This
         * permutation also maps (3,4) to the two remaining pentachoron
         * vertices in a manner that preserves orientation as you walk
         * around the triangle.  See Dim4Pentachoron::getTriangleMapping() for
         * details.
         *
         * @return a mapping from the vertices of this triangle to the
         * vertices of getPentachoron().
         */
        NPerm5 getVertices() const;

        /**
         * Tests whether this and the given embedding are identical.
         * Here "identical" means that they refer to the same triangle of
         * the same pentachoron.
         *
         * @param rhs the embedding to compare with this.
         * @return \c true if and only if both embeddings are identical.
         */
        bool operator == (const Dim4TriangleEmbedding& rhs) const;

        /**
         * Tests whether this and the given embedding are different.
         * Here "different" means that they do not refer to the same triangle
         * of the same pentachoron.
         *
         * @param rhs the embedding to compare with this.
         * @return \c true if and only if both embeddings are identical.
         */
        bool operator != (const Dim4TriangleEmbedding& rhs) const;
};

/**
 * Represents a triangle in the skeleton of a 4-manifold triangulation.
 * Triangles are highly temporary; once a triangulation changes, all its
 * triangle objects will be deleted and new ones will be created.
 */
class REGINA_API Dim4Triangle :
        public Output<Dim4Triangle>,
        public boost::noncopyable,
        public NMarkedElement {
    public:
        /**
         * A table that maps vertices of a pentachoron to triangle numbers.
         *
         * Triangles in a pentachoron are numbered 0,...,9.  This table
         * converts vertices to triangle numbers; in particular, the triangle
         * spanned by vertices \a i, \a j and \a k of a pentachoron is triangle
         * number <tt>triangleNumber[i][j][k]</tt>.  Here \a i, \a j and \a k
         * must be distinct, must be between 0 and 4 inclusive, and may
         * be given in any order.  The resulting triangle number will be
         * between 0 and 9 inclusive.
         *
         * Note that triangle \i is always opposite edge \i in a pentachoron.
         *
         * This is analagous to the lookup table NEdge::edgeNumber
         * for 3-manifold triangulations (and its deprecated predecessor,
         * the old global array regina::edgeNumber).
         */
        static const int triangleNumber[5][5][5];

        /**
         * A table that maps triangles of a pentachoron to vertex numbers.
         *
         * Triangles in a pentachoron are numbered 0,...,9.  This table converts
         * triangle numbers to vertices; in particular, triangle \a i in a
         * pentachoron is spanned by vertices <tt>triangleVertex[i][0]</tt>,
         * <tt>triangleVertex[i][1]</tt> and <tt>triangleVertex[i][2]</tt>.
         * Here \a i must be between 0 and 9 inclusive; the resulting
         * vertex numbers will be between 0 and 4 inclusive.
         *
         * Note that triangle \i is always opposite edge \i in a pentachoron.
         * It is guaranteed that <tt>triangleVertex[i][0]</tt> will always
         * be smaller than <tt>triangleVertex[i][1]</tt>, which in turn will
         * always be smaller than <tt>triangleVertex[i][2]</tt>.
         *
         * This is analagous to the lookup table NEdge::edgeVertex
         * for 3-manifold triangulations (and its deprecated predecessors,
         * the old global arrays regina::edgeStart and regina::edgeEnd).
         */
        static const int triangleVertex[10][3];

        /**
         * An array that maps triangle numbers within a pentachoron to the
         * canonical ordering of the individual pentachoron vertices
         * that form each triangle.
         *
         * This means that the vertices of triangle \a i in a pentachoron
         * are, in canonical order, <tt>ordering[i][0..2]</tt>.  The
         * images of 3 and 4 under each permutation are chosen to make
         * each permutation even.
         *
         * This table does \e not describe the mapping from specific
         * triangles of the triangulation into individual pentachora (for that,
         * see Dim4Pentachoron::getTriangleMapping() instead).  This table
         * merely provides a neat and consistent way of listing the
         * vertices of any given triangle in a pentachoron.
         */
        static const NPerm5 ordering[10];

    private:
        std::deque<Dim4TriangleEmbedding> emb_;
            /**< A list of descriptors telling how this triangle forms a part of
                 each individual pentachoron that it belongs to. */
        Dim4Component* component_;
            /**< The component that this triangle is a part of. */
        Dim4BoundaryComponent* boundaryComponent_;
            /**< The boundary component that this triangle is a part of,
                 or 0 if this triangle is internal. */
        bool valid_;
            /**< Is this triangle valid? */

    public:
        /**
         * Returns the index of this triangle in the underlying
         * triangulation.  This is identical to calling
         * <tt>getTriangulation()->triangleIndex(this)</tt>.
         *
         * @return the index of this triangle.
         */
        unsigned long index() const;

        /**
         * Returns the list of descriptors detailing how this triangle forms a
         * part of various pentachora in the 4-manifold triangulation.
         * Note that if this triangle represents multiple triangles of a
         * particular pentachoron, then there will be multiple embedding
         * descriptors in the list regarding that pentachoron.
         *
         * These embedding descriptors will be stored in order in the
         * list, so that if you run through the list and follow in turn
         * the edges of each pentachoron defined by the images of (3,4)
         * under Dim4TriangleEmbedding::getVertices(), then you will obtain
         * an ordered chain circling this triangle.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of embedding descriptors.
         * @see Dim4TriangleEmbedding
         */
        const std::deque<Dim4TriangleEmbedding>& getEmbeddings() const;

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
        const Dim4TriangleEmbedding& getEmbedding(unsigned long index) const;

        /**
         * Returns the triangulation to which this triangle belongs.
         *
         * @return the triangulation containing this triangle.
         */
        Dim4Triangulation* getTriangulation() const;

        /**
         * Returns the component of the triangulation to which this
         * triangle belongs.
         *
         * @return the component containing this triangle.
         */
        Dim4Component* getComponent() const;

        /**
         * Returns the boundary component of the triangulation to which
         * this triangle belongs.
         *
         * @return the boundary component containing this triangle, or 0 if this
         * triangle does not lie entirely within the boundary of the
         * triangulation.
         */
        Dim4BoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex of the 4-manifold triangulation corresponding
         * to the given vertex of this triangle.
         *
         * Note that vertex \a i of a triangle is opposite edge \a i of
         * the triangle.
         *
         * @param vertex the vertex of this triangle to examine.  This should
         * be 0, 1 or 2.
         * @return the corresponding vertex of the 4-manifold triangulation.
         */
        Dim4Vertex* getVertex(int vertex) const;

        /**
         * Returns the edge of the 4-manifold triangulation corresponding
         * to the given edge of this triangle.
         *
         * Note that edge \a i of a triangle is opposite vertex \a i of
         * the triangle.
         *
         * @param edge the edge of this triangle to examine.  This should
         * be 0, 1 or 2.
         * @return the corresponding edge of the 4-manifold triangulation.
         */
        Dim4Edge* getEdge(int edge) const;

        /**
         * Examines the given edge of this triangle, and returns a mapping
         * from the "canonical" vertices of the corresponding edge of
         * the triangulation to the vertices of this triangle.
         *
         * This routine behaves much the same way as
         * Dim4Pentachoron::getEdgeMapping(), except that it maps the
         * edge vertices into a triangle, not into a pentachoron.  See
         * Dim4Pentachoron::getEdgeMapping() for a more detailed
         * explanation of precisely what this mapping means.
         *
         * This routine differs from Dim4Pentachoron::getEdgeMapping()
         * in how it handles the images of 2, 3 and 4.  This routine
         * will always map 2 to the remaining vertex of this triangle (which
         * is equal to the argument \a edge), and will always map 3 and 4
         * to themselves.
         *
         * @param edge the edge of this triangle to examine.  This should be
         * 0, 1 or 2.
         * @return a mapping from vertices (0,1) of the requested edge
         * to the vertices of this triangle.
         */
        NPerm5 getEdgeMapping(int edge) const;

        /**
         * Returns the degree of this triangle.  Note that this is identical
         * to getNumberOfEmbeddings().
         *
         * @return the degree of this triangle.
         */
        unsigned long getDegree() const;

        /**
         * Determines if this triangle lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this triangle lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Determines if this triangle is valid.
         * A triangle is valid if and only if it is not glued to itself
         * using a non-trivial rotation or reflection.
         *
         * @return \c true if and only if this triangle is valid.
         */
        bool isValid() const;

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
         * Creates a new triangle and marks it as belonging to the
         * given triangulation component.
         *
         * @param component the triangulation component to which this
         * triangle belongs.
         */
        Dim4Triangle(Dim4Component* component);

    friend class Triangulation<4>;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim4/dim4pentachoron.h"
namespace regina {

// Inline functions for Dim4TriangleEmbedding

inline Dim4TriangleEmbedding::Dim4TriangleEmbedding() : pent_(0) {
}

inline Dim4TriangleEmbedding::Dim4TriangleEmbedding(Dim4Pentachoron* pent,
        int tri) :
        pent_(pent), tri_(tri) {
}

inline Dim4TriangleEmbedding::Dim4TriangleEmbedding(
        const Dim4TriangleEmbedding& cloneMe) :
        pent_(cloneMe.pent_), tri_(cloneMe.tri_) {
}

inline Dim4TriangleEmbedding& Dim4TriangleEmbedding::operator =
        (const Dim4TriangleEmbedding& cloneMe) {
    pent_ = cloneMe.pent_;
    tri_ = cloneMe.tri_;
    return *this;
}

inline Dim4Pentachoron* Dim4TriangleEmbedding::getPentachoron() const {
    return pent_;
}

inline int Dim4TriangleEmbedding::getTriangle() const {
    return tri_;
}

inline NPerm5 Dim4TriangleEmbedding::getVertices() const {
    return pent_->getTriangleMapping(tri_);
}

inline bool Dim4TriangleEmbedding::operator ==
        (const Dim4TriangleEmbedding& other) const {
    return ((pent_ == other.pent_) && (tri_ == other.tri_));
}

inline bool Dim4TriangleEmbedding::operator !=
        (const Dim4TriangleEmbedding& other) const {
    return ((pent_ != other.pent_) || (tri_ != other.tri_));
}

// Inline functions for Dim4Triangle

inline Dim4Triangle::Dim4Triangle(Dim4Component* component) :
        component_(component), boundaryComponent_(0), valid_(true) {
}

inline unsigned long Dim4Triangle::index() const {
    return markedIndex();
}

inline const std::deque<Dim4TriangleEmbedding>& Dim4Triangle::getEmbeddings()
        const {
    return emb_;
}

inline unsigned long Dim4Triangle::getNumberOfEmbeddings() const {
    return emb_.size();
}

inline const Dim4TriangleEmbedding& Dim4Triangle::getEmbedding(
        unsigned long index) const {
    return emb_[index];
}

inline Dim4Triangulation* Dim4Triangle::getTriangulation() const {
    return emb_.front().getPentachoron()->getTriangulation();
}

inline Dim4Component* Dim4Triangle::getComponent() const {
    return component_;
}

inline Dim4BoundaryComponent* Dim4Triangle::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline Dim4Vertex* Dim4Triangle::getVertex(int vertex) const {
    const Dim4TriangleEmbedding& e(emb_.front());
    return e.getPentachoron()->getVertex(e.getVertices()[vertex]);
}

inline unsigned long Dim4Triangle::getDegree() const {
    return emb_.size();
}

inline bool Dim4Triangle::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline bool Dim4Triangle::isValid() const {
    return valid_;
}

inline void Dim4Triangle::writeTextShort(std::ostream& out) const {
    out << (boundaryComponent_ ? "Boundary " : "Internal ")
        << "triangle of degree " << emb_.size();
}

} // namespace regina

#endif

