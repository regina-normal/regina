
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file dim4/dim4triangulation.h
 *  \brief Deals with 4-dimensional triangulations.
 */

#ifndef __DIM4TRIANGULATION_H
#ifndef __DOXYGEN
#define __DIM4TRIANGULATION_H
#endif

#include <list>
#include <memory>
#include <vector>
#include "regina-core.h"
#include "algebra/nabeliangroup.h"
#include "algebra/ngrouppresentation.h"
#include "generic/triangulation.h"
#include "packet/npacket.h"
#include "utilities/nmarkedvector.h"
#include "utilities/nproperty.h"

// The following headers are necessary so that std::unique_ptr can invoke
// destructors where necessary.
#include "dim4/dim4isomorphism.h"

// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim4BoundaryComponent;
class Dim4Edge;
class Dim4Tetrahedron;
class Dim4Triangle;
class Dim4Vertex;
class NXMLDim4TriangulationReader;
class NXMLPacketReader;

template <int> class Component;
template <int> class Isomorphism;
template <int> class SimplexBase;
template <int> class Simplex;
typedef Component<4> Dim4Component;
typedef Isomorphism<4> Dim4Isomorphism;
typedef Simplex<4> Dim4Pentachoron;

/**
 * \addtogroup dim4 4-Manifold Triangulations
 * Triangulations of 4-manifolds.
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_DIM4TRIANGULATION> {
    typedef Triangulation <4>Class;
    inline static const char* name() {
        return "4-Manifold Triangulation";
    }
};
#endif

/**
 * Represents a 4-dimensional triangulation, typically of a 4-manifold.
 *
 * This is a specialisation of the generic Triangulation class template;
 * see the Triangulation documentation for a general overview of how
 * the triangulation classes work.
 *
 * This 4-dimensional specialisation offers significant extra functionality,
 * including many functions specific to 4-manifolds, plus rich details of
 * the combinatorial structure of the triangulation.
 *
 * In particular, this class also tracks vertices, edges, triangles and
 * tetrahedra of the triangulation (as represented by the classes Dim4Vertex,
 * Dim4Edge, Dim4Triangle and Dim4Tetrahedron), as well as boundary components
 * (as represented by the class Dim4BoundaryComponent).  Such objects are
 * temporary: whenever the triangulation changes, these objects will be
 * deleted and rebuilt, and so any pointers to them will become invalid.
 * Likewise, if the triangulation is deleted then these objects will be
 * deleted alongside it.
 *
 * A 4-manifold triangulation is built from pentachora: a \e pentachoron is a
 * 4-dimensional simplex, with five vertices.
 */
