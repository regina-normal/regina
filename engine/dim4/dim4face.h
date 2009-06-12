
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

/*! \file dim4/dim4face.h
 *  \brief Deals with faces in a 4-manifold triangulation.
 */

#ifndef __DIM4FACE_H
#ifndef __DOXYGEN
#define __DIM4FACE_H
#endif

#include <deque>
#include "shareableobject.h"
#include "dim4/dim4pentachoron.h"

namespace regina {

class Dim4Component;
class Dim4BoundaryComponent;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Details how a face in the skeleton of a 4-manifold triangulation forms
 * part of an individual pentachoron.
 */
class Dim4FaceEmbedding {
    private:
        Dim4Pentachoron* pent_;
            /**< The pentachoron in which this face is contained. */
        int face_;
            /**< The face number of the pentachoron that is this face. */

    public:
        /**
         * Default constructor.  The embedding descriptor created is
         * unusable until it has some data assigned to it using
         * <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        Dim4FaceEmbedding();

        /**
         * Creates an embedding descriptor containing the given data.
         *
         * @param pent the pentachoron in which this face is contained.
         * @param face the face number of \a pent that is this face.
         */
        Dim4FaceEmbedding(Dim4Pentachoron* pent, int face);

        /**
         * Creates an embedding descriptor containing the same data as
         * the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim4FaceEmbedding(const Dim4FaceEmbedding& cloneMe);

        /**
         * Assigns to this embedding descriptor the same data as is
         * contained in the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim4FaceEmbedding& operator =(const Dim4FaceEmbedding& cloneMe);

        /**
         * Returns the pentachoron in which this face is contained.
         *
         * @return the pentachoron.
         */
        Dim4Pentachoron* getPentachoron() const;

        /**
         * Returns the face number within getPentachoron() that is this face.
         *
         * @return the face number that is this face.
         */
        int getFace() const;

        /**
         * Returns a mapping from vertices (0,1,2) of this face to the
         * corresponding vertex numbers in getPentachoron().  This
         * permutation also maps (3,4) to the two remaining pentachoron
         * vertices in a manner that preserves orientation as you walk
         * around the face.  See Dim4Pentachoron::getFaceMapping() for details.
         *
         * @return a mapping from the vertices of this face to the
         * vertices of getPentachoron().
         */
        NPerm5 getVertices() const;
};

/**
 * Represents a face in the skeleton of a 4-manifold triangulation.
 * Faces are highly temporary; once a triangulation changes, all its
 * face objects will be deleted and new ones will be created.
 */
class Dim4Face : public ShareableObject, public NMarkedElement {
    public:
        /**
         * A table that maps vertices of a pentachoron to face numbers.
         *
         * Faces in a pentachoron are numbered 0,...,9.  This table
         * converts vertices to face numbers; in particular, the face
         * spanned by vertices \a i, \a j and \a k of a pentachoron is
         * face number <tt>faceNumber[i][j][k]</tt>.  Here \a i, \a j and \a k
         * must be distinct, must be between 0 and 4 inclusive, and may
         * be given in any order.  The resulting face number will be
         * between 0 and 9 inclusive.
         *
         * Note that face \i is always opposite edge \i in a pentachoron.
         *
         * This is analagous to the lookup table NEdge::edgeNumber
         * for 3-manifold triangulations (and its deprecated predecessor,
         * the old global array regina::edgeNumber).
         */
        static const int faceNumber[5][5][5];

