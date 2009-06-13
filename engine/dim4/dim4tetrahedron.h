
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

/*! \file dim4/dim4tetrahedron.h
 *  \brief Deals with tetrahedra in the 3-skeleton of a 4-manifold
 *  triangulation.
 */

#ifndef __DIM4TETRAHEDRON_H
#ifndef __DOXYGEN
#define __DIM4TETRAHEDRON_H
#endif

#include "dim4/dim4pentachoron.h"

namespace regina {

class Dim4Component;
class Dim4BoundaryComponent;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Details how a tetrahedron in the 3-skeleton of a 4-manifold triangulation
 * forms part of an individual pentachoron.
 */
class Dim4TetrahedronEmbedding {
    private:
        Dim4Pentachoron* pent_;
            /**< The pentachoron in which this tetrahedron is contained. */
        int tet_;
            /**< The facet number of the pentachoron that is this
                 tetrahedron. */

    public:
        /**
         * Default constructor.  The embedding descriptor created is
         * unusable until it has some data assigned to it using
         * <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        Dim4TetrahedronEmbedding();

        /**
         * Creates an embedding descriptor containing the given data.
         *
         * @param pent the pentachoron in which this tetrahedron is contained.
         * @param tet the facet number of \a pent that is this tetrahedron.
         */
        Dim4TetrahedronEmbedding(Dim4Pentachoron* pent, int tet);

        /**
         * Creates an embedding descriptor containing the same data as
         * the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim4TetrahedronEmbedding(const Dim4TetrahedronEmbedding& cloneMe);

        /**
         * Assigns to this embedding descriptor the same data as is
         * contained in the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        Dim4TetrahedronEmbedding& operator = (
            const Dim4TetrahedronEmbedding& cloneMe);

        /**
         * Returns the pentachoron in which this tetrahedron is contained.
         *
         * @return the pentachoron.
         */
        Dim4Pentachoron* getPentachoron() const;

        /**
         * Returns the facet number within getPentachoron() that is this
         * tetrahedron.
         *
         * @return the facet number that is this tetrahedron.
         */
        int getTetrahedron() const;

        /**
         * Returns a mapping from vertices (0,1,2,3) of this tetrahedron to the
         * corresponding vertex numbers in getPentachoron(), as described
         * in Dim4Pentachoron::getTetrahedronMapping().
         *
         * @return a mapping from the vertices of this tetrahedron to the
         * vertices of getPentachoron().
         */
        NPerm5 getVertices() const;
};

/**
 * Represents a tetrahedron in the 3-skeleton of a 4-manifold triangulation.
 * Tetrahedra are highly temporary; once a triangulation changes, all its
 * tetrahedron objects will be deleted and new ones will be created.
 */
class Dim4Tetrahedron : public ShareableObject, public NMarkedElement {
    public:
        /**
         * An array that maps tetrahedron numbers within a pentachoron
         * (i.e., facet numbers) to the canonical ordering of the
         * individual pentachoron vertices that form each tetrahedron.
         *
         * This means that the vertices of tetrahedron \a i in a pentachoron
         * are, in canonical order, <tt>ordering[i][0..3]</tt>.  As an
         * immediate consequence, we obtain <tt>ordering[i][4] == i</tt>.
         *
         * This table does \e not describe the mapping from specific
         * tetrahedra within a triangulation into individual pentachora
         * (for that, see Dim4Pentachoron::getTetrahedronMapping() instead).
         * This table merely provides a neat and consistent way of
         * listing the vertices of any given pentachoron facet.
         */
        static const NPerm5 ordering[5];

    private:
        Dim4TetrahedronEmbedding emb_[2];
            /**< A list of descriptors telling how this tetrahedron forms a
                 part of each individual pentachoron that it belongs to. */
        unsigned nEmb_;
            /**< The number of descriptors stored in the list \a emb_.
                 This will never exceed two. */
        Dim4Component* component_;
            /**< The component that this tetrahedron is a part of. */
        Dim4BoundaryComponent* boundaryComponent_;
            /**< The boundary component that this tetrahedron is a part of,
                 or 0 if this tetrahedron is internal. */
        bool inDualMaximalForest_;
            /**< Does this tetrahedron belong to the maximal forest in
                 the dual 1-skeleton? */

