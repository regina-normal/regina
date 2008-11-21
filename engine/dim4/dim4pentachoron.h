
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file dim4pentachoron.h
 *  \brief Deals with 4-dimensional simplices in a 4-manifold triangulation.
 */

#ifndef __DIM4PENTACHORON_H
#ifndef __DOXYGEN
#define __DIM4PENTACHORON_H
#endif

#include "shareableobject.h"
#include "dim4/nperm5.h"
#include "utilities/nmarkedvector.h"

namespace regina {

class Dim4Tetrahedron;
class Dim4Face;
class Dim4Edge;
class Dim4Vertex;
class Dim4Component;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a pentachoron (a 4-dimensional simplex) in a
 * 4-manifold triangulation.
 *
 * With each pentachoron is stored various pieces of information
 * regarding the overall skeletal structure and component structure of
 * the triangulation.  This information will be allocated, calculated
 * and deallocated by the Dim4Triangulation object containing the
 * corresponding pentachora.
 *
 * Whenever the gluings of pentachora are altered, the external routine
 * responsible for changing the gluings (the routine that calls joinTo()
 * and unjoin()) <b>must</b> call Dim4Triangulation::gluingsHaveChanged()
 * for the triangulation containing the pentachora concerned; this
 * will ensure that skeletal information and other properties of the
 * triangulation are recalculated when necessary.
 *
 * It is probably worth noting here the difference between a \e face and
 * a \e facet.  For our purposes, a \e face is a simplex in the 2-skeleton
 * of a 4-manifold triangulation, whereas a \e facet is a simplex in the
 * 3-skeleton of a 4-manifold triangulation.  In particular, a facet is one
 * of the five tetrahedra that forms the boundary of a pentachoron (hence
 * the name \e facet).
 */
class Dim4Pentachoron : public ShareableObject, public NMarkedElement {
    private:
        Dim4Pentachoron* adj_[5];
            /**< Stores the adjacent pentachora glued to each facet of this
                 pentachoron.  Specifically, <tt>adj_[f]</tt>
                 represents the pentachoron joined to facet \c f
                 of this pentachoron, or is 0 if facet \c f lies on the
                 triangulation boundary.  Facets are numbered from 0 to 4
                 inclusive, where facet \c i is opposite vertex \c i. */
        NPerm5 adjPerm_[5];
            /**< Stores the corresponence between vertices of this
                 pentachoron and adjacent pentachora.  If facet \c f is
                 joined to another pentachoron, <tt>adjPerm_[f]</tt>
                 represents the permutation \c p whereby vertex \c v of
                 this pentachoron is identified with vertex <tt>p[v]</tt> of
                 the adjacent pentachoron along facet \c f. */
        std::string desc_;
            /**< A text description of this pentachoron.
                 Descriptions are not mandatory and need not be unique. */

        Dim4Vertex* vertex_[5];
            /**< Vertices in the triangulation skeleton that are
                 vertices of this pentachoron. */
        Dim4Edge* edge_[10];
            /**< Edges in the triangulation skeleton that are
                 edges of this pentachoron. */
        Dim4Face* face_[10];
            /**< 2-dimensional faces in the triangulation skeleton that are
                 faces of this pentachoron. */
        Dim4Face* tet_[5];
            /**< Tetrahedra in the triangulation skeleton that are
                 facets of this pentachoron. */

        NPerm5 edgeMapping_[10];
            /**< Maps (0,1) to the vertices of this pentachoron that form
                 each edge, as described in getEdgeMapping(). */
        NPerm5 faceMapping_[10];
            /**< Maps (0,1,2) to the vertices of this pentachoron that form
                 each 2-dimensional face, as described in getFaceMapping(). */
        NPerm5 tetMapping_[5];
            /**< Maps (0,1,2,3) to the vertices of this pentachoron that form
                 each 3-dimensional facet, as described in
                 getTetrahedronMapping(). */
        int orientation_;
            /**< The orientation of this pentachoron in the triangulation.
                 This will either be 1 or -1. */
        Dim4Component* component_;
            /**< The component to which this pentachoron belongs in the
                 triangulation. */

    public:
        /**
         * Creates a new pentachoron with empty description and no
         * facets joined to anything.
         */
        Dim4Pentachoron();
        /**
         * Creates a new pentachoron with the given description and
         * no faces joined to anything.
         *
         * @param desc the description to give the new pentachoron.
         */
        Dim4Pentachoron(const std::string& desc);
        /**
         * Destroys this pentachoron.
         */
        virtual ~Dim4Pentachoron();