        /**
         * A table that maps faces of a pentachoron to vertex numbers.
         *
         * Faces in a pentachoron are numbered 0,...,9.  This table
         * converts face numbers to vertices; in particular, face \a i
         * in a pentachoron is spanned by vertices <tt>faceVertex[i][0]</tt>,
         * <tt>faceVertex[i][1]</tt> and <tt>faceVertex[i][2]</tt>.
         * Here \a i must be between 0 and 9 inclusive; the resulting
         * vertex numbers will be between 0 and 4 inclusive.
         *
         * Note that face \i is always opposite edge \i in a pentachoron.
         * It is guaranteed that <tt>faceVertex[i][0]</tt> will always
         * be smaller than <tt>faceVertex[i][1]</tt>, which in turn will
         * always be smaller than <tt>faceVertex[i][2]</tt>.
         *
         * This is analagous to the lookup table NEdge::edgeVertex
         * for 3-manifold triangulations (and its deprecated predecessors,
         * the old global arrays regina::edgeStart and regina::edgeEnd).
         */
        static const int faceVertex[10][3];

        /**
         * An array that maps face numbers within a pentachoron to the
         * canonical ordering of the individual pentachoron vertices
         * that form each face.
         *
         * This means that the vertices of face \a i in a pentachoron
         * are, in canonical order, <tt>ordering[i][0..2]</tt>.  The
         * images of 3 and 4 under each permutation are chosen to make
         * each permutation even.
         *
         * This table does \e not describe the mapping from specific
         * triangulation faces into individual pentachora (for that, see
         * Dim4Pentachoron::getFaceMapping() instead).  This table
         * merely provides a neat and consistent way of listing the
         * vertices of any given pentachoron face.
         */
        static const NPerm5 ordering[10];

    private:
        std::deque<Dim4FaceEmbedding> emb_;
            /**< A list of descriptors telling how this face forms a part of
                 each individual pentachoron that it belongs to. */
        Dim4Component* component_;
            /**< The component that this face is a part of. */
        Dim4BoundaryComponent* boundaryComponent_;
            /**< The boundary component that this face is a part of,
                 or 0 if this face is internal. */
        bool valid_;
            /**< Is this face valid? */

    public:
        /**
         * Default destructor.
         */
        ~Dim4Face();

        /**
         * Returns the list of descriptors detailing how this face forms a
         * part of various pentachora in the 4-manifold triangulation.
         * Note that if this face represents multiple faces of a
         * particular pentachoron, then there will be multiple embedding
         * descriptors in the list regarding that pentachoron.
         *
         * These embedding descriptors will be stored in order in the
         * list, so that if you run through the list and follow in turn
         * the edges of each pentachoron defined by the images of (3,4)
         * under Dim4FaceEmbedding::getVertices(), then you will obtain
         * an ordered chain circling this face.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of embedding descriptors.
         * @see Dim4FaceEmbedding
         */
        const std::deque<Dim4FaceEmbedding>& getEmbeddings() const;

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
        const Dim4FaceEmbedding& getEmbedding(unsigned long index) const;

        /**
         * Returns the component of the triangulation to which this
         * face belongs.
         *
         * @return the component containing this face.
         */
        Dim4Component* getComponent() const;

        /**
         * Returns the boundary component of the triangulation to which
         * this face belongs.
         *
         * @return the boundary component containing this face, or 0 if this
         * face does not lie entirely within the boundary of the triangulation.
         */
        Dim4BoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex of the 4-manifold triangulation corresponding
         * to the given vertex of this face.
         *
         * Note that vertex \a i of a face is opposite edge \a i of the face.
         *
         * @param vertex the vertex of this face to examine.  This should
         * be 0, 1 or 2.
         * @return the corresponding vertex of the 4-manifold triangulation.
         */
        Dim4Vertex* getVertex(int vertex) const;

        /**
         * Returns the edge of the 4-manifold triangulation corresponding
         * to the given edge of this face.
         *
         * Note that edge \a i of a face is opposite vertex \a i of the face.
         *
         * @param edge the edge of this face to examine.  This should
         * be 0, 1 or 2.
         * @return the corresponding edge of the 4-manifold triangulation.
         */
        Dim4Edge* getEdge(int edge) const;

