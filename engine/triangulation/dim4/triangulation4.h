
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

/*! \file triangulation/dim4/triangulation4.h
 *  \brief Internal header for 4-dimensional triangulations.
 *
 *  This file is automatically included from triangulation/dim4.h; there is
 *  no need for end users to include this header explicitly.
 */

#ifndef __TRIANGULATION4_H
#ifndef __DOXYGEN
#define __TRIANGULATION4_H
#endif

#include <list>
#include <memory>
#include <vector>
#include "regina-core.h"
#include "triangulation/generic/triangulation.h"
#include "utilities/markedvector.h"

// NOTE: More #includes for faces, components and boundary components
// follow after the class declarations.

namespace regina {

class XMLPacketReader;

template <int> class XMLTriangulationReader;

/**
 * \addtogroup dim4 4-Manifold Triangulations
 * Details for implementing triangulations of 4-manifolds.
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_TRIANGULATION4> {
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
 * including many functions specific to 4-manifolds.
 *
 * A 4-manifold triangulation is built from pentachora: a \e pentachoron is a
 * 4-dimensional simplex, with five vertices.
 *
 * \headerfile triangulation/dim4.h
 */
template <>
class REGINA_API Triangulation<4> :
        public Packet,
        public detail::TriangulationBase<4> {
    REGINA_PACKET(Triangulation<4>, PACKET_TRIANGULATION4)

    public:
        typedef std::vector<Pentachoron<4>*>::const_iterator
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

    private:
        bool knownSimpleLinks_;
            /**< Is it known that all vertex links are 3-spheres or 3-balls?
                 This may be \c true even if the skeleton has not yet been
                 calculated (thereby allowing us to avoid costly 3-sphere or
                 3-ball recognition when the skeleton is eventually computed).
                 A value of \c false may mean that there are other vertex
                 links, or it may mean that the vertex links have not yet
                 been calculated. */

        bool ideal_;
            /**< Is the triangulation ideal? */

        mutable Property<AbelianGroup, StoreManagedPtr> H2_;
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
         * Creates a copy of the given triangulation.
         * The packet tree structure and packet label are \e not copied.
         *
         * This will clone any computed properties (such as homology,
         * fundamental group, and so on) of the given triangulation also.
         * If you want a "clean" copy that resets all properties to unknown,
         * you can use the two-argument copy constructor instead.
         *
         * @param copy the triangulation to copy.
         */
        Triangulation(const Triangulation& copy);
        /**
         * Creates a new copy of the given triangulation, with the option
         * of whether or not to clone its computed properties also.
         *
         * Regardless of the argument \a cloneProps, if it is known that
         * all vertex links of \a copy are 3-sphere or 3-balls, this
         * knowledge will be copied over to the new triangulation.
         *
         * @param copy the triangulation to copy.
         * @param cloneProps \c true if this should also clone any computed
         * properties of the given triangulation (such as homology,
         * fundamental group, and so on), or \c false if the new triangulation
         * should have all properties marked as unknown.
         */
        Triangulation(const Triangulation& copy, bool cloneProps);
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
        Pentachoron<4>* newPentachoron();
        /**
         * A dimension-specific alias for newSimplex().
         *
         * See newSimplex() for further information.
         */
        Pentachoron<4>* newPentachoron(const std::string& desc);
        /**
         * A dimension-specific alias for removeSimplex().
         *
         * See removeSimplex() for further information.
         */
        void removePentachoron(Pentachoron<4>* tet);
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
         * \name Basic Properties
         */
        /*@{*/

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
         * Vertex<4>::isBoundary() for details.
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
        const AbelianGroup& homologyH2() const;

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
        bool fourTwoMove(Edge<4>* e, bool check = true, bool perform = true);
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
         * <tt>pentachora().back()->triangle(Triangle<4>::triangleNumber[0][1][2])</tt>.
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
        bool threeThreeMove(Triangle<4>* t, bool check = true,
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
         * <tt>pentachora().back()->edge(Edge<4>::edgeNumber[0][1])</tt>.
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
        bool twoFourMove(Tetrahedron<4>* f, bool check = true,
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
        bool oneFiveMove(Pentachoron<4>* p, bool check = true,
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
        bool twoZeroMove(Triangle<4>* t, bool check = true,
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
        bool twoZeroMove(Edge<4>* e, bool check = true, bool perform = true);
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
        bool openBook(Tetrahedron<4>* t,
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
        bool shellBoundary(Pentachoron<4>* p,
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
        bool collapseEdge(Edge<4>* e, bool check = true, bool perform = true);

        /*@}*/
        /**
         * \name Subdivisions and Covers
         */
        /*@{*/

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

        static XMLPacketReader* xmlReader(Packet* parent,
            XMLTreeResolver& resolver);

    protected:
        virtual Packet* internalClonePacket(Packet* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

    private:
        /**
         * Clears any calculated properties, including skeletal data,
         * and declares them all unknown.  This must be called by any
         * internal function that changes the triangulation.
         *
         * In most cases this routine is followed immediately by firing
         * a packet change event.
         */
        void clearAllProperties();
        /**
         * Swaps all calculated properties, including skeletal data,
         * with the given triangulation.  This is called by
         * TriangulationBase::swapContents(), and by nothing else.
         */
        void swapAllProperties(Triangulation<4>& other);

        void calculateSkeleton();

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

    friend class regina::Face<4, 4>;
    friend class regina::detail::SimplexBase<4>;
    friend class regina::detail::TriangulationBase<4>;
    friend class regina::XMLTriangulationReader<4>;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4Triangulation, you should use
 * the templated class name Triangulation<4>.
 */
REGINA_DEPRECATED typedef Triangulation<4> Dim4Triangulation;

// Additional face typedefs that do not have their own headers:

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4TriangleEmbedding, you should
 * use either the new alias TriangleEmbedding<4>, or the full class name
 * FaceEmbedding<4, 2>.
 */
REGINA_DEPRECATED typedef FaceEmbedding<4, 2> Dim4TriangleEmbedding;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4TetrahedronEmbedding, you should
 * use either the new alias TetrahedronEmbedding<4>, or the full class name
 * FaceEmbedding<4, 3>.
 */
REGINA_DEPRECATED typedef FaceEmbedding<4, 3> Dim4TetrahedronEmbedding;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4Triangle, you should use
 * either the new alias Triangle<4>, or the full class name Face<4, 2>.
 */
REGINA_DEPRECATED typedef Face<4, 2> Dim4Triangle;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4Tetrahedron, you should use
 * either the new alias Tetrahedron<4>, or the full class name Face<4, 3>.
 */
REGINA_DEPRECATED typedef Face<4, 3> Dim4Tetrahedron;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/dim4/pentachoron4.h"
#include "triangulation/dim4/edge4.h"
#include "triangulation/dim4/vertex4.h"
#include "triangulation/dim4/component4.h"
#include "triangulation/dim4/boundarycomponent4.h"
namespace regina {

// Inline functions for Triangulation<4>

inline Triangulation<4>::Triangulation() : knownSimpleLinks_(false) {
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

inline Pentachoron<4>* Triangulation<4>::newPentachoron() {
    return newSimplex();
}

inline Pentachoron<4>* Triangulation<4>::newPentachoron(
        const std::string& desc) {
    return newSimplex(desc);
}

inline void Triangulation<4>::removePentachoron(Pentachoron<4>* pent) {
    removeSimplex(pent);
}

inline void Triangulation<4>::removePentachoronAt(size_t index) {
    removeSimplexAt(index);
}

inline void Triangulation<4>::removeAllPentachora() {
    removeAllSimplices();
}

inline bool Triangulation<4>::isIdeal() const {
    ensureSkeleton();
    return ideal_;
}

inline bool Triangulation<4>::isClosed() const {
    ensureSkeleton();
    return boundaryComponents().empty();
}

inline Packet* Triangulation<4>::internalClonePacket(Packet*) const {
    return new Triangulation<4>(*this);
}

} // namespace regina

#endif

