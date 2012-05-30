
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

/*! \file dim2/dim2face.h
 *  \brief Deals with 2-dimensional faces in a 2-manifold triangulation.
 */

#ifndef __DIM2FACE_H
#ifndef __DOXYGEN
#define __DIM2FACE_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "maths/nperm3.h"
#include "utilities/nmarkedvector.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim2Edge;
class Dim2Vertex;
class Dim2Component;
class Dim2Triangulation;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a triangular face in a 2-manifold triangulation.
 *
 * With each face is stored various pieces of information
 * regarding the overall skeletal structure and component structure of
 * the triangulation.  This skeletal information will be allocated, calculated
 * and deallocated by the Dim2Triangulation object containing the
 * corresponding faces.
 *
 * A face must always belong to a 2-manifold triangulation.  You can
 * construct new faces using either Dim2Triangulation::newFace()
 * or Dim2Triangulation::newFace(const std::string&); these
 * routines will automatically add the new faces to the triangulation.
 * You can destroy faces by calling Dim2Trianguation::removeFace(),
 * Dim2Trianguation::removeFaceAt() or
 * Dim2Triangulation::removeAllFaces(); these routines will
 * automatically destroy the faces as they are removed.
 */
class REGINA_API Dim2Face : public ShareableObject, public NMarkedElement {
    private:
        Dim2Face* adj_[3];
            /**< Stores the adjacent faces glued to each edge of this
                 face.  Specifically, <tt>adj_[e]</tt>
                 represents the face joined to edge \c e
                 of this face, or is 0 if edge \c e lies on the
                 triangulation boundary.  Edges are numbered from 0 to 2
                 inclusive, where edge \c i is opposite vertex \c i. */
        NPerm3 adjPerm_[3];
            /**< Stores the corresponence between vertices of this
                 face and adjacent faces.  If edge \c e is
                 joined to another face, <tt>adjPerm_[3]</tt>
                 represents the permutation \c p whereby vertex \c v of
                 this face is identified with vertex <tt>p[v]</tt> of
                 the adjacent face along edge \c e. */
        std::string desc_;
            /**< A text description of this face.
                 Descriptions are not mandatory and need not be unique. */

        Dim2Vertex* vertex_[3];
            /**< Vertices in the triangulation skeleton that are
                 vertices of this face. */
        Dim2Edge* edge_[3];
            /**< Edges in the triangulation skeleton that are
                 edges of this face. */

        NPerm3 vertexMapping_[3];
            /**< Maps 0 to each vertex of this face in turn whilst
                 mapping (1,2) in a suitably "orientation-preserving" way,
                 as described in getVertexMapping(). */
        NPerm3 edgeMapping_[3];
            /**< Maps (0,1) to the vertices of this face that form
                 each edge, as described in getEdgeMapping(). */
        int orientation_;
            /**< The orientation of this face in the triangulation.
                 This will either be 1 or -1. */
        Dim2Triangulation* tri_;
            /**< The triangulation to which this face belongs. */
        Dim2Component* component_;
            /**< The component to which this face belongs in the
                 triangulation. */

    public:
        /**
         * Destroys this face.
         */
        virtual ~Dim2Face();

        /**
         * Returns the text description associated with this
         * face.
         *
         * @return the description of this face.
         */
        const std::string& getDescription() const;

        /**
         * Sets the text description associated with this face.
         * Note that descriptions need not be unique, and may be empty.
         *
         * @param desc the new description to assign to this
         * face.
         */
        void setDescription(const std::string& desc);

