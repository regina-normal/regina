
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

/*! \file dim2/dim2triangulation.h
 *  \brief Deals with 2-manifold triangulations.
 */

#ifndef __DIM2TRIANGULATION_H
#ifndef __DOXYGEN
#define __DIM2TRIANGULATION_H
#endif

#include <memory>
#include <vector>
#include "regina-core.h"
#include "generic/ngenerictriangulation.h"
#include "packet/npacket.h"
#include "utilities/nmarkedvector.h"
#include "utilities/nproperty.h"

// The following headers are necessary so that std::unique_ptr can invoke
// destructors where necessary.
#include "dim2/dim2isomorphism.h"

// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim2BoundaryComponent;
class Dim2Component;
class Dim2Edge;
class Dim2Triangle;
class Dim2Triangulation;
class Dim2Vertex;
class NXMLDim2TriangulationReader;
class NXMLPacketReader;

/**
 * \addtogroup dim2 2-Manifold Triangulations
 * Triangulations of 2-manifolds.
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_DIM2TRIANGULATION> {
    typedef Dim2Triangulation Class;
    inline static const char* name() {
        return "2-Manifold Triangulation";
    }
};
#endif

/**
 * Stores the triangulation of a 2-manifold along with its
 * various cellular structures and other information.  A 2-manifold
 * triangulation is built from triangular faces.
 *
 * When the triangulation is deleted, the corresponding
 * triangles, the cellular structure and all other properties
 * will be deallocated.
 *
 * Elements of the 1- and 0-skeletons (edges and vertices respectively) are
 * always temporary, as are components and
 * boundary components.  Whenever a change occurs with the triangulation,
 * these objects will all be deleted and a new skeletal structure will be
 * calculated.  The same is true of various other triangulation properties.
 */
