
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

/*! \file triangulation/nvertex.h
 *  \brief Deals with vertices in a triangulation.
 */

#ifndef __NVERTEX_H
#ifndef __DOXYGEN
#define __NVERTEX_H
#endif

#include <vector>
#include "regina-core.h"
#include "shareableobject.h"
#include "maths/nperm4.h"
#include "utilities/nmarkedvector.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class NBoundaryComponent;
class NComponent;
class Dim2Triangulation;
class NTetrahedron;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Details how a vertex in the skeleton forms part of an individual
 * tetrahedron.
 */
class REGINA_API NVertexEmbedding {
    private:
        NTetrahedron* tetrahedron;
            /**< The tetrahedron in which this vertex is contained. */
        int vertex;
            /**< The vertex number of the tetrahedron that is this vertex. */

    public:
        /**
         * Default constructor.  The embedding descriptor created is
         * unusable until it has some data assigned to it using
         * <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        NVertexEmbedding();

        /**
         * Creates an embedding descriptor containing the given data.
         *
         * @param newTet the tetrahedron in which this vertex is
         * contained.
         * @param newVertex the vertex number of \a newTet that is this vertex.
         */
        NVertexEmbedding(NTetrahedron* newTet, int newVertex);

        /**
         * Creates an embedding descriptor containing the same data as
         * the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        NVertexEmbedding(const NVertexEmbedding& cloneMe);

        /**
         * Assigns to this embedding descriptor the same data as is
         * contained in the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        NVertexEmbedding& operator =(const NVertexEmbedding& cloneMe);

        /**
         * Returns the tetrahedron in which this vertex is contained.
         *
         * @return the tetrahedron.
         */
        NTetrahedron* getTetrahedron() const;

        /**
         * Returns the vertex number within getTetrahedron() that is
         * this vertex.
         *
         * @return the vertex number that is this vertex.
         */
        int getVertex() const;

        /**
         * Returns a permutation that maps 0 to the vertex number within
         * getTetrahedron() that is this vertex.  The real point of this
         * routine is that (1,2,3) maps to the three remaining tetrahedron
         * vertices in a manner that preserves orientation as you walk around
         * the vertex (assuming this is actually possible).  See
         * NTetrahedron::getVertexMapping() for details.
         *
         * @return a permutation that maps 0 to the vertex number that
         * is this vertex.
         */
        NPerm4 getVertices() const;
};

/**
 * Represents a vertex in the skeleton of a triangulation.
 * Vertices are highly temporary; once a triangulation changes, all its
 * vertex objects will be deleted and new ones will be created.
 *
 * \testpart
 */
class REGINA_API NVertex : public ShareableObject, public NMarkedElement {
    public:
        static const int SPHERE;
            /**< Specifies a vertex link that is a sphere. */
        static const int DISC;
            /**< Specifies a vertex link that is a disc. */
        static const int TORUS;
            /**< Specifies a vertex link that is a torus. */
        static const int KLEIN_BOTTLE;
            /**< Specifies a vertex link that is a Klein bottle. */
        static const int NON_STANDARD_CUSP;
            /**< Specifies a vertex link that is closed and is not a
                 sphere, torus or Klein bottle. */
        static const int NON_STANDARD_BDRY;
            /**< Specifies a vertex link that has boundary and is not a
                 disc. */
    private:
        std::vector<NVertexEmbedding> embeddings;
            /**< A list of descriptors telling how this vertex forms a part of
                 each individual tetrahedron that it belongs to. */
        NComponent* component;
            /**< The component that this vertex is a part of. */
        NBoundaryComponent* boundaryComponent;
            /**< The boundary component that this vertex is a part of,
                 or 0 if this vertex is internal. */
        int link;
            /**< Specifies the link of the vertex according to one of the
                 predefined vertex link constants in NVertex. */
        bool linkOrientable;
            /**< Specifies whether the vertex link is orientable. */
        long linkEulerCharacteristic;
            /**< Specifies the Euler characteristic of the vertex link. */
        Dim2Triangulation* linkTri;
            /**< A triangulation of the vertex link.  This will only be
                 constructed on demand; until then it will be null. */