        /**
         * Returns the text description associated with this
         * pentachoron.
         *
         * @return the description of this pentachoron.
         */
        const std::string& getDescription() const;

        /**
         * Sets the text description associated with this pentachoron.
         * Note that descriptions need not be unique, and may be empty.
         *
         * @param desc the new description to assign to this
         * pentachoron.
         */
        void setDescription(const std::string& desc);

        /**
         * Returns the adjacent pentachoron glued to the given facet of this
         * pentachoron, or 0 if the given facet is on the boundary of the
         * 4-manifold triangulation.
         *
         * @param facet the facet of this pentachoron to examine.  This
         * should be between 0 and 4 inclusive, where facet \c i is
         * opposite vertex \c i of the pentachoron.
         * @return the adjacent pentachoron glued to the given facet, or 0
         * if the given facet lies on the boundary.
         */
        Dim4Pentachoron* adjacent(int facet) const;
        /**
         * Returns a permutation describing the correspondence between
         * vertices of this pentachoron and vertices of the adjacent
         * pentachoron glued to the given face of this pentachoron.
         *
         * If we call this permutation \c p, then for each vertex \c v of this
         * pentachoron, <tt>p[v]</tt> will be the vertex of the adjacent
         * pentachoron that is identified with \c v according to the gluing
         * along the given face of this pentachoron.
         *
         * \pre The given face of this pentachoron has some pentachoron
         * (possibly this one) glued to it.
         *
         * @param facet the facet of this pentachoron whose gluing we
         * will examine.  This should be between 0 and 4 inclusive, where
         * facet \c i is opposite vertex \c i of the pentachoron.
         * @return a permutation mapping the vertices of this
         * pentachoron to the vertices of the pentachoron adjacent along
         * the given facet.
         */
        NPerm5 adjacentGluing(int facet) const;
        /**
         * Examines the pentachoron glued to the given facet of this
         * pentachoron, and returns the corresponding facet of that
         * pentachoron.  That is, the returned facet of the adjacent
         * pentachoron is glued to the given face of this pentachoron.
         *
         * \pre The given facet of this pentachoron has some pentachoron
         * (possibly this one) glued to it.
         *
         * @param facet the facet of this pentachoron whose gluing we
         * will examine.  This should be between 0 and 4 inclusive, where
         * facet \c i is opposite vertex \c i of the pentachoron.
         * @return the facet of the pentachoron adjacent along the given
         * facet that is in fact glued to the given facet of this pentachoron.
         */
        int adjacentFacet(int facet) const;
        /**
         * Determines if this pentachoron has any facets that are
         * boundary facets.
         *
         * @return \c true if and only if this pentachoron has any
         * boundary facets.
         */
        bool hasBoundary() const;

        /**
         * Joins the given facet of this pentachoron to another pentachoron.
         * The other pentachoron involved will be automatically updated.
         *
         * \warning Note that Dim4Triangulation::gluingsHaveChanged() will
         * have to be called after all joins and unjoins have been performed.
         *
         * \pre The given facet of this pentachoron is not currently glued
         * to anything.
         * \pre The facet of the other pentachoron that will be glued to the
         * given facet of this pentachoron is not currently glued to anything.
         * \pre If the other pentachoron involved is this pentachoron, we are
         * not attempting to glue a facet to itself.
         *
         * @param myFacet the facet of this pentachoron that will be glued to
         * the given other pentachoron.  This should be between 0 and 4
         * inclusive, where facet \c i is opposite vertex \c i of the pentachoron.
         * @param you the pentachoron (possibly this one) that will be
         * glued to the given facet of this pentachoron.
         * @param gluing a permutation describing the mapping of
         * vertices by which the two pentachora will be joined.  Each
         * vertex \c v of this pentachoron that lies on the given facet will
         * be identified with vertex <tt>gluing[v]</tt> of pentachoron
         * <tt>you</tt>.  In addition, the facet of <tt>you</tt> that
         * will be glued to the given facet of this pentachoron will be
         * facet number <tt>gluing[myFacet]</tt>.
         */
        void joinTo(int myFacet, Dim4Pentachoron* you, NPerm5 gluing);
        /**
         * Unglues the given facet of this pentachoron from whatever is
         * joined to it.  The other pentachoron involved (possibly this
         * one) will be automatically updated.
         *
         * \warning Note that Dim4Triangulation::gluingsHaveChanged() will
         * have to be called after all joins and unjoins have been performed.
         *
         * \pre The given facet of this pentachoron has some pentachoron
         * (possibly this one) glued to it.
         *
         * @param myFacet the facet of this pentachoron whose gluing we
         * will undo.  This should be between 0 and 4 inclusive, where
         * facet \c i is opposite vertex \c i of the pentachoron.
         * @return the ex-adjacent pentachoron that was originally glued to the
         * given facet of this pentachoron.
         */
        Dim4Pentachoron* unjoin(int myFacet);
        /**
         * Undoes any facet gluings involving this pentachoron.
         * Any other pentachora involved will be automatically updated.
         */
        void isolate();

