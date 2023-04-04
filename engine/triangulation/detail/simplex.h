
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file triangulation/detail/simplex.h
 *  \brief Implementation details for top-dimensional simplices in a
 *  triangulation.
 */

#ifndef __REGINA_SIMPLEX_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_SIMPLEX_H_DETAIL
#endif

#include "regina-core.h"
#include "core/output.h"
#include "triangulation/facenumbering.h"
#include "triangulation/forward.h"
#include "utilities/markedvector.h"
#include <array>
#include <cassert>
#include <tuple>

namespace regina {

template <int> class XMLSimplexReader;

namespace detail {

template <int> class TriangulationBase;

/**
 * Helper class that provides core functionality for a top-dimensional
 * simplex in a <i>dim</i>-manifold triangulation.
 *
 * Each top-dimensional simplex is represented by the class Simplex<dim>,
 * which uses this as a base class.  End users should not need to refer
 * to SimplexBase directly.
 *
 * See the Simplex template class notes for further information, including
 * details of how the vertices and facets of each simplex are numbered.
 *
 * Neither this class nor the "end user" class Simplex<dim> support
 * value semantics: they cannot be copied, swapped, or manually constructed.
 * Their memory is managed by the Triangulation class, and their locations
 * in memory define them.  See Simplex<dim> for further details.
 *
 * \python This base class is not present, but the "end user" class
 * Simplex<dim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \ingroup detail
 */
template <int dim>
class SimplexBase : public MarkedElement, public Output<SimplexBase<dim>> {
    static_assert(dim >= 2, "Simplex requires dimension >= 2.");

    public:
        static constexpr int dimension = dim;
            /**< A compile-time constant that gives the dimension of the
                 triangulation containing this simplex. */
        static constexpr int subdimension = dim;
            /**< A compile-time constant that gives the dimension of this
                 simplex. */

        using FacetMask = typename IntOfMinBits<dim + 1>::utype;
            /**< An unsigned integer type with at least <i>dim</i>+1 bits.
                 This can be used as a bitmask for the <i>dim</i>+1 facets
                 (or vertices) of a <i>dim</i>-simplex. */
        using LockMask = typename IntOfMinBits<dim + 2>::utype;
            /**< An unsigned integer type with at least <i>dim</i>+2 bits.
                 The <i>i</i>th bit indicates whether facet \a i of the simplex
                 is locked for 0 ≤ \a i ≤ \a dim, and the (<i>dim</i>+2)th bit
                 indicates whether the simplex itself is locked. */

    private:
        /**
         * The sequence of all subface dimensions 0,...,(<i>dim</i>-1).
         */
        using subdimensions = std::make_integer_sequence<int, dim>;

        /**
         * A non-existent function used to construct the type of the \a faces_
         * tuple.  Essentially, this lets us pull apart the integer pack
         * \a subdimensions.  The return type is the tuple type that we want.
         */
        template <int... subdim>
        static auto seqToFaces(std::integer_sequence<int, subdim...>) ->
            std::tuple<std::array<Face<dim, subdim>*,
                FaceNumbering<dim, subdim>::nFaces>...>;

        /**
         * A non-existent function used to construct the type of the
         * \a mappings_ tuple.  Again, this lets us pull apart the integer pack
         * \a subdimensions, and the return type is the tuple type that we want.
         */
        template <int... subdim>
        static auto seqToMappings(std::integer_sequence<int, subdim...>) ->
            std::tuple<std::array<Perm<dim+1>,
                FaceNumbering<dim, subdim>::nFaces>...>;

        decltype(seqToFaces(subdimensions())) faces_;
            /**< A tuple of arrays of faces of this simplex.
                 Specifically, std::get<k>(faces_)[i] is a pointer to the
                 ith k-face of this simplex. */

        decltype(seqToMappings(subdimensions())) mappings_;
            /**< A tuple of arrays of permutations, showing how the faces
                 of the triangulation map to the faces of this simplex.
                 Specifically, std::get<k>(mappings_)[i] describes the ith
                 k-face of this simplex, and maps the vertices (0,1,...,k) of
                 the underlying k-face of the triangulation to the corresponding
                 vertices of this simplex.  See Simplex<dim>::faceMapping()
                 for details. */

        Simplex<dim>* adj_[dim + 1];
            /**< Stores the adjacent simplex glued to each facet of this
                 simplex.  Specifically, `adj_[f]` represents the
                 simplex joined to facet \a f of this simplex, or is 0
                 if facet \a f lies on the triangulation boundary. */
        Perm<dim+1> gluing_[dim + 1];
            /**< Indicates how vertices map to each other across each gluing.
                 Specifically, if facet \a f is joined to some other simplex
                 (i.e., it is not boundary), then \a gluing_[\a f] represents
                 the induced mapping from vertices of this simplex to
                 vertices of the adjacent simplex. */
        std::string description_;
            /**< The description of this simplex, or the empty string
                 if there is no description. */

        Triangulation<dim>* tri_;
            /**< The triangulation to which this simplex belongs. */

        int orientation_;
            /**< The orientation of this simplex in the triangulation.
                 This will either be +1 or -1, and will only be set
                 if/when the skeleton of the triangulation is computed. */
        Component<dim>* component_;
            /**< The component to which this simplex belongs in the
                 triangulation.  This will only be set if/when the
                 skeleton of the triangulation is computed. */

        FacetMask dualForest_;
            /**< The <i>i</i>th bit of this integer indicates whether facet
                 \a i of this simplex belongs to the maximal forest in the
                 dual 1-skeleton.  See facetInMaximalForest() for details.
                 This will only be set if/when the skeleton is computed. */
        LockMask locks_ { 0 };
            /**< Indicates whether this simplex and/or any of its facets are
                 locked.  This is a bitmask; see the LockMask documentation
                 for how the individual bits of \a locks_ are interpreted.
                 The locking/unlocking code must always ensure that facets
                 that are glued together have consistent locks; that is, if a
                 facet is glued to a facet of some adjacent simplex, then
                 either both corresponding facets must be locked or both
                 corresponding facets must be unlocked. */

