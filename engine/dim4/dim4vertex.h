
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

/*! \file dim4/dim4vertex.h
 *  \brief Deals with vertices in a 4-manifold triangulation.
 */

#ifndef __DIM4VERTEX_H
#ifndef __DOXYGEN
#define __DIM4VERTEX_H
#endif

#include <vector>
#include "dim4/dim4pentachoron.h"

namespace regina {

class Dim4Component;
class Dim4BoundaryComponent;
class NTriangulation;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Details how a vertex in the skeleton of a 4-manifold triangulation forms
 * part of an individual pentachoron.
 */
class Dim4VertexEmbedding {
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
};

/**
 * Represents a vertex in the skeleton of a 4-manifold triangulation.
 * Vertices are highly temporary; once a triangulation changes, all its
 * vertex objects will be deleted and new ones will be created.
 */
class Dim4Vertex : public ShareableObject, public NMarkedElement {
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
        virtual ~Dim4Vertex();

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
         * Returns the link of this vertex.
         *
         * The vertex link is a 3-manifold, and so this routine returns
         * a full-blown 3-manifold triangulation.  Note that this
         * triangulation is read-only (though of course you can clone it
         * and then operate upon the clone).
         *
         * It is guaranteed that 3-sphere recognition has already been
         * run over this vertex link.  This means that, amongst other
         * things, calls to NTriangulation::isThreeSphere() will be very
         * fast, and calls to NTriangulation::knowsThreeSphere() will
         * return \c true.
         *
         * The triangulation of the vertex link is built as follows.
         * Let \a i lie between 0 and getDegree()-1 inclusive, let
         * \a pent represent <tt>getEmbedding(i).getPentachoron()</tt>,
         * and let \a v represent <tt>getEmbedding(i).getVertex()</tt>.
         * Then <tt>getLink()->getTetrahedron(i)</tt> corresponds to 
         * <tt>pent->getTetrahedron(v)</tt>. Moreover, the correspondence
         * from the vertices of <tt>getLink()->getTetrahedron(i)</tt> 
         * to the vertices of <tt>pent->getTetrahedron(v)</tt> is given by
         * the map <tt>getEmbedding(i).getVertices().inverse()*
         * pent->getTetrahedronMapping(v)</tt>.  This is an <tt>NPerm5</tt>
         * object, sending 4 to 0, and sending 0123 (the vertices of the
         * tetrahedron in the domain) to 1234 (the vertices of the tetrahedron
         * in the range). 
         *
         * @return the link of this vertex.
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

        void writeTextShort(std::ostream& out) const;

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

// Inline functions for Dim4Vertex

inline Dim4Vertex::Dim4Vertex(Dim4Component* component) :
        component_(component), boundaryComponent_(0), link_(0),
        valid_(true), ideal_(false) {
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

inline Dim4Component* Dim4Vertex::getComponent() const {
    return component_;
}

inline Dim4BoundaryComponent* Dim4Vertex::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline unsigned long Dim4Vertex::getDegree() const {
    return emb_.size();
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