    public:
        /**
         * Default destructor.
         */
        virtual ~NVertex();

        /**
         * Returns the list of descriptors detailing how this vertex forms a
         * part of various tetrahedra in the triangulation.
         * Note that if this vertex represents multiple vertices of a
         * particular tetrahedron, then there will be multiple embedding
         * descriptors in the list regarding that tetrahedron.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of embedding descriptors.
         * @see NVertexEmbedding
         */
        const std::vector<NVertexEmbedding>& getEmbeddings() const;

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
        const NVertexEmbedding& getEmbedding(unsigned long index) const;

        /**
         * Returns the component of the triangulation to which this
         * vertex belongs.
         *
         * @return the component containing this vertex.
         */
        NComponent* getComponent() const;

        /**
         * Returns the boundary component of the triangulation to which
         * this vertex belongs.
         *
         * See the note in the NBoundaryComponent overview regarding what
         * happens if the vertex link is a multiply punctured surface.
         * Note that this makes the vertex non-standard and the
         * triangulation invalid.
         *
         * An ideal vertex will have its own individual boundary
         * component to which it belongs.
         *
         * @return the boundary component containing this vertex,
         * or 0 if this vertex is not on the boundary of the triangulation
         * as determined by isBoundary().
         */
        NBoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the degree of this vertex.  Note that this is
         * identical to getNumberOfEmbeddings().
         *
         * @return the degree of this vertex.
         */
        unsigned long getDegree() const;

        /**
         * Returns a description of the link of the vertex.
         *
         * This routine does not require a full triangulation of the
         * vertex link, and so can be much faster than analysing the
         * result of buildLink().
         *
         * @return one of the predefined link constants in NVertex.
         */
        int getLink() const;

        /**
         * Returns a full triangulation of the link of this vertex.
         *
         * The vertex link is a 2-manifold, and this routine returns a
         * full-blown 2-manifold triangulation.  Note that this
         * triangulation is read-only (though of course you can clone it
         * and then operate upon the clone).
         *
         * The triangulation of the vertex link is built as follows.
         * Let \a i lie between 0 and getDegree()-1 inclusive, let
         * \a tet represent <tt>getEmbedding(i).getTetrahedron()</tt>,
         * and let \a v represent <tt>getEmbedding(i).getVertex()</tt>.
         * Then <tt>buildLink()->getFace(i)</tt> is the triangle
         * in the vertex link that "slices off" vertex \a v from
         * tetrahedron \a tet.  In other words,
         * <tt>buildLink()->getFace(i)</tt> in the vertex link
         * corresponds to <tt>tet->getFace(v)</tt> in the
         * surrounding 3-manifold triangulation.
         *
         * The vertices of each triangle in the vertex link are
         * numbered as follows.  Following the discussion above,
         * suppose that <tt>buildLink()->getFace(i)</tt>
         * corresponds to <tt>tet->getFace(v)</tt>.
         * The correspondence from the vertices of
         * <tt>buildLink()->getFace(i)</tt> to the vertices of
         * <tt>tet->getFace(v)</tt> is given by the map
         * <tt>tet->getFaceMapping(v)</tt>.
         * This is an <tt>NPerm4</tt> object, sending 4 to \a v, and sending
         * the vertices of the face in the domain (012) to
         * the vertices of the face in the range (0123 excluding \a v).
         *
         * @return the triangulated link of this vertex.
         */
        const Dim2Triangulation* buildLink() const;

        /**
         * Determines if the link of this vertex is closed.
         *
         * @return \c true if and only if the link of this vertex is
         * closed.
         */
        bool isLinkClosed() const;

        /**
         * Determines if this vertex is an ideal vertex.
         * This requires the vertex link to be closed and not a
         * 2-sphere.
         *
         * @return \c true if and only if this is an ideal vertex.
         */
        bool isIdeal() const;