    public:
        /**
         * Returns the description associated with this simplex.
         *
         * \return the description of this simplex, or the empty string
         * if no description is stored.
         */
        const std::string& description() const;

        /**
         * Sets the description associated with this simplex.
         *
         * This may be any text whatsoever; typically it is intended to
         * be human-readable.  Descriptions do not need to be unique.
         *
         * To remove an existing description, you can simply set the
         * description to the empty string.
         *
         * \param desc the new description to assign to this simplex.
         */
        void setDescription(const std::string& desc);

        /**
         * Returns the index of this simplex in the underlying triangulation.
         *
         * The index will be an integer between 0 and
         * `triangulation().size()-1` inclusive.
         *
         * Note that indexing may change when a simplex is added to or removed
         * from the underlying triangulation.
         *
         * \return the index of this simplex.
         */
        size_t index() const;

        /**
         * Returns the adjacent simplex that is glued to the given facet of
         * this simplex.  If there is no adjacent simplex (i.e., the
         * given facet lies on the triangulation boundary), then this
         * routine will return \c null.
         *
         * \param facet the facet of this simplex to examine; this must
         * be between 0 and \a dim inclusive.
         * \return the adjacent simplex glued to the given facet, or \c null
         * if the given facet lies on the boundary.
         */
        Simplex<dim>* adjacentSimplex(int facet) const;

        /**
         * Returns a permutation that indicates precisely how this
         * simplex is glued to the adjacent simplex across the given facet.
         *
         * In detail: suppose that the given facet of this simplex is glued to
         * an adjacent simplex \a A.  Then this gluing induces a mapping from
         * the vertices of this simplex to the vertices of \a A.  We can
         * express this mapping in the form of a permutation \a p, where:
         *
         * - for any \a v ≠ \a facet, the gluing identifies vertex \a v
         *   of this simplex with vertex \a p[\a v] of simplex \a A;
         *
         * - \a p[\a facet] indicates the facet of \a A that is on the
         *   other side of the gluing (i.e., the facet of \a A that is
         *   glued to the given facet of this simplex).
         *
         * \pre The given facet of this simplex has some adjacent simplex
         * (possibly this one) glued to it.  In other words,
         * adjacentSimplex(\a facet) is not \c null.
         *
         * \param facet the facet of this simplex that we are examining.
         * This must be between 0 and \a dim inclusive.
         * \return a permutation that maps the vertices of this simplex to the
         * vertices of the adjacent simplex, as described above.
         */
        Perm<dim+1> adjacentGluing(int facet) const;
        /**
         * If the given facet of this simplex is glued to facet \a f of
         * some adjacent simplex, then this routine returns the adjacent
         * facet number \a f.
         *
         * The return value from this routine is identical to
         * adjacentGluing(\a facet)[\a facet].
         *
         * \pre The given facet of this simplex has some adjacent simplex
         * (possibly this one) glued to it.  In other words,
         * adjacentSimplex(\a facet) is not \c null.
         *
         * \param facet the facet of this simplex that we are examining.
         * This must be between 0 and \a dim inclusive.
         * \return the corresponding facet number of the adjacent simplex
         * that is glued to the given facet of this simplex.
         */
        int adjacentFacet(int facet) const;
        /**
         * Determines if this simplex has any facets that lie on the
         * triangulation boundary.  In other words, this routine
         * determines whether any facet of this simplex is not currently
         * glued to an adjacent simplex.
         *
         * \return \c true if and only if this simplex has any boundary facets.
         */
        bool hasBoundary() const;

        /**
         * Joins the given facet of this simplex to some facet of another
         * simplex.  The other simplex will be updated automatically
         * (i.e., you only need to call join() from one side of the gluing).
         *
         * You may join a facet of this simplex to some different facet of
         * the same simplex (i.e., you may pass \a you == \c this),
         * though you cannot join a facet to itself.
         *
         * \pre This and the given simplex belong to the same triangulation.
         * \pre The given facet of this simplex is not currently glued to
         * anything.
         * \pre The corresponding facet of the other simplex
         * (i.e., facet \a gluing[\a myFacet] of \a you) is likewise not
         * currently glued to anything.
         * \pre We are not attempting to glue a facet to itself
         * (i.e., we do not have both \a you == \c this and
         * \a gluing[\a myFacet] == \a myFacet).
         *
         * \exception InvalidArgument At least one of the conditions above
         * fails; that is, either the two simplices being joined belong
         * to different triangulations, or one of the two facets being
         * joined is already joined to something, or you are trying to
         * join the same facet of the same simplex to itself.
         *
         * \param myFacet the facet of this simplex that will be glued
         * to the given simplex \a you.  This facet number must be between
         * 0 and \a dim inclusive.
         * \param you the other simplex that will be glued to the given facet
         * of this simplex.
         * \param gluing a permutation that describes how the vertices of
         * this simplex will map to the vertices of \a you across the
         * new gluing.  This permutation should be in the form described
         * by adjacentGluing().
         */
        void join(int myFacet, Simplex<dim>* you, Perm<dim+1> gluing);
        /**
         * Unglues the given facet of this simplex from whatever it is
         * joined to.  As a result, the given facet of this simplex
         * will become a boundary facet.
         *
         * If there was an adjacent simplex to begin with, then this other
         * simplex will be updated automatically (i.e., you only need to
         * call unjoin() from one side of the gluing).
         *
         * This routine is safe to call even if the given facet is
         * already a boundary facet (in which case it will do nothing).
         *
         * \param myFacet the facet of this simplex whose gluing we
         * will undo.  This should be between 0 and \a dim inclusive.
         * \return the simplex that was originally glued to the given facet
         * of this simplex, or \c null if this was already a boundary facet.
         */
        Simplex<dim>* unjoin(int myFacet);
        /**
         * Unglues this simplex from any adjacent simplices.
         * As a result, every facet of this simplex will become a boundary
         * facet, and this simplex will form its own separate component
         * of the underlying triangulation.
         *
         * If there were any adjacent simplices to begin with, these
         * will be updated automatically.
         *
         * This routine is safe to call even if there are no adjacent
         * simplices (in which case it will do nothing).
         */
        void isolate();

