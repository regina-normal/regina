
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file triangulation/dim4/triangulation4.h
 *  \brief Internal header for 4-dimensional triangulations.
 *
 *  This file is automatically included from triangulation/dim4.h; there is
 *  no need for end users to include this header explicitly.
 */

#ifndef __REGINA_TRIANGULATION4_H
#ifndef __DOXYGEN
#define __REGINA_TRIANGULATION4_H
#endif

#include <list>
#include <memory>
#include <vector>
#include "regina-core.h"
#include "hypersurface/normalhypersurface.h"
#include "progress/progresstracker.h"
#include "triangulation/detail/retriangulate.h"
#include "triangulation/generic/triangulation.h"
#include "utilities/markedvector.h"

// NOTE: More #includes for faces, components and boundary components
// follow after the class declarations.

namespace regina {

class IntersectionForm;
class ProgressTracker;
class ProgressTrackerOpen;

template <int> class XMLTriangulationReader;

/**
 * \defgroup dim4 4-Manifold Triangulations
 * Details for implementing triangulations of 4-manifolds.
 */

/**
 * Represents a 4-dimensional triangulation, typically of a 4-manifold.
 *
 * This is a specialisation of the generic Triangulation class template;
 * see the generic Triangulation documentation for a general overview of how
 * the triangulation classes work.  In Python, you can read this generic
 * documentation by looking at a higher dimension: try `help(Triangulation5)`.
 *
 * This 4-dimensional specialisation offers significant extra functionality,
 * including many functions specific to 4-manifolds.
 *
 * A 4-manifold triangulation is built from pentachora: a _pentachoron_ is a
 * 4-dimensional simplex, with five vertices.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \headerfile triangulation/dim4.h
 *
 * \ingroup dim4
 */
template <>
class Triangulation<4> : public detail::TriangulationBase<4> {
    private:
        /**
         * Represents different contexts in which a member function might try
         * to simplify a triangulation.
         */
        enum class SimplifyContext {
            /**
             * Indicates that we want to use all available techniques to
             * simplify the triangulation as much as possible.
             */
            Best,
            /**
             * Indicates that we are within a "down" sequence of
             * simplifyUpDown().  Only 2-0 edge moves, 2-0 triangle moves and
             * 3-3 moves will be considered.
             */
            UpDownDescent
        };

        long vertexLinkSummary_ { -1 };
            /**< This is a computed property that caches information about the
                 vertex links, and is optimised for valid triangulations.
                 If \a vertexLinkSummary_ is non-negative, this means that
                 every vertex is valid (i.e., each vertex link is either
                 a 3-ball or a closed 3-manifold); moreover, the numeric value
                 of \a vertexLinkSummary_ indicates how many vertices are ideal
                 (i.e., have links that are closed but not 3-spheres).
                 If \a vertexLinkSummary_ is negative, this means that either
                 one or more vertices are invalid, and/or the skeleton has
                 not yet been computed.
                 Crucially, this property may be known _before_ the skeleton
                 is computed (thereby allowing us to avoid costly 3-sphere or
                 3-ball recognition when the skeleton is computed later on). */

        /**
         * A struct that holds all of our calculated properties.
         * This is a convenience so we can use its implicitly defined
         * assignment operators and copy constructors.  It is mutable so that
         * expensive read-only calculations can cache their results.
         *
         * All std::optional properties are std::nullopt if they have
         * not yet been computed.
         */
        mutable struct {
            std::optional<AbelianGroup> H2_;
                /**< Second homology group of the triangulation. */
        } prop_;

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
        Triangulation() = default;
        /**
         * Creates a new copy of the given triangulation.
         *
         * This will also clone any computed properties (such as homology,
         * fundamental group, and so on), as well as the skeleton (vertices,
         * edges, components, etc.).  In particular, the same numbering and
         * labelling will be used for all skeletal objects.
         *
         * If \a src has any locks on top-dimensional simplices and/or their
         * facets, these locks will also be copied across.
         *
         * If you want a "clean" copy that resets all properties to unknown,
         * you can use the two-argument copy constructor instead.
         *
         * \param src the triangulation to copy.
         */
        Triangulation(const Triangulation& src) = default;
        /**
         * Creates a new copy of the given triangulation, with the option
         * of whether or not to clone its computed properties and/or locks also.
         *
         * If \a cloneProps is \c true, then this constructor will also clone
         * any computed properties (such as homology, fundamental group, and
         * so on).  If \a cloneProps is \c false, then these properties
         * will be marked as unknown in the new triangulation, and will be
         * recomputed on demand if/when they are required.
         *
         * Regardless of \a cloneProps, the skeleton (vertices, edges,
         * components, etc.) will _always_ be cloned.  This is to ensure that
         * the same numbering and labelling will be used for all skeletal
         * objects in both triangulations.
         *
         * Likewise, regardless of \a cloneProps, if it is known that
         * all vertex links of \a copy are 3-sphere or 3-balls, this
         * knowledge will be copied over to the new triangulation.
         *
         * If \a cloneLocks is \c true then any locks on the top-dimensional
         * simplices and/or facets of \a src will be copied across.
         * If \a cloneLocks is \c false then the new triangulation will have
         * no locks at all.
         *
         * \param src the triangulation to copy.
         * \param cloneProps \c true if this should also clone any computed
         * properties of the given triangulation, or \c false if the new
         * triangulation should have such properties marked as unknown.
         * \param cloneLocks \c true if this should also clone any simplex
         * and/or facet locks from the given triangulation, or \c false if
         * the new triangulation should have no locks at all.
         */
        Triangulation(const Triangulation& src, bool cloneProps,
            bool cloneLocks = true);
        /**
         * Moves the given triangulation into this new triangulation.
         *
         * This is much faster than the copy constructor, but is still linear
         * time.  This is because every pentachoron must be adjusted to point
         * back to this new triangulation instead of \a src.
         *
         * All pentachora and skeletal objects (faces, components and
         * boundary components) that belong to \a src will be moved into
         * this triangulation, and so any pointers or references to
         * Pentachoron<4>, Face<4, subdim>, Component<4> or
         * BoundaryComponent<4> objects will remain valid.  Likewise, all
         * cached properties will be moved into this triangulation.
         *
         * If \a src has any locks on top-dimensional simplices and/or their
         * facets, these locks will also be moved across.
         *
         * The triangulation that is passed (\a src) will no longer be usable.
         *
         * \note This operator is marked \c noexcept, and in particular
         * does not fire any change events.  This is because this triangulation
         * is freshly constructed (and therefore has no listeners yet), and
         * because we assume that \a src is about to be destroyed (an action
         * that _will_ fire a packet destruction event).
         *
         * \param src the triangulation to move.
         */
        Triangulation(Triangulation&& src) noexcept = default;
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
         * \exception InvalidArgument Regina could not interpret the given
         * string as representing a triangulation using any of the supported
         * string types.
         *
         * \param description a string that describes a 4-manifold
         * triangulation.
         */
        Triangulation(const std::string& description);
        /**
         * Destroys this triangulation.
         *
         * The constituent pentachora, the cellular structure and all other
         * properties will also be deallocated.
         */
        ~Triangulation();

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
         * A dimension-specific alias for newSimplices().
         *
         * See newSimplices() for further information.
         */
        template <int k>
        std::array<Pentachoron<4>*, k> newPentachora();
        /**
         * A dimension-specific alias for newSimplices().
         *
         * See newSimplices() for further information.
         */
        void newPentachora(size_t k);
        /**
         * A dimension-specific alias for removeSimplex().
         *
         * See removeSimplex() for further information.
         *
         * \exception LockViolation The given pentachoron and/or one of its
         * facets is currently locked.  This exception will be thrown
         * before any changes are made.  See Simplex<4>::lock() and
         * Simplex<4>::lockFacet() for further details on how such locks
         * work and what their implications are.
         *
         * \param pent the pentachoron to remove.
         */
        void removePentachoron(Pentachoron<4>* pent);
        /**
         * A dimension-specific alias for removeSimplexAt().
         *
         * See removeSimplexAt() for further information.
         *
         * \exception LockViolation The requested pentachoron and/or one of its
         * facets is currently locked.  This exception will be thrown
         * before any changes are made.  See Simplex<4>::lock() and
         * Simplex<4>::lockFacet() for further details on how such locks
         * work and what their implications are.
         *
         * \param index specifies which pentachoron to remove; this
         * must be between 0 and size()-1 inclusive.
         */
        void removePentachoronAt(size_t index);
        /**
         * A dimension-specific alias for removeAllSimplices().
         *
         * See removeAllSimplices() for further information.
         *
         * \exception LockViolation This triangulation contains at least one
         * locked pentachoron and/or facet.  This exception will be
         * thrown before any changes are made.  See Simplex<4>::lock() and
         * Simplex<4>::lockFacet() for further details on how such locks
         * work and what their implications are.
         */
        void removeAllPentachora();