template <>
class REGINA_API Triangulation<4> :
        public NPacket,
        public TriangulationBase<4> {
    REGINA_PACKET(Triangulation<4>, PACKET_DIM4TRIANGULATION)

    public:
        typedef std::vector<Dim4Pentachoron*>::const_iterator
                PentachoronIterator;
            /**< A dimension-specific alias for SimplexIterator, used to
                 iterate through pentachora. */
        typedef std::vector<Dim4Tetrahedron*>::const_iterator
                TetrahedronIterator;
            /**< Used to iterate through tetrahedra. */
        typedef std::vector<Dim4Triangle*>::const_iterator TriangleIterator;
            /**< Used to iterate through triangles. */
        typedef std::vector<Dim4Edge*>::const_iterator EdgeIterator;
            /**< Used to iterate through edges. */
        typedef std::vector<Dim4Vertex*>::const_iterator VertexIterator;
            /**< Used to iterate through vertices. */
        typedef std::vector<Dim4BoundaryComponent*>::const_iterator
                BoundaryComponentIterator;
            /**< Used to iterate through boundary components. */

    private:
        mutable bool knownSimpleLinks_;
            /**< Is it known that all vertex links are 3-spheres or 3-balls?
                 This may be \c true even if the skeleton has not yet been
                 calculated (thereby allowing us to avoid costly 3-sphere or
                 3-ball recognition when the skeleton is eventually computed).
                 A value of \c false may mean that there are other vertex
                 links, or it may mean that the vertex links have not yet
                 been calculated. */

        mutable NMarkedVector<Dim4Tetrahedron> tetrahedra_;
            /**< The tetrahedra in the triangulation skeleton. */
        mutable NMarkedVector<Dim4Triangle> triangles_;
            /**< The triangles in the triangulation skeleton. */
        mutable NMarkedVector<Dim4Edge> edges_;
            /**< The edges in the triangulation skeleton. */
        mutable NMarkedVector<Dim4Vertex> vertices_;
            /**< The vertices in the triangulation skeleton. */
        mutable NMarkedVector<Dim4BoundaryComponent> boundaryComponents_;
            /**< The components that form the boundary of the
                 triangulation. */

        mutable bool valid_;
            /**< Is the triangulation valid? */
        mutable bool ideal_;
            /**< Is the triangulation ideal? */

        mutable NProperty<NGroupPresentation, StoreManagedPtr> fundGroup_;
            /**< Fundamental group of the triangulation. */
        mutable NProperty<NAbelianGroup, StoreManagedPtr> H1_;
            /**< First homology group of the triangulation. */
        mutable NProperty<NAbelianGroup, StoreManagedPtr> H2_;
            /**< Second homology group of the triangulation. */

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
        Triangulation();
        /**
         * Copy constructor.
         *
         * Creates a copy of the given triangulation.
         * The packet tree structure and packet label are \e not copied.
         *
         * @param copy the triangulation to copy.
         */
        Triangulation(const Triangulation& copy);
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
         * @param description a string that describes a 4-manifold
         * triangulation.
         */
        Triangulation(const std::string& description);
        /**
         * Destroys this triangulation.
         *
         * The constituent pentachora, the cellular structure and all other
         * properties will also be deallocated.
         */
        virtual ~Triangulation();

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
         * \name Pentachora
         */
        /*@{*/

        /**
         * A dimension-specific alias for size().
         *
         * See size() for further information.
         */
        unsigned long getNumberOfPentachora() const;
        /**
         * A dimension-specific alias for simplices().
         *
         * See simplices() for further information.
         */
        const std::vector<Dim4Pentachoron*>& getPentachora() const;
        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        Dim4Pentachoron* getPentachoron(unsigned long index);
        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        const Dim4Pentachoron* getPentachoron(unsigned long index) const;
        /**
         * A dimension-specific alias for simplexIndex().
         *
         * See simplexIndex() for further information.
         */
        long pentachoronIndex(const Dim4Pentachoron* tet) const;
        /**
         * A dimension-specific alias for newSimplex().
         *
         * See newSimplex() for further information.
         */
        Dim4Pentachoron* newPentachoron();
        /**
         * A dimension-specific alias for newSimplex().
         *
         * See newSimplex() for further information.
         */
        Dim4Pentachoron* newPentachoron(const std::string& desc);
        /**
         * A dimension-specific alias for removeSimplex().
         *
         * See removeSimplex() for further information.
         */
        void removePentachoron(Dim4Pentachoron* tet);
        /**
         * A dimension-specific alias for removeSimplexAt().
         *
         * See removeSimplexAt() for further information.
         */
        void removePentachoronAt(unsigned long index);
        /**
         * A dimension-specific alias for removeAllSimplices().
         *
         * See removeAllSimplices() for further information.
         */
        void removeAllPentachora();

        /*@}*/
        /**
         * \name Skeletal Queries
         */
        /*@{*/

        /**
         * Returns the number of boundary components in this triangulation.
         *
         * Note that each ideal vertex forms its own boundary component, and
         * some invalid vertices do also.  See the Dim4BoundaryComponent
         * class notes and Dim4Vertex::isBoundary() for details.
         *
         * @return the number of boundary components.
         */
        unsigned long getNumberOfBoundaryComponents() const;
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
         * Returns the number of triangles in this triangulation.
         *
         * @return the number of triangles.
         */
        unsigned long getNumberOfTriangles() const;
        /**
         * Returns the number of tetrahedra in this triangulation.
         *
         * @return the number of tetrahedra.
         */
        unsigned long getNumberOfTetrahedra() const;
        /**
         * Returns the number of faces of the given dimension in this
         * triangulation.
         *
         * This template function is to assist with writing dimension-agnostic
         * code that can be reused to work in different dimensions.
         *
         * \pre the template argument \a dim is between 0 and 4 inclusive.
         *
         * \ifacespython Not present.
         *
         * @return the number of faces of the given dimension.
         */
        template <int dim>
        unsigned long getNumberOfFaces() const;

        /**
         * Returns all boundary components of this triangulation.
         *
         * Note that each ideal vertex forms its own boundary component, and
         * some invalid vertices do also.  See the Dim4BoundaryComponent
         * class notes and Dim4Vertex::isBoundary() for details.
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
        const std::vector<Dim4BoundaryComponent*>& getBoundaryComponents()
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
        const std::vector<Dim4Vertex*>& getVertices() const;
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
        const std::vector<Dim4Edge*>& getEdges() const;
        /**
         * Returns all triangles of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * triangles will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all triangles.
         */
        const std::vector<Dim4Triangle*>& getTriangles() const;
        /**
         * Returns all tetrahedra of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * tetrahedra will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all triangles.
         */
        const std::vector<Dim4Tetrahedron*>& getTetrahedra() const;
        /**
         * Returns the requested boundary component of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * boundary components will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired boundary component, ranging
         * from 0 to getNumberOfBoundaryComponents()-1 inclusive.
         * @return the requested boundary component.
         */
        Dim4BoundaryComponent* getBoundaryComponent(unsigned long index) const;
        /**
         * Returns the requested vertex in this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * vertices will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired vertex, ranging from 0
         * to getNumberOfVertices()-1 inclusive.
         * @return the requested vertex.
         */
        Dim4Vertex* getVertex(unsigned long index) const;
        /**
         * Returns the requested edge in this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * edges will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired edge, ranging from 0
         * to getNumberOfEdges()-1 inclusive.
         * @return the requested edge.
         */
        Dim4Edge* getEdge(unsigned long index) const;
        /**
         * Returns the requested triangle in this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * triangles will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired triangle, ranging from 0
         * to getNumberOfTriangles()-1 inclusive.
         * @return the requested triangle.
         */
        Dim4Triangle* getTriangle(unsigned long index) const;
        /**
         * Returns the requested tetrahedron in this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * tetrahedra will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired tetrahedron, ranging from 0
         * to getNumberOfTetrahedra()-1 inclusive.
         * @return the requested tetrahedron.
         */
        Dim4Tetrahedron* getTetrahedron(unsigned long index) const;
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
        long boundaryComponentIndex(const Dim4BoundaryComponent* bc) const;
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
        long vertexIndex(const Dim4Vertex* vertex) const;
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
        long edgeIndex(const Dim4Edge* edge) const;
        /**
         * Returns the index of the given triangle in the triangulation.
         *
         * \pre The given triangle belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param tri specifies which triangle to find in the triangulation.
         * @return the index of the specified triangle, where 0 is the first
         * triangle, 1 is the second and so on.
         */
        long triangleIndex(const Dim4Triangle* tri) const;
        /**
         * Returns the index of the given tetrahedron in the triangulation.
         *
         * \pre The given tetrahedron belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param tet specifies which tetrahedron to find in the triangulation.
         * @return the index of the specified tetrahedron, where 0 is the
         * first tetrahedron, 1 is the second and so on.
         */
        long tetrahedronIndex(const Dim4Tetrahedron* tet) const;

        /*@}*/
        /**
         * \name Basic Properties
         */
        /*@{*/

        /**
         * Returns the Euler characteristic of this triangulation.
         * This will be evaluated strictly as \a V-E+F-T+P.
         *
         * Note that this routine handles cusps in a non-standard way.
         * Since it computes the Euler characteristic of the
         * triangulation (and not the underlying manifold), this routine
         * will treat each cusp as a single vertex, and \e not as
         * a surface boundary component.
         *
         * For a routine that handles cusps properly (i.e., treats them
         * as 3-manifold boundary components when computing the Euler
         * characteristic), see getEulerCharManifold() instead.
         *
         * @return the Euler characteristic of this triangulation.
         */
        long getEulerCharTri() const;

        /**
         * Returns the Euler characteristic of the corresponding compact
         * manifold.
         *
         * Instead of simply calculating \a V-E+F-T+P, this routine also
         * treats ideal vertices as 3-manifold boundary components (i.e.,
         * effectively truncates them).
         *
         * For ideal triangulations, this routine therefore computes the
         * proper Euler characteristic of the manifold (unlike
         * getEulerCharTri(), which does not).
         *
         * For triangulations whose vertex links are all 3-spheres or
         * 3-balls, this routine and getEulerCharTri() give identical results.
         *
         * This routine does \e not yet handle invalid triangulations
         * correctly.  For this reason, this routine currently insists on
         * a valid triangulation as a precondition.
         *
         * \pre This triangulation is valid.
         *
         * @return the Euler characteristic of the corresponding compact
         * manifold.
         */
        long getEulerCharManifold() const;

        /**
         * Determines if this triangulation is valid.
         *
         * A triangulation is valid unless it contains an invalid
         * vertex, edge or triangle.
         *
         * - An invalid vertex has a bad vertex link (specifically, the
         *   link is either an invalid 3-manifold triangulation, an ideal
         *   3-manifold triangulation, or bounded but not a 3-ball).
         *
         * - An invalid edge has a bad edge link (neither a 2-sphere nor
         *   a disc), and/or is identified with itself in reverse.
         *
         * - An invalid triangle is identified with itself using a
         *   non-trivial rotation or reflection.
         *
         * If you wish to find out why a particular triangulation is
         * invalid, see Dim4Vertex::isValid(), Dim4Edge::isValid() and
         * Dim4Triangle::isValid() respectively.
         *
         * @return \c true if and only if this triangulation is valid.
         */
        bool isValid() const;
        /**
         * Determines if this triangulation is ideal.
         *
         * A triangulation is ideal if and only if (i) the triangulation
         * is valid, and (ii) one of the vertex links is closed but not
         * a 3-sphere.
         *
         * Note that for 4-manifolds, isIdeal() will only return \c true
         * if the triangulation is valid (unlike 3-manifolds, where
         * invalid ideal triangulations are allowed).  This is to avoid
         * situations like 4-manifold vertices whose links are cusped
         * 3-manifolds (a situation that has no analogue in lower dimensions).
         *
         * @return \c true if and only if this triangulation is ideal.
         */
        bool isIdeal() const;
        bool hasBoundaryFacets() const;
        /**
         * A dimension-specific alias for hasBoundaryFacets().
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryTetrahedra() const;
        size_t countBoundaryFacets() const;
        /**
         * A dimension-specific alias for countBoundaryFacets().
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryTetrahedra() const;
        /**
         * Determines if this triangulation is closed.
         * This is the case if and only if it has no boundary components.
         *
         * Note that ideal triangulations are not closed.  Triangulations
         * with invalid vertices are also considered not closed; see
         * Dim4Vertex::isBoundary() for details.
         *
         * @return \c true if and only if this triangulation is closed.
         */
        bool isClosed() const;

        /*@}*/
        /**
         * \name Algebraic Properties
         */
        /*@{*/

        /**
         * Returns the fundamental group of this triangulation.
         * If this triangulation contains any ideal vertices, the
         * fundamental group will be calculated as if each such vertex
         * had been truncated.
         *
         * Bear in mind that each time the triangulation changes, the
         * fundamental group will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, getFundamentalGroup() should be called again; this will
         * be instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         * \pre This triangulation has at most one component.
         *
         * @return the fundamental group.
         */
        const NGroupPresentation& getFundamentalGroup() const;
        /**
         * Notifies the triangulation that you have simplified the
         * presentation of its fundamental group.  The old group
         * presentation will be destroyed, and this triangulation will take
         * ownership of the new (hopefully simpler) group that is passed.
         *
         * This routine is useful for situations in which some external
         * body (such as GAP) has simplified the group presentation
         * better than Regina can.
         *
         * Regina does \e not verify that the new group presentation is
         * equivalent to the old, since this is - well, hard.
         *
         * If the fundamental group has not yet been calculated for this
         * triangulation, this routine will nevertheless take ownership
         * of the new group, under the assumption that you have worked
         * out the group through some other clever means without ever
         * having needed to call getFundamentalGroup() at all.
         *
         * Note that this routine will not fire a packet change event.
         *
         * @param newGroup a new (and hopefully simpler) presentation of
         * the fundamental group of this triangulation.
         */
        void simplifiedFundamentalGroup(NGroupPresentation* newGroup);

        /**
         * Returns the first homology group for this triangulation.
         * If this triangulation contains any ideal vertices, the homology
         * group will be calculated as if each such vertex had been truncated.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, getHomologyH1() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         *
         * @return the first homology group.
         */
        const NAbelianGroup& getHomologyH1() const;

        /**
         * Returns the second homology group for this triangulation.
         * If this triangulation contains any ideal vertices, the homology
         * group will be calculated as if each such vertex had been truncated.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, getHomologyH2() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         *
         * @return the second homology group.
         */
        const NAbelianGroup& getHomologyH2() const;

        /*@}*/
        /**
         * \name Skeletal Transformations
         */
        /*@{*/

        /**
         * Attempts to simplify the triangulation as intelligently as
         * possible without further input.  Specifically, this routine will
         * attempt to reduce the number of pentachora in the triangulation.
         *
         * Currently this routine uses simplifyToLocalMinimum() in
         * combination with random 3-3 moves and book opening moves.
         *
         * \warning The specific behaviour of this routine will almost
         * certainly change between releases.  At present,
         * simplification for 4-manifold triangulations is extremely
         * weak (as opposed to 3-manifolds, where a rich library of
         * simplification techinques is available to call upon).
         *
         * @return \c true if and only if the triangulation was changed.
         */
        bool intelligentSimplify();
        /**
         * Uses all known simplification moves to reduce the triangulation
         * monotonically to some local minimum number of pentachora.
         * Note that this will probably not give a globally minimal
         * triangulation; see intelligentSimplify() for further
         * assistance in achieving this goal.
         *
         * The moves used currently include collapsing edges, 4-2 moves,
         * and boundary shelling moves.
         *
         * Note that moves that do not reduce the number of pentachora
         * (such as book opening moves) are not used in this routine.
         * Such moves do however feature in intelligentSimplify().
         *
         * \warning The specific behaviour of this routine will almost
         * certainly change between releases.  At present,
         * simplification for 4-manifold triangulations is extremely
         * weak (as opposed to 3-manifolds, where a rich library of
         * simplification techinques is available to call upon).
         *
         * @param perform \c true if we are to perform the simplifications,
         * or \c false if we are only to investigate whether simplifications
         * are possible (defaults to \c true).
         * @return if \a perform is \c true, this routine returns \c true
         * if and only if the triangulation was changed to reduce the
         * number of pentachora; if \a perform is \c false, this routine
         * returns \c true if and only if it determines that it is
         * capable of performing such a change.
         */
        bool simplifyToLocalMinimum(bool perform = true);

        /**
         * Checks the eligibility of and/or performs a 4-2 move
         * about the given edge.  This involves replacing the four pentachora
         * joined at that edge with two pentachora joined along a single facet.
         * This can be done iff (i) the edge is valid and non-boundary,
         * (ii) the four pentachora are distinct, and (iii) the pentachora
         * are joined in such a way that the link of the edge is the
         * standard 3-simplex triangulation of the 2-sphere.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (facets, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * See the page on \ref pachner for definitions and terminology
         * relating to Pachner moves.  After the move, the new belt
         * face will be <tt>getPentachora().back()->getTetrahedron(4)</tt>.
         *
         * \pre If the move is being performed and no check is being run,
         * it must be known in advance that the move is legal.
         * \pre The given edge is an edge of this triangulation.
         *
         * @param e the edge about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool fourTwoMove(Dim4Edge* e, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 3-3 move
         * about the given triangle.
         * This involves replacing the three pentachora joined along that
         * triangle with three pentachora joined along a transverse triangle.
         * This can be done iff (i) the triangle is valid and non-boundary,
         * and (ii) the three pentachora are distinct.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (facets, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a f)
         * can no longer be used.
         *
         * See the page on \ref pachner for definitions and terminology
         * relating to Pachner moves.  After the move, the new belt face will be
         * <tt>getPentachora().back()->getTriangle(Dim4Triangle::triangleNumber[0][1][2])</tt>.
         *
         * \pre If the move is being performed and no check is being run,
         * it must be known in advance that the move is legal.
         * \pre The given triangle is a triangle of this triangulation.
         *
         * @param t the triangle about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool threeThreeMove(Dim4Triangle* t, bool check = true,
            bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 2-4 move
         * about the given facet.
         * This involves replacing the two pentachora joined along that
         * facet with four pentachora joined along a transverse edge.
         * This can be done iff the two pentachora are distinct.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (facets, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a f)
         * can no longer be used.
         *
         * See the page on \ref pachner for definitions and terminology
         * relating to Pachner moves.  After the move, the new belt face will be
         * <tt>getPentachora().back()->getEdge(Dim4Edge::edgeNumber[0][1])</tt>.
         *
         * \pre If the move is being performed and no check is being run,
         * it must be known in advance that the move is legal.
         * \pre The given facet is a facet of this triangulation.
         *
         * @param f the facet about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool twoFourMove(Dim4Tetrahedron* f, bool check = true,
            bool perform = true);

        /**
         * Checks the eligibility of and/or performs a 1-5 move
         * upon the given pentachoron.
         * This involves replacing one pentachoron with five pentachora:
         * each new pentachoron runs from one facet of
         * the original pentachoron to a new common internal degree five vertex.
         *
         * This move can always be performed.  The \a check argument is
         * present (as for other moves), but is simply ignored (since
         * the move is always legal).  The \a perform argument is also
         * present for consistency with other moves, but if it is set to
         * \c false then this routine does nothing and returns no useful
         * information.
         *
         * Note that after performing this move, all skeletal objects
         * (facets, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a p)
         * can no longer be used.
         *
         * See the page on \ref pachner for definitions and terminology
         * relating to Pachner moves.  After the move, the new belt face will be
         * <tt>getPentachora().back()->getVertex(4)</tt>.
         *
         * \pre The given pentachoron is a pentachoron of this triangulation.
         *
         * @param p the pentachoron about which to perform the move.
         * @param check this argument is ignored, since this move is
         * always legal (see the notes above).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return \c true always.
         */
        bool oneFiveMove(Dim4Pentachoron* p, bool check = true,
            bool perform = true);

        /**
         * Checks the eligibility of and/or performs a 2-0 move about
         * the given triangle of degree 2.
         * This involves taking the two pentachora joined at that triangle
         * and squashing them flat.
         *
         * The eligibility requirements for this move are somewhat
         * involved, and are discussed in detail in the source code for
         * those who are interested.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (tetrahedra, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a f)
         * can no longer be used.
         *
         * \pre If the move is being performed and no check is being run,
         * it must be known in advance that the move is legal.
         * \pre The given triangle is a triangle of this triangulation.
         *
         * @param t the triangle about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool twoZeroMove(Dim4Triangle* t, bool check = true,
            bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 2-0 move about
         * the given edge of degree 2.
         * This involves taking the two pentachora joined at that edge
         * and squashing them flat.
         * This can be done if:
         *
         * - the edge is valid and non-boundary and has a 2-sphere edge link;
         *
         * - the two pentachora are distinct;
         *
         * - the triangles opposite \a e in each pentachoron are
         *   distinct and not both boundary;
         *
         * - if facets \a f1 and \a f2 of one pentachoron are to be
         *   flattened onto facets \a g1 and \a g2 of the other
         *   respectively, then
         *   (a) \a f1 and \a g1 are distinct,
         *   (b) \a f2 and \a g2 are distinct,
         *   (c) we do not have both \a f1 = \a g2 and \a g1 = \a f2,
         *   (d) we do not have both \a f1 = \a f2 and \a g1 = \a g2, and
         *   (e) we do not have two of the facets boundary and the other
         *   two identified;
         *
         * - the two pentachora meet each other on all three facets
         *   touching the edge (as opposed to meeting each other on one
         *   facet and being glued to themselves along the other two).
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (tetrahedra, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a f)
         * can no longer be used.
         *
         * \pre If the move is being performed and no check is being run,
         * it must be known in advance that the move is legal.
         * \pre The given edge is an edge of this triangulation.
         *
         * @param e the edge about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool twoZeroMove(Dim4Edge* e, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a book opening move
         * about the given tetrahedron.
         * This involves taking a tetrahedron meeting the boundary along
         * precisely one, two or three triangles, and ungluing it to create two
         * new boundary facets (thus exposing the pentachora it initially
         * joined).  This move is intended to open the way for new
         * shellBoundary() moves.
         *
         * This move can be done if:
         *
         * - all vertices, edges and triangles of the tetrahedron are valid;
         *
         * - the tetrahedron meets the boundary in precisely one, two or
         *   three triangles (and therefore also joins two pentachora);
         *
         * - if the tetrahedron meets the boundary in precisely one triangle,
         *   then the remaining vertex of the tetrahedron is non-boundary, and
         *   no two of the remaining three edges of the tetrahedron are
         *   identified;
         *
         * - if the tetrahedron meets the boundary in precisely two triangles,
         *   then the remaining edge of the tetrahedron is non-boundary, and
         *   the remaining two triangles of the tetrahedron are not identified.
         *
         * The validity condition in particular is stronger than it
         * needs to be, but the resulting "lost opportunities" only
         * affect invalid triangulations.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (edges, components, etc.) will be reconstructed, which means
         * that any pointers to old skeletal objects (such as the argument
         * \a t) can no longer be used.
         *
         * \pre If the move is being performed and no check is being run,
         * it must be known in advance that the move is legal.
         * \pre The given tetrahedron is a tetrahedron of this triangulation.
         *
         * @param t the tetrahedron about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool openBook(Dim4Tetrahedron* t,
            bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a boundary shelling
         * move on the given pentachoron.
         * This involves simply popping off a pentachoron that touches
         * the boundary.
         * This can be done if:
         *
         * - all edges and triangles of the pentachoron are valid;
         *
         * - precisely one, two, three or four facets of the pentachoron
         *   lie in the boundary;
         *
         * - if one facet lies in the boundary, then the opposite vertex
         *   does not lie in the boundary, and no two of the remaining
         *   four edges are identified;
         *
         * - if two facets lie in the boundary, then the edge that sits
         *   opposite their common triangle does not lie in the boundary, and
         *   no two of the remaining three triangles are identified;
         *
         * - if three facets lie in the boundary, then the triangle that sits
         *   opposite their common edge does not lie in the boundary, and
         *   the remaining two facets of the tetrahedron are not identified.
         *
         * The validity condition in particular is stronger than it
         * needs to be, but the resulting "lost opportunities" only
         * affect invalid triangulations.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (edges, components, etc.) will be reconstructed, which means
         * that any pointers to old skeletal objects can no longer be used.
         *
         * \pre If the move is being performed and no check is being
         * run, it must be known in advance that the move is legal.
         * \pre The given pentachoron is a pentachoron of this triangulation.
         *
         * @param p the pentachoron upon which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \a true, this function returns \c true
         * if and only if the requested move may be performed without
         * changing the topology of the manifold.  If \a check is \c false,
         * this function simply returns \c true.
         */
        bool shellBoundary(Dim4Pentachoron* p,
            bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a collapse of
         * an edge in such a way that the topology of the manifold
         * does not change and the number of vertices of the triangulation
         * decreases by one.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (facets, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * The eligibility requirements for this move are somewhat
         * involved, and are discussed in detail in the collapseEdge()
         * source code for those who are interested.
         *
         * \pre If the move is being performed and no check is being run,
         * it must be known in advance that the move is legal.
         * \pre The given edge is an edge of this triangulation.
         *
         * @param e the edge to collapse.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the given edge may be collapsed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool collapseEdge(Dim4Edge* e, bool check = true, bool perform = true);

        /*@}*/
        /**
         * \name Subdivisions and Covers
         */
        /*@{*/

        /**
         * Does a barycentric subdivision of the triangulation.
         * Each pentachoron is divided into 120 pentachora by placing an
         * extra vertex at the centroid of each pentachoron,
         * tetrahedron, triangle, and edge.
         */
        void barycentricSubdivision();

        /**
         * Converts an ideal triangulation into a finite triangulation.
         * All ideal or invalid vertices are truncated and thus
         * converted into real boundary components made from unglued
         * facets of pentachora.
         *
         * @return \c true if and only if the triangulation was changed.
         */
        bool idealToFinite();

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
        void cloneFrom(const Triangulation& from);

    private:
        /**
         * Clears any calculated properties and declares them all
         * unknown.  All dynamic memory used for storing known
         * properties is deallocated.
         *
         * In most cases this routine is followed immediately by firing
         * a packet change event.
         */
        void clearAllProperties();

        void deleteSkeleton();
        void calculateSkeleton() const;

        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateTetrahedra() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateVertices() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateEdges() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateTriangles() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateBoundary() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateVertexLinks() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateEdgeLinks() const;

    friend class regina::Simplex<4>;
    friend class regina::SimplexBase<4>;
    friend class regina::TriangulationBase<4>;
    friend class regina::NXMLDim4TriangulationReader;
};

/**
 * A convenience typedef for Triangulation<4>.
 */
typedef Triangulation<4> Dim4Triangulation;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4tetrahedron.h"
#include "dim4/dim4triangle.h"
#include "dim4/dim4edge.h"
#include "dim4/dim4vertex.h"
#include "dim4/dim4component.h"
#include "dim4/dim4boundarycomponent.h"
namespace regina {

// Inline functions for Dim4Triangulation

inline Triangulation<4>::Triangulation() : knownSimpleLinks_(false) {
}

inline Triangulation<4>::Triangulation(const Triangulation& cloneMe) :
        knownSimpleLinks_(false) {
    cloneFrom(cloneMe);
}

inline Triangulation<4>::~Triangulation() {
    clearAllProperties();
}

inline void Triangulation<4>::writeTextShort(std::ostream& out) const {
    out << "Triangulation with " << size()
        << (size() == 1 ? " pentachoron" : " pentachora");
}

inline bool Triangulation<4>::dependsOnParent() const {
    return false;
}

inline unsigned long Triangulation<4>::getNumberOfPentachora() const {
    return size();
}

inline const std::vector<Dim4Pentachoron*>& Triangulation<4>::getPentachora()
        const {
    return getSimplices();
}

inline Dim4Pentachoron* Triangulation<4>::getPentachoron(unsigned long index) {
    return getSimplex(index);
}

inline const Dim4Pentachoron* Triangulation<4>::getPentachoron(
        unsigned long index) const {
    return getSimplex(index);
}

inline long Triangulation<4>::pentachoronIndex(const Dim4Pentachoron* pent)
        const {
    return pent->markedIndex();
}

inline Dim4Pentachoron* Triangulation<4>::newPentachoron() {
    return newSimplex();
}

inline Dim4Pentachoron* Triangulation<4>::newPentachoron(
        const std::string& desc) {
    return newSimplex(desc);
}

inline void Triangulation<4>::removePentachoron(Dim4Pentachoron* pent) {
    removeSimplex(pent);
}

inline void Triangulation<4>::removePentachoronAt(unsigned long index) {
    removeSimplexAt(index);
}

inline void Triangulation<4>::removeAllPentachora() {
    removeAllSimplices();
}

inline unsigned long Triangulation<4>::getNumberOfBoundaryComponents() const {
    ensureSkeleton();
    return boundaryComponents_.size();
}

inline unsigned long Triangulation<4>::getNumberOfVertices() const {
    ensureSkeleton();
    return vertices_.size();
}

inline unsigned long Triangulation<4>::getNumberOfEdges() const {
    ensureSkeleton();
    return edges_.size();
}

inline unsigned long Triangulation<4>::getNumberOfTriangles() const {
    ensureSkeleton();
    return triangles_.size();
}

inline unsigned long Triangulation<4>::getNumberOfTetrahedra() const {
    ensureSkeleton();
    return tetrahedra_.size();
}

template <>
inline unsigned long Triangulation<4>::getNumberOfFaces<0>() const {
    return getNumberOfVertices();
}

template <>
inline unsigned long Triangulation<4>::getNumberOfFaces<1>() const {
    return getNumberOfEdges();
}

template <>
inline unsigned long Triangulation<4>::getNumberOfFaces<2>() const {
    return getNumberOfTriangles();
}

template <>
inline unsigned long Triangulation<4>::getNumberOfFaces<3>() const {
    return getNumberOfTetrahedra();
}

template <>
inline unsigned long Triangulation<4>::getNumberOfFaces<4>() const {
    return getNumberOfPentachora();
}

inline const std::vector<Dim4BoundaryComponent*>&
        Triangulation<4>::getBoundaryComponents() const {
    ensureSkeleton();
    return (const std::vector<Dim4BoundaryComponent*>&)(boundaryComponents_);
}

inline const std::vector<Dim4Vertex*>& Triangulation<4>::getVertices() const {
    ensureSkeleton();
    return (const std::vector<Dim4Vertex*>&)(vertices_);
}

inline const std::vector<Dim4Edge*>& Triangulation<4>::getEdges() const {
    ensureSkeleton();
    return (const std::vector<Dim4Edge*>&)(edges_);
}

inline const std::vector<Dim4Triangle*>& Triangulation<4>::getTriangles()
        const {
    ensureSkeleton();
    return (const std::vector<Dim4Triangle*>&)(triangles_);
}

inline const std::vector<Dim4Tetrahedron*>& Triangulation<4>::getTetrahedra()
        const {
    ensureSkeleton();
    return (const std::vector<Dim4Tetrahedron*>&)(tetrahedra_);
}

inline Dim4BoundaryComponent* Triangulation<4>::getBoundaryComponent(
        unsigned long index) const {
    ensureSkeleton();
    return boundaryComponents_[index];
}

inline Dim4Vertex* Triangulation<4>::getVertex(unsigned long index) const {
    ensureSkeleton();
    return vertices_[index];
}

inline Dim4Edge* Triangulation<4>::getEdge(unsigned long index) const {
    ensureSkeleton();
    return edges_[index];
}

inline Dim4Triangle* Triangulation<4>::getTriangle(unsigned long index) const {
    ensureSkeleton();
    return triangles_[index];
}

inline Dim4Tetrahedron* Triangulation<4>::getTetrahedron(unsigned long index)
        const {
    ensureSkeleton();
    return tetrahedra_[index];
}

inline long Triangulation<4>::boundaryComponentIndex(
        const Dim4BoundaryComponent* boundaryComponent) const {
    return boundaryComponent->markedIndex();
}

inline long Triangulation<4>::vertexIndex(const Dim4Vertex* vertex) const {
    return vertex->markedIndex();
}

inline long Triangulation<4>::edgeIndex(const Dim4Edge* edge) const {
    return edge->markedIndex();
}

inline long Triangulation<4>::triangleIndex(const Dim4Triangle* tri) const {
    return tri->markedIndex();
}

inline long Triangulation<4>::tetrahedronIndex(const Dim4Tetrahedron* tet)
        const {
    return tet->markedIndex();
}

inline long Triangulation<4>::getEulerCharTri() const {
    ensureSkeleton();

    // Cast away the unsignedness of std::vector::size().
    return static_cast<long>(vertices_.size())
        - static_cast<long>(edges_.size())
        + static_cast<long>(triangles_.size())
        - static_cast<long>(tetrahedra_.size())
        + static_cast<long>(size());
}

inline bool Triangulation<4>::isValid() const {
    ensureSkeleton();
    return valid_;
}

inline bool Triangulation<4>::isIdeal() const {
    ensureSkeleton();
    return ideal_;
}

inline bool Triangulation<4>::hasBoundaryFacets() const {
    // Override, since we can do this faster in dimension 4.
    ensureSkeleton();
    return (2 * tetrahedra_.size() > 5 * size());
}

inline bool Triangulation<4>::hasBoundaryTetrahedra() const {
    return hasBoundaryFacets();
}

inline size_t Triangulation<4>::countBoundaryFacets() const {
    // Override, since we can do this faster in dimension 4.
    ensureSkeleton();
    return 2 * tetrahedra_.size() - 5 * size();
}

inline size_t Triangulation<4>::countBoundaryTetrahedra() const {
    return countBoundaryFacets();
}

inline bool Triangulation<4>::isClosed() const {
    ensureSkeleton();
    return boundaryComponents_.empty();
}

inline void Triangulation<4>::simplifiedFundamentalGroup(
        NGroupPresentation* newGroup) {
    fundGroup_ = newGroup;
}

inline NPacket* Triangulation<4>::internalClonePacket(NPacket*) const {
    return new Dim4Triangulation(*this);
}

} // namespace regina

#endif