        /**
         * Locks this top-dimensional simplex.
         *
         * Essentially, locking a simplex means that that simplex must not
         * change.  Specifically:
         *
         * - A locked simplex cannot be removed completely (e.g., via
         *   Triangulation<dim>::removeSimplex() or via moves such as
         *   edge collapses or 2-0 moves).
         *
         * - A locked simplex cannot be subdivided (e.g., via
         *   Triangulation<dim>::subdivide(), or via a 1-(<i>dim</i>+1)
         *   Pachner move).
         *
         * - A locked simplex cannot be merged with adjacent simplices
         *   (e.g., via any of the other Pachner moves).
         *
         * Regina's own automatic retriangulation routines (such as
         * Triangulation<dim>::intelligentSimplify() or
         * Triangulation<dim>::retriangulate()) will simply avoid changing
         * any locked simplices.  If the user attempts to manually force a
         * change (e.g., by calling Triangulation<dim>::subdivide()), then
         * a FailedPrecondition exception will be thrown.
         *
         * It is safe to call this function even if this simplex is
         * already locked.
         *
         * Note that you can also lock the individual facets of a simplex
         * (that is, its (<i>dim</i>-1)-faces); see lockFacet() for details.
         * Locking a simplex does _not_ imply that its facets will be
         * automatically locked also; these are independent concepts.
         *
         * The Triangulation copy constructor and assignment operators will
         * preserve locks (i.e., the simplices/facets of the new triangulation
         * will be locked in the same way as the simplices/facets of the
         * source).
         *
         * Locks will not interfere with the destruction of a triangulation
         * (i.e., the Triangulation destructor does not check for locks).
         *
         * Changing locks is considered a modification of the triangulation
         * (in particular, if the triangulation is wrapped in a packet
         * then the appropriate change events will be fired).
         */
        void lock();
        /**
         * Locks the given facet of this top-dimensional simplex.
         *
         * Essentially, locking a facet means that that facet must not
         * change.  Specifically:
         *
         * - A locked boundary facet cannot be glued to some other
         *   top-dimensional simplex (e.g., via join()).
         *
         * - A locked internal (non-boundary) facet cannot made boundary
         *   by explicitly ungluing.  As an exception, however, we _do_ allow
         *   a locked internal facet to become boundary because a move was
         *   performed on one side with the side-effect of removing all of the
         *   top-dimensional simplices on that side (e.g., a 2-0 move,
         *   edge collapse, or shell boundary move, where the region being
         *   removed sits between the locked internal facet and the boundary
         *   of the triangulation).
         *
         * - A locked facet cannot be removed completely (e.g., a facet
         *   that is internal to the region that is removed by a 2-0 move
         *   or an edge collapse, or a facet internal to the region where
         *   a Pachner move is performed, or a boundary facet of the simplex
         *   that is removed by a shell boundary move).
         *
         * - A locked facet cannot be subdivided (e.g., via
         *   Triangulation<dim>::subdivide().
         *
         * Regina's own automatic retriangulation routines (such as
         * Triangulation<dim>::intelligentSimplify() or
         * Triangulation<dim>::retriangulate()) will simply avoid changing
         * any locked facets.  If the user attempts to manually force a
         * change (e.g., by calling Triangulation<dim>::subdivide()), then
         * a FailedPrecondition exception will be thrown.
         *
         * Regina will always ensure that the locks on facets are consistent.
         * That is, if some facet \a F of some top-dimensional simplex is
         * glued to some facet \a G of some top-dimensional simplex, then
         * whenever \a F is locked/unlocked, Regina will automatically
         * lock/unlock \a G also.
         *
         * It is safe to call this function even if the given facet is
         * already locked.
         *
         * Note that you can also lock an entire top-dimensional simplex;
         * see lock() for details.  Locking a simplex does _not_ imply
         * that its facets will be automatically locked also, or vice versa;
         * these are independent concepts.
         *
         * The Triangulation copy constructor and assignment operators will
         * preserve locks (i.e., the simplices/facets of the new triangulation
         * will be locked in the same way as the simplices/facets of the
         * source).
         *
         * Locks will not interfere with the destruction of a triangulation
         * (i.e., the Triangulation destructor does not check for locks).
         *
         * Changing locks is considered a modification of the triangulation
         * (in particular, if the triangulation is wrapped in a packet
         * then the appropriate change events will be fired).
         *
         * \param facet indicates which facet of this simplex to lock;
         * this must be between 0 and \a dim inclusive.
         */
        void lockFacet(int facet);
        /**
         * Unlocks this top-dimensional simplex.
         *
         * Essentially, locking a simplex means that that simplex must not
         * change.  See lock() for full details on how locks work and
         * what their implications are.
         *
         * It is safe to call this function even if this simplex is
         * already unlocked.
         *
         * Note that you can also lock the individual facets of a simplex
         * (that is, its (<i>dim</i>-1)-faces); see lockFacet() for details.
         * Unlocking a simplex does _not_ imply that its facets will be
         * automatically unlocked also; these are independent concepts.
         *
         * See unlockAll() for a convenient way to unlock this simplex and
         * all of its facets in a single function call.  Also,
         * Triangulation<dim>::unlockAll() offers a simple way to unlock all
         * <i>dim</i>-simplices and their facets across an entire triangulation.
         */
        void unlock();
        /**
         * Unlocks the given facet of this top-dimensional simplex.
         *
         * Essentially, locking a facet means that that facet must not
         * change.  See lockFacet() for full details on how locks work and
         * what their implications are.
         *
         * Regina will always ensure that the locks on facets are consistent.
         * That is, if some facet \a F of some top-dimensional simplex is
         * glued to some facet \a G of some top-dimensional simplex, then
         * whenever \a F is locked/unlocked, Regina will automatically
         * lock/unlock \a G also.
         *
         * It is safe to call this function even if the given facet is
         * already unlocked.
         *
         * Note that you can also lock an entire top-dimensional simplex;
         * see lock() for details.  Unlocking a simplex does _not_ imply
         * that its facets will be automatically unlocked also, or vice versa;
         * these are independent concepts.
         *
         * See unlockAll() for a convenient way to unlock this simplex and
         * all of its facets in a single function call.  Also,
         * Triangulation<dim>::unlockAll() offers a simple way to unlock all
         * <i>dim</i>-simplices and their facets across an entire triangulation.
         *
         * \param facet indicates which facet of this simplex to unlock;
         * this must be between 0 and \a dim inclusive.
         */
        void unlockFacet(int facet);
        /**
         * Unlocks this top-dimensional simplex and all of its facets.
         *
         * Essentially, locking a simplex or one of its facets means that that
         * simplex or facet must not change.  See lock() and lockFacet() for
         * full details on how locks work and what their implications are.
         *
         * Regina will always ensure that the locks on facets are consistent.
         * That is, if some facet \a F of some top-dimensional simplex is
         * glued to some facet \a G of some top-dimensional simplex, then
         * whenever \a F is locked/unlocked, Regina will automatically
         * lock/unlock \a G also.
         *
         * It is safe to call this function even if this simplex and all
         * of its facets are already unlocked.
         *
         * See also Triangulation<dim>::unlockAll() for a simple way to unlock
         * all <i>dim</i>-simplices and their facets across an entire
         * triangulation.
         */
        void unlockAll();
        /**
         * Determines whether this top-dimensional simplex is locked.
         *
         * Essentially, locking a simplex means that that simplex must not
         * change.  See lock() for full details on how locks work and
         * what their implications are.
         *
         * Note that you can also lock the individual facets of a simplex
         * (that is, its (<i>dim</i>-1)-faces); see lockFacet() for details.
         * This routine does _not_ test whether any facets of this simplex are
         * locked; it only tests for a lock on the top-dimensional
         * simplex itself.
         *
         * See lockMask() for a convenient way to test in a single query
         * whether this simplex and/or any of its facets are locked.  Also,
         * Triangulation<dim>::hasLocks() offers a simple way to test whether
         * a triangulation has any locked <i>dim</i>-simplices or facets at all.
         *
         * \return \c true if and only if this simplex is locked.
         */
        bool isLocked() const;
        /**
         * Determines whether the given facet of this top-dimensional simplex
         * is locked.
         *
         * Essentially, locking a facet means that that facet must not
         * change.  See lockFacet() for full details on how locks work and
         * what their implications are.
         *
         * Note that you can also lock an entire top-dimensional simplex;
         * see lock() for details.  This routine does _not_ test whether
         * the top-dimensional simplex is locked; it only tests for a lock
         * on the given facet.
         *
         * See lockMask() for a convenient way to test in a single query
         * whether this simplex and/or any of its facets are locked.  Also,
         * Triangulation<dim>::hasLocks() offers a simple way to test whether
         * a triangulation has any locked <i>dim</i>-simplices or facets at all.
         *
         * \param facet indicates which facet of this simplex to examine;
         * this must be between 0 and \a dim inclusive.
         * \return \c true if and only if the given facet of this simplex
         * is locked.
         */
        bool isFacetLocked(int facet) const;
        /**
         * Returns a bitmask indicating which of this simplex and/or its
         * individual facets are locked.
         *
         * Essentially, locking a top-dimensional simplex or one of its facets
         * means that that simplex or facet must not change.  See lock() and
         * lockFacet() for full details on how locks work and what their
         * implications are.
         *
         * This routine returns a bitmask containing `dim+2` bits (here
         * we number the bits so that the 0th bit is the least significant).
         * The <i>k</i>th bit is set if and only if the <i>k</i>th facet
         * of this simplex is locked, for 0 ≤ \a k ≤ \a dim.  Finally,
         * the (<i>dim</i>+1)th bit is set if and only if this simplex itself
         * is locked.
         *
         * See also isLocked() and isFacetLocked() for a more convenient way
         * to query the simplex and/or one of its facets individually, and
         * Triangulation<dim>::hasLocks() for a simple way to query all
         * top-dimensional simplices and their facets across the entire
         * triangulation.
         *
         * \return a bitmask indicating which of this simplex and/or its
         * facets are locked.  This bitmask will be returned using a native
         * C++ unsigned integer type of the appropriate size.
         */
        LockMask lockMask() const;

