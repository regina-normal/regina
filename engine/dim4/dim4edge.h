
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

/*! \file dim4/dim4edge.h
 *  \brief Deals with edges in a 4-manifold triangulation.
 */

#ifndef __DIM4EDGE_H
#ifndef __DOXYGEN
#define __DIM4EDGE_H
#endif

#include <vector>
#include "regina-core.h"
#include "output.h"
#include "maths/nperm5.h"
#include "utilities/nmarkedvector.h"
#include <boost/noncopyable.hpp>
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim2Triangulation;
class Dim4Component;
class Dim4BoundaryComponent;
class Dim4Pentachoron;
class Dim4Vertex;
class Dim4Triangulation;
class Dim4Isomorphism;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Details how an edge in the skeleton of a 4-manifold triangulation forms
 * part of an individual pentachoron.
 */
class REGINA_API Dim4EdgeEmbedding {
    private:
        Dim4Pentachoron* pent_;
            /**< The pentachoron in which this edge is contained. */
        int edge_;
            /**< The edge number of the pentachoron that is this edge. */

    public:
        /**
         * Default constructor.  The embedding descriptor created is
         * unusable until it has some data assigned to it using
         * <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        Dim4EdgeEmbedding();

        /**
         * Creates an embedding descriptor containing the given data.
         *
         * @param pent the pentachoron in which this edge is contained.
         * @param edge the edge number of \a pent that is this edge.
         */
        Dim4EdgeEmbedding(Dim4Pentachoron* pent, int edge);

        /**
         * Creates an embedding descriptor containing the same data as
         * the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim4EdgeEmbedding(const Dim4EdgeEmbedding& cloneMe);

        /**
         * Assigns to this embedding descriptor the same data as is
         * contained in the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim4EdgeEmbedding& operator =(const Dim4EdgeEmbedding& cloneMe);

        /**
         * Returns the pentachoron in which this edge is contained.
         *
         * @return the pentachoron.
         */
        Dim4Pentachoron* getPentachoron() const;

        /**
         * Returns the edge number within getPentachoron() that is
         * this edge.
         *
         * @return the edge number that is this edge.
         */
        int getEdge() const;

        /**
         * Returns a mapping from vertices (0,1) of this edge to the
         * corresponding vertex numbers in getPentachoron().  This
         * permutation also maps (2,3,4) to the three remaining
         * pentachoron vertices in a way that preserves orientation as
         * you walk around the edge (assuming this is actually possible).
         * See Dim4Pentachoron::getEdgeMapping() for details.
         *
         * @return a mapping from the vertices of this edge to the
         * vertices of getPentachoron().
         */
        NPerm5 getVertices() const;

        /**
         * Tests whether this and the given embedding are identical.
         * Here "identical" means that they refer to the same edge of
         * the same pentachoron.
         *
         * @param rhs the embedding to compare with this.
         * @return \c true if and only if both embeddings are identical.
         */
        bool operator == (const Dim4EdgeEmbedding& rhs) const;

        /**
         * Tests whether this and the given embedding are different.
         * Here "different" means that they do not refer to the same edge of
         * the same pentachoron.
         *
         * @param rhs the embedding to compare with this.
         * @return \c true if and only if both embeddings are identical.
         */
        bool operator != (const Dim4EdgeEmbedding& rhs) const;
};

/**
 * Represents an edge in the skeleton of a 4-manifold triangulation.
 * Edges are highly temporary; once a triangulation changes, all its
 * edge objects will be deleted and new ones will be created.
 */
