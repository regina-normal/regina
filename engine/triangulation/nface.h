
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file nface.h
 *  \brief Deals with faces in a triangulation.
 */

#ifndef __NFACE_H
#ifndef __DOXYGEN
#define __NFACE_H
#endif

#include "shareableobject.h"
#include "triangulation/nperm.h"
#include "triangulation/ntetrahedron.h"

namespace regina {

class NComponent;
class NBoundaryComponent;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Details how a face in the skeleton forms part of an individual
 * tetrahedron.
 */
class NFaceEmbedding {
    private:
        NTetrahedron* tetrahedron;
            /**< The tetrahedron in which this face is contained. */
        int face;
            /**< The face number of the tetrahedron that is this face. */

    public:
        /**
         * Creates an embedding descriptor containing the given data.
         *
         * @param newTet the tetrahedron in which this face is
         * contained.
         * @param newFace the face number of \a newTet that is this face.
         */
        NFaceEmbedding(NTetrahedron* newTet, int newFace);

        /**
         * Creates an embedding descriptor containing the same data as
         * the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        NFaceEmbedding(const NFaceEmbedding& cloneMe);

        /**
         * Returns the tetrahedron in which this face is contained.
         *
         * @return the tetrahedron.
         */
        NTetrahedron* getTetrahedron() const;

        /**
         * Returns the face number within getTetrahedron() that is
         * this face.
         *
         * @return the face number that is this face.
         */
        int getFace() const;

        /**
         * Returns a mapping from vertices (0,1,2) of this face to the
         * corresponding vertex numbers in getTetrahedron(), as described
         * in NTetrahedron::getFaceMapping().
         *
         * @return a mapping from the vertices of this face to the
         * vertices of getTetrahedron().
         */
        NPerm getVertices() const;
};

/**
 * Represents a face in the skeleton of a triangulation.
 * Faces are highly temporary; once a triangulation changes, all its
 * face objects will be deleted and new ones will be created.
 */
class NFace : public ShareableObject {
    public:
        static const int TRIANGLE;
            /**< Specifies a face with no identified vertices or edges. */
        static const int SCARF;
            /**< Specifies a face with two identified vertices. */
        static const int PARACHUTE;
            /**< Specifies a face with three identified vertices. */
        static const int CONE;
            /**< Specifies a face with two edges identified to form a cone. */
        static const int MOBIUS;
            /**< Specifies a face with two edges identified to form a
                 mobius band. */
        static const int HORN;
            /**< Specifies a face with two edges identified to form a
                 cone with all three vertices identified. */
        static const int DUNCEHAT;
            /**< Specifies a face with all three edges identified, some
                 via orientable and some via non-orientable gluings. */
        static const int L31;
            /**< Specifies a face with all three edges identified using
                 non-orientable gluings.  Note that this forms a spine for
                 the Lens space L(3,1). */
    private:
        NFaceEmbedding* embeddings[2];
            /**< An array of descriptors of how this face forms a part of
                 each individual tetrahedron it belongs to.
                 These embeddings will be automatically deleted when the
                 face itself is deleted. */
        int nEmbeddings;
            /**< The number of embedding descriptors stored in
                 the embeddings array. */
        NComponent* component;
            /**< The component that this vertex is a part of. */
        NBoundaryComponent* boundaryComponent;
            /**< The boundary component that this face is a part of,
                 or 0 if this face is internal. */
        int type;
            /**< Specifies the face type according to one of the
                 predefined face type constants in NFace, or 0 if type has
                 not yet been determined. */
        int subtype;
            /**< Specifies the vertex or edge that plays a special role
                 for the face type specified by \a type.  This is only
                 relevant for some face types. */

    public:
        /**
         * Creates a new face and specifies it as belonging to the
         * given triangulation component.
         *
         * \ifacespython Not present.
         *
         * @param myComponent the triangulation component to which this
         * face belongs.
         */
        NFace(NComponent* myComponent);

        /**
         * Default destructor.
         * All embedding descriptors stored in this face will be
         * automatically deleted.
         */
        virtual ~NFace();

        /**
         * Determines if this face lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this face lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Returns a description of the face type.
         * The face type describes how the edges and vertices of the
         * face are identified.
         *
         * @return one of the predefined face type constants in NFace.
         */
        int getType();

        /**
         * Return the face vertex or face edge that plays a special role
         * for the face type of this face.  Note that this routine is
         * only relevant for some face types.  The face type is returned by
         * getType().
         *
         * @return The vertex or edge that plays a special role (this
         * will be 0, 1 or 2), or -1 if this face type has no special
         * vertex or edge.
         */
        int getSubtype();

        /**
         * Determines whether this face is wrapped up to form a Mobius band.
         *
         * Note that several different face types (as returned by
         * getType()) can produce this result.
         * Note also that a face can be both a Mobius band \a and a cone.
         *
         * @return \c true if and only if this face is a Mobius band.
         */
        bool isMobiusBand();

        /**
         * Determines whether this face is wrapped up to form a cone.
         *
         * Note that several different face types (as returned by
         * getType()) can produce this result.
         * Note also that a face can be both a Mobius band \a and a cone.
         *
         * @return \c true if and only if this face is a cone.
         */
        bool isCone();