        /**
         * Returns the triangulation to which this simplex belongs.
         *
         * \return a reference to the triangulation containing this simplex.
         */
        Triangulation<dim>& triangulation() const;

        /**
         * Returns the connected component of the triangulation to
         * which this simplex belongs.
         *
         * \return the component containing this simplex.
         */
        Component<dim>* component() const;

        /**
         * Returns the <i>subdim</i>-face of the underlying triangulation
         * that appears as the given <i>subdim</i>-face of this simplex.
         *
         * See FaceNumbering<dim, subdim> for the conventions of how
         * <i>subdim</i>-faces are numbered within a <i>dim</i>-simplex.
         *
         * \python Python does not support templates.  Instead,
         * Python users should call this function in the form
         * `face(subdim, face)`; that is, the template parameter
         * \a subdim becomes the first argument of the function.
         *
         * \tparam subdim the dimension of the subface to examine.
         * This must be between 0 and (\a dim - 1) inclusive.
         *
         * \param face the <i>subdim</i>-face of this simplex to examine.
         * This should be between 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1
         * inclusive.
         * \return the corresponding <i>subdim</i>-face of the triangulation.
         */
        template <int subdim>
        Face<dim, subdim>* face(int face) const;

        /**
         * A dimension-specific alias for face<0>().
         *
         * This alias is available for all dimensions \a dim.
         *
         * See face() for further information.
         */
        Face<dim, 0>* vertex(int i) const;

        /**
         * A dimension-specific alias for face<1>().
         *
         * This alias is available for all dimensions \a dim.
         *
         * See face() for further information.
         */
        Face<dim, 1>* edge(int i) const;

        /**
         * A dimension-specific alias for face<2>().
         *
         * This alias is available for dimensions \a dim ≥ 3.
         *
         * See face() for further information.
         */
        Face<dim, 2>* triangle(int i) const;