class REGINA_API Dim2Triangulation : public NPacket,
        public NGenericTriangulation<2> {
    REGINA_PACKET(Dim2Triangulation, PACKET_DIM2TRIANGULATION)

    public:
        typedef std::vector<Dim2Triangle*>::const_iterator TriangleIterator;
            /**< Used to iterate through triangles. */
        typedef std::vector<Dim2Edge*>::const_iterator EdgeIterator;
            /**< Used to iterate through edges. */
        typedef std::vector<Dim2Vertex*>::const_iterator VertexIterator;
            /**< Used to iterate through vertices. */
        typedef std::vector<Dim2Component*>::const_iterator ComponentIterator;
            /**< Used to iterate through components. */
        typedef std::vector<Dim2BoundaryComponent*>::const_iterator
                BoundaryComponentIterator;
            /**< Used to iterate through boundary components. */

    private:
        mutable bool calculatedSkeleton_;
            /**< Has the skeleton been calculated? */

        NMarkedVector<Dim2Triangle> triangles_;
            /**< The triangular faces that form the triangulation. */
        mutable NMarkedVector<Dim2Edge> edges_;
            /**< The edges in the triangulation skeleton. */
        mutable NMarkedVector<Dim2Vertex> vertices_;
            /**< The vertices in the triangulation skeleton. */
        mutable NMarkedVector<Dim2Component> components_;
            /**< The components that form the triangulation. */
        mutable NMarkedVector<Dim2BoundaryComponent> boundaryComponents_;
            /**< The components that form the boundary of the
                 triangulation. */

        mutable bool orientable_;
            /**< Is the triangulation orientable? */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Default constructor.
         *
         * Creates an empty triangulation.
         */
        Dim2Triangulation();
        /**
         * Copy constructor.
         *
         * Creates a new triangulation identical to the given triangulation.
         * The packet tree structure and packet label are \e not copied.
         *
         * @param cloneMe the triangulation to clone.
         */
        Dim2Triangulation(const Dim2Triangulation& cloneMe);
        /**
         * "Magic" constructor that tries to find some way to interpret
         * the given string as a triangulation.
         *
         * At present, Regina understands the following types of strings
         * (and attempts to parse them in the following order):
         *
         * - isomorphism signatures (see fromIsoSig()).
         *
         * This list may grow in future versions of Regina.
         *
         * Regina will also set the packet label accordingly.
         *
         * If Regina cannot interpret the given string, this will be
         * left as the empty triangulation.
         *
         * @param description a string that describes a 2-manifold
         * triangulation.
         */
        Dim2Triangulation(const std::string& description);
        /**
         * Destroys this triangulation.
         *
         * The constituent triangles, the cellular structure and all other
         * properties will also be deallocated.
         */
        virtual ~Dim2Triangulation();

        /*@}*/
        /**
         * \name Packet Administration
         */
        /*@{*/

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        virtual bool dependsOnParent() const;

        /*@}*/
        /**
         * \name Triangles
         */
        /*@{*/

        /**
         * Returns the number of triangular faces in the triangulation.
         *
         * @return the number of triangles.
         */
        unsigned long getNumberOfTriangles() const;
        /**
         * A dimension-agnostic alias for getNumberOfTriangles().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 2-manifold triangulations means a triangle).
         * 
         * See getNumberOfTriangles() for further information.
         */
        unsigned long getNumberOfSimplices() const;
        /**
         * Returns all triangular faces in the triangulation.
         *
         * The reference returned will remain valid for as long as the
         * triangulation exists, always reflecting the triangles currently
         * in the triangulation.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all triangles.
         */
        const std::vector<Dim2Triangle*>& getTriangles() const;
        /**
         * A dimension-agnostic alias for getTriangles().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 2-manifold triangulations means a triangle).
         * 
         * See getTriangles() for further information.
         */
        const std::vector<Dim2Triangle*>& getSimplices() const;
        /**
         * Returns the triangle with the given index number in the
         * triangulation.  Note that triangle indexing may change when
         * a triangle is added or removed from the triangulation.
         *
         * @param index specifies which triangle to return; this
         * value should be between 0 and getNumberOfTriangles()-1 inclusive.
         * @return the <tt>index</tt>th triangle in the triangulation.
         */
        Dim2Triangle* getTriangle(unsigned long index);
        /**
         * A dimension-agnostic alias for getTriangle().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 2-manifold triangulations means a triangle).
         * 
         * See getTriangle() for further information.
         */
        Dim2Triangle* getSimplex(unsigned long index);
        /**
         * Returns the triangle with the given index number in the
         * triangulation.  Note that triangle indexing may change when
         * a triangle is added or removed from the triangulation.
         *
         * @param index specifies which triangle to return; this
         * value should be between 0 and getNumberOfTriangles()-1 inclusive.
         * @return the <tt>index</tt>th triangle in the triangulation.
         */
        const Dim2Triangle* getTriangle(unsigned long index) const;
        /**
         * A dimension-agnostic alias for getTriangle().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 2-manifold triangulations means a triangle).
         * 
         * See getTriangle() for further information.
         */
        const Dim2Triangle* getSimplex(unsigned long index) const;
        /**
         * Returns the index of the given triangle in the triangulation.
         *
         * Note that triangle indexing may change when a triangle
         * is added or removed from the triangulation.
         *
         * \pre The given triangle is contained in this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.  If you are passing the result of some other
         * routine that \e might return null (such as
         * Dim2Triangle::adjacentTriangle), it might be worth explicitly
         * testing for null beforehand.
         *
         * @param tri specifies which triangle to find in the triangulation.
         * @return the index of the specified triangle, where 0 is
         * the first triangle, 1 is the second and so on.
         */
        long triangleIndex(const Dim2Triangle* tri) const;
        /**
         * A dimension-agnostic alias for triangleIndex().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 2-manifold triangulations means a triangle).
         * 
         * See triangleIndex() for further information.
         */
        long simplexIndex(const Dim2Triangle* tri) const;
        /**
         * Creates a new triangle and adds it to this triangulation.
         * The new triangle will have an empty description.
         * All three edges of the new triangle will be boundary edges.
         *
         * The new triangle will become the last triangle in this
         * triangulation.
         *
         * @return the new triangle.
         */
        Dim2Triangle* newTriangle();
        /**
         * A dimension-agnostic alias for newTriangle().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 2-manifold triangulations means a triangle).
         * 
         * See newTriangle() for further information.
         */
        Dim2Triangle* newSimplex();
        /**
         * Creates a new triangle with the given description and adds
         * it to this triangulation.
         * All three edges of the new triangle will be boundary edges.
         *
         * @param desc the description to assign to the new triangle.
         * @return the new triangle.
         */
        Dim2Triangle* newTriangle(const std::string& desc);
        /**
         * A dimension-agnostic alias for newTriangle().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 2-manifold triangulations means a triangle).
         * 
         * See newTriangle() for further information.
         */
        Dim2Triangle* newSimplex(const std::string& desc);
        /**
         * Removes the given triangle from the triangulation.
         * All triangles glued to this triangle will be unglued.
         * The triangle will be deallocated.
         *
         * \pre The given triangle exists in the triangulation.
         *
         * @param tri the triangle to remove.
         */
        void removeTriangle(Dim2Triangle* tri);
        /**
         * A dimension-agnostic alias for removeTriangle().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 2-manifold triangulations means a triangle).
         * 
         * See removeTriangle() for further information.
         */
        void removeSimplex(Dim2Triangle* tri);
        /**
         * Removes the triangle with the given index number
         * from the triangulation.  Note that triangle indexing may
         * change when a triangle is added or removed from the
         * triangulation.
         *
         * All triangles glued to this triangle will be unglued.
         * The triangle will be deallocated.
         *
         * @param index specifies which triangle to remove; this
         * should be between 0 and getNumberOfTriangles()-1 inclusive.
         */
        void removeTriangleAt(unsigned long index);
        /**
         * A dimension-agnostic alias for removeTriangleAt().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 2-manifold triangulations means a triangle).
         * 
         * See removeTriangleAt() for further information.
         */
        void removeSimplexAt(unsigned long index);
        /**
         * Removes all triangles from the triangulation.
         * All triangles will be deallocated.
         */
        void removeAllTriangles();
        /**
         * A dimension-agnostic alias for removeAllTriangles().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         *
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 2-manifold triangulations means a triangle).
         * 
         * See removeAllTriangles() for further information.
         */
        void removeAllSimplices();
        /**
         * Swaps the contents of this and the given triangulation.
         * That is, all triangles that belong to this triangulation
         * will be moved to \a other, and all triangles that belong to
         * \a other will be moved to this triangulation.
         *
         * All Dim2Triangle pointers or references will remain valid.
         *
         * @param other the triangulation whose contents should be
         * swapped with this.
         */
        void swapContents(Dim2Triangulation& other);
        /**
         * Moves the contents of this triangulation into the given
         * destination triangulation, without destroying any pre-existing
         * contents.  That is, all triangles that currently belong to
         * \a dest will remain there, and all triangles that belong to this
         * triangulation will be moved across as additional
         * triangles in \a dest.
         *
         * All Dim2Triangle pointers or references will remain valid.
         * After this operation, this triangulation will be empty.
         *
         * @param dest the triangulation to which triangles should be
         * moved.
         */
        void moveContentsTo(Dim2Triangulation& dest);

        /*@}*/
        /**
         * \name Skeletal Queries
         */
        /*@{*/

        /**
         * Returns the number of boundary components in this triangulation.
         *
         * @return the number of boundary components.
         */
        unsigned long getNumberOfBoundaryComponents() const;
        /**
         * Returns the number of components in this triangulation.
         *
         * @return the number of components.
         */
        unsigned long getNumberOfComponents() const;
        /**
         * Returns the number of vertices in this triangulation.
         *
         * @return the number of vertices.
         */
        unsigned long getNumberOfVertices() const;
        /**
         * Returns the number of edges in this triangulation.
         *
         * @return the number of edges.
         */
        unsigned long getNumberOfEdges() const;
        /**
         * Returns the number of faces of the given dimension in this
         * triangulation.
         *
         * This template function is to assist with writing dimension-agnostic
         * code that can be reused to work in different dimensions.
         *
         * \pre The template argument \a subdim is between 0 and 2 inclusive.
         *
         * \ifacespython Not present.
         *
         * @return the number of faces of the given dimension.
         */
        template <int subdim>
        unsigned long getNumberOfFaces() const;

        /**
         * Returns all components of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * components will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all components.
         */
        const std::vector<Dim2Component*>& getComponents() const;
        /**
         * Returns all boundary components of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * boundary components will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all boundary components.
         */
        const std::vector<Dim2BoundaryComponent*>& getBoundaryComponents()
            const;
        /**
         * Returns all vertices of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * vertices will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all vertices.
         */
        const std::vector<Dim2Vertex*>& getVertices() const;
        /**
         * Returns all edges of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * edges will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all edges.
         */
        const std::vector<Dim2Edge*>& getEdges() const;
        /**
         * Returns the requested triangulation component.
         *
         * Bear in mind that each time the triangulation changes, the
         * components will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired component, ranging from 0
         * to getNumberOfComponents()-1 inclusive.
         * @return the requested component.
         */
        Dim2Component* getComponent(unsigned long index) const;
        /**
         * Returns the requested triangulation boundary component.
         *
         * Bear in mind that each time the triangulation changes, the
         * boundary components will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired boundary component, ranging
         * from 0 to getNumberOfBoundaryComponents()-1 inclusive.
         * @return the requested boundary component.
         */
        Dim2BoundaryComponent* getBoundaryComponent(unsigned long index) const;
        /**
         * Returns the requested triangulation vertex.
         *
         * Bear in mind that each time the triangulation changes, the
         * vertices will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired vertex, ranging from 0
         * to getNumberOfVertices()-1 inclusive.
         * @return the requested vertex.
         */
        Dim2Vertex* getVertex(unsigned long index) const;
        /**
         * Returns the requested triangulation edge.
         *
         * Bear in mind that each time the triangulation changes, the
         * edges will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired edge, ranging from 0
         * to getNumberOfEdges()-1 inclusive.
         * @return the requested edge.
         */
        Dim2Edge* getEdge(unsigned long index) const;
        /**
         * Returns the requested face of the given dimension in this
         * triangulation.
         *
         * This template function is to assist with writing dimension-agnostic
         * code that can be reused to work in different dimensions.
         *
         * \pre The template argument \a subdim is between 0 and 2 inclusive.
         *
         * \ifacespython Not present.
         *
         * @param index the index of the desired face, ranging from 0 to
         * getNumberOfFaces<subdim>()-1 inclusive.
         * @return the requested face.
         */
        template <int subdim>
        typename FaceTraits<2, subdim>::Face* getFace(unsigned long index)
            const;
        /**
         * Returns the index of the given component in the triangulation.
         *
         * \pre The given component belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param component specifies which component to find in the
         * triangulation.
         * @return the index of the specified component, where 0 is the first
         * component, 1 is the second and so on.
         */
        long componentIndex(const Dim2Component* component) const;
        /**
         * Returns the index of the given boundary component
         * in the triangulation.
         *
         * \pre The given boundary component belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param bc specifies which boundary component to find in the
         * triangulation.
         * @return the index of the specified boundary component,
         * where 0 is the first boundary component, 1 is the second and so on. 
         */
        long boundaryComponentIndex(const Dim2BoundaryComponent* bc) const;
        /**
         * Returns the index of the given vertex in the triangulation.
         *
         * \pre The given vertex belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param vertex specifies which vertex to find in the triangulation.
         * @return the index of the specified vertex, where 0 is the first
         * vertex, 1 is the second and so on.
         */
        long vertexIndex(const Dim2Vertex* vertex) const;
        /**
         * Returns the index of the given edge in the triangulation.
         *
         * \pre The given edge belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param edge specifies which edge to find in the triangulation.
         * @return the index of the specified edge, where 0 is the first
         * edge, 1 is the second and so on.
         */
        long edgeIndex(const Dim2Edge* edge) const;
        /**
         * Returns the index of the given face of the given dimension in this
         * triangulation.
         *
         * This template function is to assist with writing dimension-agnostic
         * code that can be reused to work in different dimensions.
         *
         * \pre The template argument \a subdim is between 0 and 2 inclusive.
         * \pre The given face belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * \ifacespython Not present.
         *
         * @param face specifies which face to find in the triangulation.
         * @return the index of the specified face, where 0 is the first
         * \a subdim-face, 1 is the second \a subdim-face, and so on.
         */
        template <int subdim>
        long faceIndex(const typename FaceTraits<2, subdim>::Face* face) const;

        /*@}*/
        /**
         * \name Isomorphism Testing
         */
        /*@{*/

        using NGenericTriangulation<2>::isIdenticalTo;
        using NGenericTriangulation<2>::isIsomorphicTo;
        using NGenericTriangulation<2>::isContainedIn;
        using NGenericTriangulation<2>::findAllIsomorphisms;
        using NGenericTriangulation<2>::findAllSubcomplexesIn;
        using NGenericTriangulation<2>::makeCanonical;

        /*@}*/
        /**
         * \name Basic Properties
         */
        /*@{*/

        using NGenericTriangulation<2>::isEmpty;

        /**
         * Always returns \c true.
         *
         * This routine determines if this triangulation is valid; however,
         * there is nothing that can go wrong with vertex links in 2-manifold
         * triangulations, and so this routine always returns \c true.
         *
         * This no-op routine is provided for consistency with higher
         * dimensional triangulations, and to assist with writing
         * dimension-agnostic code.
         *
         * @return \c true.
         */
        bool isValid() const;
        /**
         * Returns the Euler characteristic of this triangulation.
         * This will be evaluated as \a V-E+F.
         *
         * @return the Euler characteristic of this triangulation.
         */
        long getEulerChar() const;
        /**
         * Determines if this triangulation is closed.
         * This is the case if and only if it has no boundary components.
         *
         * @return \c true if and only if this triangulation is closed.
         */
        bool isClosed() const;
        /**
         * Determines if this triangulation has any boundary edges.
         *
         * This routine is redundant in dimension two, since it returns
         * \c true if and only if isClosed() returns \c false.
         * It is provided simply for compatibility with higher-dimensional
         * triangulation classes.
         *
         * @return \c true if and only if there are boundary edges.
         */
        bool hasBoundaryEdges() const;
        /**
         * Returns the number of boundary edges in this triangulation.
         *
         * @return the total number of boundary edges.
         */
        unsigned long getNumberOfBoundaryEdges() const;
        /**
         * Determines if this triangulation is orientable.
         *
         * @return \c true if and only if this triangulation is orientable.
         */
        bool isOrientable() const;
        /**
         * Determines if this triangulation is connected.
         *
         * @return \c true if and only if this triangulation is connected.
         */
        bool isConnected() const;
        /**
         * Always returns \c false.
         *
         * This routine determines if this triangulation is ideal (has a
         * non-trivial vertex link); however, every vertex link in a
         * 2-manifold triangulation is either the interval or the
         * circle, and so ideal triangulations cannot exist.
         * Therefore this routine always returns \c false.
         *
         * This no-op routine is provided for consistency with higher
         * dimensional triangulations, and to assist with writing
         * dimension-agnostic code.
         *
         * @return \c false.
         */
        bool isIdeal() const;

        /**
         * Determines whether this is a minimal triangulation of the
         * underlying 2-manifold; that is, it uses the fewest possible
         * triangles.
         *
         * Testing for minimality is simple in two dimensions (unlike
         * higher dimensions, where it becomes extremely difficult).
         * With the exception of the sphere, disc and projective plane
         * (which require a minimum of 2, 1 and 2 triangles respectively),
         * a closed triangulation is minimal if and only if it has one
         * vertex, and a bounded triangulation is minimal if and only if
         * it has one vertex per boundary component and no internal vertices.
         *
         * The proof is based on a simple Euler characteristic calculation,
         * whereby the number of triangles <tt>T</tt> is
         * <tt>T = 2Vi + Vb - 2C</tt>, where <tt>Vi</tt> and <tt>Vb</tt>
         * are the number of internal and boundary vertices respectively,
         * and where <tt>C</tt> is the Euler characteristic of the
         * underlying manifold.
         *
         * @return \c true if and only if this is a minimal triangulation.
         */
        bool isMinimal() const;

        /*@}*/
        /**
         * \name Skeletal Transformations
         */
        /*@{*/

        /**
         * Checks the eligibility of and/or performs a 1-3 move
         * upon the given triangle.
         * This involves replacing one triangle with three triangles:
         * each new triangle runs from one edge of
         * the original triangle to a new common internal degree three vertex.
         *
         * This move can always be performed.  The \a check argument is
         * present (as for other moves), but is simply ignored (since
         * the move is always legal).  The \a perform argument is also
         * present for consistency with other moves, but if it is set to
         * \c false then this routine does nothing and returns no useful
         * information.
         *
         * Note that after performing this move, all skeletal objects
         * (edges, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a t)
         * can no longer be used.
         *
         * \pre The given triangle is a triangle of this triangulation.
         *
         * @param t the triangle about which to perform the move.
         * @param check this argument is ignored, since this move is
         * always legal (see the notes above).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return \c true always.
         */
        bool oneThreeMove(Dim2Triangle* t, bool check = true,
            bool perform = true);

        /*@}*/
        /**
         * \name Building Triangulations
         */
        /*@{*/

        /**
         * Inserts a copy of the given triangulation into this triangulation.
         *
         * The new triangles will be inserted into this triangulation
         * in the order in which they appear in the given triangulation,
         * and the numbering of their vertices (0-2) will not change.
         * They will be given the same descriptions as appear in the
         * given triangulation.
         *
         * @param source the triangulation whose copy will be inserted.
         */
        void insertTriangulation(const Dim2Triangulation& source);

        /**
         * Inserts into this triangulation a set of triangles and their
         * gluings as described by the given integer arrays.
         *
         * This routine is provided to make it easy to hard-code a
         * medium-sized triangulation in a C++ source file.  All of the
         * pertinent data can be hard-coded into a pair of integer arrays at
         * the beginning of the source file, avoiding an otherwise tedious
         * sequence of many joinTo() calls.
         *
         * An additional \a nTriangles triangles will be inserted into
         * this triangulation.  The relationships between these triangles
         * should be stored in the two arrays as follows.  Note that the
         * new triangles are numbered from 0 to (\a nTriangles - 1), and
         * individual triangle edges are numbered from 0 to 2.
         *
         * The \a adjacencies array describes which triangle edges are
         * joined to which others.  Specifically, <tt>adjacencies[f][e]</tt>
         * should contain the number of the triangle joined to edge \a e
         * of triangle \a f.  If this edge is to be left as a
         * boundary edge, <tt>adjacencies[f][e]</tt> should be -1.
         *
         * The \a gluings array describes the particular gluing permutations
         * used when joining these triangle edges together.  Specifically,
         * <tt>gluings[f][e][0..2]</tt> should describe the permutation
         * used to join edge \a e of triangle \a f to its adjacent
         * triangle.  These three integers should be 0, 1 and 2 in some
         * order, so that <tt>gluings[f][e][i]</tt> contains the image of
         * \a i under this permutation.  If edge \a e of triangle \a f
         * is to be left as a boundary edge, <tt>gluings[f][e][0..2]</tt>
         * may contain anything (and will be duly ignored).
         *
         * It is the responsibility of the caller of this routine to
         * ensure that the given arrays are correct and consistent.
         * No error checking will be performed by this routine.
         *
         * Note that, for an existing triangulation, dumpConstruction()
         * will output a pair of C++ arrays that can be copied into a
         * source file and used to reconstruct the triangulation via
         * this routine.
         *
         * \ifacespython Not present.
         *
         * @param nTriangles the number of additional triangles to insert.
         * @param adjacencies describes which of the new triangle edges
         * are to be identified.  This array must have initial
         * dimension at least \a nTriangles.
         * @param gluings describes the specific gluing permutations by
         * which these new triangle edges should be identified.  This
         * array must also have initial dimension at least \a nTriangles.
         */
        void insertConstruction(unsigned long nTriangles,
            const int adjacencies[][3], const int gluings[][3][3]);

        /*@}*/
        /**
         * \name Exporting Triangulations
         */
        /*@{*/

        using NGenericTriangulation<2>::isoSig;

        /**
         * Returns C++ code that can be used with insertConstruction()
         * to reconstruct this triangulation.
         *
         * The code produced will consist of the following:
         *
         * - the declaration and initialisation of two integer arrays,
         *   describing the triangle gluings in this trianguation;
         * - two additional lines that declare a new Dim2Triangulation and
         *   call insertConstruction() to rebuild this triangulation.
         *
         * The main purpose of this routine is to generate the two integer
         * arrays, which can be tedious and error-prone to code up by hand.
         *
         * Note that the number of lines of code produced grows linearly
         * with the number of triangles.  If this triangulation is very
         * large, the returned string will be very large as well.
         *
         * @return the C++ code that was generated.
         */
        std::string dumpConstruction() const;

        /*@}*/
        /**
         * \name Importing Triangulations
         */
        /*@{*/

        using NGenericTriangulation<2>::fromIsoSig;
        using NGenericTriangulation<2>::isoSigComponentSize;

        /*@}*/

        static NXMLPacketReader* getXMLReader(NPacket* parent,
            NXMLTreeResolver& resolver);

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

        /**
         * Turns this triangulation into a clone of the given triangulation.
         * The tree structure and label of this triangulation are not touched.
         *
         * @param from the triangulation from which this triangulation
         * will be cloned.
         */
        void cloneFrom(const Dim2Triangulation& from);

    private:
        void deleteTriangles();
            /**< Deallocates all triangles and empties the list. */
        void deleteSkeleton();
            /**< Deallocates all skeletal objects and empties all
                 corresponding lists. */

        /**
         * Clears any calculated properties and declares them all
         * unknown.  All dynamic memory used for storing known
         * properties is deallocated.
         *
         * In most cases this routine is followed immediately by firing
         * a packet change event.
         */
        virtual void clearAllProperties();

        /**
         * Recalculates vertices, edges, components and
         * boundary components, as well as various other skeletal properties.
         * All appropriate lists are filled.
         *
         * \pre All skeletal lists are empty.
         */
        void calculateSkeleton() const;

        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateComponents() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateVertices() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateBoundary() const;

        /**
         * Determines if an isomorphic copy of this triangulation is
         * contained within the given triangulation.
         *
         * If the argument \a completeIsomorphism is \c true, the
         * isomorphism must be onto and boundary complete.
         * That is, this triangulation must be combinatorially
         * isomorphic to the given triangulation.
         *
         * If the argument \a completeIsomorphism is \c false, the
         * isomorphism may be boundary incomplete and may or may not be
         * onto.  That is, this triangulation must appear as a
         * subcomplex of the given triangulation, possibly with some
         * original boundary edges joined to new triangles.
         *
         * See the Dim2Isomorphism class notes for further details
         * regarding boundary complete and boundary incomplete
         * isomorphisms.
         *
         * The isomorphisms found, if any, will be appended to the
         * list \a results.  This list will not be emptied before
         * calculations begin.  All isomorphisms will be newly created,
         * and the caller of this routine is responsible for destroying
         * them.
         *
         * If \a firstOnly is passed as \c true, only the first
         * isomorphism found (if any) will be returned, after which the
         * routine will return immediately.  Otherwise all isomorphisms
         * will be returned.
         *
         * @param other the triangulation in which to search for an
         * isomorphic copy of this triangulation.
         * @param results the list in which any isomorphisms found will
         * be stored.
         * @param completeIsomorphism \c true if isomorphisms must be
         * onto and boundary complete, or \c false if neither of these
         * restrictions should be imposed.
         * @param firstOnly \c true if only one isomorphism should be
         * returned (if any), or \c false if all isomorphisms should be
         * returned.
         * @return the total number of isomorphisms found.
         */
        unsigned long findIsomorphisms(const Dim2Triangulation& other,
                std::list<Dim2Isomorphism*>& results,
                bool completeIsomorphism, bool firstOnly) const;

        /**
         * Internal to findIsomorphisms().
         *
         * Examines properties of the given triangle to find any
         * immediate evidence that \a src may not map to \a dest in a
         * boundary complete isomorphism (in which the vertices of \a src
         * are mapped to the vertices of \a dest according to the
         * permutation \a p).
         *
         * In particular, the degrees of vertices are examined.
         *
         * @param src the first of the two triangles to examine.
         * @param dest the second of the two triangles to examine.
         * @param p the permutation under which the vertices of \a src
         * must map to the vertices of \a dest.
         * @return \c true if no immediate incompatibilities between the
         * triangles were found, or \c false if properties of the
         * triangles were found that differ between \a src and \a dest.
         */
        static bool compatibleTriangles(Dim2Triangle* src, Dim2Triangle* dest,
            NPerm3 p);

    friend class regina::NGenericTriangulation<2>;
    friend class regina::Dim2Triangle;
    friend class regina::NXMLDim2TriangulationReader;
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim2/dim2triangle.h"
#include "dim2/dim2edge.h"
#include "dim2/dim2vertex.h"
#include "dim2/dim2component.h"
#include "dim2/dim2boundarycomponent.h"
namespace regina {

// Inline functions for Dim2Triangulation

inline Dim2Triangulation::Dim2Triangulation() :
        NPacket(), calculatedSkeleton_(false) {
}

inline Dim2Triangulation::Dim2Triangulation(const Dim2Triangulation& cloneMe) :
        NPacket(), calculatedSkeleton_(false) {
    cloneFrom(cloneMe);
}

inline Dim2Triangulation::~Dim2Triangulation() {
    clearAllProperties();
    deleteTriangles();
}

inline void Dim2Triangulation::writeTextShort(std::ostream& out) const {
    out << "Triangulation with " << triangles_.size()
        << (triangles_.size() == 1 ? " triangle" : " triangles");
}

inline bool Dim2Triangulation::dependsOnParent() const {
    return false;
}

inline unsigned long Dim2Triangulation::getNumberOfTriangles() const {
    return triangles_.size();
}

inline unsigned long Dim2Triangulation::getNumberOfSimplices() const {
    return triangles_.size();
}

inline const std::vector<Dim2Triangle*>& Dim2Triangulation::getTriangles()
        const {
    return (const std::vector<Dim2Triangle*>&)(triangles_);
}

inline const std::vector<Dim2Triangle*>& Dim2Triangulation::getSimplices()
        const {
    return (const std::vector<Dim2Triangle*>&)(triangles_);
}

inline Dim2Triangle* Dim2Triangulation::getTriangle(unsigned long index) {
    return triangles_[index];
}

inline Dim2Triangle* Dim2Triangulation::getSimplex(unsigned long index) {
    return triangles_[index];
}

inline const Dim2Triangle* Dim2Triangulation::getTriangle(unsigned long index)
        const {
    return triangles_[index];
}

inline const Dim2Triangle* Dim2Triangulation::getSimplex(unsigned long index)
        const {
    return triangles_[index];
}

inline long Dim2Triangulation::triangleIndex(const Dim2Triangle* tri) const {
    return tri->markedIndex();
}

inline long Dim2Triangulation::simplexIndex(const Dim2Triangle* tri) const {
    return tri->markedIndex();
}

inline Dim2Triangle* Dim2Triangulation::newTriangle() {
    ChangeEventSpan span(this);
    Dim2Triangle* tri = new Dim2Triangle(this);
    triangles_.push_back(tri);
    clearAllProperties();
    return tri;
}

inline Dim2Triangle* Dim2Triangulation::newSimplex() {
    return newTriangle();
}

inline Dim2Triangle* Dim2Triangulation::newTriangle(const std::string& desc) {
    ChangeEventSpan span(this);
    Dim2Triangle* tri = new Dim2Triangle(desc, this);
    triangles_.push_back(tri);
    clearAllProperties();
    return tri;
}

inline Dim2Triangle* Dim2Triangulation::newSimplex(const std::string& desc) {
    return newTriangle(desc);
}

inline void Dim2Triangulation::removeTriangle(Dim2Triangle* tri) {
    ChangeEventSpan span(this);

    tri->isolate();
    triangles_.erase(triangles_.begin() + triangleIndex(tri));
    delete tri;

    clearAllProperties();
}

inline void Dim2Triangulation::removeSimplex(Dim2Triangle* tri) {
    removeTriangle(tri);
}

inline void Dim2Triangulation::removeTriangleAt(unsigned long index) {
    ChangeEventSpan span(this);

    Dim2Triangle* ans = triangles_[index];
    ans->isolate();
    triangles_.erase(triangles_.begin() + index);
    delete ans;

    clearAllProperties();
}

inline void Dim2Triangulation::removeSimplexAt(unsigned long index) {
    removeTriangleAt(index);
}

inline void Dim2Triangulation::removeAllTriangles() {
    ChangeEventSpan span(this);
    deleteTriangles();
    clearAllProperties();
}

inline void Dim2Triangulation::removeAllSimplices() {
    removeAllTriangles();
}

inline unsigned long Dim2Triangulation::getNumberOfBoundaryComponents() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return boundaryComponents_.size();
}

inline unsigned long Dim2Triangulation::getNumberOfComponents() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return components_.size();
}