        /**
         * Examines the given edge of this face, and returns a mapping
         * from the "canonical" vertices of the corresponding edge of
         * the triangulation to the vertices of this face.
         *
         * This routine behaves much the same way as
         * Dim4Pentachoron::getEdgeMapping(), except that it maps the
         * edge vertices into a face, not into a pentachoron.  See
         * Dim4Pentachoron::getEdgeMapping() for a more detailed
         * explanation of precisely what this mapping means.
         *
         * This routine differs from Dim4Pentachoron::getEdgeMapping()
         * in how it handles the images of 2, 3 and 4.  This routine
         * will always map 2 to the remaining vertex of this face (which
         * is equal to the argument \a edge), and will always map 3 and 4
         * to themselves.
         *
         * @param edge the edge of this face to examine.  This should be
         * 0, 1 or 2.
         * @return a mapping from vertices (0,1) of the requested edge
         * to the vertices of this face.
         */
        NPerm5 getEdgeMapping(int edge) const;

        /**
         * Returns the degree of this face.  Note that this is identical
         * to getNumberOfEmbeddings().
         *
         * @return the degree of this face.
         */
        unsigned long getDegree() const;

        /**
         * Determines if this face lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this face lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Determines if this face is valid.
         * A face is valid if and only if it is not glued to itself
         * using a non-trivial rotation or reflection.
         *
         * @return \c true if and only if this face is valid.
         */
        bool isValid() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new face and marks it as belonging to the
         * given triangulation component.
         *
         * @param component the triangulation component to which this
         * face belongs.
         */
        Dim4Face(Dim4Component* component);

    friend class Dim4Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

// Inline functions for Dim4FaceEmbedding

inline Dim4FaceEmbedding::Dim4FaceEmbedding() : pent_(0) {
}

inline Dim4FaceEmbedding::Dim4FaceEmbedding(Dim4Pentachoron* pent, int face) :
        pent_(pent), face_(face) {
}

inline Dim4FaceEmbedding::Dim4FaceEmbedding(const Dim4FaceEmbedding& cloneMe) :
        pent_(cloneMe.pent_), face_(cloneMe.face_) {
}

inline Dim4FaceEmbedding& Dim4FaceEmbedding::operator =
        (const Dim4FaceEmbedding& cloneMe) {
    pent_ = cloneMe.pent_;
    face_ = cloneMe.face_;
    return *this;
}

inline Dim4Pentachoron* Dim4FaceEmbedding::getPentachoron() const {
    return pent_;
}

inline int Dim4FaceEmbedding::getFace() const {
    return face_;
}

inline NPerm5 Dim4FaceEmbedding::getVertices() const {
    return pent_->getFaceMapping(face_);
}

// Inline functions for Dim4Face

inline Dim4Face::Dim4Face(Dim4Component* component) :
        component_(component), boundaryComponent_(0), valid_(true) {
}

inline Dim4Face::~Dim4Face() {
}

inline const std::deque<Dim4FaceEmbedding>& Dim4Face::getEmbeddings() const {
    return emb_;
}

inline unsigned long Dim4Face::getNumberOfEmbeddings() const {
    return emb_.size();
}

inline const Dim4FaceEmbedding& Dim4Face::getEmbedding(unsigned long index)
        const {
    return emb_[index];
}

inline Dim4Component* Dim4Face::getComponent() const {
    return component_;
}

inline Dim4BoundaryComponent* Dim4Face::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline Dim4Vertex* Dim4Face::getVertex(int vertex) const {
    const Dim4FaceEmbedding& e(emb_.front());
    return e.getPentachoron()->getVertex(e.getVertices()[vertex]);
}

inline unsigned long Dim4Face::getDegree() const {
    return emb_.size();
}

inline bool Dim4Face::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline bool Dim4Face::isValid() const {
    return valid_;
}

inline void Dim4Face::writeTextShort(std::ostream& out) const {
    out << (boundaryComponent_ ? "Boundary " : "Internal ")
        << "face of degree " << emb_.size();
}

} // namespace regina

#endif