        /**
         * A dimension-specific alias for face<3>().
         *
         * This alias is available for dimensions \a dim ≥ 4.
         *
         * See face() for further information.
         */
        Face<dim, 3>* tetrahedron(int i) const;

        /**
         * A dimension-specific alias for face<4>().
         *
         * This alias is available for dimensions \a dim ≥ 5.
         *
         * See face() for further information.
         */
        Face<dim, 4>* pentachoron(int i) const;

        /**
         * Returns the edge of this simplex that connects the
         * two given vertices of this simplex.
         *
         * This is a convenience routine to avoid more cumbersome calls to
         * Edge<dim>::faceNumber().  In dimensions 3 and 4 (where the array
         * Edge<dim>::edgeNumber is defined), this routine is identical to
         * calling `edge(Edge<dim>::edgeNumber[i][j])`.
         *
         * \param i the vertex of this simplex that forms one endpoint
         * of the edge; this must be between 0 and \a dim inclusive.
         * \param j the vertex of this simplex that forms the other endpoint
         * of the edge; this must be between 0 and \a dim inclusive, and
         * must also be different from \a i.
         * \return the edge of this simplex that connects vertices
         * \a i and \a j of this simplex.
         */
        Face<dim, 1>* edge(int i, int j) const {
            static_assert(! standardDim(dim),
                "The default implementation of Simplex<dim>::edge(i,j) "
                "should not be used for standard dimensions.");
            if (i > j)
                std::swap(i, j);
            return (i == j ? nullptr : face<1>(
                (i * dim) + j - 1 - i * (i + 1) / 2));
        }

        /**
         * Examines the given <i>subdim</i>-face of this simplex, and
         * returns the mapping between the underlying <i>subdim</i>-face
         * of the triangulation and the individual vertices of this simplex.
         *
         * Specifically:
         *
         * - Suppose several <i>subdim</i>-faces of several top-dimensional
         *   simplices are identified within the overall triangulation.  Then
         *   we call this a single "<i>subdim</i>-face of the triangulation",
         *   and arbitrarily label its vertices (0, ..., \a subdim).
         *
         * - Now let \a F denote the <i>subdim</i>-face of the triangulation
         *   that corresponds to <i>subdim</i>-face number \a face of this
         *   simplex.  Then this routine returns a map from vertices
         *   (0, ..., \a subdim) of \a F to the corresponding vertex numbers
         *   of this simplex.
         *
         * - In particular, if this routine returns the permutation \a p,
         *   then the images \a p[0,...,\a subdim] will be some permutation
         *   of the vertices Face<dim, subdim>::ordering[0,...,\a subdim].
         *
         * - If \a F also appears as face number \a k in some other simplex
         *   \a s, then for each \a i in the range 0 ≤ \a i ≤ \a subdim,
         *   vertex `p[i]` of this simplex will be identified with
         *   vertex `s.faceMapping(k)[i]` of simplex \a s.
         *
         * If the link of the underlying <i>subdim</i>-face is orientable,
         * then this permutation maps the remaining numbers
         * (<i>subdim</i>+1, ..., \a dim) to the remaining vertex numbers of
         * this simplex in a manner that preserves orientation as you walk
         * through the many different simplices that contain the same
         * underlying <i>subdim</i>-face.  Specifically:
         *
         * - The images of (<i>subdim</i>+1, ..., \a dim) under this permutation
         *   imply an orientation for the (\a dim - \a subdim - 1)-face
         *   opposite \a F in this simplex.  These orientations will be
         *   consistent for all simplices containing \a F.
         *
         * - For faces of codimension two (e.g., edges in a 3-manifold
         *   triangulation), this orientation condition is even stronger.
         *   Here the link of the face \a F must be a path (for a boundary
         *   face) or a cycle (for an internal face).
         *   In each simplex we can form a directed edge from the image of
         *   <i>dim</i>-1 to the image of \a dim under this permutation, and
         *   together these directed edges form a directed path or cycle
         *   that follows the link of the face \a F.  Moreover, an iteration
         *   through the corresponding FaceEmbedding<dim, subdim> objects in
         *   order from `F.begin()` to `F.end()`, will follow
         *   this directed path in order from start to end.  (In the case where
         *   the link of \a F is a cycle, the start point in the list of
         *   FaceEmbedding objects will be arbitrary.)
         *
         * Note that, even if the link is orientable, there are still
         * arbitrary decisions to be made for the images of
         * (<i>subdim</i>+1, ..., \a dim), since there will always be
         * (<i>dim</i>-<i>subdim</i>)!/2 possible mappings that yield the
         * correct orientation.
         *
         * If this simplex (and therefore the face \a F) belongs to an
         * _orientable_ component of the triangulation, then there will also
         * be connections between faceMapping() and the orientations of the
         * top-dimensional simplices (as returned by orientation()):
         *
         * - If \a subdim is less than (\a dim - 1), then the sign of the
         *   permutation returned by faceMapping() will always be equal to the
         *   orientation of this simplex.
         *
         * - If \a subdim is equal to (\a dim - 1), then the face \a F can only
         *   belong to either one or two top-dimensional simplices; let
         *   \a s0 and \a s1 be the simplices corresponding to
         *   `F.embedding(0)` and (if it exists) `F.embedding(1)`
         *   respectively.  Then in the simplex \a s0, the sign of the
         *   faceMapping() permutation will match the orientation of \a s0,
         *   and in \a s1 (if it exists), the sign of the faceMapping()
         *   permutation will be negative the orientation of \a s1.
         *
         * \note This routine returns the same permutation as
         * FaceEmbedding<dim, subdim>::vertices(), in the context of the
         * FaceEmbedding<dim, subdim> object that refers to
         * <i>subdim</i>-face number \a face of this simplex.
         *
         * \python Python does not support templates.  Instead,
         * Python users should call this function in the form
         * `faceMapping(subdim, face)`; that is, the template
         * parameter \a subdim becomes the first argument of the function.
         *
         * \tparam subdim the dimension of the subface to examine.
         * This must be between 0 and (\a dim - 1) inclusive.
         *
         * \param face the <i>subdim</i>-face of this simplex to examine.
         * This should be between 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1
         * inclusive.
         * \return a mapping from the vertices of the underlying
         * <i>subdim</i>-face of the triangulation to the vertices of
         * this simplex.
         */
        template <int subdim>
        Perm<dim + 1> faceMapping(int face) const;