        /**
         * Returns the 4-manifold triangulation component to which this
         * pentachoron belongs.
         *
         * \pre This pentachoron belongs to a 4-manifold triangulation whose
         * skeletal information has already been calculated.
         *
         * @return the component containing this pentachoron.
         */
        Dim4Component* getComponent() const;
        /**
         * Returns the vertex in the 4-manifold triangulation skeleton
         * corresponding to the given vertex of this pentachoron.
         *
         * \pre This pentachoron belongs to a 4-manifold triangulation whose
         * skeletal information has already been calculated.
         *
         * @param vertex the vertex of this pentachoron to examine.
         * This should be between 0 and 4 inclusive.
         * @return the vertex of the skeleton corresponding to the
         * requested pentachoron vertex.
         */
        Dim4Vertex* getVertex(int vertex) const;
        /**
         * Returns the edge in the 4-manifold triangulation skeleton
         * corresponding to the given edge of this pentachoron.
         *
         * \pre This pentachoron belongs to a 4-manifold triangulation whose
         * skeletal information has already been calculated.
         *
         * @param edge the edge of this pentachoron to examine.
         * This should be between 0 and 9 inclusive.  Note that edge \c i
         * lies opposite face \c i.
         * @return the edge of the skeleton corresponding to the
         * requested pentachoron edge.
         */
        Dim4Edge* getEdge(int edge) const;
        /**
         * Returns the face in the 4-manifold triangulation skeleton
         * corresponding to the given face of this pentachoron.
         * Note that this is a piece of the 2-skeleton (as opposed to a
         * \e facet, which is a piece of the 3-skeleton).
         *
         * \pre This pentachoron belongs to a 4-manifold triangulation whose
         * skeletal information has already been calculated.
         *
         * @param face the face of this pentachoron to examine.
         * This should be between 0 and 9 inclusive.  Note that face \c i
         * lies opposite edge \c i.
         * @return the face of the skeleton corresponding to the
         * requested pentachoron face.
         */
        Dim4Face* getFace(int face) const;
        /**
         * Returns the tetrahedron in the 4-manifold triangulation skeleton
         * corresponding to the given facet of this pentachoron.
         * Note that this is a piece of the 3-skeleton (as opposed to a
         * \e face, which is a piece of the 2-skeleton).
         *
         * \pre This pentachoron belongs to a 4-manifold triangulation whose
         * skeletal information has already been calculated.
         *
         * @param tet the tetrahedral facet of this pentachoron to examine.
         * This should be between 0 and 4 inclusive, where facet \c i
         * lies opposite vertex \c i.
         * @return the tetrahedron of the skeleton corresponding to the
         * requested pentachoron facet.
         */
        Dim4Face* getTetrahedron(int tet) const;
        /**
         * The edge in the skeleton corresponding to the requested
         * edge of this pentachoron is examined, and a
         * permutation mapping vertices (0,1) of the skeleton
         * edge to the corresponding vertices of this pentachoron is returned.
         *
         * Thus, for each <tt>i=0,1</tt> and each pentachoron containing
         * this skeleton edge,
         * <tt>getEdgeMapping(...)[i]</tt> will refer to vertices
         * that are all identified to each other along the skeleton
         * edge concerned.
         *
         * \pre This pentachoron belongs to a 4-manifold triangulation whose
         * skeletal information has already been calculated.
         *
         * @param edge the edge of this pentachoron to examine.
         * This should be between 0 and 9 inclusive.
         * @return a mapping from vertices (0,1) of the requested edge
         * to the vertices of this pentachoron.
         */
        NPerm5 getEdgeMapping(int edge) const;
        /**
         * The face in the skeleton corresponding to the requested
         * face of this pentachoron is examined, and a
         * permutation mapping vertices (0,1,2) of the skeleton
         * face to the corresponding vertices of this pentachoron is returned.
         *
         * Thus, for each <tt>i=0,1,2</tt> and each pentachoron containing
         * this skeleton face,
         * <tt>getFaceMapping(...)[i]</tt> will refer to vertices
         * that are all identified to each other along the skeleton face
         * concerned.
         *
         * Furthermore, following the ordered edges defined by the
         * images of (3,4) under the returned permutations
         * will produce an ordered chain
         * circling the skeleton face.  That is, if pentachora \c A
         * and \c B are adjacent along the face concerned
         * and also at vertex \a V,
         * then if <tt>A.getFaceMapping(...)[4]</tt> refers to \a V, we will
         * have <tt>B.getFaceMapping(...)[3]</tt> referring to \a V also, and
         * \c B will appear immediately after \c A in the list of
         * embeddings stored in the corresponding Dim4Face object.
         * See Dim4Face::getEmbeddings() for further details.
         *
         * \pre This pentachoron belongs to a 4-manifold triangulation whose
         * skeletal information has already been calculated.
         *
         * @param face the face of this pentachoron to examine.
         * This should be between 0 and 9 inclusive.
         * @return a mapping from vertices (0,1,2) of the requested face
         * to the vertices of this pentachoron.
         */
        NPerm5 getFaceMapping(int face) const;
        /**
         * The tetrahedron in the skeleton corresponding to the requested
         * facet of this pentachoron is examined, and a
         * permutation mapping vertices (0,1) of the skeleton tetrahedron
         * to the corresponding vertices of this pentachoron is returned.
         *
         * Thus, for each <tt>i=0,1,2,3</tt> and each pentachoron containing
         * this skeleton tetrahedron,
         * <tt>getTetrahedronMapping(...)[i]</tt> will refer to vertices
         * that are all identified to each other along the skeleton
         * tetrahedron concerned.
         *
         * \pre This pentachoron belongs to a 4-manifold triangulation whose
         * skeletal information has already been calculated.
         *
         * @param tet the tetrahedral facet of this pentachoron to examine.
         * This should be between 0 and 4 inclusive.
         * @return a mapping from vertices (0,1,2,3) of the requested
         * tetrahedron to the vertices of this pentachoron.
         */
        NPerm5 getTetrahedronMapping(int tet) const;
        /**
         * Returns the orientation of this pentachoron in the 4-manifold
         * triangulation.
         *
         * The orientation of each pentachoron is always +1 or -1.
         * In an orientable component of a triangulation,
         * adjacent pentachora have the same orientations if one could be
         * transposed onto the other without reflection, and they have
         * opposite orientations if a reflection would be required.
         * In a non-orientable component, orientations are still +1 and
         * -1 but no further guarantees can be made.
         *
         * \pre This pentachoron belongs to a 4-manifold triangulation whose
         * skeletal information has already been calculated.
         *
         * @return +1 or -1 according to the orientation of this pentachoron.
         */
        int orientation() const;

