
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

/*! \file dim4/dim4vertex.h
 *  \brief Deals with vertices in a 4-manifold triangulation.
 */

#ifndef __DIM4VERTEX_H
#ifndef __DOXYGEN
#define __DIM4VERTEX_H
#endif

#include <vector>
#include "regina-core.h"
#include "output.h"
#include "maths/nperm5.h"
#include "utilities/nmarkedvector.h"
#include <boost/noncopyable.hpp>
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim4BoundaryComponent;
class Dim4Component;
class Dim4Isomorphism;
class Dim4Pentachoron;
class Dim4Triangulation;
class NTriangulation;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Details how a vertex in the skeleton of a 4-manifold triangulation forms
 * part of an individual pentachoron.
 */
class REGINA_API Dim4VertexEmbedding {
    private:
        Dim4Pentachoron* pent_;
            /**< The pentachoron in which this vertex is contained. */
        int vertex_;
            /**< The vertex number of the pentachoron that is this vertex. */

    public:
        /**
         * Default constructor.  The embedding descriptor created is
         * unusable until it has some data assigned to it using
         * <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        Dim4VertexEmbedding();

        /**
         * Creates an embedding descriptor containing the given data.
         *
         * @param pent the pentachoron in which this vertex is contained.
         * @param vertex the vertex number of \a pent that is this vertex.
         */
        Dim4VertexEmbedding(Dim4Pentachoron* pent, int vertex);

        /**
         * Creates an embedding descriptor containing the same data as
         * the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim4VertexEmbedding(const Dim4VertexEmbedding& cloneMe);

        /**
         * Assigns to this embedding descriptor the same data as is
         * contained in the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim4VertexEmbedding& operator =(const Dim4VertexEmbedding& cloneMe);

        /**
         * Returns the pentachoron in which this vertex is contained.
         *
         * @return the pentachoron.
         */
        Dim4Pentachoron* getPentachoron() const;

        /**
         * Returns the vertex number within getPentachoron() that is
         * this vertex.
         *
         * @return the vertex number that is this vertex.
         */
        int getVertex() const;

        /**
         * Returns a permutation that maps 0 to the vertex number within
         * getPentachoron() that is this vertex.  The real point of this
         * routine is that (1,2,3,4) maps to the four remaining pentachoron
         * vertices in a manner that preserves orientation as you walk
         * around the vertex (assuming this is actually possible).  See
         * Dim4Pentachoron::getVertexMapping() for details.
         *
         * @return a permutation that maps 0 to the vertex number that
         * is this vertex.
         */
        NPerm5 getVertices() const;

        /**
         * Tests whether this and the given embedding are identical.
         * Here "identical" means that they refer to the same vertex of
         * the same pentachoron.
         *
         * @param rhs the embedding to compare with this.
         * @return \c true if and only if both embeddings are identical.
         */
        bool operator == (const Dim4VertexEmbedding& rhs) const;

        /**
         * Tests whether this and the given embedding are different.
         * Here "different" means that they do not refer to the same vertex of
         * the same pentachoron.
         *
         * @param rhs the embedding to compare with this.
         * @return \c true if and only if both embeddings are identical.
         */
        bool operator != (const Dim4VertexEmbedding& rhs) const;
};

/**
 * Represents a vertex in the skeleton of a 4-manifold triangulation.
 * Vertices are highly temporary; once a triangulation changes, all its
 * vertex objects will be deleted and new ones will be created.
 */
