
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

/*! \file utilities/topologylock.h
 *  \brief Support for "topology locks" on objects such as triangulations or
 *  links.
 */

#ifndef __REGINA_TOPOLOGYLOCK_H
#ifndef __DOXYGEN
#define __REGINA_TOPOLOGYLOCK_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * A base class for objects that support topology locks, such as
 * triangulations or links.
 *
 * Essentially, a "topology lock" means the following.  In routines that would
 * normally clear computed properties of an object (e.g., operations that edit
 * a triangulation or a link diagram), if there are any topology locks
 * currently held on that object then properties that are purely topological
 * will be preserved.  This is essentially an optimisation that allows us to
 * avoid recomputing expensive invariants when we know in advance they will
 * not change.
 *
 * By a "purely topological" property, we mean the following:
 *
 * - For triangulations, these are properties of the underlying manifold,
 *   and must be invariant under reflection.  Examples include homology,
 *   fundamental group, and Turaev-Viro invariants of 3-manifolds.
 *   Non-examples include zero-efficiency of 3-manifold triangulations (which
 *   depends upon the specific triangulation), intersection forms of 4-manifolds
 *   (which can change under reflection), or validity of triangulations (since
 *   subdivision could change a triangulation from invalid to valid).
 *
 * - For links, these are properties of the underlying link, and they do _not_
 *   need to be invariant under reflection or reversal.  Examples include
 *   the Jones and HOMFLY-PT polynomials (even though these can change under
 *   reflection).  Non-examples include the bracket polynomial (which depends
 *   upon the specific diagram).
 *
 * This specific notion of "preserved properties" is subject to change in
 * future versions of Regina.
 *
 * Currently end users cannot access TopologyLockable or TopologyLock at all;
 * instead topology locks are restricted to member functions, since it is too
 * easy to accidentally abuse them and generate incorrect mathematical results
 * as a result.  This may change in a future release of Regina if a good way
 * is found to make them safer for end users.
 *
 * TopologyLockable should be used as a protected base class for objects that
 * support topology locks (e.g., both Triangulation<dim> and Link derive from
 * TopologyLockable).  To create a temporary topology lock on an object \a obj
 * (e.g., a triangulation or link), typically within a member function
 * (e.g., functions that implement Pachner or Reidemeister moves), you should
 * create a TopologyLock on the stack and pass \a obj to its constructor.
 *
 * Subclass implementations should remember:
 *
 * - TopologyLockable should be a _protected_ base class, since this machinery
 *   should not be directly accessible to end users.
 *
 * - This base class is not copyable, movable or swappable, since topology
 *   locks cannot be transferred from one object (e.g., triangulation or link)
 *   to another.  This means that subclass assignment or swap operations
 *   should not touch topology lock counts, and subclass copy/move constructors
 *   should always use the default TopologyLockable constructor that sets the
 *   lock count to zero.
 *
 * - In subclass "hook routines" that would normally clear computed properties
 *   when an object is modified (such as Link::clearAllProperties(),
 *   Triangulation<dim>::clearAllProperties() or
 *   TriangulationBase<dim>::clearBaseProperties()), these hook routines
 *   should query topologyLocked(), and if this returns \c true then they
 *   should _preserve_ any properties that are purely topological.
 *
 * /nopython
 */
class TopologyLockable {
    protected:
        uint8_t topologyLock_ { 0 };
            /**< The number of topology locks currently held on this object.
                 Any non-zero number of locks implies that "hook routines"
                 that clear computed properties (as described in the class
                 notes) will preserve properties that are purely topological. */

    public:
        /**
         * Default constructor that initialises the lock count to zero.
         */
        TopologyLockable() = default;

        // Locks cannot be transferred from one object to another.
        TopologyLockable(const TopologyLockable&) = delete;
        TopologyLockable& operator = (const TopologyLockable&) = delete;