    public:
        /**
         * Default destructor.
         */
        ~Dim4Tetrahedron();

        /**
         * Returns the number of descriptors available through getEmbedding().
         * Note that this number will never be greater than two.
         *
         * @return the number of embedding descriptors.
         */
        unsigned getNumberOfEmbeddings() const;

        /**
         * Returns the requested descriptor detailing how this tetrahedron
         * forms a part of a particular pentachoron in the triangulation.
         * Note that if this tetrahedron represents multiple facets of a
         * particular pentachoron, then there will be multiple embedding
         * descriptors available regarding that pentachoron.
         *
         * @param index the index of the requested descriptor.  This
         * should be between 0 and getNumberOfEmbeddings()-1 inclusive.
         * @return the requested embedding descriptor.
         */
        const Dim4TetrahedronEmbedding& getEmbedding(unsigned index) const;

        /**
         * Returns the component of the triangulation to which this
         * tetrahedron belongs.
         *
         * @return the component containing this tetrahedron.
         */
        Dim4Component* getComponent() const;

        /**
         * Returns the boundary component of the triangulation to which
         * this tetrahedron belongs.
         *
         * @return the boundary component containing this tetrahedron, or 0
         * if this tetrahedron does not lie entirely within the boundary of
         * the triangulation.
         */
        Dim4BoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex of the 4-manifold triangulation corresponding
         * to the given vertex of this tetrahedron.
         *
         * @param vertex the vertex of this tetrahedron to examine.  This
         * should be between 0 and 3 inclusive.
         * @return the corresponding vertex of the 4-manifold triangulation.
         */
        Dim4Vertex* getVertex(int vertex) const;

        /**
         * Returns the edge of the 4-manifold triangulation corresponding
         * to the given edge of this tetrahedron.
         *
         * Edges of a tetrahedron are numbered from 0 to 5, as described
         * by the arrays NEdge::edgeNumber and NEdge::edgeVertex.
         * Edge \a i of a tetrahedron is always opposite edge \a 5-i.
         *
         * @param edge the edge of this tetrahedron to examine.  This should
         * be between 0 and 5 inclusive.
         * @return the corresponding edge of the 4-manifold triangulation.
         */
        Dim4Edge* getEdge(int edge) const;

        /**
         * Returns the face of the 4-manifold triangulation corresponding
         * to the given face of this tetrahedron.
         *
         * Note that face \a i of a tetrahedron is opposite vertex \a i of
         * the tetrahedron.
         *
         * @param face the face of this tetrahedron to examine.  This
         * should be between 0 and 3 inclusive.
         * @return the corresponding face of the 4-manifold triangulation.
         */
        Dim4Face* getFace(int face) const;

        /**
         * Examines the given edge of this tetrahedron, and returns a mapping
         * from the "canonical" vertices of the corresponding edge of
         * the triangulation to the vertices of this tetrahedron.
         *
         * This routine behaves much the same way as
         * Dim4Pentachoron::getEdgeMapping(), except that it maps the
         * edge vertices into a tetrahedron, not into a pentachoron.  See
         * Dim4Pentachoron::getEdgeMapping() for a more detailed
         * explanation of precisely what this mapping means.
         *
         * This routine differs from Dim4Pentachoron::getEdgeMapping()
         * in how it handles the images of 2, 3 and 4.  This routine
         * will always map 2 and 3 to the remaining vertices of this
         * tetrahedron (in arbitrary order), and will always map 4 to itself.
         *
         * @param edge the edge of this tetrahedron to examine.  This should
         * be between 0 and 5 inclusive.
         * @return a mapping from vertices (0,1) of the requested edge
         * to the vertices of this tetrahedron.
         */
        NPerm5 getEdgeMapping(int edge) const;