        /**
         * Sets this to be a (deep) copy of the given triangulation.
         *
         * This will also clone any computed properties (such as homology,
         * fundamental group, and so on), as well as the skeleton (vertices,
         * edges, components, etc.).  In particular, this triangulation
         * will use the same numbering and labelling for all skeletal objects
         * as in the source triangulation.
         *
         * If \a src has any locks on top-dimensional simplices and/or their
         * facets, these locks will also be copied across.
         *
         * \param src the triangulation to copy.
         * \return a reference to this triangulation.
         */
        Triangulation& operator = (const Triangulation& src);

        /**
         * Moves the contents of the given triangulation into this
         * triangulation.
         *
         * This is much faster than copy assignment, but is still linear
         * time.  This is because every pentachoron must be adjusted to point
         * back to this triangulation instead of \a src.
         *
         * All pentachora and skeletal objects (faces, components and
         * boundary components) that belong to \a src will be moved into
         * this triangulation, and so any pointers or references to
         * Pentachoron<4>, Face<4, subdim>, Component<4> or
         * BoundaryComponent<4> objects will remain valid.  Likewise, all
         * cached properties will be moved into this triangulation.
         *
         * If \a src has any locks on top-dimensional simplices and/or their
         * facets, these locks will also be moved across.
         *
         * The triangulation that is passed (\a src) will no longer be usable.
         *
         * \note This operator is _not_ marked \c noexcept, since it fires
         * change events on this triangulation which may in turn call arbitrary
         * code via any registered packet listeners.  It deliberately does
         * _not_ fire change events on \a src, since it assumes that \a src is
         * about to be destroyed (which will fire a destruction event instead).
         *
         * \param src the triangulation to move.
         * \return a reference to this triangulation.
         */
        Triangulation& operator = (Triangulation&& src);

        /**
         * Swaps the contents of this and the given triangulation.
         *
         * All pentachora that belong to this triangulation
         * will be moved to \a other, and all pentachora
         * that belong to \a other will be moved to this triangulation.
         * Likewise, all skeletal objects (such as lower-dimensional faces,
         * components, and boundary components) and all cached properties
         * (such as homology and fundamental group) will be swapped.
         *
         * In particular, any pointers or references to Pentachoron<4> and/or
         * Face<4, subdim> objects will remain valid.
         *
         * This routine will behave correctly if \a other is in fact
         * this triangulation.
         *
         * \note This swap function is _not_ marked \c noexcept, since it
         * fires change events on both triangulations which may in turn call
         * arbitrary code via any registered packet listeners.
         *
         * \param other the triangulation whose contents should be
         * swapped with this.
         */
        void swap(Triangulation<4>& other);

        /*@}*/
        /**
         * \name Skeletal Queries
         */
        /*@{*/

        /**
         * A dimension-specific alias for hasBoundaryFacets().
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryTetrahedra() const;

        /**
         * A dimension-specific alias for countBoundaryFacets().
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryTetrahedra() const;

        /*@}*/
        /**
         * \name Basic Properties
         */
        /*@{*/

        /**
         * Returns the Euler characteristic of the corresponding compact
         * manifold.
         *
         * Instead of simply calculating `V-E+F-T+P`, this routine also
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
         * This routine does _not_ yet handle invalid triangulations
         * correctly.  For this reason, this routine currently insists on
         * a valid triangulation as a precondition.
         *
         * \pre This triangulation is valid.
         *
         * \exception FailedPrecondition This triangulation is not valid.
         *
         * \return the Euler characteristic of the corresponding compact
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
         * \return \c true if and only if this triangulation is ideal.
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
         * \return \c true if and only if this triangulation is closed.
         */
        bool isClosed() const;

        /*@}*/
        /**
         * \name Algebraic Properties
         */
        /*@{*/