        void writeTextShort(std::ostream& out) const;

    friend class Dim4Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

// Inline functions for Dim4Pentachoron

inline Dim4Pentachoron::~Dim4Pentachoron() {
}

inline const std::string& Dim4Pentachoron::getDescription() const {
    return desc_;
}

inline void Dim4Pentachoron::setDescription(const std::string& desc) {
    desc_ = desc;
}

inline Dim4Pentachoron* Dim4Pentachoron::adjacent(int facet) const {
    return adj_[facet];
}

inline NPerm5 Dim4Pentachoron::adjacentGluing(int facet) const {
    return adjPerm_[facet];
}

inline int Dim4Pentachoron::adjacentFace(int facet) const {
    return adjPerm_[facet][facet];
}

inline Dim4Component* Dim4Pentachoron::getComponent() const {
    return component_;
}

inline Dim4Vertex* Dim4Pentachoron::getVertex(int vertex) const {
    return vertex_[vertex];
}

inline Dim4Edge* Dim4Pentachoron::getEdge(int edge) const {
    return edge_[edge];
}

inline Dim4Face* Dim4Pentachoron::getFace(int face) const {
    return face_[face];
}

inline Dim4Face* Dim4Pentachoron::getTetrahedron(int tet) const {
    return tet_[tet];
}

inline NPerm5 Dim4Pentachoron::getEdgeMapping(int edge) const {
    return edgeMapping_[edge];
}

inline NPerm5 Dim4Pentachoron::getFaceMapping(int face) const {
    return faceMapping_[face];
}

inline NPerm5 Dim4Pentachoron::getTetrahedronMapping(int tet) const {
    return tetMapping_[tet];
}

inline int Dim4Pentachoron::orientation() const {
    return orientation_;
}

inline void Dim4Pentachoron::writeTextShort(std::ostream& out) const {
    out << "Pentachoron";
    if (description.length() > 0)
        out << " " << description;
}

} // namespace regina

#endif

