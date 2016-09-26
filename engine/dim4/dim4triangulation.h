
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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
class NXMLPacketReader;

template <int> class Component;
template <int> class Isomorphism;
template <int> class SimplexBase;
template <int> class Simplex;
template <int> class XMLTriangulationReader;
template <int, int> class Face;
typedef Component<4> Dim4Component;
typedef Isomorphism<4> Dim4Isomorphism;
typedef Simplex<4> Dim4Pentachoron;
typedef Face<4, 3> Dim4Tetrahedron;
typedef Face<4, 2> Dim4Triangle;
typedef Face<4, 1> Dim4Edge;
typedef Face<4, 0> Dim4Vertex;

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
        public detail::TriangulationBase<4> {
    REGINA_PACKET(Triangulation<4>, PACKET_DIM4TRIANGULATION)

    public:
        typedef std::vector<Dim4Pentachoron*>::const_iterator
                PentachoronIterator;
            /**< A dimension-specific alias for SimplexIterator, used to
                 iterate through pentachora. */
        typedef FaceList<4, 3>::Iterator TetrahedronIterator;
            /**< Used to iterate through tetrahedra. */
        typedef FaceList<4, 2>::Iterator TriangleIterator;
            /**< Used to iterate through triangles. */
        typedef FaceList<4, 1>::Iterator EdgeIterator;
            /**< Used to iterate through edges. */
        typedef FaceList<4, 0>::Iterator VertexIterator;
            /**< Used to iterate through vertices. */
        typedef std::vector<Dim4BoundaryComponent*>::const_iterator
                BoundaryComponentIterator;
            /**< Used to iterate through boundary components. */

    private:
        bool knownSimpleLinks_;
            /**< Is it known that all vertex links are 3-spheres or 3-balls?
                 This may be \c true even if the skeleton has not yet been
                 calculated (thereby allowing us to avoid costly 3-sphere or
                 3-ball recognition when the skeleton is eventually computed).
                 A value of \c false may mean that there are other vertex
                 links, or it may mean that the vertex links have not yet
                 been calculated. */

        NMarkedVector<Dim4BoundaryComponent> boundaryComponents_;
            /**< The components that form the boundary of the
                 triangulation. */

        bool ideal_;
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
        void removePentachoronAt(size_t index);
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
        size_t countBoundaryComponents() const;

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
        const std::vector<Dim4BoundaryComponent*>& boundaryComponents() const;
        /**
         * Deprecated routine that returns all boundary components of this
         * triangulation.
         *
         * \deprecated This routine has been renamed to boundaryComponents().
         * See the boundaryComponents() documentation for further details.
         */
        REGINA_DEPRECATED const std::vector<Dim4BoundaryComponent*>&
            getBoundaryComponents() const;
        /**
         * Returns the requested boundary component of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * boundary components will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired boundary component, ranging
         * from 0 to countBoundaryComponents()-1 inclusive.
         * @return the requested boundary component.
         */
        Dim4BoundaryComponent* boundaryComponent(size_t index) const;
        /**
         * Deprecated routine that returns the requested boundary component
         * of this triangulation.
         *
         * \deprecated This routine has been renamed to boundaryComponent().
         * See the boundaryComponent() documentation for further details.
         */
        REGINA_DEPRECATED Dim4BoundaryComponent* getBoundaryComponent(
            size_t index) const;

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
         * characteristic), see eulerCharManifold() instead.
         *
         * @return the Euler characteristic of this triangulation.
         */
        long eulerCharTri() const;

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
         * eulerCharTri(), which does not).
         *
         * For triangulations whose vertex links are all 3-spheres or
         * 3-balls, this routine and eulerCharTri() give identical results.
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
        long eulerCharManifold() const;

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
         * Instead, fundamentalGroup() should be called again; this will
         * be instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         * \pre This triangulation has at most one component.
         *
         * @return the fundamental group.
         */
        const NGroupPresentation& fundamentalGroup() const;
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
         * having needed to call fundamentalGroup() at all.
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
         * This routine can also be accessed via the alias homologyH1()
         * (a name that is more specific, but a little longer to type).
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, homology() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         *
         * @return the first homology group.
         */
        const NAbelianGroup& homology() const;

        /**
         * Returns the first homology group for this triangulation.
         * If this triangulation contains any ideal vertices, the homology
         * group will be calculated as if each such vertex had been truncated.
         *
         * This routine can also be accessed via the alias homology()
         * (a name that is less specific, but a little easier to type).
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, homologyH1() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         *
         * @return the first homology group.
         */
        const NAbelianGroup& homologyH1() const;

        /**
         * Returns the second homology group for this triangulation.
         * If this triangulation contains any ideal vertices, the homology
         * group will be calculated as if each such vertex had been truncated.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, homologyH2() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         *
         * @return the second homology group.
         */
        const NAbelianGroup& homologyH2() const;

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
         *
         * End users will probably not want to call this routine.
         * You should call intelligentSimplify() if you want a fast
         * method of simplifying a triangulation.
         *
         * The moves used by this routine include collapsing edges, 4-2 moves,
         * and boundary shelling moves.
         *
         * Moves that do not reduce the number of pentachora
         * (such as 3-3 moves or book opening moves) are not used in this
         * routine.  Such moves do however feature in intelligentSimplify().
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
         * face will be <tt>pentachora().back()->tetrahedron(4)</tt>.
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
         * <tt>pentachora().back()->triangle(Dim4Triangle::triangleNumber[0][1][2])</tt>.
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
         * <tt>pentachora().back()->edge(Dim4Edge::edgeNumber[0][1])</tt>.
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
         * <tt>pentachora().back()->vertex(4)</tt>.
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

        static NXMLPacketReader* xmlReader(NPacket* parent,
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
        void calculateSkeleton();

        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateBoundary();
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateVertexLinks();
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateEdgeLinks();

    friend class regina::Simplex<4>;
    friend class regina::detail::SimplexBase<4>;
    friend class regina::detail::TriangulationBase<4>;
    friend class regina::XMLTriangulationReader<4>;
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

inline void Triangulation<4>::removePentachoronAt(size_t index) {
    removeSimplexAt(index);
}

inline void Triangulation<4>::removeAllPentachora() {
    removeAllSimplices();
}

inline size_t Triangulation<4>::countBoundaryComponents() const {
    ensureSkeleton();
    return boundaryComponents_.size();
}

inline const std::vector<Dim4BoundaryComponent*>&
        Triangulation<4>::boundaryComponents() const {
    ensureSkeleton();
    return (const std::vector<Dim4BoundaryComponent*>&)(boundaryComponents_);
}

inline const std::vector<Dim4BoundaryComponent*>&
        Triangulation<4>::getBoundaryComponents() const {
    ensureSkeleton();
    return (const std::vector<Dim4BoundaryComponent*>&)(boundaryComponents_);
}

inline Dim4BoundaryComponent* Triangulation<4>::boundaryComponent(
        size_t index) const {
    ensureSkeleton();
    return boundaryComponents_[index];
}

inline Dim4BoundaryComponent* Triangulation<4>::getBoundaryComponent(
        size_t index) const {
    ensureSkeleton();
    return boundaryComponents_[index];
}

inline long Triangulation<4>::eulerCharTri() const {
    ensureSkeleton();

    // Cast away the unsignedness of std::vector::size().
    return static_cast<long>(countVertices())
        - static_cast<long>(countEdges())
        + static_cast<long>(countTriangles())
        - static_cast<long>(countTetrahedra())
        + static_cast<long>(size());
}

inline bool Triangulation<4>::isIdeal() const {
    ensureSkeleton();
    return ideal_;
}

inline bool Triangulation<4>::isClosed() const {
    ensureSkeleton();
    return boundaryComponents_.empty();
}

inline void Triangulation<4>::simplifiedFundamentalGroup(
        NGroupPresentation* newGroup) {
    fundGroup_ = newGroup;
}

inline const NAbelianGroup& Triangulation<4>::homology() const {
    return homologyH1();
}

inline NPacket* Triangulation<4>::internalClonePacket(NPacket*) const {
    return new Dim4Triangulation(*this);
}

} // namespace regina

#endif