        /**
         * Returns the intersection form of this 4-manifold.
         *
         * The intersection form is stored as a square matrix with respect to
         * an arbitrary basis of the second homology group; you can access this
         * matrix via IntersectionForm::matrix().  This matrix is guaranteed
         * to be symmetric and unimodular, but will not be normalised in any
         * way.  You can, however, query invariants of the intersection form
         * via routines such as IntersectionForm::signature() and
         * IntersectionForm::even(), which in the simply connected case are
         * enough to determine the topology of the underlying 4-manifold.
         *
         * (As an implementation detail, the basis is currently chosen to be
         * the one produced by constructing a MarkedAbelianGroup using the
         * boundary maps on the dual faces.  This specific choice of basis is
         * subject to change in future releases of Regina.)
         *
         * The sign convention for counting intersections is as follows: in a
         * pentachoron with positive orientation and vertices (0,1,2,3,4),
         * the triangles with ordered vertices (0,1,2) and ordered vertices
         * (2,3,4) have positive intersection.
         *
         * \pre This triangulation is valid, non-empty, orientable and closed.
         *
         * \exception FailedPrecondition This triangulation is invalid,
         * empty, non-orientable, or not closed.
         *
         * \return the intersection form of this 4-manifold.
         */
        IntersectionForm intersectionForm() const;

        /*@}*/
        /**
         * \name Skeletal Transformations
         */
        /*@{*/

        /**
         * Attempts to simplify this triangulation as intelligently as possible
         * using fast and greedy heuristics.  Specifically, this routine will
         * attempt to reduce the number of pentachora in the triangulation.
         *
         * Currently this routine uses simplifyToLocalMinimum() in
         * combination with random 3-3 moves and book opening moves.
         *
         * If simplify() fails to improve the triangulation (which in four
         * dimensions is likely), you may instead wish to try the well-climbing
         * routine simplifyUpDown(), or the powerful but *much* slower
         * simplifyExhaustive().
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * If any pentachora and/or tetrahedra are locked, these locks will be
         * respected: that is, the simplification operation will avoid any
         * moves that would violate these locks (and in particular, no
         * LockViolation exceptions should be thrown).  Of course, however,
         * having locks may make the simplification less effective in reducing
         * the number of pentachora.
         *
         * \warning The specific behaviour of this routine will almost
         * certainly change between releases.  At present,
         * simplification for 4-manifold triangulations is extremely
         * weak (as opposed to 3-manifolds, where a rich library of
         * simplification techinques is available to call upon).
         *
         * \note For long-term users of Regina: this is the routine that was
         * for a long time called intelligentSimplify().  It was renamed to
         * simplify() in Regina 7.4.
         *
         * \return \c true if and only if the triangulation was changed.
         */
        bool simplify();
        /**
         * Deprecated alias for simplify(), which attempts to simplify this
         * triangulation as intelligently as possible using fast and greedy
         * heuristics.
         *
         * \deprecated This routine has been renamed to simplify().
         * See simplify() for further details.
         *
         * \return \c true if and only if the triangulation was changed.
         */
        [[deprecated]] bool intelligentSimplify();
        /**
         * Uses all known simplification moves to reduce the triangulation
         * monotonically to some local minimum number of pentachora.
         *
         * End users will probably not want to call this routine.
         * You should call simplify() if you want a fast
         * method of simplifying a triangulation.
         *
         * The moves used by this routine include collapsing edges, 4-2 moves,
         * and boundary shelling moves.
         *
         * Moves that do not reduce the number of pentachora
         * (such as 3-3 moves or book opening moves) are not used in this
         * routine.  Such moves do however feature in simplify().
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * If any pentachora and/or tetrahedra are locked, these locks will be
         * respected: that is, the simplification operation will avoid any
         * moves that would violate these locks (and in particular, no
         * LockViolation exceptions should be thrown).  Of course, however,
         * having locks may make the simplification less effective in reducing
         * the number of pentachora.
         *
         * \warning The specific behaviour of this routine will almost
         * certainly change between releases.  At present,
         * simplification for 4-manifold triangulations is extremely
         * weak (as opposed to 3-manifolds, where a rich library of
         * simplification techinques is available to call upon).
         *
         * \param perform \c true if we are to perform the simplifications,
         * or \c false if we are only to investigate whether simplifications
         * are possible (defaults to \c true).
         * \return if \a perform is \c true, this routine returns \c true
         * if and only if the triangulation was changed to reduce the
         * number of pentachora; if \a perform is \c false, this routine
         * returns \c true if and only if it determines that it is
         * capable of performing such a change.
         */
        bool simplifyToLocalMinimum(bool perform = true);

        /**
         * Attempts to simplify this triangulation by making increasingly long
         * sequences of 2-4 moves and then attempting to simplify back down.
         *
         * This routine will _only_ perform 2-4 moves, 2-0 edge moves,
         * 2-0 triangle moves, and 3-3 moves.
         *
         * The main purpose of this routine is to offer a "well-climbing"
         * technique that explores more widely than simplify(),
         * but that is not nearly as slow as simplifyExhaustive().
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * If any pentachora and/or tetrahedra are locked, these locks will be
         * respected: that is, this routine will avoid any moves that would
         * violate these locks (and in particular, no LockViolation exceptions
         * should be thrown).  Of course, however, having locks may make the
         * simplification less effective in reducing the number of pentachora.
         *
         * \warning The specific behaviour of this routine will almost
         * certainly change between releases.
         *
         * \param max24 the maximum number of consecutive 2-4 moves to perform
         * in a single "up" sequence.  Note that this routine will attempt
         * several "up" sequences of differing lengths, and in particular
         * may eventually pass through triangulations with more than
         * `size() + max24` pentachora.  If this is -1, then a sensible
         * default will be chosen.
         * \param max33 the maximum number of consecutive 3-3 moves to perform
         * immediately after each "up" sequence.  If this is -1, then a sensible
         * default will be chosen.
         * \param alwaysModify \c true if this triangulation should be modified
         * after this operation, even if the final endpoint has _more_
         * pentachora than the triangulation began with, or \c false if this
         * triangulation should only be modified if the total number of
         * pentachora was strictly reduced.
         * \return \c true if and only if the number of pentachora was
         * strictly reduced.
         *
         * \author Rhuaidi Burke
         */
        bool simplifyUpDown(ssize_t max24 = -1, ssize_t max33 = -1,
            bool alwaysModify = false);