        /**
         * Returns the number of descriptors available through
         * getEmbedding().
         *
         * @return the number of embedding descriptors.
         */
        unsigned getNumberOfEmbeddings() const;

        /**
         * Returns the requested descriptor of how this face forms a
         * part of a particular tetrahedron in the triangulation.
         * Note that if this face represents multiple faces of a
         * particular tetrahedron, there will be multiple embedding
         * descriptors available regarding that tetrahedron.
         *
         * @param index the index of the requested descriptor.  This
         * should be between 0 and getNumberOfEmbeddings()-1 inclusive.
         * @return the requested embedding descriptor.
         */
        const NFaceEmbedding& getEmbedding(unsigned index) const;

        /**
         * Returns the component of the triangulation to which this
         * face belongs.
         *
         * @return the component containing this face.
         */
        NComponent* getComponent() const;

        /**
         * Returns the boundary component of the triangulation to which
         * this face belongs.
         *
         * @return the boundary component containing this face,
         * or 0 if this face is not on the boundary of the triangulation.
         */
        NBoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex in the triangulation skeleton corresponding
         * to the given vertex of this face.
         *
         * Note that face vertex \a i is opposite face edge \a i.
         *
         * @param vertex the vertex of this face to examine.  This should
         * be 0, 1 or 2.
         * @return the vertex of the skeleton corresponding to the
         * requested face vertex.
         */
        NVertex* getVertex(int vertex) const;
        /**
         * Returns the edge of the triangulation skeleton corresponding
         * to the given edge of this face.
         *
         * Note that face vertex \a i is opposite face edge \a i.
         *
         * @param edge the edge of this face to examine.  This should be
         * 0, 1 or 2.
         * @return the edge of the skeleton corresponding to the
         * requested face edge.
         */
        NEdge* getEdge(int edge) const;
        /**
         * The edge in the skeleton corresponding to the requested edge
         * of this face is examined, and a permutation mapping vertices
         * (0,1) of the skeleton edge to the corresponding vertices of
         * this face is returned.
         *
         * Thus for each <tt>i=0,1</tt> and each face (or tetrahedron)
         * containing this
         * skeleton edge, <tt>getEdgeMapping(...)[i]</tt> will refer to
         * vertices that are all identified to each other along the
         * skeleton edge concerned.
         *
         * The permutation returned will map 2 to the remaining face
         * vertex (which will be parameter \a edge) and will map 3 to 3.
         *
         * @param edge the edge of this face to examine.  This should be
         * 0, 1 or 2.
         * @return a mapping from vertices (0,1) of the requested edge to
         * the vertices of this face.
         */
        NPerm getEdgeMapping(int edge) const;

        void writeTextShort(std::ostream& out) const;

    friend class NTriangulation;
        /**< Allow access to private members. */
};

/*@}*/

// Inline functions for NFace

inline NFace::NFace(NComponent* myComponent) : nEmbeddings(0),
        component(myComponent), boundaryComponent(0), type(0) {
}

inline NFace::~NFace() {
    if (nEmbeddings > 0)
        delete embeddings[0];
    if (nEmbeddings > 1)
        delete embeddings[1];
}

inline NComponent* NFace::getComponent() const {
    return component;
}

inline NBoundaryComponent* NFace::getBoundaryComponent() const {
    return boundaryComponent;
}

inline NVertex* NFace::getVertex(int vertex) const {
    return embeddings[0]->getTetrahedron()->getVertex(
        embeddings[0]->getVertices()[vertex]);
}

inline bool NFace::isBoundary() const {
    return (boundaryComponent != 0);
}

inline int NFace::getSubtype() {
    getType();
    return subtype;
}

inline bool NFace::isMobiusBand() {
    getType();
    return (type == L31 || type == DUNCEHAT || type == MOBIUS);
}

inline bool NFace::isCone() {
    getType();
    return (type == DUNCEHAT || type == CONE || type == HORN);
}

inline unsigned NFace::getNumberOfEmbeddings() const {
    return nEmbeddings;
}

inline const NFaceEmbedding& NFace::getEmbedding(unsigned index) const {
    return *(embeddings[index]);
}

inline void NFace::writeTextShort(std::ostream& out) const {
    out << (isBoundary() ? "Boundary " : "Internal ") << "face";
}

inline NFaceEmbedding::NFaceEmbedding(NTetrahedron* newTet, int newFace) :
        tetrahedron(newTet), face(newFace) {
}

inline NFaceEmbedding::NFaceEmbedding(const NFaceEmbedding& cloneMe) :
        tetrahedron(cloneMe.tetrahedron), face(cloneMe.face) {
}

inline NTetrahedron* NFaceEmbedding::getTetrahedron() const {
    return tetrahedron;
}

inline int NFaceEmbedding::getFace() const {
    return face;
}

inline NPerm NFaceEmbedding::getVertices() const {
    return tetrahedron->getFaceMapping(face);
}

} // namespace regina

#endif

