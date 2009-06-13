
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

#ifndef __NBOUNDARYCOMPONENT_H
#ifndef __DOXYGEN
#define __NBOUNDARYCOMPONENT_H
#endif

/*! \file triangulation/nboundarycomponent.h
 *  \brief Deals with components of the boundary of a triangulation.
 */

#include <vector>
#include "triangulation/nvertex.h"

namespace regina {

class NTetrahedron;
class NFace;
class NEdge;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Represents a component of the boundary of a triangulation.
 * Note that an ideal vertex constitutes a boundary component of its
 * own. 
 *
 * We can run into some interesting cases with invalid triangulations.
 * Suppose some vertex link is a multiply punctured surface (which makes
 * the vertex and hence the entire triangulation invalid).  This means
 * that different parts of the 3-manifold boundary are effectively
 * "pinched" together.  If this happens, the different parts of the
 * boundary that are pinched might or might not be listed as part of the
 * same boundary component; if not then the offending vertex will be
 * included in all of these boundary components.  Nevertheless, only one
 * of these can be considered the "official" boundary component of the
 * vertex as returned by NVertex::getBoundaryComponent().  This is all a
 * bit of a mess, but then again the entire triangulation is invalid and
 * so you almost certainly have bigger problems to deal with.
 *
 * Boundary components are highly temporary; once a triangulation
 * changes, all its boundary component objects will be deleted and new
 * ones will be created.
 */
class NBoundaryComponent : public ShareableObject, public NMarkedElement {
    private:
        std::vector<NFace*> faces;
            /**< List of faces in the component. */
        std::vector<NEdge*> edges;
            /**< List of edges in the component. */
        std::vector<NVertex*> vertices;
            /**< List of vertices in the component. */

        bool orientable;
            /**< Is this boundary component orientable? */

    public:
        /**
         * Default destructor.
         */
        virtual ~NBoundaryComponent();

        /**
         * Returns the number of faces in this boundary component.
         *
         * @return the number of faces.
         */
        unsigned long getNumberOfFaces() const;

        /**
         * Returns the number of edges in this boundary component.
         *
         * @return the number of edges.
         */
        unsigned long getNumberOfEdges() const;

        /**
         * Returns the number of vertices in this boundary component.
         *
         * @return the number of vertices.
         */
        unsigned long getNumberOfVertices() const;

        /**
         * Returns the requested face in this boundary component.
         *
         * @param index the index of the requested face in the boundary
         * component.  This should be between 0 and getNumberOfFaces()-1
         * inclusive.
         * Note that the index of a face in the boundary component need
         * not be the index of the same face in the entire
         * triangulation.
         * @return the requested face.
         */
        NFace* getFace(unsigned long index) const;

        /**
         * Returns the requested edge in this boundary component.
         *
         * @param index the index of the requested edge in the boundary
         * component.  This should be between 0 and getNumberOfEdges()-1
         * inclusive.
         * Note that the index of a edge in the boundary component need
         * not be the index of the same edge in the entire
         * triangulation.
         * @return the requested edge.
         */
        NEdge* getEdge(unsigned long index) const;

        /**
         * Returns the requested vertex in this boundary component.
         *
         * @param index the index of the requested vertex in the boundary
         * component.  This should be between 0 and getNumberOfVertices()-1
         * inclusive.
         * Note that the index of a vertex in the boundary component need
         * not be the index of the same vertex in the entire
         * triangulation.
         * @return the requested vertex.
         */
        NVertex* getVertex(unsigned long index) const;

        /**
         * Returns the Euler characteristic of this boundary component.
         * If this boundary component is ideal, the Euler characteristic
         * of the link of the corresponding ideal vertex is returned.
         *
         * @return the Euler characteristic.
         */
        long getEulerCharacteristic() const;

        /**
         * Determines if this boundary component is ideal.
         * This is the case if and only if it consists of a single
         * (ideal) vertex and no faces.
         *
         * @return \c true if and only if this boundary component is
         * ideal.
         */
        bool isIdeal() const;

        /**
         * Determines if this boundary component is orientable.
         * If the boundary component is ideal, the orientability 
         * of the link of the corresponding ideal vertex is returned.
         *
         * @return \c true if and only if this boundary component is
         * orientable.
         */
        bool isOrientable() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Default constructor.
         */
        NBoundaryComponent();

        /**
         * Creates a new boundary component consisting only of the given
         * ideal vertex.
         *
         * \pre The given vertex is ideal as returned by NVertex::isIdeal().
         *
         * @param idealVertex the vertex to place in the new boundary
         * component.
         */
        NBoundaryComponent(NVertex* idealVertex);

    friend class NTriangulation;
        /**< Allow access to private members. */
};

/*@}*/

// Inline functions for NBoundaryComponent

inline NBoundaryComponent::NBoundaryComponent() {
}

inline NBoundaryComponent::NBoundaryComponent(NVertex* idealVertex) {
    vertices.push_back(idealVertex);
}

inline NBoundaryComponent::~NBoundaryComponent() {
}

inline unsigned long NBoundaryComponent::getNumberOfFaces() const {
    return faces.size();
}

inline unsigned long NBoundaryComponent::getNumberOfEdges() const {
    return edges.size();
}

inline unsigned long NBoundaryComponent::getNumberOfVertices() const {
    return vertices.size();
}

inline NFace* NBoundaryComponent::getFace(unsigned long index) const {
    return faces[index];
}

inline NEdge* NBoundaryComponent::getEdge(unsigned long index) const {
    return edges[index];
}

inline NVertex* NBoundaryComponent::getVertex(unsigned long index) const {
    return vertices[index];
}

inline long NBoundaryComponent::getEulerCharacteristic() const {
    return (isIdeal() ?
        vertices.front()->getLinkEulerCharacteristic() :
        long(vertices.size()) - long(edges.size()) + long(faces.size()));
}

inline bool NBoundaryComponent::isIdeal() const {
    return faces.empty();
}

inline bool NBoundaryComponent::isOrientable() const {
    return orientable;
}
        
inline void NBoundaryComponent::writeTextShort(std::ostream& out) const {
    out << (isIdeal() ? "Ideal " : "Finite ") << "boundary component";
}

} // namespace regina

#endif