        /**
         * Attempts to simplify this triangulation using a slow but
         * exhaustive search through the Pachner graph.  This routine is
         * more powerful but much slower than simplify().
         *
         * Specifically, this routine will iterate through all
         * triangulations that can be reached from this triangulation via
         * 1-5, 2-4, 3-3 and 4-2 Pachner moves, without ever exceeding
         * \a height additional pentachora beyond the original number,
         * and without violating any simplex and/or facet locks.
         * Note that 5-1 moves are currently not supported, though this
         * may be added in a future verson of Regina.
         *
         * If at any stage it finds a triangulation with _fewer_
         * pentachora than the original, then this routine will call
         * simplify() to shrink the triangulation further if
         * possible and will then return \c true.  If it cannot find a
         * triangulation with fewer pentachora then it will leave this
         * triangulation unchanged and return \c false.
         *
         * This routine can be very slow and very memory-intensive: the
         * number of triangulations it visits may be superexponential in
         * the number of pentachora, and it records every triangulation
         * that it visits (so as to avoid revisiting the same triangulation
         * again).  It is highly recommended that you begin with \a height = 1,
         * and if this fails then try increasing \a height one at a time until
         * either you find a simplification or the routine becomes
         * too expensive to run.
         *
         * If \a height is negative, then there will be _no_ bound on the
         * number of additional pentachora.  This means that the routine will
         * not terminate until a simpler triangulation is found (unless there
         * are so many locks that the number of reachable triangulations is
         * finite).  This means that, if no simpler triangulation exists, the
         * only way to terminate this function is to cancel the operation via
         * a progress tracker (read on for details).
         *
         * If any pentachora and/or tetrahedra are locked, these locks will be
         * respected: that is, the retriangulation will avoid any moves that
         * would violate these locks (and in particular, no LockViolation
         * exceptions should be thrown).  Of course, however, having locks may
         * reduce the number of distinct triangulations that can be reached.
         *
         * If you want a _fast_ simplification routine, you should call
         * simplify() instead.  The benefit of simplifyExhaustive()
         * is that, for very stubborn triangulations where simplify()
         * finds itself stuck at a local minimum, simplifyExhaustive() is able
         * to "climb out" of such wells.
         *
         * Since Regina 7.0, this routine will not return until either the
         * triangulation is simplified or the exhaustive search is complete,
         * regardless of whether a progress tracker was passed.  If you
         * need the old behaviour (where passing a progress tracker caused
         * the exhaustive search to start in the background), simply call
         * this routine in a new detached thread.
         *
         * To assist with performance, this routine can run in parallel
         * (multithreaded) mode; simply pass the number of parallel threads
         * in the argument \a threads.  Even in multithreaded mode, this
         * routine will not return until processing has finished (i.e., either
         * the triangulation was simplified or the search was exhausted), and
         * any change to this triangulation will happen in the calling thread.
         *
         * If this routine is unable to simplify the triangulation, then
         * the triangulation will not be changed.
         *
         * \pre This triangulation is connected.
         *
         * \exception FailedPrecondition This triangulation has more
         * than one connected component.  If a progress tracker was passed,
         * it will be marked as finished before the exception is thrown.
         *
         * \python The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * \param height the maximum number of _additional_ pentachora to
         * allow beyond the number of pentachora originally present in the
         * triangulation, or a negative number if this should not be bounded.
         * \param threads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * \return \c true if and only if the triangulation was successfully
         * simplified to fewer pentachora.
         */
        bool simplifyExhaustive(int height = 1, int threads = 1,
            ProgressTrackerOpen* tracker = nullptr);

        /**
         * Explores all triangulations that can be reached from this via
         * Pachner moves, without exceeding a given number of additional
         * pentachora.
         *
         * Specifically, this routine will iterate through all
         * triangulations that can be reached from this triangulation via
         * 1-5, 2-4, 3-3 and 4-2 Pachner moves, without ever exceeding
         * \a height additional pentachora beyond the original number,
         * and without violating any simplex and/or facet locks.
         * Note that 5-1 moves are currently not supported, though this
         * may be added in a future verson of Regina.
         *
         * For every such triangulation (including this starting
         * triangulation), this routine will call \a action (which must
         * be a function or some other callable object).
         *
         * - \a action must take the following initial argument(s).
         *   Either (a) the first argument must be a triangulation (the precise
         *   type is discussed below), representing the triangluation that has
         *   been found; or else (b) the first two arguments must be of types
         *   const std::string& followed by a triangulation, representing both
         *   the triangulation and its isomorphism signature.
         *   The second form is offered in order to avoid unnecessary
         *   recomputation within the \a action function; however, note that
         *   the signature might not be of the IsoSigClassic type (i.e., it
         *   might not match the output from the default version of isoSig()).
         *   If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - The triangulation argument will be passed as an rvalue; a typical
         *   action could (for example) take it by const reference and query it,
         *   or take it by value and modify it, or take it by rvalue reference
         *   and move it into more permanent storage.
         *
         * - \a action must return a boolean.  If \a action ever returns
         *   \c true, then this indicates that processing should stop
         *   immediately (i.e., no more triangulations will be processed).
         *
         * - \a action may, if it chooses, make changes to this triangulation
         *   (i.e., the original triangulation upon which retriangulate()
         *   was called).  This will not affect the search: all triangulations
         *   that this routine visits will be obtained via Pachner moves
         *   from the original form of this triangulation, before any
         *   subsequent changes (if any) were made.
         *
         * - \a action will only be called once for each triangulation
         *   (including this starting triangulation).  In other words, no
         *   triangulation will be revisited a second time in a single call
         *   to retriangulate().
         *
         * This routine can be very slow and very memory-intensive, since the
         * number of triangulations it visits may be superexponential in
         * the number of tetrahedra, and it records every triangulation
         * that it visits (so as to avoid revisiting the same triangulation
         * again).  It is highly recommended that you begin with \a height = 1,
         * and if necessary try increasing \a height one at a time until
         * this routine becomes too expensive to run.
         *
         * If \a height is negative, then there will be _no_ bound on the
         * number of additional pentachora.  This means that the routine will
         * _never terminate_, unless \a action returns \c true for some
         * triangulation that is passed to it (or unless there are so many
         * locks that the number of reachable triangulations becomes finite).
         *
         * If any pentachora and/or tetrahedra are locked, these locks will be
         * respected: that is, the retriangulation will avoid any moves that
         * would violate these locks (and in particular, no LockViolation
         * exceptions should be thrown).  Of course, however, having locks may
         * reduce the number of distinct triangulations that can be reached.
         *
         * Since Regina 7.0, this routine will not return until the exploration
         * of triangulations is complete, regardless of whether a progress
         * tracker was passed.  If you need the old behaviour (where passing a
         * progress tracker caused the enumeration to start in the background),
         * simply call this routine in a new detached thread.
         *
         * To assist with performance, this routine can run in parallel
         * (multithreaded) mode; simply pass the number of parallel threads in
         * the argument \a threads.  Even in multithreaded mode, this routine
         * will not return until processing has finished (i.e., either \a action
         * returned \c true, or the search was exhausted).  All calls to
         * \a action will be protected by a mutex (i.e., different threads will
         * never be calling \a action at the same time); as a corollary, the
         * action should avoid expensive operations where possible (otherwise
         * it will become a serialisation bottleneck in the multithreading).
         *
         * If this triangulation is not connected, then this routine will do
         * nothing: it will immediately return \c false, and if a progress
         * tracker was passed then it will be immediately marked as finished.
         *
         * \pre This triangulation is connected.
         *
         * \exception FailedPrecondition This triangulation has more
         * than one connected component.  If a progress tracker was passed,
         * it will be marked as finished before the exception is thrown.
         *
         * \apinotfinal
         *
         * \python This function is available in Python, and the
         * \a action argument may be a pure Python function.  However, its
         * form is more restricted: the arguments \a tracker and \a args are
         * removed, so you call it as retriangulate(height, threads, action).
         * Moreover, \a action must take exactly two arguments
         * (const std::string&, Triangulation<4>&&) representing a signature
         * and the triangulation, as described in option (b) above.
         *
         * \param height the maximum number of _additional_ pentachora to
         * allow beyond the number of pentachora originally present in the
         * triangulation, or a negative number if this should not be bounded.
         * \param threads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * \param action a function (or other callable object) to call
         * for each triangulation that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial triangulation argument(s).
         * \return \c true if some call to \a action returned \c true (thereby
         * terminating the search early), or \c false if the search ran to
         * completion.
         */
        template <typename Action, typename... Args>
        bool retriangulate(int height, int threads,
            ProgressTrackerOpen* tracker,
            Action&& action, Args&&... args) const;