        /**
         * A dimension-specific alias for faceMapping<0>().
         *
         * This alias is available for all dimensions \a dim.
         *
         * See faceMapping() for further information.
         */
        Perm<dim + 1> vertexMapping(int face) const;

        /**
         * A dimension-specific alias for faceMapping<1>().
         *
         * This alias is available for all dimensions \a dim.
         *
         * See faceMapping() for further information.
         */
        Perm<dim + 1> edgeMapping(int face) const;

        /**
         * A dimension-specific alias for faceMapping<2>().
         *
         * This alias is available for dimensions \a dim ≥ 3.
         *
         * See faceMapping() for further information.
         */
        Perm<dim + 1> triangleMapping(int face) const;

        /**
         * A dimension-specific alias for faceMapping<3>().
         *
         * This alias is available for dimensions \a dim ≥ 4.
         *
         * See faceMapping() for further information.
         */
        Perm<dim + 1> tetrahedronMapping(int face) const;

        /**
         * A dimension-specific alias for faceMapping<4>().
         *
         * This alias is available for dimensions \a dim ≥ 5.
         *
         * See faceMapping() for further information.
         */
        Perm<dim + 1> pentachoronMapping(int face) const;

        /**
         * Returns the orientation of this simplex in the
         * <i>dim</i>-dimensional triangulation.
         *
         * The orientation of each top-dimensional simplex is always +1 or -1.
         * In an orientable component of a triangulation,
         * adjacent simplices have the same orientations if one could be
         * transposed onto the other without reflection, and they have
         * opposite orientations if a reflection would be required.
         * In a non-orientable component, orientations are arbitrary
         * (but they will still all be +1 or -1).
         *
         * In each component, the top-dimensional simplex with smallest index
         * will always have orientation +1.  In particular, simplex 0 will
         * always have orientation +1.
         *
         * \return +1 or -1 according to the orientation of this simplex.
         */
        int orientation() const;

        /**
         * Determines whether the given facet of this simplex belongs to the
         * maximal forest that has been chosen for the dual 1-skeleton of the
         * underlying triangulation.
         *
         * When the skeletal structure of a triangulation is first computed,
         * a maximal forest in the dual 1-skeleton of the triangulation is
         * also constructed.  Each dual edge in this maximal forest
         * represents a (<i>dim</i>-1)-face of the (primal) triangulation.
         *
         * This maximal forest will remain fixed until the triangulation
         * changes, at which point it will be recomputed (as will all
         * other skeletal objects, such as connected components and so on).
         * There is no guarantee that, when it is recomputed, the
         * maximal forest will use the same dual edges as before.
         *
         * This routine identifies which (<i>dim</i>-1)-faces of the
         * triangulation belong to the dual forest.  Because it lives in the
         * Simplex class, this routine can even be used for those dimensions
         * that do not have explicit classes for (<i>dim</i>-1)-faces of the
         * triangulation.
         *
         * If the skeleton has already been computed, then this routine is
         * very fast (since it just returns a precomputed answer).
         *
         * \param facet the facet of this simplex that we are examining.
         * This must be between 0 and \a dim inclusive.
         * \return \c true if and only if the given facet of this simplex
         * corresponds to a dual edge in the maximal forest chosen for the
         * dual 1-skeleton.
         */
        bool facetInMaximalForest(int facet) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        // Make this class non-copyable.
        SimplexBase(const SimplexBase&) = delete;
        SimplexBase& operator = (const SimplexBase&) = delete;

    protected:
        /**
         * Creates a new simplex with no description and no facets joined
         * to anything.
         *
         * \param tri the triangulation to which the new simplex belongs.
         */
        SimplexBase(Triangulation<dim>* tri);
        /**
         * Creates a new simplex whose description and locks are cloned
         * from the given simplex, and with no facets joined to anything.
         *
         * \param clone the simplex whose details should be cloned.
         * \param tri the triangulation to which the new simplex belongs.
         */
        SimplexBase(const SimplexBase& clone, Triangulation<dim>* tri);
        /**
         * Creates a new simplex with the given description, no locks,
         * and no facets joined to anything.
         *
         * \param desc the description to give the new simplex.
         * \param tri the triangulation to which the new simplex belongs.
         */
        SimplexBase(std::string desc, Triangulation<dim>* tri);

        /**
         * Tests whether the <i>useDim</i>-face degrees of this and the
         * given simplex are identical, under the given relabelling.
         *
         * \param other the simplex to compare against this.
         * \param p a mapping from the vertices of this simplex to the
         * vertices of \a other.
         * \return \c true if and only if, for every \a i,
         * <i>useDim</i>-face number \a i of this simplex has the same degree
         * as its image in \a other under the relabelling \a p.
         */
        template <int useDim>
        bool sameDegreesAt(const SimplexBase& other, Perm<dim+1> p) const;

        /**
         * Tests whether the <i>k</i>-face degrees of this and the given
         * simplex are identical, under the given relabelling, for all
         * faces whose dimensions are contained in the integer pack \a useDim.
         *
         * \param other the simplex to compare against this.
         * \param p a mapping from the vertices of this simplex to the
         * vertices of \a other.
         * \return \c true if and only if, for every \a i and every
         * facial dimension \a k in the integer pack \a useDim, <i>k</i>-face
         * number \a i of this simplex has the same degree as its image in
         * \a other under the relabelling \a p.
         */
        template <int... useDim>
        bool sameDegreesAt(const SimplexBase& other, Perm<dim+1> p,
            std::integer_sequence<int, useDim...>) const;

