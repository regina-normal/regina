
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

/*! \file nedge.h
 *  \brief Deals with edges in a triangulation.
 */

#ifndef __NEDGE_H
#ifndef __DOXYGEN
#define __NEDGE_H
#endif

#include <deque>
#include "shareableobject.h"
#include "triangulation/nperm.h"
#include "triangulation/ntetrahedron.h"

namespace regina {

class NComponent;
class NBoundaryComponent;

/**
 * <tt>edgeNumber[i][j]</tt> is the number of the
 * edge linking vertices <tt>i</tt> and <tt>j</tt>
 * in a tetrahedron.  <tt>i</tt> and <tt>j</tt>
 * must be between 0 and 3 inclusive and may be given
 * in any order.
 * The resulting edge number will be between 0 and 5 inclusive.
 *
 * Note that edge numbers of opposite edges will always add to 5.
 *
 * \ifacesjava This array is a static member of \a NEdge.
 */
extern const int edgeNumber[4][4];

/**
 * <tt>edgeStart[k]</tt> is the vertex of a tetrahedron
 * at which edge <tt>k</tt> of the tetrahedron begins.
 * <tt>k</tt> must be between 0 and 5 inclusive.
 * The resulting vertex number will be between 0 and 3 inclusive.
 *
 * Note that edge numbers of opposite edges will always add to 5.
 * You are guaranteed that <tt>edgeStart[e]</tt> will always be smaller
 * than <tt>edgeEnd[e]</tt>.
 *
 * \ifacesjava This array is a static member of \a NEdge.
 */
extern const int edgeStart[6];

/**
 * <tt>edgeEnd[k]</tt> is the vertex of a tetrahedron
 * at which edge <tt>k</tt> of the tetrahedron ends.
 * <tt>k</tt> must be between 0 and 5 inclusive.
 * The resulting vertex number will be between 0 and 3 inclusive.
 *
 * Note that edge numbers of opposite edges will always add to 5.
 * You are guaranteed that <tt>edgeStart[e]</tt> will always be smaller
 * than <tt>edgeEnd[e]</tt>.
 *
 * \ifacesjava This array is a static member of \a NEdge.
 */
extern const int edgeEnd[6];

/**
 * Details how an edge in the skeleton forms part of an individual
 * tetrahedron.
 */
class NEdgeEmbedding {
    private:
        NTetrahedron* tetrahedron;
            /**< The tetrahedron in which this face is contained. */
        int edge;
            /**< The edge number of the tetrahedron that is this edge. */

    public:
        /**
         * Default constructor.  The embedding descriptor created is
         * unusable until it has some data assigned to it using
         * <tt>operator =</tt>.
         *
         * \ifaces Not present.
         */
        NEdgeEmbedding();
        
        /**
         * Creates an embedding descriptor containing the given data.
         *
         * @param newTet the tetrahedron in which this edge is
         * contained.
         * @param newEdge the edge number of \a newTet that is this edge.
         */
        NEdgeEmbedding(NTetrahedron* newTet, int newEdge);
        
        /**
         * Creates an embedding descriptor containing the same data as
         * the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        NEdgeEmbedding(const NEdgeEmbedding& cloneMe);

        /**
         * Assigns to this embedding descriptor the same data as is
         * contained in the given embedding descriptor.
         *
         * \ifaces Not present.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        NEdgeEmbedding& operator =(const NEdgeEmbedding& cloneMe);

        /**
         * Returns the tetrahedron in which this edge is contained.
         *
         * @return the tetrahedron.
         */
        NTetrahedron* getTetrahedron() const;
        
        /**
         * Returns the edge number within getTetrahedron() that is
         * this edge.
         *
         * @return the edge number that is this edge.
         */
        int getEdge() const;

        /**
         * Returns a mapping from vertices (0,1) of this edge to the
         * corresponding vertex numbers in getTetrahedron(), as described
         * in NTetrahedron::getEdgeMapping().
         *
         * @return a mapping from the vertices of this edge to the
         * vertices of getTetrahedron().
         */
        NPerm getVertices() const;
};

/**
 * Represents an edge in the skeleton of a triangulation.
 * Edges are highly temporary; once a triangulation changes, all its
 * edge objects will be deleted and new ones will be created.
 */
class NEdge : public ShareableObject {
    private:
        std::deque<NEdgeEmbedding> embeddings;
            /**< A list of descriptors of how this edge forms a part of
                 each individual tetrahedron it belongs to. */
        NComponent* component;
            /**< The component that this edge is a part of. */
        NBoundaryComponent* boundaryComponent;
            /**< The boundary component that this edge is a part of,
                 or 0 if this edge is internal. */
        bool valid;
            /**< Is this edge valid? */

