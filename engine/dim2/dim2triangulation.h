
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

/*! \file dim2/dim2triangulation.h
 *  \brief Deals with 2-manifold triangulations.
 */

#ifndef __DIM2TRIANGULATION_H
#ifndef __DOXYGEN
#define __DIM2TRIANGULATION_H
#endif

#include <list>
#include <memory>
#include <vector>
#include "regina-core.h"
#include "packet/npacket.h"
#include "utilities/nmarkedvector.h"
#include "utilities/nproperty.h"

// The following headers are necessary so that std::auto_ptr can invoke
// destructors where necessary.
#include "dim2/dim2isomorphism.h"

// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim2BoundaryComponent;
class Dim2Component;
class Dim2Edge;
class Dim2Triangle;
class Dim2Vertex;
class NXMLDim2TriangulationReader;
class NXMLPacketReader;

/**
 * \addtogroup dim2 2-Manifold Triangulations
 * Triangulations of 2-manifolds.
 * @{
 */

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
class REGINA_API Dim2Triangulation : public NPacket {
    public:
        static const int packetType;

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
         * Destroys this triangulation.
         *
         * The constituent triangles, the cellular structure and all other
         * properties will also be deallocated.
         */
        virtual ~Dim2Triangulation();

        /*@}*/
        /**
         * (end: Constructors and Destructors)
         */

        /**
         * \name Packet Administration
         */
        /*@{*/

        virtual int getPacketType() const;
        virtual std::string getPacketTypeName() const;

        virtual void writePacket(NFile& out) const;
        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        virtual bool dependsOnParent() const;

        /*@}*/
        /**
         * (end: Packet Administration)
         */

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
         * (end: Triangles)
         */

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

        /*@}*/
        /**
         * (end: Skeletal Queries)
         */

        /**
         * \name Isomorphism Testing
         */
        /*@{*/

        /**
         * Determines if this triangulation is combinatorially
         * isomorphic to the given triangulation.
         *
         * Specifically, this routine determines if there is a
         * one-to-one and onto boundary complete combinatorial
         * isomorphism from this triangulation to \a other.  Boundary
         * complete isomorphisms are described in detail in the
         * Dim2Isomorphism class notes.
         *
         * In particular, note that this triangulation and \a other must
         * contain the same number of triangles for such an isomorphism
         * to exist.
         *
         * If a boundary complete isomorphism is found, the details of
         * this isomorphism are returned.  The isomorphism is newly
         * constructed, and so to assist with memory management is
         * returned as a std::auto_ptr.  Thus, to test whether an
         * isomorphism exists without having to explicitly deal with the
         * isomorphism itself, you can call
         * <tt>if (isIsomorphicTo(other).get())</tt> and the newly
         * created isomorphism (if it exists) will be automatically
         * destroyed.
         *
         * @param other the triangulation to compare with this one.
         * @return details of the isomorphism if the two triangulations
         * are combinatorially isomorphic, or a null pointer otherwise.
         */
        std::auto_ptr<Dim2Isomorphism> isIsomorphicTo(
            const Dim2Triangulation& other) const;

        /**
         * Determines if an isomorphic copy of this triangulation is
         * contained within the given triangulation, possibly as a
         * subcomplex of some larger component (or components).
         *
         * Specifically, this routine determines if there is a boundary
         * incomplete combinatorial isomorphism from this triangulation
         * to \a other.  Boundary incomplete isomorphisms are described
         * in detail in the Dim2Isomorphism class notes.
         *
         * In particular, note that boundary edges of this triangulation
         * need not correspond to boundary edges of \a other, and that
         * \a other can contain more triangles than this triangulation.
         *
         * If a boundary incomplete isomorphism is found, the details of
         * this isomorphism are returned.  The isomorphism is newly
         * constructed, and so to assist with memory management is
         * returned as a std::auto_ptr.  Thus, to test whether an
         * isomorphism exists without having to explicitly deal with the
         * isomorphism itself, you can call
         * <tt>if (isContainedIn(other).get())</tt> and the newly
         * created isomorphism (if it exists) will be automatically
         * destroyed.
         *
         * If more than one such isomorphism exists, only one will be
         * returned.  For a routine that returns all such isomorphisms,
         * see findAllSubcomplexesIn().
         *
         * @param other the triangulation in which to search for an
         * isomorphic copy of this triangulation.
         * @return details of the isomorphism if such a copy is found,
         * or a null pointer otherwise.
         */
        std::auto_ptr<Dim2Isomorphism> isContainedIn(
            const Dim2Triangulation& other) const;

