
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "shareableobject.h"
    #include "nperm.h"
    #include "ntetrahedron.h"
#else
    #include "engine/shareableobject.h"
    #include "engine/triangulation/nperm.h"
    #include "engine/triangulation/ntetrahedron.h"
#endif

class NTetrahedron;
class NComponent;
class NBoundaryComponent;

/**
 * Details how a face in the skeleton forms part of an individual
 * tetrahedron.
 *
 * \ifacescorba Not present.
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
 *
 * \idlfile <tt>Triangulation/NTetrahedron.idl</tt>
 */
class NFace : public ShareableObject {
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

    public:
        /**
         * Creates a new face and specifies it as belonging to the
         * given triangulation component.
         *
         * \ifaces Not present.
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
         * @param \c true if and only if this face lies on the
         * boundary.
         */
        bool isBoundary() const;
        
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

        void writeTextShort(ostream& out) const;

    friend class NTriangulation;
};

// Inline functions for NFace

inline NFace::NFace(NComponent* myComponent) : nEmbeddings(0),
        component(myComponent), boundaryComponent(0) {
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

inline bool NFace::isBoundary() const {
    return (boundaryComponent != 0);
}

inline unsigned NFace::getNumberOfEmbeddings() const {
    return nEmbeddings;
}

inline const NFaceEmbedding& NFace::getEmbedding(unsigned index) const {
    return *(embeddings[index]);
}

inline void NFace::writeTextShort(ostream& out) const {
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

#endif