        /**
         * If possible, performs a 4-4 move about the given edge of this
         * triangulation.  This involves replacing the four pentachora joined
         * along that edge with four new pentachora joined along a different
         * (and new) edge instead.
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as discussed below), _and_ it will not violate any
         * simplex and/or facet locks.  See Simplex<4>::lock() and
         * Simplex<4>::lockFacet() for further details on locks.
         *
         * For this move to make sense, the initial configuration of four
         * pentachora around the given edge \a e should be equivalent to the
         * join of a double edge and a square with diagonal given by the
         * edge \a e. The 4-4 move then essentially performs a 2-2 move on this
         * square, with the new diagonal of the square corresponding precisely
         * to the new edge of this triangulation that will join the four new
         * pentachora.
         *
         * Therefore, to be able to perform this move (and to do so without
         * changing the topology), we require that:
         *
         * - the link of the given edge is a 2-2 move away from being
         *   combinatorially isomorphic to the boundary of a tetrahedron; and
         *
         * - the four pentachora joined along the given edge are distinct.
         *
         * If this triangulation is currently oriented, then this 4-4 move
         * will label the new pentachora in a way that preserves the
         * orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \return \c true if and only if the requested move was able to be
         * performed.
         *
         * \author Alex He
         */
        bool move44(Edge<4>* e);
        /**
         * If possible, performs a book opening move about the given
         * tetrahedron.  This involves taking a tetrahedron that meets the
         * boundary along precisely one, two or three triangles, and ungluing
         * it to create two new boundary facets.  The resulting effect is to
         * expose the pentachora it initially joined, and hopefully open the
         * way for subsequent boundary shelling moves.
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as discussed below), _and_ it will not violate any
         * facet locks.  See Simplex<4>::lockFacet() for further details on
         * facet locks.  Note that simplex locks are never a concern for this
         * type of move.
         *
         * In order not to change the topology, we impose the following
         * requirements:
         *
         * - all vertices, edges and triangles of the given tetrahedron are
         *   valid;
         *
         * - the given tetrahedron meets the boundary in precisely one, two or
         *   three triangles (and therefore also joins two pentachora);
         *
         * - if the given tetrahedron meets the boundary in precisely one
         *   triangle, then the remaining vertex of the tetrahedron is
         *   non-boundary, and no two of the remaining three edges of the
         *   tetrahedron are identified;
         *
         * - if the given tetrahedron meets the boundary in precisely two
         *   triangles, then the remaining edge of the tetrahedron is
         *   non-boundary, and the remaining two triangles of the tetrahedron
         *   are not identified.
         *
         * These requirements are stronger than they need to be, but the
         * resulting "lost opportunities" only affect invalid triangulations.
         *
         * If this triangulation is currently oriented, then this operation
         * will (trivially) preserve the orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a t)
         * can no longer be used.
         *
         * \pre The given tetrahedron is a tetrahedron of this triangulation.
         *
         * \param t the tetrahedron about which to perform the move.
         * \return \c true if and only if the requested move was able to be
         * performed.
         */
        bool openBook(Tetrahedron<4>* t);

        /**
         * If possible, performs an edge collapse move upon the given edge.
         * This involves collapsing the edge to a point, merging its two
         * endpoints together, and flattening all of the pentachora that
         * contain it.  The resulting effect is to reduce the number of
         * vertices in this triangulation by one.
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as discussed below), _and_ it will not violate any
         * simplex and/or facet locks.  See Simplex<4>::lock() and
         * Simplex<4>::lockFacet() for further details on locks.
         *
         * The requirements for this move to not change the topology are
         * complex, and are discussed in detail in the collapseEdge() source
         * code for those who are interested.  The most important requirement
         * is that the given edge should join two distinct vertices.  It is
         * also important to note that checking the full requirements is
         * expensive (amongst other things, we need to build two union-find
         * structures to implement the test).
         *
         * If you are trying to reduce the number of vertices without changing
         * the topology, and if \a e is an edge connecting an internal vertex
         * with some different vertex, then either collapseEdge() or snapEdge()
         * may be more appropriate for your situation.
         *
         * - The advantage of collapseEdge() is that it decreases the number
         *   of tetrahedra, whereas snapEdge() increases this number (but only
         *   by four).
         *
         * - The disadvantages of collapseEdge() are that it cannot always be
         *   performed, and its validity tests are expensive; snapEdge() on
         *   the other hand can always be used for edges \a e of the type
         *   described above.
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge to collapse.
         * \return \c true if and only if the requested move was able to be
         * performed.
         */
        bool collapseEdge(Edge<4>* e);