        /**
         * Finds all ways in which an isomorphic copy of this triangulation
         * is contained within the given triangulation, possibly as a
         * subcomplex of some larger component (or components).
         *
         * This routine behaves identically to isContainedIn(), except
         * that instead of returning just one isomorphism (which may be
         * boundary incomplete and need not be onto), all such isomorphisms
         * are returned.
         *
         * See the isContainedIn() notes for additional information.
         *
         * The isomorphisms that are found will be inserted into the
         * given list.  These isomorphisms will be newly created, and
         * the caller of this routine is responsible for destroying
         * them.  The given list will not be emptied before the new
         * isomorphisms are inserted.
         *
         * \ifacespython Not present.
         *
         * @param other the triangulation in which to search for
         * isomorphic copies of this triangulation.
         * @param results the list in which any isomorphisms found will
         * be stored.
         * @return the number of isomorphisms that were found.
         */
        unsigned long findAllSubcomplexesIn(const Dim2Triangulation& other,
                std::list<Dim2Isomorphism*>& results) const;

        /**
         * Relabel the triangles and their vertices so that this
         * triangulation is in canonical form.  This is essentially
         * the lexicographically smallest labelling when the edge
         * gluings are written out in order.
         *
         * Two triangulations are isomorphic if and only if their canonical
         * forms are identical.
         *
         * The lexicographic ordering assumes that the edge gluings are
         * written in order of triangle index and then edge number.
         * Each gluing is written as the destination triangle index
         * followed by the gluing permutation (which in turn is written
         * as the images of 0,1,2 in order).
         *
         * \pre This routine currently works only when the triangulation
         * is connected.  It may be extended to work with disconnected
         * triangulations in later versions of Regina.
         *
         * @return \c true if the triangulation was changed, or \c false
         * if the triangulation was in canonical form to begin with.
         */
        bool makeCanonical();

        /*@}*/
        /**
         * (end: Isomorphism Testing)
         */

        /**
         * \name Basic Properties
         */
        /*@{*/

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
         * (end: Basic Properties)
         */

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
         * Constructs the isomorphism signature for this triangulation.
         *
         * An <i>isomorphism signature</i> is a compact text representation of
         * a triangulation.  Unlike dehydrations for 3-manifold triangulations,
         * an isomorphism signature uniquely determines a triangulation up
         * to combinatorial isomorphism.  That is, two 2-manifold
         * triangulations are combinatorially isomorphic if and only if
         * their isomorphism signatures are the same.
         *
         * The isomorphism signature is constructed entirely of
         * printable characters, and has length proportional to
         * <tt>n log n</tt>, where \a n is the number of triangles.
         *
         * Isomorphism signatures are more general than dehydrations:
         * they can be used with any triangulation (including closed,
         * bounded and/or disconnected triangulations, as well
         * as triangulations with large numbers of triangles).
         *
         * The time required to construct the isomorphism signature of a
         * triangulation is <tt>O(n^2 log^2 n)</tt>.
         *
         * The routine fromIsoSig() can be used to recover a
         * triangulation from an isomorphism signature.  The triangulation
         * recovered might not be identical to the original, but it will be
         * combinatorially isomorphic.
         *
         * @return the isomorphism signature of this triangulation.
         */
        std::string isoSig() const;
        /**
         * Recovers a full triangulation from an isomorphism signature.
         * See isoSig() for more information on isomorphism signatures.
         *
         * The triangulation that is returned will be newly created.
         *
         * Calling isoSig() followed by fromIsoSig() is not guaranteed to
         * produce an identical triangulation to the original, but it
         * \e is guaranteed to produce a combinatorially isomorphic
         * triangulation.
         *
         * @param signature the isomorphism signature of the
         * triangulation to construct.  Note that, unlike dehydration
         * strings for 3-manifold triangulations, case is important for
         * isomorphism signatures.
         * @return a newly allocated triangulation if the reconstruction was
         * successful, or null if the given string was not a valid
         * isomorphism signature.
         */
        static Dim2Triangulation* fromIsoSig(const std::string& signature);
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
         * (end: Building Triangulations)
         */

        static NXMLPacketReader* getXMLReader(NPacket* parent);
        static Dim2Triangulation* readPacket(NFile& in, NPacket* parent);

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

        /**
         * Internal to isoSig().
         *
         * Constructs a candidate isomorphism signature for a single
         * component of this triangulation.  This candidate signature
         * assumes that the given triangle with the given labelling
         * of its vertices becomes triangle zero with vertices 0,1,2
         * under the "canonical isomorphism".
         *
         * @param tri the index of some triangle in this triangulation.
         * @param vertices some ordering of the three vertices of the
         * given triangle.
         * @return the candidate isomorphism signature.
         */
        std::string isoSig(unsigned tri, const NPerm3& vertices) const;

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

inline void Dim2Triangulation::writePacket(NFile&) const {
    // No need to add support for the old pre-2002 binary format.
    // This format is deprecated and about to be removed from Regina entirely.
}

inline void Dim2Triangulation::writeTextShort(std::ostream& out) const {
    out << "Triangulation with " << triangles_.size() << " triangles.";
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

inline Dim2Triangulation* Dim2Triangulation::readPacket(NFile&, NPacket*) {
    // Don't bother supporting the old pre-2002 binary format.
    return 0;
}

inline NPacket* Dim2Triangulation::internalClonePacket(NPacket*) const {
    return new Dim2Triangulation(*this);
}

} // namespace regina

#endif