        /**
         * Returns the adjacent face glued to the given edge of this
         * face, or 0 if the given edge is on the boundary of the
         * 2-manifold triangulation.
         *
         * @param edge the edge of this face to examine.  This
         * should be between 0 and 2 inclusive, where edge \c i is
         * opposite vertex \c i of the face.
         * @return the adjacent face glued to the given edge, or 0
         * if the given edge lies on the boundary.
         */
        Dim2Face* adjacentFace(int edge) const;
        /**
         * Returns a permutation describing the correspondence between
         * vertices of this face and vertices of the adjacent
         * face glued to the given edge of this face.
         *
         * If we call this permutation \c p, then for each vertex \c v of this
         * face, <tt>p[v]</tt> will be the vertex of the adjacent
         * face that is identified with \c v according to the gluing
         * along the given edge of this face.
         *
         * \pre The given edge of this face has some face
         * (possibly this one) glued to it.
         *
         * @param edge the edge of this face whose gluing we
         * will examine.  This should be between 0 and 2 inclusive, where
         * edge \c i is opposite vertex \c i of the face.
         * @return a permutation mapping the vertices of this
         * face to the vertices of the face adjacent along
         * the given edge.
         */
        NPerm3 adjacentGluing(int edge) const;
        /**
         * Examines the face glued to the given edge of this
         * face, and returns the corresponding edge of that
         * face.  That is, the returned edge of the adjacent
         * face is glued to the given edge of this face.
         *
         * \pre The given edge of this face has some face
         * (possibly this one) glued to it.
         *
         * @param edge the edge of this face whose gluing we
         * will examine.  This should be between 0 and 2 inclusive, where
         * edge \c i is opposite vertex \c i of the face.
         * @return the edge of the face adjacent along the given
         * edge that is in fact glued to the given edge of this face.
         */
        int adjacentEdge(int edge) const;
        /**
         * Determines if this face has any edges that are boundary edges.
         *
         * @return \c true if and only if this face has any boundary edges.
         */
        bool hasBoundary() const;

        /**
         * Joins the given edge of this face to another face.
         * The other face involved will be automatically updated.
         *
         * \pre This and the given face do not belong to
         * different triangulations.
         * \pre The given edge of this face is not currently glued
         * to anything.
         * \pre The edge of the other face that will be glued to the
         * given edge of this face is not currently glued to anything.
         * \pre If the other face involved is this face, we are
         * not attempting to glue an edge to itself.
         *
         * @param myEdge the edge of this face that will be glued to
         * the given other face.  This should be between 0 and 2
         * inclusive, where edge \c i is opposite vertex \c i of the face.
         * @param you the face (possibly this one) that will be
         * glued to the given edge of this face.
         * @param gluing a permutation describing the mapping of
         * vertices by which the two faces will be joined.  Each
         * vertex \c v of this face that lies on the given edge will
         * be identified with vertex <tt>gluing[v]</tt> of face
         * <tt>you</tt>.  In addition, the edge of <tt>you</tt> that
         * will be glued to the given edge of this face will be
         * edge number <tt>gluing[myEdge]</tt>.
         */
        void joinTo(int myEdge, Dim2Face* you, NPerm3 gluing);
        /**
         * Unglues the given edge of this face from whatever is
         * joined to it.  The other face involved (possibly this
         * one) will be automatically updated.
         *
         * \pre The given edge of this face has some face
         * (possibly this one) glued to it.
         *
         * @param myEdge the edge of this face whose gluing we
         * will undo.  This should be between 0 and 2 inclusive, where
         * edge \c i is opposite vertex \c i of the face.
         * @return the ex-adjacent face that was originally glued to the
         * given edge of this face.
         */
        Dim2Face* unjoin(int myEdge);
        /**
         * Undoes any edge gluings involving this face.
         * Any other faces involved will be automatically updated.
         */
        void isolate();

        /**
         * Returns the triangulation to which this face belongs.
         *
         * @return the triangulation containing this face.
         */
        Dim2Triangulation* getTriangulation() const;