    protected:
        /**
         * Returns whether or not there are any topology locks currently held
         * on this object.
         *
         * Strictly speaking, this routine could return a false negative:
         * the number of locks is stored as an 8-bit integer and so in reality
         * this tests whether the number of locks is a multiple of 256.
         * False negatives are mathematically harmless, since the worst that
         * will happen is that topological properties will be cleared when
         * they could have been preserved, and so unnecessary extra computation
         * may be required to compute them again.
         *
         * This routine will never return a false positive.
         *
         * \return \c false if there are no topology locks currently held on
         * this object, or if a false negative occurs (as described above); or
         * \c true to indicate that there are currently topology locks held on
         * this object.
         */
        bool topologyLocked() const {
            return (topologyLock_ != 0);
        }

        /**
         * Creates a temporary lock on the topological properties of the given
         * object.  See the TopologyLockable class notes for details on what a
         * "topology lock" means, and what effect it has upon the given object.
         *
         * This lock will be created by the class constructor, and removed by
         * the class destructor.  That is, the lock will remain in effect
         * until the TopologyLock object goes out of scope (or is otherwise
         * destroyed).
         *
         * The easiest way to create a TopologyLock is to have it automatically
         * rolled in to a Triangulation<dim>::ChangeAndClearSpan or a
         * Link::ChangeAndClearSpan, by declaring a local
         * ChangeAndClearSpan<ChangeType::PreserveTopology> on the stack
         * whose lifespan covers all of the modifications that you are making
         * to the triangulation or link.
         *
         * Alternatively, you can of course create a TopologyLock manually.
         * This would, again, be a local stack variable whose lifespan covers
         * all of your modifications to the given object.  In this case, the
         * TopologyLock lifespan would need to cover all points where computed
         * properties are cleared, such as:
         *
         * - modifying function calls, such as Simplex<dim>::join();
         * - ChangeAndClearSpan objects being destroyed; or
         * - manual calls to Triangulation<dim>::clearAllProperties() or
         *   Link::clearAllProperties().
         *
         * In particular, if you are declaring a TopologyLock separately from
         * a ChangeAndClearSpan, then the TopologyLock must be declared _first_
         * (so that it is still active when the ChangeAndClearSpan calls
         * clearAllProperties() in its destructor).
         *
         * Multiple locks are allowed.  If multiple locks are created, then
         * topological properties of the given object will be preserved as
         * long as any one of these locks still exists.  Multiple locks
         * do not necessarily need to be nested (i.e., the order of
         * destruction does not need to be the reverse order of construction).
         *
         * Regina is currently only able to handle 255 distinct locks on
         * the same triangulation at a time.  This should be enormously more
         * than enough (since external users cannot construct TopologyLock
         * objects, and Regina's own code should not be recursing deeply
         * inside TopologyLock scopes).  However, even if there are somehow
         * more than 255 locks, the worst that will happen is some CPU wastage:
         * some properties may be cleared and need to be recomputed when this
         * was not mathematically necessary.
         *
         * TopologyLock objects are not copyable, movable or swappable.
         * In particular, Regina does not offer any way for a TopologyLock
         * to transfer its destructor's responsibilities (i.e., "unlocking"
         * the topological properties of the triangulation) to another object.
         *
         * \nopython
         */
        class TopologyLock {
            private:
                TopologyLockable& obj_;
                    /**< The object whose topological properties are being
                         locked. */

            public:
                /**
                 * Creates a new lock on the given object.
                 *
                 * \param obj the object (e.g., a triangulation or link) whose
                 * topological properties are to be locked.
                 */
                TopologyLock(TopologyLockable& obj) : obj_(obj) {
                    ++obj_.topologyLock_;
                }

                /**
                 * Removes this lock on the associated object.
                 */
                ~TopologyLock() {
                    --obj_.topologyLock_;
                }

                // Make this class non-copyable.
                TopologyLock(const TopologyLock&) = delete;
                TopologyLock& operator = (const TopologyLock&) = delete;
        };
};

} // namespace regina

#endif
