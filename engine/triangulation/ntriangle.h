
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

/*! \file triangulation/ntriangle.h
 *  \brief Deals with triangles in a triangulation.
 */

#ifndef __NFACE_H
#ifndef __DOXYGEN
#define __NFACE_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "maths/nperm4.h"
#include "utilities/nmarkedvector.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class NBoundaryComponent;
class NComponent;
class NEdge;
class NVertex;

template <int> class Simplex;
template <int> class Triangulation;
typedef Simplex<3> NTetrahedron;
typedef Triangulation<3> NTriangulation;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Details how a triangle in the skeleton forms part of an individual
 * tetrahedron.
 */
class REGINA_API NTriangleEmbedding {
    private:
        NTetrahedron* tetrahedron_;
            /**< The tetrahedron in which this triangle is contained. */
        int tri_;
            /**< The face number of the tetrahedron that is this triangle. */

    public:
        /**
         * Creates an embedding descriptor containing the given data.
         *
         * @param tet the tetrahedron in which this triangle is contained.
         * @param tri the face number of \a tet that is this triangle.
         */
        NTriangleEmbedding(NTetrahedron* tet, int tri);

        /**
         * Creates an embedding descriptor containing the same data as
         * the given embedding descriptor.
         *
         * @param cloneMe the embedding descriptor to clone.
         */
        NTriangleEmbedding(const NTriangleEmbedding& cloneMe);

        /**
         * Returns the tetrahedron in which this triangle is contained.
         *
         * @return the tetrahedron.
         */
        NTetrahedron* getTetrahedron() const;

        /**
         * Returns the triangle number within getTetrahedron() that is
         * this triangle.
         *
         * @return the triangle number that is this triangle.
         */
        int getTriangle() const;
        /**
         * A deprecated alias for getTriangle().
         *
         * This routine returns the triangle number within getTetrahedron()
         * that is this triangle.  See getTriangle() for further details.
         *
         * \deprecated This routine will be removed in a future version
         * of Regina.  Please use getTriangle() instead.
         *
         * @return the triangle number that is this triangle.
         */
        int getFace() const;

        /**
         * Returns a mapping from vertices (0,1,2) of this triangle to the
         * corresponding vertex numbers in getTetrahedron(), as described
         * in NTetrahedron::getTriangleMapping().
         *
         * @return a mapping from the vertices of this triangle to the
         * vertices of getTetrahedron().
         */
        NPerm4 getVertices() const;

        /**
         * Tests whether this and the given embedding are identical.
         * Here "identical" means that they refer to the same face of
         * the same tetrahedron.
         *
         * @param rhs the embedding to compare with this.
         * @return \c true if and only if both embeddings are identical.
         */
        bool operator == (const NTriangleEmbedding& rhs) const;

        /**
         * Tests whether this and the given embedding are different.
         * Here "different" means that they do not refer to the same
         * face of the same tetrahedron.
         *
         * @param rhs the embedding to compare with this.
         * @return \c true if and only if both embeddings are identical.
         */
        bool operator != (const NTriangleEmbedding& rhs) const;
};

/**
 * Represents a triangle in the skeleton of a triangulation.
 * Triangles are highly temporary; once a triangulation changes, all its
 * triangle objects will be deleted and new ones will be created.
 */
class REGINA_API NTriangle : public ShareableObject, public NMarkedElement {
    public:
        /**
         * The \e type of a triangle, which indicates how the vertices and
         * edges of the triangle are identified together.  Here the vertices
         * of a triangle are considered unlabelled (so a relabelling
         * will not change the triangle type).
         *
         * @see getType
         */
        enum Type {
            UNKNOWN_TYPE = 0,
                /**< Indicates that the triangle type has not yet been
                     determined. */
            TRIANGLE = 1,
                /**< Specifies a triangle with no identified vertices or
                     edges. */
            SCARF = 2,
                /**< Specifies a triangle with two identified vertices. */
            PARACHUTE = 3,
                /**< Specifies a triangle with three identified vertices. */
            CONE = 4,
                /**< Specifies a triangle with two edges identified to form a
                     cone. */
            MOBIUS = 5,
                /**< Specifies a triangle with two edges identified to form a
                     mobius band. */
            HORN = 6,
                /**< Specifies a triangle with two edges identified to form a
                     cone with all three vertices identified. */
            DUNCEHAT = 7,
                /**< Specifies a triangle with all three edges identified, some
                     via orientable and some via non-orientable gluings. */
            L31 = 8
                /**< Specifies a triangle with all three edges identified using
                     non-orientable gluings.  Note that this forms a spine for
                     the Lens space L(3,1). */
        };