        /**
         * Returns the 2-manifold triangulation component to which this
         * face belongs.
         *
         * @return the component containing this face.
         */
        Dim2Component* getComponent() const;
        /**
         * Returns the vertex in the 2-manifold triangulation skeleton
         * corresponding to the given vertex of this face.
         *
         * @param vertex the vertex of this face to examine.
         * This should be between 0 and 2 inclusive.
         * @return the vertex of the skeleton corresponding to the
         * requested face vertex.
         */
        Dim2Vertex* getVertex(int vertex) const;
        /**
         * Returns the edge in the 2-manifold triangulation skeleton
         * corresponding to the given edge of this face.  Edge \c i
         * of a face is always opposite vertex \c i of that face.
         *
         * @param edge the edge of this face to examine.
         * This should be between 0 and 2 inclusive.
         * @return the edge of the skeleton corresponding to the
         * requested face edge.
         */
        Dim2Edge* getEdge(int edge) const;
        /**
         * Returns a permutation that maps 0 to the given vertex of this
         * face, and that maps (1,2) to the two remaining vertices
         * in the following "orientation-preserving" fashion.
         *
         * The images of 1 and 2 under the permutations that are returned
         * have the following properties.  In each face, the images
         * of 1 and 2 under this map form a directed edge of the face
         * (running from the image of vertex 1 to the image of vertex 2).
         * For any given vertex of the triangulation, these corresponding
         * directed edges together form an ordered path within the
         * triangulation that circles the common vertex of the triangulation
         * (like a vertex link, except that it is not near to the vertex
         * and so might intersect itself).  Furthermore, if we consider
         * the individual faces in the order in which they appear in the list
         * Dim2Vertex::getEmbeddings(), these corresponding directed edges
         * appear in order from the start of this path to the finish
         * (for internal vertices this path is actually a cycle, and the
         * starting point is arbitrary).
         *
         * @param vertex the vertex of this face to examine.
         * This should be between 0 and 2 inclusive.
         * @return a permutation that maps 0 to the given vertex of this
         * face, with the properties outlined above.
         */
        NPerm3 getVertexMapping(int vertex) const;
        /**
         * Examines the given edge of this face, and returns a mapping from
         * the "canonical" vertices of the corresponding edge of the
         * triangulation to the matching vertices of this face.
         *
         * In detail:  Suppose two edges of two faces are
         * identified within the overall 2-manifold triangulation.  We call
         * this a single "edge of the triangulation", and arbitrarily
         * label its vertices (0,1).  This routine then maps the vertices
         * (0,1) of this edge of the triangulation to the individual
         * vertices of this face that make up the given edge.
         *
         * Because we are passing the argument \a edge, we already know
         * \e which vertices of this face are involved.  What this
         * routine tells us is the \a order in which they appear to form the
         * overall edge of the triangulation.
         *
         * As a consequence:  Consider two face edges that are
         * identified together as a single edge of the triangulation,
         * and choose some \a i from the set {0,1}.  Then the vertices
         * <tt>getEdgeMapping(...)[i]</tt> of the individual faces
         * are identified together, since they both become the same
         * vertex of the same edge of the triangulation (assuming of
         * course that we pass the correct edge number in each case to
         * getEdgeMapping()).
         *
         * @param edge the edge of this face to examine.
         * This should be between 0 and 2 inclusive.
         * @return a mapping from vertices (0,1) of the requested
         * edge to the corresponding vertices of this face.
         */
        NPerm3 getEdgeMapping(int edge) const;
        /**
         * Returns the orientation of this face in the 2-manifold
         * triangulation.
         *
         * The orientation of each face is always +1 or -1.
         * In an orientable component of a triangulation,
         * adjacent faces have the same orientations if one could be
         * transposed onto the other without reflection, and they have
         * opposite orientations if a reflection would be required.
         * In a non-orientable component, orientations are still +1 and
         * -1 but no further guarantees can be made.
         *
         * @return +1 or -1 according to the orientation of this face.
         */
        int orientation() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new face with empty description and no
         * edges joined to anything.
         *
         * @param tri the triangulation to which the new face belongs.
         */
        Dim2Face(Dim2Triangulation* tri);
        /**
         * Creates a new face with the given description and
         * no edges joined to anything.
         *
         * @param desc the description to give the new face.
         * @param tri the triangulation to which the new face belongs.
         */
        Dim2Face(const std::string& desc, Dim2Triangulation* tri);

    friend class Dim2Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim2/dim2triangulation.h"
namespace regina {

// Inline functions for Dim2Face

inline Dim2Face::~Dim2Face() {
}

inline const std::string& Dim2Face::getDescription() const {
    return desc_;
}

inline void Dim2Face::setDescription(const std::string& desc) {
    desc_ = desc;
}

inline Dim2Face* Dim2Face::adjacentFace(int edge) const {
    return adj_[edge];
}

inline NPerm3 Dim2Face::adjacentGluing(int edge) const {
    return adjPerm_[edge];
}

inline int Dim2Face::adjacentEdge(int edge) const {
    return adjPerm_[edge][edge];
}

inline Dim2Triangulation* Dim2Face::getTriangulation() const {
    return tri_;
}

inline Dim2Component* Dim2Face::getComponent() const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return component_;
}

inline Dim2Vertex* Dim2Face::getVertex(int vertex) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return vertex_[vertex];
}

inline Dim2Edge* Dim2Face::getEdge(int edge) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return edge_[edge];
}

inline NPerm3 Dim2Face::getVertexMapping(int vertex) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return vertexMapping_[vertex];
}

inline NPerm3 Dim2Face::getEdgeMapping(int edge) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return edgeMapping_[edge];
}

inline int Dim2Face::orientation() const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return orientation_;
}

inline void Dim2Face::writeTextShort(std::ostream& out) const {
    out << "Face";
    if (desc_.length() > 0)
        out << " " << desc_;
}

} // namespace regina

#endif