        /**
         * If possible, performs an edge snap move about the given edge.
         * This involves snapping together the endpoints of the edge, and
         * thereby reducing the number of vertices in this triangulation by
         * one (but at the cost of adding a few extra pentachora).  See below
         * for more details on the mechanics of how this move actually operates.
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as outlined below).  Simplex and/or facet locks are
         * never a concern for this type of move; again see below for details.
         *
         * In order to not change the toplogy, we require that:
         *
         * - the given edge joins two distinct vertices of the triangulation;
         *
         * - at least one of these two vertices is internal.
         *
         * Depending on your situation, collapseEdge() may be a more
         * appropriate method for reducing the number of vertices without
         * changing the topology.
         *
         * - The advantage of collapseEdge() is that it decreases the number
         *   of tetrahedra, whereas snapEdge() increases this number (but only
         *   by four).
         *
         * - The disadvantages of collapseEdge() are that it cannot always be
         *   performed, and its validity tests are expensive; snapEdge() on
         *   the other hand can always be used for edges \a e of the type
         *   described above.
         *
         * This operation essentially works by taking a triangle \a f that
         * meets the given edge \a e, and folding the other two edges of \a f
         * together about their common vertex.  To _implement_ this, we pry
         * open a tetrahedron \a t and insert a four-pentachoron gadget \a g
         * within the resulting tetrahedral pillow.  In particular, this means
         * that simplex and/or facet locks will never prevent this operation
         * from taking place: if the tetrahedron \a t happens to be locked,
         * then this lock will simply move across to one of the two tetrahedra
         * bounding the gadget \a g.
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge whose endpoints are to be snapped together.
         * \return \c true if and only if the requested move was able to be
         * performed.
         *
         * \author Alex He
         */
        bool snapEdge(Edge<4>* e);