        /**
         * An array that maps triangle numbers within a tetrahedron
         * (i.e., face numbers) to the canonical ordering of the individual
         * tetrahedron vertices that form each triangle.
         *
         * This means that the vertices of triangle \a i in a tetrahedron
         * are, in canonical order, <tt>ordering[i][0..2]</tt>.  As an
         * immediate consequence, we obtain <tt>ordering[i][3] == i</tt>.
         *
         * Regina defines canonical order to be \e increasing order.
         * That is, <tt>ordering[i][0] &lt; ... &lt; ordering[i][2]</tt>.
         *
         * This table does \e not describe the mapping from specific
         * triangles within a triangulation into individual tetrahedra (for
         * that, see NTetrahedron::getTriangleMapping() instead).  This table
         * merely provides a neat and consistent way of listing the
         * vertices of any given tetrahedron face.
         *
         * This lookup table replaces the deprecated routine
         * regina::faceOrdering().
         */
        static const NPerm4 ordering[4];

    private:
        NTriangleEmbedding* embeddings_[2];
            /**< An array of descriptors telling how this triangle forms a
                 part of each individual tetrahedron that it belongs to.
                 These embeddings will be automatically deleted when the
                 triangle itself is deleted. */
        int nEmbeddings_;
            /**< The number of embedding descriptors stored in
                 the embeddings array. */
        NComponent* component_;
            /**< The component that this triangle is a part of. */
        NBoundaryComponent* boundaryComponent_;
            /**< The boundary component that this triangle is a part of,
                 or 0 if this triangle is internal. */
        Type type_;
            /**< Specifies the triangle type, or \a UNKNOWN_TYPE if the
                 type has not yet been determined. */
        int subtype_;
            /**< Specifies the vertex or edge that plays a special role
                 for the triangle type specified by \a type.  This is only
                 relevant for some triangle types. */

    public:
        /**
         * Default destructor.
         * All embedding descriptors stored in this triangle will be
         * automatically deleted.
         */
        virtual ~NTriangle();

        /**
         * Returns the index of this triangle in the underlying
         * triangulation.  This is identical to calling
         * <tt>getTriangulation()->triangleIndex(this)</tt>.
         *
         * @return the index of this triangle.
         */
        unsigned long index() const;

        /**
         * Determines if this triangle lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this triangle lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Returns a description of the triangle type.
         * This will be one of the eight shapes described by the Type
         * enumeration, indicating how the edges and vertices of the
         * triangle are identified.
         *
         * @return the type of this triangle.  This routine will never
         * return UNKNOWN_TYPE.
         */
        Type getType();

        /**
         * Return the triangle vertex or triangle edge that plays a special role
         * for the triangle type of this triangle.  Note that this routine is
         * only relevant for some triangle types.  The triangle type is
         * returned by getType().
         *
         * @return The vertex or edge that plays a special role (this
         * will be 0, 1 or 2), or -1 if this triangle type has no special
         * vertex or edge.
         */
        int getSubtype();

        /**
         * Determines whether this triangle is wrapped up to form a Mobius band.
         *
         * Note that several different triangle types (as returned by
         * getType()) can produce this result.
         * Note also that a triangle can be both a Mobius band \a and a cone.
         *
         * @return \c true if and only if this triangle is a Mobius band.
         */
        bool isMobiusBand();

        /**
         * Determines whether this triangle is wrapped up to form a cone.
         *
         * Note that several different triangle types (as returned by
         * getType()) can produce this result.
         * Note also that a triangle can be both a Mobius band \a and a cone.
         *
         * @return \c true if and only if this triangle is a cone.
         */
        bool isCone();

        /**
         * Returns the number of descriptors available through getEmbedding().
         * Note that this number will never be greater than two.
         *
         * @return the number of embedding descriptors.
         */
        unsigned getNumberOfEmbeddings() const;

        /**
         * Returns the requested descriptor detailing how this triangle forms a
         * part of a particular tetrahedron in the triangulation.
         * Note that if this triangle represents multiple faces of a
         * particular tetrahedron, then there will be multiple embedding
         * descriptors available regarding that tetrahedron.
         *
         * @param index the index of the requested descriptor.  This
         * should be between 0 and getNumberOfEmbeddings()-1 inclusive.
         * @return the requested embedding descriptor.
         */
        const NTriangleEmbedding& getEmbedding(unsigned index) const;

        /**
         * Returns the triangulation to which this triangle belongs.
         *
         * @return the triangulation containing this triangle.
         */
        NTriangulation* getTriangulation() const;

        /**
         * Returns the component of the triangulation to which this
         * triangle belongs.
         *
         * @return the component containing this triangle.
         */
        NComponent* getComponent() const;