inline unsigned long Dim2Triangulation::getNumberOfVertices() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return vertices_.size();
}

inline unsigned long Dim2Triangulation::getNumberOfEdges() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return edges_.size();
}

template <>
inline unsigned long Dim2Triangulation::getNumberOfFaces<0>() const {
    return getNumberOfVertices();
}

template <>
inline unsigned long Dim2Triangulation::getNumberOfFaces<1>() const {
    return getNumberOfEdges();
}

template <>
inline unsigned long Dim2Triangulation::getNumberOfFaces<2>() const {
    return getNumberOfTriangles();
}

inline const std::vector<Dim2Component*>& Dim2Triangulation::getComponents()
        const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (const std::vector<Dim2Component*>&)(components_);
}

inline const std::vector<Dim2BoundaryComponent*>&
        Dim2Triangulation::getBoundaryComponents() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (const std::vector<Dim2BoundaryComponent*>&)(boundaryComponents_);
}

inline const std::vector<Dim2Vertex*>& Dim2Triangulation::getVertices() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (const std::vector<Dim2Vertex*>&)(vertices_);
}

inline const std::vector<Dim2Edge*>& Dim2Triangulation::getEdges() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (const std::vector<Dim2Edge*>&)(edges_);
}

inline Dim2Component* Dim2Triangulation::getComponent(unsigned long index)
        const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return components_[index];
}