        /**
         * Determines whether it is possible to perform a 4-4 move about the
         * given edge of this triangulation, without violating any simplex
         * and/or facet locks.
         *
         * For more detail on 4-4 moves and when they can be performed,
         * see move44().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the candidate edge about which to perform the move.
         * \return \c true if and only if the requested move can be performed.
         */
        bool has44(Edge<4>* e) const;
        /**
         * Determines whether it is possible to perform a book opening move
         * about the given tetrahedron of this triangulation, without
         * violating any facet locks.
         *
         * For more detail on book opening moves and when they can be
         * performed, see openBook().
         *
         * \pre The given tetrahedron is a tetrahedron of this triangulation.
         *
         * \param t the candidate tetrahedron about which to perform the move.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasOpenBook(Tetrahedron<4>* t) const;
        /**
         * Determines whether it is possible to collapse the given edge of
         * this triangulation, without violating any simplex and/or facet locks.
         *
         * For more detail on edge collapse moves and when they can be
         * performed, see collapseEdge().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the candidate edge to collapse.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasCollapseEdge(Edge<4>* e) const;
        /**
         * Determines whether it is possible to snap together the endpoints of
         * the given edge of this triangulation.
         *
         * For more detail on snapping moves on edges and when they can be
         * performed, see snapEdge().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the candidate edge whose endpoints would be snapped
         * together.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasSnapEdge(Edge<4>* e) const;

        /**
         * If possible, returns the triangulation obtained by performing a
         * 4-4 move about the given edge of this triangulation.
         * If such a move is not allowed, or if such a move would violate any
         * simplex and/or facet locks, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on 4-4 moves and when they can be performed,
         * see move44().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Triangulation<4>> with44(Edge<4>* e) const;
        /**
         * If possible, returns the triangulation obtained by performing a
         * book opening move about the given tetrahedron of this triangulation.
         * If such a move is not allowed, or if such a move would violate any
         * facet locks, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on book opening moves and when they can be
         * performed, see openBook().
         *
         * \pre The given tetrahedron is a tetrahedron of this triangulation.
         *
         * \param t the tetrahedron about which to perform the move.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Triangulation<4>> withOpenBook(Tetrahedron<4>* t) const;
        /**
         * If possible, returns the triangulation obtained by collapsing the
         * given edge of this triangulation.
         * If such a move is not allowed, or if such a move would violate any
         * simplex and/or facet locks, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on edge collapse moves and when they can be
         * performed, see collapseEdge().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge to collapse.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Triangulation<4>> withCollapseEdge(Edge<4>* e) const;
        /**
         * If possible, returns the triangulation obtained by snapping together
         * the endpoints of the given edge of this triangulation.
         * If such a move is not allowed, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on snapping moves on edges and when they can be
         * performed, see snapEdge().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge whose endpoints are to be snapped together.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Triangulation<4>> withSnapEdge(Edge<4>* e) const;

        /**
         * Deprecated routine that tests for and optionally performes a
         * 4-4 move about the given edge of this triangulation.
         *
         * For more details on 4-4 moves and when they can be performed,
         * see move44().
         *
         * This routine will always _check_ whether the requested move is
         * legal and will not violate any simplex and/or facet locks (see
         * Simplex<4>::lock() and Simplex<4>::lockFacet() for further details
         * on locks).  If the move _is_ allowed, and if the argument
         * \a perform is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call has44().  If you wish to both check and perform the move, call
         * move44().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         *
         * \author Alex He
         */
        [[deprecated]] bool fourFourMove(Edge<4>* e, bool ignored,
            bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a
         * book opening move about the given tetrahedron of this triangulation.
         *
         * For more details on book opening moves and when they can be
         * performed, see the variant of openBook() without the extra
         * boolean arguments.
         *
         * This routine will always _check_ whether the requested move is legal
         * and will not violate any facet locks (see Simplex<4>::lockFacet()
         * for further details on facet locks).  Note that this type of move can
         * never violate a simplex lock, and so there is no need to check for
         * those at all.  If the move _is_ allowed, and if the argument
         * \a perform is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call hasOpenBook().  If you wish to both check and perform the move,
         * call openBook() without the two extra boolean arguments.
         *
         * \pre The given tetrahedron is a tetrahedron of this triangulation.
         *
         * \param t the tetrahedron about which to perform the move.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool openBook(Tetrahedron<4>* t, bool ignored,
            bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs an
         * edge collapse move upon the given edge of this triangulation.
         *
         * For more details on edge collapse moves and when they can be
         * performed, as well as the difference between edge collapse and
         * edge snap moves, see the variant of collapseEdge() without the
         * extra boolean arguments.
         *
         * This routine will always _check_ whether the requested move is
         * legal and will not violate any simplex and/or facet locks (see
         * Simplex<4>::lock() and Simplex<4>::lockFacet() for further details
         * on locks).  If the move _is_ allowed, and if the argument
         * \a perform is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call hasCollapseEdge().  If you wish to both check and perform the
         * move, call collapseEdge() without the two extra boolean arguments.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge to collapse.
         : \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool collapseEdge(Edge<4>* e, bool ignored,
            bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs an
         * edge snap move upon the given edge of this triangulation.
         *
         * For more details on edge snap moves and when they can be performed,
         * as well as the difference between edge snap and edge collapse moves,
         * see the variant of snapEdge() without the extra boolean arguments.
         *
         * This routine will always _check_ whether the requested move is
         * legal.  Note that this type of move can never violate a simplex or
         * facet lock, and so there is no need to check for those at all.
         * If the move _is_ allowed, and if the argument \a perform
         * is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call hasSnapEdge().  If you wish to both check and perform the move,
         * call snapEdge() without the two extra boolean arguments.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         *
         * \author Alex He
         */
        [[deprecated]] bool snapEdge(Edge<4>* e, bool ignored,
            bool perform = true);

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
         * \exception LockViolation This triangulation contains at least one
         * locked top-dimensional simplex and/or facet.  (This 4-dimensional
         * algorithm does not necessarily subdivide _every_ pentachoron,
         * and so this test is stronger than necessary; however, it will be
         * enforced.)  This exception will be thrown before any changes are
         * made.  See Simplex<4>::lock() and Simplex<4>::lockFacet() for
         * further details on how such locks work and what their implications
         * are.
         *
         * \return \c true if and only if the triangulation was changed.
         */
        bool idealToFinite();

        /*@}*/
        /**
         * \name Normal Hypersurfaces
         */
        /*@{*/

        /**
         * Returns the link of the given face as a normal hypersurface.
         *
         * Constructing the link of a face begins with building the frontier
         * of a regular neighbourhood of the face.  If this is already a
         * normal hypersurface, then then link is called _thin_.  Otherwise
         * some basic normalisation steps are performed until the hypersurface
         * becomes normal; note that these normalisation steps could
         * change the topology of the hypersurface, and in some pathological
         * cases could even reduce it to the empty hypersurface.
         *
         * Although normalisation of arbitrary embedded 3-manifolds is messy,
         * for face links the process is thankfully simpler.  Essentially,
         * any changes will be limited to operations analagous to compressions
         * and boundary compressions along discs and 3-balls, as well as
         * removing trivial 4-sphere components.
         *
         * \tparam subdim the dimension of the face to link; this must be
         * between 0 and 3 inclusive.
         *
         * \pre The given face is a face of this triangulation.
         *
         * \return a pair (\a s, \a thin), where \a s is the face linking
         * normal hypersurface, and \a thin is \c true if and only if this link
         * is thin (i.e., no additional normalisation steps were required).
         */
        template <int subdim>
        std::pair<NormalHypersurface, bool> linkingSurface(
            const Face<4, subdim>& face) const;

        /*@}*/

    private:
        /**
         * Clears any calculated properties, including skeletal data,
         * and declares them all unknown.  This must be called by any
         * internal function that changes the triangulation.
         *
         * In most cases this routine is followed immediately by firing
         * a change event.
         *
         * It is recommended that you use a local ChangeAndClearSpan object
         * to manage both of these tasks (calling clearAllProperties() and
         * firing change events), rather than calling this function manually.
         */
        void clearAllProperties();

        void calculateSkeleton();
        void cloneSkeleton(const Triangulation& src);

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

        /**
         * Implements testing for and/or performing 4-4 edge moves.
         * See move44() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal and will not
         * violate any simplex and/or facet locks.
         *
         * \exception LockViolation This move would violate a simplex or facet
         * lock, and \a check was passed as \c false.  This exception will be
         * thrown before any changes are made.
         *
         * \param check indicates whether we should check whether the move is
         * legal and will not violate any locks.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         *
         * \author Alex He
         */
        bool internal44(Edge<4>* e, bool check, bool perform);

        /**
         * Implements testing for and/or performing book opening moves.
         * See openBook() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal and will not
         * violate any facet locks.
         *
         * \exception LockViolation This move would violate a facet lock, and
         * \a check was passed as \c false.  This exception will be thrown
         * before any changes are made.
         *
         * \param check indicates whether we should check whether the move is
         * legal and will not violate any locks.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internalOpenBook(Tetrahedron<4>* t, bool check, bool perform);

        /**
         * Implements testing for and/or performing edge collapse moves.
         * See collapseEdge() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal and will not
         * violate any simplex and/or facet locks.
         *
         * \exception LockViolation This move would violate a simplex or facet
         * lock, and \a check was passed as \c false.  This exception will be
         * thrown before any changes are made.
         *
         * \param check indicates whether we should check whether the move is
         * legal and will not violate any locks.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internalCollapseEdge(Edge<4>* e, bool check, bool perform);

        /**
         * Implements testing for and/or performing edge snap moves.
         * See snapEdge() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal.
         *
         * \param check indicates whether we should check whether the move is
         * legal.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         *
         * \author Alex He
         */
        bool internalSnapEdge(Edge<4>* e, bool check, bool perform);

        /**
         * Implements simplify().  The template argument indicates
         * which individual moves we are allowed to use.
         *
         * See simplify() for further details.
         */
        template <SimplifyContext>
        bool simplifyInternal();

        /**
         * Implements simplifyToLocalMinimum().  The template argument
         * indicates which individual moves we are allowed to use.
         *
         * See simplifyToLocalMinimum() for further details.
         */
        template <SimplifyContext>
        bool simplifyToLocalMinimumInternal(bool perform);

    friend class regina::Face<4, 4>;
    friend class regina::detail::SimplexBase<4>;
    friend class regina::detail::TriangulationBase<4>;
    friend class regina::XMLTriangulationReader<4>;
    friend class regina::XMLWriter<Triangulation<4>>;
};

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/dim4/pentachoron4.h"
#include "triangulation/dim4/tetrahedron4.h"
#include "triangulation/dim4/triangle4.h"
#include "triangulation/dim4/edge4.h"
#include "triangulation/dim4/vertex4.h"
#include "triangulation/dim4/component4.h"
namespace regina {

// Inline functions for Triangulation<4>

inline Triangulation<4>::~Triangulation() {
    Snapshottable<Triangulation<4>>::takeSnapshot();
    clearAllProperties();
}

inline Pentachoron<4>* Triangulation<4>::newPentachoron() {
    return newSimplex();
}

inline Pentachoron<4>* Triangulation<4>::newPentachoron(
        const std::string& desc) {
    return newSimplex(desc);
}

template <int k>
inline std::array<Pentachoron<4>*, k> Triangulation<4>::newPentachora() {
    return newSimplices<k>();
}

inline void Triangulation<4>::newPentachora(size_t k) {
    newSimplices(k);
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

inline Triangulation<4>& Triangulation<4>::operator = (
        const Triangulation& src) {
    // We need to implement copy assignment ourselves because it all
    // needs to be wrapped in a PacketChangeSpan.  This is so that the
    // final packetWasChanged event is fired *after* we modify the
    // properties specific to dimension 4.
    //
    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties.  Our intention here
    // is to clone them, not clear them.
    PacketChangeSpan span(*this);

    TriangulationBase<4>::operator = (src);

    vertexLinkSummary_ = src.vertexLinkSummary_;
    prop_ = src.prop_;

    return *this;
}

inline Triangulation<4>& Triangulation<4>::operator = (Triangulation&& src) {
    // Like copy assignment, we implement this ourselves because it all
    // needs to be wrapped in a PacketChangeSpan.
    //
    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties.  Our intention here
    // is to move them, not clear them.
    PacketChangeSpan span(*this);

    // The parent class assignment goes last, since its move invalidates src.

    vertexLinkSummary_ = src.vertexLinkSummary_;
    prop_ = std::move(src.prop_);

    TriangulationBase<4>::operator = (std::move(src));

    return *this;
}

inline bool Triangulation<4>::hasBoundaryTetrahedra() const {
    return hasBoundaryFacets();
}

inline size_t Triangulation<4>::countBoundaryTetrahedra() const {
    return countBoundaryFacets();
}

inline bool Triangulation<4>::isIdeal() const {
    ensureSkeleton();

    // Recall that for 4-manifolds we restrict "ideal" to only include
    // valid triangulations.
    if (! valid_)
        return false;

    // Since the triangulation is valid, we can deduce the answer from
    // vertexLinkSummary_.
    return (vertexLinkSummary_ > 0);
}

inline bool Triangulation<4>::isClosed() const {
    ensureSkeleton();
    return boundaryComponents().empty();
}

inline bool Triangulation<4>::simplify() {
    return simplifyInternal<SimplifyContext::Best>();
}

inline bool Triangulation<4>::intelligentSimplify() {
    return simplifyInternal<SimplifyContext::Best>();
}

inline bool Triangulation<4>::simplifyToLocalMinimum(bool perform) {
    return simplifyToLocalMinimumInternal<SimplifyContext::Best>(perform);
}

template <typename Action, typename... Args>
inline bool Triangulation<4>::retriangulate(int height, int threads,
        ProgressTrackerOpen* tracker, Action&& action, Args&&... args) const {
    if (countComponents() > 1) {
        if (tracker)
            tracker->setFinished();
        throw FailedPrecondition(
            "retriangulate() requires a connected triangulation");
    }

    // Use RetriangulateActionTraits to deduce whether the given action
    // takes a triangulation or both an isomorphism signature and triangulation
    // as its initial argument(s).
    using Traits =
        regina::detail::RetriangulateActionTraits<Triangulation<4>, Action>;
    static_assert(Traits::valid,
        "The action that is passed to retriangulate() does not take the correct initial argument type(s).");
    if constexpr (Traits::withSig) {
        return regina::detail::retriangulateInternal<Triangulation<4>, true>(
            *this, false /* rigid */, height, threads, tracker,
            [&](const std::string& sig, Triangulation<4>&& obj) {
                return action(sig, std::move(obj), std::forward<Args>(args)...);
            });
    } else {
        return regina::detail::retriangulateInternal<Triangulation<4>, false>(
            *this, false /* rigid */, height, threads, tracker,
            [&](Triangulation<4>&& obj) {
                return action(std::move(obj), std::forward<Args>(args)...);
            });
    }
}

inline bool Triangulation<4>::simplifyExhaustive(int height, int threads,
        ProgressTrackerOpen* tracker) {
    if (countComponents() > 1) {
        if (tracker)
            tracker->setFinished();
        throw FailedPrecondition(
            "simplifyExhaustive() requires a connected triangulation");
    }

    return regina::detail::simplifyExhaustiveInternal<Triangulation<4>>(
        *this, height, threads, tracker);
}

inline bool Triangulation<4>::move44(Edge<4>* e) {
    return internal44(e, true, true);
}

inline bool Triangulation<4>::openBook(Tetrahedron<4>* t) {
    return internalOpenBook(t, true, true);
}

inline bool Triangulation<4>::collapseEdge(Edge<4>* e) {
    return internalCollapseEdge(e, true, true);
}

inline bool Triangulation<4>::snapEdge(Edge<4>* e) {
    return internalSnapEdge(e, true, true);
}

inline bool Triangulation<4>::has44(Edge<4>* e) const {
    return const_cast<Triangulation<4>*>(this)->internal44(e, true, false);
}

inline bool Triangulation<4>::hasOpenBook(Tetrahedron<4>* t) const {
    return const_cast<Triangulation<4>*>(this)->internalOpenBook(t,
        true, false);
}

inline bool Triangulation<4>::hasCollapseEdge(Edge<4>* e) const {
    return const_cast<Triangulation<4>*>(this)->internalCollapseEdge(e,
        true, false);
}

inline bool Triangulation<4>::hasSnapEdge(Edge<4>* e) const {
    return const_cast<Triangulation<4>*>(this)->internalSnapEdge(e,
        true, false);
}

inline std::optional<Triangulation<4>> Triangulation<4>::with44(Edge<4>* e)
        const {
    if (! has44(e))
        return {};

    std::optional<Triangulation<4>> ans(std::in_place, *this);
    ans->internal44(ans->translate(e), false, true);
    return ans;
}

inline std::optional<Triangulation<4>> Triangulation<4>::withOpenBook(
        Tetrahedron<4>* t) const {
    if (! hasOpenBook(t))
        return {};

    std::optional<Triangulation<4>> ans(std::in_place, *this);
    ans->internalOpenBook(ans->translate(t), false, true);
    return ans;
}

inline std::optional<Triangulation<4>> Triangulation<4>::withCollapseEdge(
        Edge<4>* e) const {
    if (! hasCollapseEdge(e))
        return {};

    std::optional<Triangulation<4>> ans(std::in_place, *this);
    ans->internalCollapseEdge(ans->translate(e), false, true);
    return ans;
}

inline std::optional<Triangulation<4>> Triangulation<4>::withSnapEdge(
        Edge<4>* e) const {
    if (! hasSnapEdge(e))
        return {};

    std::optional<Triangulation<4>> ans(std::in_place, *this);
    ans->internalSnapEdge(ans->translate(e), false, true);
    return ans;
}

inline bool Triangulation<4>::fourFourMove(Edge<4>* e, bool, bool perform) {
    return internal44(e, true, perform);
}

inline bool Triangulation<4>::openBook(Tetrahedron<4>* t, bool, bool perform) {
    return internalOpenBook(t, true, perform);
}

inline bool Triangulation<4>::collapseEdge(Edge<4>* e, bool, bool perform) {
    return internalCollapseEdge(e, true, perform);
}

inline bool Triangulation<4>::snapEdge(Edge<4>* e, bool, bool perform) {
    return internalSnapEdge(e, true, perform);
}

} // namespace regina

#endif