        /**
         * Examines the given face of this tetrahedron, and returns a mapping
         * from the "canonical" vertices of the corresponding face of
         * the triangulation to the vertices of this tetrahedron.
         *
         * This routine behaves much the same way as
         * Dim4Pentachoron::getFaceMapping(), except that it maps the
         * face vertices into a tetrahedron, not into a pentachoron.  See
         * Dim4Pentachoron::getFaceMapping() for a more detailed
         * explanation of precisely what this mapping means.
         *
         * This routine differs from Dim4Pentachoron::getFaceMapping()
         * in how it handles the images of 3 and 4.  This routine
         * will always map 3 to the remaining vertex of this tetrahedron (which
         * is equal to the argument \a face), and will always map 4 to itself.
         *
         * @param face the face of this tetrahedron to examine.  This should
         * be between 0 and 3 inclusive.
         * @return a mapping from vertices (0,1,2) of the requested face
         * to the vertices of this tetrahedron.
         */
        NPerm5 getFaceMapping(int face) const;

        /**
         * Determines if this tetrahedron lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this tetrahedron lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Determines whether this tetrahedron represents an edge in the
         * maximal forest in the dual 1-skeleton of the triangulation.
         *
         * For each triangulation, a maximal forest in the dual 1-skeleton
         * is computed.  Each dual edge in this maximal forest is
         * represented by a tetrahedron in the "real" triangulation.
         * The purpose of this routine is to identify whether this
         * particular tetrahedron represents one of these dual edges
         * in the maximal forest.
         *
         * Note that this routine is very fast, since the maximal forest
         * in the dual 1-skeleton will have already been computed (it is
         * constructed at the same time as the overall skeletal structure
         * of the triangulation).
         *
         * @return \c true if and only if this tetrahedron represents a
         * dual edge in the maximal forest.
         */
        bool inDualMaximalForest() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new tetrahedron and marks it as belonging to the
         * given triangulation component.
         *
         * @param component the triangulation component to which this
         * tetrahedron belongs.
         */
        Dim4Tetrahedron(Dim4Component* component);

    friend class Dim4Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

// Inline functions for Dim4TetrahedronEmbedding

inline Dim4TetrahedronEmbedding::Dim4TetrahedronEmbedding() : pent_(0) {
}

inline Dim4TetrahedronEmbedding::Dim4TetrahedronEmbedding(
        Dim4Pentachoron* pent, int tet) :
        pent_(pent), tet_(tet) {
}

inline Dim4TetrahedronEmbedding::Dim4TetrahedronEmbedding(
        const Dim4TetrahedronEmbedding& cloneMe) :
        pent_(cloneMe.pent_), tet_(cloneMe.tet_) {
}

inline Dim4TetrahedronEmbedding& Dim4TetrahedronEmbedding::operator =
        (const Dim4TetrahedronEmbedding& cloneMe) {
    pent_ = cloneMe.pent_;
    tet_ = cloneMe.tet_;
    return *this;
}

inline Dim4Pentachoron* Dim4TetrahedronEmbedding::getPentachoron() const {
    return pent_;
}

inline int Dim4TetrahedronEmbedding::getTetrahedron() const {
    return tet_;
}

inline NPerm5 Dim4TetrahedronEmbedding::getVertices() const {
    return pent_->getTetrahedronMapping(tet_);
}

// Inline functions for Dim4Tetrahedron

inline Dim4Tetrahedron::Dim4Tetrahedron(Dim4Component* component) :
        nEmb_(0), component_(component), boundaryComponent_(0),
        inDualMaximalForest_(false) {
}

inline Dim4Tetrahedron::~Dim4Tetrahedron() {
}

inline unsigned Dim4Tetrahedron::getNumberOfEmbeddings() const {
    return nEmb_;
}

inline const Dim4TetrahedronEmbedding& Dim4Tetrahedron::getEmbedding(
        unsigned index) const {
    return emb_[index];
}

inline Dim4Component* Dim4Tetrahedron::getComponent() const {
    return component_;
}

inline Dim4BoundaryComponent* Dim4Tetrahedron::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline Dim4Vertex* Dim4Tetrahedron::getVertex(int vertex) const {
    return emb_[0].getPentachoron()->getVertex(emb_[0].getVertices()[vertex]);
}

inline bool Dim4Tetrahedron::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline bool Dim4Tetrahedron::inDualMaximalForest() const {
    return inDualMaximalForest_;
}

inline void Dim4Tetrahedron::writeTextShort(std::ostream& out) const {
    out << (boundaryComponent_ ? "Boundary " : "Internal ") << "tetrahedron";
}

} // namespace regina

#endif