    friend class TriangulationBase<dim>;
    friend class Triangulation<dim>;
    friend class regina::XMLSimplexReader<dim>;
};

// Inline functions for SimplexBase

template <int dim>
inline SimplexBase<dim>::SimplexBase(Triangulation<dim>* tri) : tri_(tri) {
    for (int i = 0; i <= dim; ++i)
        adj_[i] = nullptr;
}

template <int dim>
inline SimplexBase<dim>::SimplexBase(const SimplexBase& clone,
        Triangulation<dim>* tri) :
        description_(clone.description_), locks_(clone.locks_), tri_(tri) {
    for (int i = 0; i <= dim; ++i)
        adj_[i] = nullptr;
}

template <int dim>
inline SimplexBase<dim>::SimplexBase(std::string desc,
        Triangulation<dim>* tri) :
        description_(std::move(desc)), tri_(tri) {
    for (int i = 0; i <= dim; ++i)
        adj_[i] = nullptr;
}

template <int dim>
inline const std::string& SimplexBase<dim>::description() const {
    return description_;
}

template <int dim>
inline void SimplexBase<dim>::setDescription(const std::string& desc) {
    tri_->takeSnapshot();
    typename Triangulation<dim>::ChangeEventSpan span(*tri_);
    description_ = desc;
}

template <int dim>
inline size_t SimplexBase<dim>::index() const {
    return markedIndex();
}

template <int dim>
inline Simplex<dim>* SimplexBase<dim>::adjacentSimplex(int facet) const {
    return adj_[facet];
}

template <int dim>
inline int SimplexBase<dim>::adjacentFacet(int facet) const {
    return gluing_[facet][facet];
}

template <int dim>
inline Perm<dim+1> SimplexBase<dim>::adjacentGluing(int face) const {
    return gluing_[face];
}

template <int dim>
inline Triangulation<dim>& SimplexBase<dim>::triangulation() const {
    return *tri_;
}

template <int dim>
inline Component<dim>* SimplexBase<dim>::component() const {
    triangulation().ensureSkeleton();
    return component_;
}

template <int dim>
template <int subdim>
inline Face<dim, subdim>* SimplexBase<dim>::face(int face) const {
    static_assert(0 <= subdim && subdim < dim,
        "Simplex<dim>::face<subdim>() requires 0 <= subdim < dim.");
    triangulation().ensureSkeleton();
    return std::get<subdim>(faces_)[face];
}

template <int dim>
inline Face<dim, 0>* SimplexBase<dim>::vertex(int i) const {
    triangulation().ensureSkeleton();
    return std::get<0>(faces_)[i];
}

template <int dim>
inline Face<dim, 1>* SimplexBase<dim>::edge(int i) const {
    triangulation().ensureSkeleton();
    return std::get<1>(faces_)[i];
}

template <int dim>
inline Face<dim, 2>* SimplexBase<dim>::triangle(int i) const {
    static_assert(dim >= 3, "triangle() is only available "
        "for simplices of dimension >= 3.");
    triangulation().ensureSkeleton();
    return std::get<2>(faces_)[i];
}

template <int dim>
inline Face<dim, 3>* SimplexBase<dim>::tetrahedron(int i) const {
    static_assert(dim >= 4, "tetrahedron() is only available "
        "for simplices of dimension >= 4.");
    triangulation().ensureSkeleton();
    return std::get<3>(faces_)[i];
}

template <int dim>
inline Face<dim, 4>* SimplexBase<dim>::pentachoron(int i) const {
    static_assert(dim >= 5, "pentachoron() is only available "
        "for simplices of dimension >= 5.");
    triangulation().ensureSkeleton();
    return std::get<4>(faces_)[i];
}

template <int dim>
template <int subdim>
inline Perm<dim + 1> SimplexBase<dim>::faceMapping(int face) const {
    static_assert(0 <= subdim && subdim < dim,
        "Simplex<dim>::faceMapping<subdim>() requires 0 <= subdim < dim.");
    triangulation().ensureSkeleton();
    return std::get<subdim>(mappings_)[face];
}

template <int dim>
inline Perm<dim + 1> SimplexBase<dim>::vertexMapping(int face) const {
    triangulation().ensureSkeleton();
    return std::get<0>(mappings_)[face];
}

template <int dim>
inline Perm<dim + 1> SimplexBase<dim>::edgeMapping(int face) const {
    triangulation().ensureSkeleton();
    return std::get<1>(mappings_)[face];
}

template <int dim>
inline Perm<dim + 1> SimplexBase<dim>::triangleMapping(int face) const {
    static_assert(dim >= 3, "triangleMapping() is only available "
        "for simplices of dimension >= 3.");
    triangulation().ensureSkeleton();
    return std::get<2>(mappings_)[face];
}

template <int dim>
inline Perm<dim + 1> SimplexBase<dim>::tetrahedronMapping(int face) const {
    static_assert(dim >= 4, "tetrahedronMapping() is only available "
        "for simplices of dimension >= 4.");
    triangulation().ensureSkeleton();
    return std::get<3>(mappings_)[face];
}

template <int dim>
inline Perm<dim + 1> SimplexBase<dim>::pentachoronMapping(int face) const {
    static_assert(dim >= 5, "pentachoronMapping() is only available "
        "for simplices of dimension >= 5.");
    triangulation().ensureSkeleton();
    return std::get<4>(mappings_)[face];
}

template <int dim>
inline int SimplexBase<dim>::orientation() const {
    triangulation().ensureSkeleton();
    return orientation_;
}

template <int dim>
inline bool SimplexBase<dim>::facetInMaximalForest(int facet) const {
    triangulation().ensureSkeleton();
    return dualForest_ & (FacetMask(1) << facet);
}

template <int dim>
inline void SimplexBase<dim>::writeTextShort(std::ostream& out) const {
    out << dim << "-simplex " << index();

    int glued = 0;
    int facet, j;
    for (facet = dim; facet >= 0; --facet) {
        if (! adj_[facet])
            continue;

        out << (glued == 0 ? ": " : ", ");
        ++glued;

        for (j = 0; j <= dim; ++j)
            if (j != facet)
                out << regina::digit(j);
        out << " -> " << adj_[facet]->markedIndex() << " (";
        for (j = 0; j <= dim; ++j)
            if (j != facet)
                out << regina::digit(gluing_[facet][j]);
        out << ')';
    }

    if (glued == 0)
        out << ": all facets boundary";
}

template <int dim>
bool SimplexBase<dim>::hasBoundary() const {
    for (int i = 0; i <= dim; ++i)
        if (! adj_[i])
            return true;
    return false;
}

template <int dim>
void SimplexBase<dim>::isolate() {
    for (int i = 0; i <= dim; ++i)
        if (adj_[i])
            unjoin(i);
}

template <int dim>
inline void SimplexBase<dim>::lock() {
    static constexpr LockMask mask = (LockMask(1) << (dim + 1));
    if (! (locks_ & mask)) {
        tri_->takeSnapshot();
        typename Triangulation<dim>::ChangeEventSpan span(*tri_);

        locks_ |= mask;
    }
}

template <int dim>
void SimplexBase<dim>::lockFacet(int facet) {
    const LockMask mask = (LockMask(1) << facet);
    if (! (locks_ & mask)) {
        tri_->takeSnapshot();
        typename Triangulation<dim>::ChangeEventSpan span(*tri_);

        locks_ |= mask;

        if (adj_[facet]) {
            int adjFacet = gluing_[facet][facet];
            adj_[facet]->locks_ |= (LockMask(1) << adjFacet);
        }
    }
}

template <int dim>
inline void SimplexBase<dim>::unlock() {
    static constexpr LockMask mask = (LockMask(1) << (dim + 1));
    if (locks_ & mask) {
        tri_->takeSnapshot();
        typename Triangulation<dim>::ChangeEventSpan span(*tri_);

        locks_ &= ~mask;
    }
}

template <int dim>
void SimplexBase<dim>::unlockFacet(int facet) {
    const LockMask mask = (LockMask(1) << facet);
    if (locks_ & mask) {
        tri_->takeSnapshot();
        typename Triangulation<dim>::ChangeEventSpan span(*tri_);

        locks_ &= ~mask;

        if (adj_[facet]) {
            int adjFacet = gluing_[facet][facet];
            adj_[facet]->locks_ &= ~(LockMask(1) << adjFacet);
        }
    }
}

template <int dim>
void SimplexBase<dim>::unlockAll() {
    if (locks_) {
        tri_->takeSnapshot();
        typename Triangulation<dim>::ChangeEventSpan span(*tri_);

        locks_ = 0;

        for (int facet = 0; facet <= dim; ++facet)
            if (adj_[facet] && adj_[facet] != this) {
                int adjFacet = gluing_[facet][facet];
                adj_[facet]->locks_ &= ~(LockMask(1) << adjFacet);
            }
    }
}

template <int dim>
inline bool SimplexBase<dim>::isLocked() const {
    return (locks_ & (LockMask(1) << (dim + 1)));
}

template <int dim>
inline bool SimplexBase<dim>::isFacetLocked(int facet) const {
    return (locks_ & (LockMask(1) << facet));
}

template <int dim>
inline typename SimplexBase<dim>::LockMask SimplexBase<dim>::lockMask() const {
    return locks_;
}

template <int dim>
Simplex<dim>* SimplexBase<dim>::unjoin(int myFacet) {
    if (! adj_[myFacet])
        return nullptr;

    tri_->takeSnapshot();
    typename Triangulation<dim>::ChangeEventSpan span(*tri_);

    Simplex<dim>* you = adj_[myFacet];
    int yourFacet = gluing_[myFacet][myFacet];
    assert(you->adj_[yourFacet] == this);
    you->adj_[yourFacet] = nullptr;
    adj_[myFacet] = nullptr;

    tri_->clearAllProperties();
    return you;
}

template <int dim>
void SimplexBase<dim>::join(int myFacet, Simplex<dim>* you,
        Perm<dim+1> gluing) {
    if (tri_ != you->tri_)
        throw InvalidArgument("You cannot join simplices from "
            "two different triangulations");

    int yourFacet = gluing[myFacet];

    if (adj_[myFacet] || you->adj_[yourFacet])
        throw InvalidArgument("You cannot join facets of simplices that "
            "are already joined to something");
    if (you == this && yourFacet == myFacet)
        throw InvalidArgument("You cannot join a facet of a simplex to itself");

    tri_->takeSnapshot();
    typename Triangulation<dim>::ChangeEventSpan span(*tri_);

    adj_[myFacet] = you;
    gluing_[myFacet] = gluing;
    you->adj_[yourFacet] = static_cast<Simplex<dim>*>(this);
    you->gluing_[yourFacet] = gluing.inverse();

    tri_->clearAllProperties();
}

template <int dim>
void SimplexBase<dim>::writeTextLong(std::ostream& out) const {
    out << dim << "-simplex " << index();
    if (! description_.empty())
        out << ": " << description_;
    out << std::endl;

    int facet, j;
    for (facet = dim; facet >= 0; --facet) {
        for (j = 0; j <= dim; ++j)
            if (j != facet)
                out << regina::digit(j);
        out << " -> ";
        if (! adj_[facet])
            out << "boundary";
        else {
            out << adj_[facet]->markedIndex() << " (";
            for (j = 0; j <= dim; ++j)
                if (j != facet)
                    out << regina::digit(gluing_[facet][j]);
            out << ')';
        }
        out << std::endl;
    }
}

template <int dim>
template <int useDim>
inline bool SimplexBase<dim>::sameDegreesAt(
        const SimplexBase<dim>& other, Perm<dim + 1> p) const {
    for (int i = 0; i < FaceNumbering<dim, useDim>::nFaces; ++i) {
        int j = FaceNumbering<dim, useDim>::faceNumber(
            p * FaceNumbering<dim, useDim>::ordering(i));
        if (std::get<useDim>(faces_)[i]->degree() !=
                std::get<useDim>(other.faces_)[j]->degree())
            return false;
    }
    return true;
}

template <int dim>
template <int... useDim >
inline bool SimplexBase<dim>::sameDegreesAt(const SimplexBase& other,
        Perm<dim+1> p, std::integer_sequence<int, useDim...>) const {
    return (sameDegreesAt<useDim>(other, p) && ...);
}

} } // namespace regina::detail

#endif