class REGINA_API Dim4Vertex :
        public Output<Dim4Vertex>,
        public boost::noncopyable,
        public NMarkedElement {
    private:
        std::vector<Dim4VertexEmbedding> emb_;
            /**< A list of descriptors telling how this vertex forms a part of
                 each individual pentachoron that it belongs to. */
        Dim4Component* component_;
            /**< The component that this vertex is a part of. */
        Dim4BoundaryComponent* boundaryComponent_;
            /**< The boundary component that this vertex is a part of,
                 or 0 if this vertex is internal. */
        NTriangulation* link_;
            /**< The link of this vertex, given as a full-blown
                 3-manifold triangulation.  It is guaranteed that 3-sphere
                 recognition has already been run over this triangulation
                 (and so future 3-sphere queries will be very fast). */
        bool valid_;
            /**< Is this vertex valid? */
        bool ideal_;
            /**< Is this vertex ideal? */

    public:
        /**
         * Default destructor.
         */
        ~Dim4Vertex();

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
         * a part of various pentachora in the triangulation.
         * Note that if this vertex represents multiple vertices of a
         * particular pentachoron, then there will be multiple embedding
         * descriptors in the list regarding that pentachoron.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of embedding descriptors.
         * @see Dim4VertexEmbedding
         */
        const std::vector<Dim4VertexEmbedding>& getEmbeddings() const;

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
        const Dim4VertexEmbedding& getEmbedding(unsigned long index) const;

        /**
         * Returns the triangulation to which this vertex belongs.
         *
         * @return the triangulation containing this vertex.
         */
        Dim4Triangulation* getTriangulation() const;

        /**
         * Returns the component of the triangulation to which this
         * vertex belongs.
         *
         * @return the component containing this vertex.
         */
        Dim4Component* getComponent() const;

        /**
         * Returns the boundary component of the triangulation to which
         * this vertex belongs.
         *
         * See the note in the Dim4BoundaryComponent overview regarding what
         * happens if the vertex link itself has more than one boundary
         * component.  Note that such a vertex link makes the triangulation
         * invalid.
         *
         * An ideal vertex will have its own individual boundary
         * component to which it belongs.
         *
         * An invalid vertex will be given its own individual boundary
         * component if (and only if) it does not already belong to some
         * larger boundary component.
         *
         * @return the boundary component containing this vertex,
         * or 0 if this vertex is not on the boundary of the triangulation
         * as determined by isBoundary().
         */
        Dim4BoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the degree of this vertex.  Note that this is
         * identical to getNumberOfEmbeddings().
         *
         * @return the degree of this vertex.
         */
        unsigned long getDegree() const;

        /**
         * Returns a full 3-manifold triangulation describing
         * the link of this vertex.
         *
         * This routine is fast (it uses a pre-computed triangulation).
         * The downside is that the triangulation is read-only, and does
         * not contain any information on how the tetrahedra in the link
         * correspond to pentachora in the original triangulation
         * (though this is easily deduced; see below).  If you want a
         * writable triangulation, or one with this extra information,
         * then call buildLinkDetail() instead.
         *
         * The triangulation of the vertex link is built as follows.
         * Let \a i lie between 0 and getDegree()-1 inclusive, let
         * \a pent represent <tt>getEmbedding(i).getPentachoron()</tt>,
         * and let \a v represent <tt>getEmbedding(i).getVertex()</tt>.
         * Then <tt>buildLink()->getTetrahedron(i)</tt> is the tetrahedron
         * in the vertex link that "slices off" vertex \a v from
         * pentachoron \a pent.  In other words,
         * <tt>buildLink()->getTetrahedron(i)</tt> in the vertex link
         * is parallel to tetrahedron <tt>pent->getTetrahedron(v)</tt> in the
         * surrounding 4-manifold triangulation.
         *
         * The vertices of each tetrahedron in the vertex link are
         * numbered as follows.  Following the discussion above,
         * suppose that <tt>buildLink()->getTetrahedron(i)</tt>
         * sits within \c pent and is parallel to
         * <tt>pent->getTetrahedron(v)</tt>.
         * Then vertices 0,1,2,3 of the tetrahedron in the link will be
         * parallel to vertices 0,1,2,3 of the corresponding Dim4Tetrahedron.
         * The permutation <tt>pent->getTetrahedronMapping(v)</tt> will map
         * vertices 0,1,2,3 of the tetrahedron in the link to the
         * corresponding vertices of \c pent (those opposite \c v),
         * and will map 4 to \c v itself.
         *
         * This Dim4Vertex object will retain ownership of the triangulation
         * that is returned.  If you wish to edit the triangulation, you
         * should make a new clone and edit the clone instead.
         *
         * @return the read-only triangulated link of this vertex.
         */
        const NTriangulation* buildLink() const;

        /**
         * Returns a full 3-manifold triangulation describing
         * the link of this vertex.
         *
         * This routine is heavyweight (it computes a new triangulation
         * each time).  The benefit is that the triangulation is writeable,
         * and optionally contain detailed information on how the tetrahedra
         * in the link correspond to pentachora in the original triangulation.
         * If you do not need this extra information, consider using the
         * faster buildLink() instead.
         *
         * See the buildLink() documentation for an explanation of
         * exactly how the triangulation will be constructed.
         *
         * If \a labels is passed as \c true, each tetrahedron of the new
         * vertex link will be given a text description of the form
         * <tt>p&nbsp;(v)</tt>, where \c p is the index of the pentachoron
         * the tetrahedron is from, and \c v is the vertex of that pentachoron
         * that this tetrahedron links.
         *
         * If \a inclusion is non-null (i.e., it points to some
         * Dim4Isomorphism pointer \a p), then it will be modified to
         * point to a new Dim4Isomorphism that describes in detail how the
         * individual tetrahedra of the link sit within pentachora of
         * the original triangulation.  Specifically, after this routine
         * is called, <tt>p->pentImage(i)</tt> will indicate which pentachoron
         * \a pent of the 4-manifold triangulation contains the <i>i</i>th
         * tetrahedron of the link.  Moreover, <tt>p->facetPerm(i)</tt> will
         * indicate exactly where the <i>i</i>th tetrahedron sits within
         * \a pent: it will send 4 to the vertex of \a pent that the
         * tetrahedron links, and it will send 0,1,2,3 to the vertices of
         * \a pent that are parallel to vertices 0,1,2,3 of this tetrahedron.
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
         * @return a newly constructed triangulation of the link of this vertex.
         */
        NTriangulation* buildLinkDetail(bool labels = true,
            Dim4Isomorphism** inclusion = 0) const;

        /**
         * A synonym for buildLink().  This is provided for consistency
         * with the 3-dimensional analogue NTriangulation::getLink().
         *
         * See buildLink() for further details.
         *
         * @return the read-only triangulated link of this vertex.
         */
        const NTriangulation* getLink() const;

        /**
         * Determines if this vertex is valid.
         *
         * A vertex is valid if and only if its link is (i) a 3-ball,
         * or (ii) a closed compact valid 3-manifold.  Conversely, a
         * vertex is invalid if and only if its link is (i) invalid,
         * (ii) ideal, and/or (iii) bounded but not a 3-ball.
         *
         * Note that all invalid vertices are considered to be on the
         * boundary; see isBoundary() for details.
         *
         * @return \c true if and only if this vertex is valid.
         */
        bool isValid() const;

        /**
         * Determines if this vertex is an ideal vertex.
         * To be an ideal, a vertex must (i) be valid, and (ii) have
         * a closed vertex link that is not a 3-sphere.
         *
         * @return \c true if and only if this is an ideal vertex.
         */
        bool isIdeal() const;

        /**
         * Determines if this vertex lies on the boundary of the
         * triangulation.
         *
         * Ideal vertices form their own boundary components, and are
         * therefore considered to be on the boundary.
         *
         * Invalid vertices are always considered to be on the boundary;
         * if an invalid vertex is not already part of some larger boundary
         * component then it is given its own boundary component (much
         * like an ideal vertex).
         *
         * As a matter of fact, the only vertices that are \e not
         * considered as on the boundary are those whose links are 3-spheres.
         *
         * @return \c true if and only if this vertex lies on the boundary.
         * @see isIdeal()
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
        Dim4Vertex(Dim4Component* component);

    friend class Dim4Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim4/dim4pentachoron.h"
namespace regina {

// Inline functions for Dim4VertexEmbedding

inline Dim4VertexEmbedding::Dim4VertexEmbedding() : pent_(0) {
}

inline Dim4VertexEmbedding::Dim4VertexEmbedding(Dim4Pentachoron* pent,
        int vertex) :
        pent_(pent), vertex_(vertex) {
}

inline Dim4VertexEmbedding::Dim4VertexEmbedding(
        const Dim4VertexEmbedding& cloneMe) :
        pent_(cloneMe.pent_), vertex_(cloneMe.vertex_) {
}

inline Dim4VertexEmbedding& Dim4VertexEmbedding::operator =
        (const Dim4VertexEmbedding& cloneMe) {
    pent_ = cloneMe.pent_;
    vertex_ = cloneMe.vertex_;
    return *this;
}

inline Dim4Pentachoron* Dim4VertexEmbedding::getPentachoron() const {
    return pent_;
}

inline int Dim4VertexEmbedding::getVertex() const {
    return vertex_;
}

inline bool Dim4VertexEmbedding::operator == (const Dim4VertexEmbedding& other)
        const {
    return ((pent_ == other.pent_) && (vertex_ == other.vertex_));
}

inline bool Dim4VertexEmbedding::operator != (const Dim4VertexEmbedding& other)
        const {
    return ((pent_ != other.pent_) || (vertex_ != other.vertex_));
}

// Inline functions for Dim4Vertex

inline Dim4Vertex::Dim4Vertex(Dim4Component* component) :
        component_(component), boundaryComponent_(0), link_(0),
        valid_(true), ideal_(false) {
}

inline unsigned long Dim4Vertex::index() const {
    return markedIndex();
}

inline const std::vector<Dim4VertexEmbedding>& Dim4Vertex::getEmbeddings()
        const {
    return emb_;
}

inline unsigned long Dim4Vertex::getNumberOfEmbeddings() const {
    return emb_.size();
}

inline const Dim4VertexEmbedding& Dim4Vertex::getEmbedding(unsigned long index)
        const {
    return emb_[index];
}

inline NPerm5 Dim4VertexEmbedding::getVertices() const {
    return pent_->getVertexMapping(vertex_);
}

inline Dim4Triangulation* Dim4Vertex::getTriangulation() const {
    return emb_.front().getPentachoron()->getTriangulation();
}

inline Dim4Component* Dim4Vertex::getComponent() const {
    return component_;
}

inline Dim4BoundaryComponent* Dim4Vertex::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline unsigned long Dim4Vertex::getDegree() const {
    return emb_.size();
}

inline const NTriangulation* Dim4Vertex::buildLink() const {
    return link_;
}

inline const NTriangulation* Dim4Vertex::getLink() const {
    return link_;
}

inline bool Dim4Vertex::isValid() const {
    return valid_;
}

inline bool Dim4Vertex::isIdeal() const {
    return ideal_;
}

inline bool Dim4Vertex::isBoundary() const {
    return (boundaryComponent_ != 0);
}

} // namespace regina

#endif