        /**
         * Returns the boundary component of the triangulation to which
         * this triangle belongs.
         *
         * @return the boundary component containing this triangle, or 0 if this
         * triangle does not lie entirely within the boundary of the
         * triangulation.
         */
        NBoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex of the triangulation that corresponds
         * to the given vertex of this triangle.
         *
         * Note that vertex \a i of a triangle is opposite edge \a i of the
         * triangle.
         *
         * @param vertex the vertex of this triangle to examine.  This should
         * be 0, 1 or 2.
         * @return the corresponding vertex of the triangulation.
         */
        NVertex* getVertex(int vertex) const;
        /**
         * Returns the edge of the triangulation that corresponds
         * to the given edge of this triangle.
         *
         * Note that edge \a i of a triangle is opposite vertex \a i of the
         * triangle.
         *
         * @param edge the edge of this triangle to examine.  This should be
         * 0, 1 or 2.
         * @return the corresponding edge of the triangulation.
         */
        NEdge* getEdge(int edge) const;
        /**
         * Examines the given edge of this triangle, and returns a mapping
         * from the "canonical" vertices of the corresponding edge of
         * the triangulation to the vertices of this triangle.
         *
         * This routine behaves much the same as
         * NTetrahedron::getEdgeMapping(), except that it maps the edge
         * vertices into a triangle, not into a tetrahedron.  See
         * NTetrahedron::getEdgeMapping() for a more detailed
         * explanation of precisely what this mapping means.
         *
         * This routine differs from NTetrahedron::getEdgeMapping() in
         * how it handles the images of 2 and 3.  This routine will
         * always map 2 to the remaining vertex of this triangle (which is
         * equal to the argument \a edge), and will always map 3 to itself.
         *
         * @param edge the edge of this triangle to examine.  This should be
         * 0, 1 or 2.
         * @return a mapping from vertices (0,1) of the requested edge to
         * the vertices of this triangle.
         */
        NPerm4 getEdgeMapping(int edge) const;

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new triangle and marks it as belonging to the
         * given triangulation component.
         *
         * @param myComponent the triangulation component to which this
         * triangle belongs.
         */
        NTriangle(NComponent* myComponent);

    friend class Triangulation<3>;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/ntetrahedron.h"
namespace regina {

// Inline functions for NTriangle

inline NTriangle::NTriangle(NComponent* myComponent) : nEmbeddings_(0),
        component_(myComponent), boundaryComponent_(0), type_(UNKNOWN_TYPE) {
}

inline NTriangle::~NTriangle() {
    if (nEmbeddings_ > 0)
        delete embeddings_[0];
    if (nEmbeddings_ > 1)
        delete embeddings_[1];
}

inline unsigned long NTriangle::index() const {
    return markedIndex();
}

inline NTriangulation* NTriangle::getTriangulation() const {
    return embeddings_[0]->getTetrahedron()->getTriangulation();
}

inline NComponent* NTriangle::getComponent() const {
    return component_;
}

inline NBoundaryComponent* NTriangle::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline NVertex* NTriangle::getVertex(int vertex) const {
    return embeddings_[0]->getTetrahedron()->getVertex(
        embeddings_[0]->getVertices()[vertex]);
}

inline bool NTriangle::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline int NTriangle::getSubtype() {
    getType();
    return subtype_;
}

inline bool NTriangle::isMobiusBand() {
    getType();
    return (type_ == L31 || type_ == DUNCEHAT || type_ == MOBIUS);
}

inline bool NTriangle::isCone() {
    getType();
    return (type_ == DUNCEHAT || type_ == CONE || type_ == HORN);
}

inline unsigned NTriangle::getNumberOfEmbeddings() const {
    return nEmbeddings_;
}

inline const NTriangleEmbedding& NTriangle::getEmbedding(unsigned index) const {
    return *(embeddings_[index]);
}

inline void NTriangle::writeTextShort(std::ostream& out) const {
    out << (isBoundary() ? "Boundary " : "Internal ") << "triangle";
}

inline NTriangleEmbedding::NTriangleEmbedding(NTetrahedron* tet, int tri) :
        tetrahedron_(tet), tri_(tri) {
}

inline NTriangleEmbedding::NTriangleEmbedding(
        const NTriangleEmbedding& cloneMe) :
        tetrahedron_(cloneMe.tetrahedron_), tri_(cloneMe.tri_) {
}

inline NTetrahedron* NTriangleEmbedding::getTetrahedron() const {
    return tetrahedron_;
}

inline int NTriangleEmbedding::getTriangle() const {
    return tri_;
}

inline int NTriangleEmbedding::getFace() const {
    return tri_;
}

inline NPerm4 NTriangleEmbedding::getVertices() const {
    return tetrahedron_->getTriangleMapping(tri_);
}

inline bool NTriangleEmbedding::operator == (const NTriangleEmbedding& other)
        const {
    return ((tetrahedron_ == other.tetrahedron_) && (tri_ == other.tri_));
}

inline bool NTriangleEmbedding::operator != (const NTriangleEmbedding& other)
        const {
    return ((tetrahedron_ != other.tetrahedron_) || (tri_ != other.tri_));
}

} // namespace regina

#endif