class REGINA_API Dim4Edge :
        public Output<Dim4Edge>,
        public boost::noncopyable,
        public NMarkedElement {
    public:
        /**
         * A table that maps vertices of a pentachoron to edge numbers.
         *
         * Edges in a pentachoron are numbered 0,...,9.  This table
         * converts vertices to edge numbers; in particular, the edge
         * joining vertices \a i and \a j of a pentachoron is
         * edge number <tt>edgeNumber[i][j]</tt>.  Here \a i and \a j
         * must be distinct, must be between 0 and 4 inclusive, and may
         * be given in any order.  The resulting edge number will be
         * between 0 and 9 inclusive.
         *
         * Note that edge \i is always opposite triangle \i in a pentachoron.
         *
         * This table is the 4-manifold analogue to the 3-manifold table
         * NEdge::edgeNumber.  Note that NEdge::edgeNumber is still used
         * by 4-manifold triangulations to describe the numbering of
         * \a tetrahedron edges 0,...,5; see Dim4Tetrahedron::getEdge()
         * for details.
         */
        static const int edgeNumber[5][5];

        /**
         * A table that maps edges of a pentachoron to vertex numbers.
         *
         * Edges in a pentachoron are numbered 0,...,9.  This table
         * converts edge numbers to vertices; in particular, edge \a i
         * in a pentachoron joins vertices <tt>edgeVertex[i][0]</tt> and
         * <tt>edgeVertex[i][1]</tt>.  Here \a i must be between 0 and 9
         * inclusive; the resulting vertex numbers will be between 0 and 4
         * inclusive.
         *
         * Note that edge \i is always opposite triangle \i in a pentachoron.
         * It is guaranteed that <tt>edgeVertex[i][0]</tt> will always
         * be smaller than <tt>edgeVertex[i][1]</tt>.
         *
         * This table is the 4-manifold analogue to the 3-manifold table
         * NEdge::edgeVertex.  Note that NEdge::edgeVertex is still used
         * by 4-manifold triangulations to describe the numbering of
         * \a tetrahedron edges 0,...,5; see Dim4Tetrahedron::getEdge()
         * for details.
         */
        static const int edgeVertex[10][2];

        /**
         * An array that maps edge numbers within a pentachoron to the
         * canonical ordering of the individual pentachoron vertices
         * that form each edge.
         *
         * This means that the vertices of edge \a i in a pentachoron
         * are, in canonical order, <tt>ordering[i][0,1]</tt>.  The
         * images of 2, 3 and 4 under each permutation are arbitrary.
         *
         * This table does \e not describe the mapping from specific
         * triangulation edges into individual pentachora (for that,
         * see Dim4Pentachoron::getEdgeMapping() instead).  This table
         * merely provides a neat and consistent way of listing the
         * vertices of any given pentachoron edge.
         */
        static const NPerm5 ordering[10];

    private:
        enum {
            INVALID_IDENTIFICATION = 1,
                /**< Signifies that an edge is identified with itself in
                     reverse. */
            INVALID_LINK = 2
                /**< Signifies that the link of an edge is something
                     other than a 2-sphere or a disc. */
        };

        std::vector<Dim4EdgeEmbedding> emb_;
            /**< A list of descriptors telling how this edge forms a part of
                 each individual pentachoron that it belongs to. */
        Dim4Component* component_;
            /**< The component that this edge is a part of. */
        Dim4BoundaryComponent* boundaryComponent_;
            /**< The boundary component that this edge is a part of,
                 or 0 if this edge is internal. */
        unsigned invalid_;
            /**< Is this edge invalid?  This will be 0 if the edge is
                 valid, or a bitwise combination of \a INVALID_IDENTIFICATION
                 and/or \a INVALID_LINK if the edge is invalid. */
        Dim2Triangulation* link_;
            /**< A triangulation of the edge link.  This will only be
             * constructed on demand; until then it will be null. */

    public:
        /**
         * Returns the index of this edge in the underlying
         * triangulation.  This is identical to calling
         * <tt>getTriangulation()->edgeIndex(this)</tt>.
         *
         * @return the index of this edge.
         */
        unsigned long index() const;

        /**
         * Returns the list of descriptors detailing how this edge forms a
         * part of various pentachora in the 4-manifold triangulation.
         * Note that if this edge represents multiple edges of a
         * particular pentachoron, then there will be multiple embedding
         * descriptors in the list regarding that pentachoron.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of embedding descriptors.
         * @see Dim4EdgeEmbedding
         */
        const std::vector<Dim4EdgeEmbedding>& getEmbeddings() const;

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
        const Dim4EdgeEmbedding& getEmbedding(unsigned long index) const;

        /**
         * Returns the triangulation to which this edge belongs.
         *
         * @return the triangulation containing this edge.
         */
        Dim4Triangulation* getTriangulation() const;

        /**
         * Returns the component of the triangulation to which this
         * edge belongs.
         *
         * @return the component containing this edge.
         */
        Dim4Component* getComponent() const;

        /**
         * Returns the boundary component of the triangulation to which
         * this edge belongs.
         *
         * See the note in the Dim4BoundaryComponent overview regarding
         * what happens if the edge link itself has more than one
         * boundary component.  Note that such an edge link makes the
         * triangulation invalid.
         *
         * @return the boundary component containing this edge, or 0 if this
         * edge does not lie entirely within the boundary of the triangulation.
         */
        Dim4BoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex of the 4-manifold triangulation corresponding
         * to the given vertex of this edge.
         *
         * @param vertex the vertex of this edge to examine.  This should
         * be 0 or 1.
         * @return the corresponding vertex of the 4-manifold triangulation.
         */
        Dim4Vertex* getVertex(int vertex) const;

        /**
         * Returns the degree of this edge.  Note that this is identical
         * to getNumberOfEmbeddings().
         *
         * @return the degree of this edge.
         */
        unsigned long getDegree() const;

        /**
         * Determines if this edge lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this edge lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Determines if this edge is valid.
         * An edge is valid if and only if (i) it is not glued to itself
         * in reverse, and (ii) the link of the edge is a 2-sphere or a disc.
         *
         * For specific reasons why an edge is invalid, see
         * hasBadIdentification() and hasBadLink().
         *
         * @return \c true if and only if this edge is valid.
         */
        bool isValid() const;

        /**
         * Determines if this edge is identified with itself in reverse.
         *
         * Such an edge is invalid.  However, there can be other types of
         * invalid edges also (i.e., an edge might be invalid even if this
         * routine returns \c false).  See isValid() for a full discussion
         * of edge validity.
         *
         * @return \c true if and only if this edge is identified with
         * itself in reverse.
         */
        bool hasBadIdentification() const;

        /**
         * Determines if the link of this edge is something other than a
         * 2-sphere or a disc.
         *
         * Such an edge is invalid.  However, there can be other types of
         * invalid edges also (i.e., an edge might be invalid even if this
         * routine returns \c false).  See isValid() for a full discussion
         * of edge validity.
         *
         * @return \c true if and only if the link of this edge is
         * neither a 2-sphere nor a disc.
         */
        bool hasBadLink() const;

        /**
         * Returns a full 2-manifold triangulation describing
         * the link of this edge.
         *
         * This routine is fast (it uses a pre-computed triangulation
         * where possible).  The downside is that the triangulation is
         * read-only, and does not contain any information on how the
         * triangles in the link correspond to pentachora in the original
         * triangulation (though this is easily deduced; see below).
         * If you want a writable triangulation, or one with this extra
         * information, then call buildLinkDetail() instead.
         *
         * The triangulation of the edge link is built as follows.
         * Let \a i lie between 0 and getDegree()-1 inclusive, let
         * \a pent represent <tt>getEmbedding(i).getPentachoron()</tt>,
         * and let \a e represent <tt>getEmbedding(i).getEdge()</tt>.
         * Then <tt>buildLink()->getTriangle(i)</tt> is the triangle
         * in the edge link that links edge \a e of pentachoron \a pent.
         * In other words, <tt>buildLink()->getTriangle(i)</tt> in the edge link
         * is parallel to triangle <tt>pent->getTriangle(e)</tt> in the
         * surrounding 4-manifold triangulation.
         *
         * The vertices of each triangle in the edge link are
         * numbered as follows.  Following the discussion above,
         * suppose that <tt>buildLink()->getTriangle(i)</tt>
         * sits within \c pent and is parallel to
         * <tt>pent->getTriangle(e)</tt>.
         * Then vertices 0,1,2 of the triangle in the link will be
         * parallel to vertices 0,1,2 of the corresponding Dim4Triangle.
         * The permutation <tt>pent->getTriangleMapping(e)</tt> will map
         * vertices 0,1,2 of the triangle in the link to the
         * corresponding vertices of \c pent (those opposite \c e),
         * and will map 3 and 4 to the vertices of \c e itself.
         *
         * This Dim4Edge object will retain ownership of the triangulation
         * that is returned.  If you wish to edit the triangulation, you
         * should make a new clone and edit the clone instead.
         *
         * @return the read-only triangulated link of this edge.
         */
        const Dim2Triangulation* buildLink() const;

        /**
         * Returns a full 2-manifold triangulation describing
         * the link of this edge.
         *
         * This routine is heavyweight (it computes a new triangulation
         * each time).  The benefit is that the triangulation is writeable,
         * and optionally contain detailed information on how the triangles
         * in the link correspond to pentachora in the original triangulation.
         * If you do not need this extra information, consider using the
         * faster buildLink() instead.
         *
         * See the buildLink() documentation for an explanation of
         * exactly how the triangulation will be constructed.
         *
         * If \a labels is passed as \c true, each triangle of the new
         * edge link will be given a text description of the form
         * <tt>p&nbsp;(e)</tt>, where \c p is the index of the pentachoron
         * the triangle is from, and \c e is the edge of that pentachoron
         * that this triangle links.
         *
         * If \a inclusion is non-null (i.e., it points to some
         * Dim4Isomorphism pointer \a p), then it will be modified to
         * point to a new Dim4Isomorphism that describes in detail how the
         * individual triangles of the link sit within pentachora of
         * the original triangulation.  Specifically, after this routine
         * is called, <tt>p->pentImage(i)</tt> will indicate which pentachoron
         * \a pent of the 4-manifold triangulation contains the <i>i</i>th
         * triangle of the link.  Moreover, <tt>p->facetPerm(i)</tt> will
         * indicate exactly where the <i>i</i>th triangle sits within
         * \a pent: (i) it will send 3,4 to the vertices of \a pent that lie
         * on the edge that the triangle links, with 3 and 4 mapping to
         * vertices 0 and 1 respectively of the corresponding Dim4Edge;
         * and (ii) it will send 0,1,2 to the vertices of \a pent that
         * are parallel to vertices 0,1,2 of this triangle.
         *
         * The triangulation that is returned, as well as the isomorphism
         * if one was requested, will be newly allocated.  The caller of
         * this routine is responsible for destroying these objects.
         *
         * Strictly speaking, this is an abuse of the Dim4Isomorphism class
         * (the domain is a triangulation of the wrong dimension, and
         * the map is not 1-to-1 into the range pentachora).  We use
         * it anyway, but you should not attempt to call any high-level
         * routines (such as Dim4Isomorphism::apply).
         *
         * \ifacespython The second (isomorphism) argument is not present.
         * Instead this routine returns a pair (triangulation, isomorphism).
         * As a side-effect, the isomorphism will always be constructed
         * (i.e., it is not optional).
         *
         * @return a newly constructed triangulation of the link of this edge.
         */
        Dim2Triangulation* buildLinkDetail(bool labels = true,
            Dim4Isomorphism** inclusion = 0) const;

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
         * Creates a new edge and marks it as belonging to the
         * given triangulation component.
         *
         * Amongst other things, this constructor sets \a invalid_ to zero.
         *
         * @param component the triangulation component to which this
         * edge belongs.
         */
        Dim4Edge(Dim4Component* component);

    friend class Dim4Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim4/dim4pentachoron.h"
namespace regina {

// Inline functions for Dim4EdgeEmbedding

inline Dim4EdgeEmbedding::Dim4EdgeEmbedding() : pent_(0) {
}

inline Dim4EdgeEmbedding::Dim4EdgeEmbedding(Dim4Pentachoron* pent, int edge) :
        pent_(pent), edge_(edge) {
}

inline Dim4EdgeEmbedding::Dim4EdgeEmbedding(const Dim4EdgeEmbedding& cloneMe) :
        pent_(cloneMe.pent_), edge_(cloneMe.edge_) {
}

inline Dim4EdgeEmbedding& Dim4EdgeEmbedding::operator =
        (const Dim4EdgeEmbedding& cloneMe) {
    pent_ = cloneMe.pent_;
    edge_ = cloneMe.edge_;
    return *this;
}

inline Dim4Pentachoron* Dim4EdgeEmbedding::getPentachoron() const {
    return pent_;
}

inline int Dim4EdgeEmbedding::getEdge() const {
    return edge_;
}

inline NPerm5 Dim4EdgeEmbedding::getVertices() const {
    return pent_->getEdgeMapping(edge_);
}

inline bool Dim4EdgeEmbedding::operator == (const Dim4EdgeEmbedding& other)
        const {
    return ((pent_ == other.pent_) && (edge_ == other.edge_));
}

inline bool Dim4EdgeEmbedding::operator != (const Dim4EdgeEmbedding& other)
        const {
    return ((pent_ != other.pent_) || (edge_ != other.edge_));
}

// Inline functions for Dim4Edge

inline Dim4Edge::Dim4Edge(Dim4Component* component) :
        component_(component), boundaryComponent_(0), invalid_(0), link_(0) {
}

inline unsigned long Dim4Edge::index() const {
    return markedIndex();
}

inline const std::vector<Dim4EdgeEmbedding>& Dim4Edge::getEmbeddings() const {
    return emb_;
}

inline unsigned long Dim4Edge::getNumberOfEmbeddings() const {
    return emb_.size();
}

inline const Dim4EdgeEmbedding& Dim4Edge::getEmbedding(unsigned long index)
        const {
    return emb_[index];
}

inline Dim4Triangulation* Dim4Edge::getTriangulation() const {
    return emb_.front().getPentachoron()->getTriangulation();
}

inline Dim4Component* Dim4Edge::getComponent() const {
    return component_;
}

inline Dim4BoundaryComponent* Dim4Edge::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline Dim4Vertex* Dim4Edge::getVertex(int vertex) const {
    const Dim4EdgeEmbedding& e(emb_.front());
    return e.getPentachoron()->getVertex(e.getVertices()[vertex]);
}

inline unsigned long Dim4Edge::getDegree() const {
    return emb_.size();
}

inline bool Dim4Edge::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline bool Dim4Edge::isValid() const {
    return ! invalid_;
}

inline bool Dim4Edge::hasBadIdentification() const {
    return (invalid_ & Dim4Edge::INVALID_IDENTIFICATION);
}

inline bool Dim4Edge::hasBadLink() const {
    return (invalid_ & Dim4Edge::INVALID_LINK);
}

inline const Dim2Triangulation* Dim4Edge::buildLink() const {
    if (! link_) {
        // This is a construct-on-demand member; cast away constness to
        // set it here.
        const_cast<Dim4Edge*>(this)->link_ = buildLinkDetail(false, 0);
    }
    return link_;
}

inline void Dim4Edge::writeTextShort(std::ostream& out) const {
    out << (boundaryComponent_ ? "Boundary " : "Internal ")
        << "edge of degree " << emb_.size();
}

} // namespace regina

#endif