inline Dim2BoundaryComponent* Dim2Triangulation::getBoundaryComponent(
        unsigned long index) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return boundaryComponents_[index];
}

inline Dim2Vertex* Dim2Triangulation::getVertex(unsigned long index) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return vertices_[index];
}

inline Dim2Edge* Dim2Triangulation::getEdge(unsigned long index) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return edges_[index];
}

template <>
inline Dim2Vertex* Dim2Triangulation::getFace<0>(unsigned long index) const {
    return vertices_[index];
}

template <>
inline Dim2Edge* Dim2Triangulation::getFace<1>(unsigned long index) const {
    return edges_[index];
}

template <>
inline Dim2Triangle* Dim2Triangulation::getFace<2>(unsigned long index) const {
    return triangles_[index];
}

inline long Dim2Triangulation::componentIndex(const Dim2Component* component)
        const {
    return component->markedIndex();
}

inline long Dim2Triangulation::boundaryComponentIndex(
        const Dim2BoundaryComponent* boundaryComponent) const {
    return boundaryComponent->markedIndex();
}

inline long Dim2Triangulation::vertexIndex(const Dim2Vertex* vertex) const {
    return vertex->markedIndex();
}

inline long Dim2Triangulation::edgeIndex(const Dim2Edge* edge) const {
    return edge->markedIndex();
}