        /**
         * Determines if this vertex lies on the boundary of the
         * triangulation.  Ideal vertices are included as
         * being on the boundary.  In fact, the only vertices not
         * considered as on the boundary are those whose links are
         * spheres.
         *
         * @return \c true if and only if this vertex lies on the boundary.
         * @see isIdeal()
         */
        bool isBoundary() const;

        /**
         * Determines if this vertex is standard.
         * This requires the vertex link to be a sphere, disc, torus or
         * Klein bottle.
         *
         * @return \c true if and only if this vertex is standard.
         */
        bool isStandard() const;

        /**
         * Determines if the vertex link is orientable.
         *
         * This routine does not require a full triangulation of the
         * vertex link, and so can be much faster than calling
         * buildLink().isOrientable().
         *
         * @return \c true if and only if the vertex link is orientable.
         */
        bool isLinkOrientable() const;

        /**
         * Returns the Euler characteristic of the vertex link.
         *
         * This routine does not require a full triangulation of the
         * vertex link, and so can be much faster than calling
         * buildLink().getEulerChar().
         *
         * @return the Euler characteristic of the vertex link.
         */
        long getLinkEulerCharacteristic() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new vertex and marks it as belonging to the
         * given triangulation component.
         *
         * @param myComponent the triangulation component to which this
         * vertex belongs.
         */
        NVertex(NComponent* myComponent);

    friend class NTriangulation;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/ntetrahedron.h"
namespace regina {

// Inline functions for NVertex

inline NVertex::NVertex(NComponent* myComponent) : component(myComponent),
        boundaryComponent(0), linkOrientable(true),
        linkEulerCharacteristic(0), linkTri(0) {
}

inline NComponent* NVertex::getComponent() const {
    return component;
}

inline NBoundaryComponent* NVertex::getBoundaryComponent() const {
    return boundaryComponent;
}

inline unsigned long NVertex::getDegree() const {
    return embeddings.size();
}

inline int NVertex::getLink() const {
    return link;
}

inline bool NVertex::isLinkClosed() const {
    return (link != DISC && link != NON_STANDARD_BDRY);
}

inline bool NVertex::isIdeal() const {
    return (link == TORUS || link == KLEIN_BOTTLE ||
        link == NON_STANDARD_CUSP);
}

inline bool NVertex::isBoundary() const {
    return (boundaryComponent != 0);
}

inline bool NVertex::isStandard() const {
    return (link != NON_STANDARD_CUSP && link != NON_STANDARD_BDRY);
}

inline bool NVertex::isLinkOrientable() const {
    return linkOrientable;
}

inline long NVertex::getLinkEulerCharacteristic() const {
    return linkEulerCharacteristic;
}

inline const std::vector<NVertexEmbedding>& NVertex::getEmbeddings() const {
    return embeddings;
}

inline unsigned long NVertex::getNumberOfEmbeddings() const {
    return embeddings.size();
}

inline const NVertexEmbedding& NVertex::getEmbedding(unsigned long index)
        const {
    return embeddings[index];
}

inline NVertexEmbedding::NVertexEmbedding() : tetrahedron(0) {
}

inline NVertexEmbedding::NVertexEmbedding(NTetrahedron* newTet, int newVertex) :
        tetrahedron(newTet), vertex(newVertex) {
}

inline NVertexEmbedding::NVertexEmbedding(const NVertexEmbedding& cloneMe) :
        tetrahedron(cloneMe.tetrahedron), vertex(cloneMe.vertex) {
}

inline NVertexEmbedding& NVertexEmbedding::operator =
        (const NVertexEmbedding& cloneMe) {
    tetrahedron = cloneMe.tetrahedron;
    vertex = cloneMe.vertex;
    return *this;
}

inline NTetrahedron* NVertexEmbedding::getTetrahedron() const {
    return tetrahedron;
}

inline int NVertexEmbedding::getVertex() const {
    return vertex;
}

inline NPerm4 NVertexEmbedding::getVertices() const {
    return tetrahedron->getVertexMapping(vertex);
}

} // namespace regina

#endif