    public:
        /**
         * Creates a new edge and specifies it as belonging to the
         * given triangulation component.
         *
         * \ifaces Not present.
         *
         * @param myComponent the triangulation component to which this
         * edge belongs.
         */
        NEdge(NComponent* myComponent);

        /**
         * Default destructor.
         */
        ~NEdge();

        /**
         * Returns the list of descriptors of how this edge forms a
         * part of various tetrahedra in the triangulation.
         * Note that if this edge represents multiple edges of a
         * particular tetrahedron, there will be multiple embedding
         * descriptors in the list regarding that tetrahedron.
         *
         * These embedding descriptors will be stored in order in
         * the list, so that running through the list and
         * following in turn the edges defined by the
         * images of (2,3) under the edge embedding permutations
         * for each tetrahedron will produce an ordered
         * chain circling the edge.
         *
         * \ifaces Not present.
         *
         * @return the list of embedding descriptors.
         * @see NEdgeEmbedding
         */
        const std::deque<NEdgeEmbedding>& getEmbeddings() const;

        /**
         * Returns the number of descriptors in the list returned by
         * getEmbeddings().
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
        const NEdgeEmbedding& getEmbedding(unsigned long index) const;

        /**
         * Returns the component of the triangulation to which this
         * edge belongs.
         *
         * @return the component containing this edge.
         */
        NComponent* getComponent() const;
        
        /**
         * Returns the boundary component of the triangulation to which
         * this edge belongs.
         *
         * @return the boundary component containing this edge,
         * or 0 if this edge is not on the boundary of the triangulation.
         */
        NBoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex in the triangulation skeleton corresponding
         * to the given vertex of this edge.
         *
         * @param vertex the vertex of this edge to examine.  This should
         * be 0 or 1.
         * @return the vertex of the skeleton corresponding to the
         * requested edge vertex.
         */
        NVertex* getVertex(int vertex) const;

        /**
         * Determines if this edge lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this edge lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Determines if this edge is valid.
         * An edge is valid if and only if it is not glued to itself
         * in reverse.
         *
         * @return \c true if and only if this edge is valid.
         */
        bool isValid() const;

        void writeTextShort(std::ostream& out) const;

    friend class NTriangulation;
        /**< Allow access to private members. */
};

// Inline functions for NEdge

inline NEdge::NEdge(NComponent* myComponent) : component(myComponent),
        boundaryComponent(0), valid(true) {
}

inline NEdge::~NEdge() {
}

inline NComponent* NEdge::getComponent() const {
    return component;
}

inline NBoundaryComponent* NEdge::getBoundaryComponent() const {
    return boundaryComponent;
}

inline NVertex* NEdge::getVertex(int vertex) const {
    return embeddings.front().getTetrahedron()->getVertex(
        embeddings.front().getVertices()[vertex]);
}

inline bool NEdge::isBoundary() const {
    return (boundaryComponent != 0);
}

inline bool NEdge::isValid() const {
    return valid;
}

inline const std::deque<NEdgeEmbedding> & NEdge::getEmbeddings() const {
    return embeddings;
}

inline unsigned long NEdge::getNumberOfEmbeddings() const {
    return embeddings.size();
}

inline const NEdgeEmbedding& NEdge::getEmbedding(unsigned long index) const {
    return embeddings[index];
}

inline void NEdge::writeTextShort(std::ostream& out) const {
    out << (isBoundary() ? "Boundary " : "Internal ")
        << "edge of degree " << getNumberOfEmbeddings();
}

inline NEdgeEmbedding::NEdgeEmbedding() : tetrahedron(0) {
}

inline NEdgeEmbedding::NEdgeEmbedding(const NEdgeEmbedding& cloneMe) :
        tetrahedron(cloneMe.tetrahedron), edge(cloneMe.edge) {
}

inline NEdgeEmbedding::NEdgeEmbedding(NTetrahedron* newTet, int newEdge) :
        tetrahedron(newTet), edge(newEdge) {
}

inline NEdgeEmbedding& NEdgeEmbedding::operator =
        (const NEdgeEmbedding& cloneMe) {
    tetrahedron = cloneMe.tetrahedron;
    edge = cloneMe.edge;
    return *this;
}

inline NTetrahedron* NEdgeEmbedding::getTetrahedron() const {
    return tetrahedron;
}

inline int NEdgeEmbedding::getEdge() const {
    return edge;
}

inline NPerm NEdgeEmbedding::getVertices() const {
    return tetrahedron->getEdgeMapping(edge);
}

} // namespace regina

#endif