template <>
inline long Dim2Triangulation::faceIndex<0>(const Dim2Vertex* face) const {
    return face->markedIndex();
}

template <>
inline long Dim2Triangulation::faceIndex<1>(const Dim2Edge* face) const {
    return face->markedIndex();
}

template <>
inline long Dim2Triangulation::faceIndex<2>(const Dim2Triangle* face) const {
    return face->markedIndex();
}

inline bool Dim2Triangulation::isValid() const {
    return true;
}

inline long Dim2Triangulation::getEulerChar() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();

    // Cast away the unsignedness of std::vector::size().
    return static_cast<long>(vertices_.size())
        - static_cast<long>(edges_.size())
        + static_cast<long>(triangles_.size());
}

inline bool Dim2Triangulation::isClosed() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return boundaryComponents_.empty();
}

inline bool Dim2Triangulation::hasBoundaryEdges() const {
    return ! isClosed();
}

inline unsigned long Dim2Triangulation::getNumberOfBoundaryEdges() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return 2 * edges_.size() - 3 * triangles_.size();
}

inline bool Dim2Triangulation::isOrientable() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return orientable_;
}

inline bool Dim2Triangulation::isIdeal() const {
    return false;
}

inline bool Dim2Triangulation::isConnected() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (components_.size() <= 1);
}

inline NPacket* Dim2Triangulation::internalClonePacket(NPacket*) const {
    return new Dim2Triangulation(*this);
}

} // namespace regina

#endif

