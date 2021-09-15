
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file angle/anglestructures.h
 *  \brief Contains a packet representing a collection of angle
 *  structures on a triangulation.
 */

#ifndef __REGINA_ANGLESTRUCTURES_H
#ifndef __DOXYGEN
#define __REGINA_ANGLESTRUCTURES_H
#endif

#include <algorithm>
#include <iterator>
#include <optional>
#include <vector>
#include "regina-core.h"
#include "angle/anglestructure.h"
#include "maths/matrix.h"
#include "packet/packet.h"

namespace regina {

class AngleStructures;
class ProgressTracker;
class XMLAngleStructuresReader;
class XMLLegacyAngleStructuresReader;

/**
 * A packet representing a collection of angle structures on a triangulation.
 *
 * There are some important changes to this class as of Regina 7.0:
 *
 * - An angle structure list does \e not need to be a child packet of the
 *   underlying triangulation, and indeed does not need to interact with
 *   the packet tree at all.
 *
 * - You are welcome to modify or even destroy the original triangulation;
 *   if you do then this list will automatically make a private copy of the
 *   original triangulation as an ongoing reference.  Different angle
 *   structure lists (and normal surface lists) can all share the same
 *   private copy, so this is not an expensive process.
 *
 * - You should now create angle structure lists using the class constructor
 *   (but which, unlike the old enumerate(), does not insert the list
 *   into the packet tree).  There is no need to use enumerate() any more.
 *
 * \ingroup angle
 */
class AngleStructures : public Packet {
    REGINA_PACKET(AngleStructures, PACKET_ANGLESTRUCTURES,
        "Angle Structure List")

    private:
        std::vector<AngleStructure> structures_;
            /**< Contains all angle structures in this list. */

        SnapshotRef<Triangulation<3>> triangulation_;
            /**< The triangulation on which these angle structures lie. */

        bool tautOnly_;
            /**< Stores whether we are only interested in taut structures.
                 This is an option selected by the user before enumeration
                 takes place. */

        AngleAlg algorithm_;
            /**< Stores the details of the enumeration algorithm that was used
                 to generate this list.  This might not be the same as the
                 \a algHints flag that was originally passed to the class
                 constructor (e.g., if invalid or inappropriate flags were
                 passed). */

        mutable std::optional<bool> doesSpanStrict_;
            /**< Does the convex span of this list include a strict
                 angle structure?
                 This is determined by looking at the output angle structues
                 after enumeration has taken place.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> doesSpanTaut_;
            /**< Does this list include a taut structure?
                 This is determined by looking at the output angle structues
                 after enumeration has taken place.
                 This is std::nullopt if it has not yet been computed. */

    public:
        /**
         * A unified constructor for enumerating various classes of
         * angle structures on a given triangulation.
         *
         * If \a tautOnly is \c false (the default), then this new list will
         * be filled with all vertices of the angle structure solution space.
         * If \c tautOnly is \c true, then the list will be filled with only
         * the taut angle structures (a subset of the vertex angle structures);
         * these are usually much faster to enumerate.
         *
         * The AngleAlg argument is a combination of flags that allows
         * you to control the underlying enumeration algorithm.  These
         * flags are treated as hints only: if your selection of
         * algorithm is invalid, unavailable or unsupported then Regina
         * will choose something more appropriate.  Unless you have
         * some specialised need, the default AS_ALG_DEFAULT (which
         * makes no hints at all) will allow Regina to choose what it
         * thinks will be the most efficient method.
         *
         * Unlike the old enumerate() function, the new angle structure
         * list will \e not be inserted into the packet tree.  Moreover,
         * the given triangulation may change or even be destroyed
         * without causing problems.  See the class notes for details.
         *
         * If a progress tracker is passed:
         *
         * - The angle structure enumeration will take place in a new thread.
         *   This constructor will return immediately.
         *
         * - For progress tracking, this routine will declare and work through
         *   a series of stages whose combined weights sum to 1; typically this
         *   means that the given tracker must not have been used before.
         *
         * If no progress tracker is passed, the enumeration will run
         * in the current thread and this constructor will return only when
         * the enumeration is complete.  Note that this enumeration can
         * be extremely slow for larger triangulations.
         *
         * @param triangulation the triangulation for which the vertex
         * angle structures will be enumerated.
         * @param tautOnly \c true if only taut structures are to be
         * enuemrated, or \c false if we should enumerate all vertices
         * of the angle structure solution space.
         * @param algHints passes requests to Regina for which specific
         * enumeration algorithm should be used.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         */
        AngleStructures(const Triangulation<3>& triangulation,
            bool tautOnly = false, AngleAlg algHints = AS_ALG_DEFAULT,
            ProgressTracker* tracker = nullptr);

        /**
         * Returns the triangulation on which these angle structures lie.
         *
         * This will be a snapshot frozen in time of the triangulation
         * that was originally passed to the AngleStructures constructor.
         *
         * This will return a correct result even if the original triangulation
         * has since been modified or destroyed.  However, in order to ensure
         * this behaviour, it is possible that at different points in time
         * this function may return references to different C++ objects.
         *
         * The rules for using the triangulation() reference are:
         *
         * - Do not keep the resulting reference as a long-term reference or
         *   pointer of your own, since in time you may find yourself referring
         *   to the wrong object (see above).  Just call this function again.
         *
         * - You must respect the read-only nature of the result (i.e.,
         *   you must not cast the constness away).  The snapshotting
         *   process detects modifications, and modifying the frozen
         *   snapshot may result in an exception being thrown.
         *
         * \warning As of Regina 7.0, you \e cannot access this triangulation
         * via the packet tree as parent().  This is because angle structure
         * lists can now be kept anywhere in the packet tree, or can be kept
         * as standalone objects outside the packet tree entirely.
         *
         * @return a reference to the underlying triangulation.
         */
        const Triangulation<3>& triangulation() const;

        /**
         * Returns whether this list was produced by enumerating taut angle
         * structures only.
         *
         * @return \c true if this list was produced by enumerating
         * taut angle structures only, or \c false if the enumeration
         * procedure allowed for any angle structures.
         */
        bool isTautOnly() const;

        /**
         * Returns details of the algorithm that was used to enumerate
         * this list.
         *
         * These may not be the same AngleAlg flags that were passed to
         * the class constructor.  In particular, default values will have been
         * explicitly filled in, invalid and/or redundant values will have
         * been removed, and unavailable and/or unsupported combinations
         * of algorithm flags will be replaced with whatever algorithm was
         * actually used.
         *
         * @return details of the algorithm used to enumerate this list.
         */
        AngleAlg algorithm() const;

        /**
         * Returns the number of angle structures stored in this list.
         *
         * @return the number of angle structures.
         */
        size_t size() const;
        /**
         * Returns the angle structure at the requested index in this list.
         *
         * @param index the index of the requested angle structure in
         * this list; this must be between 0 and size()-1 inclusive.
         * @return the angle structure at the requested index.
         */
        const AngleStructure& structure(size_t index) const;

        /**
         * Returns an iterator at the beginning of this list of angle
         * structures.
         *
         * The begin() and end() functions allow you to iterate through all
         * angle structures in this list using C++11 range-based \c for loops:
         *
         * \code{.cpp}
         * AngleStructures list(...);
         * for (const AngleStructure& s : list) { ... }
         * \endcode
         *
         * In Python, an angle structure list can be treated as an iterable
         * object:
         *
         * \code{.py}
         * list = AngleStructures(...)
         * for s in list:
         *     ...
         * \endcode
         *
         * The type that is returned will be a lightweight iterator type,
         * guaranteed to satisfy the C++ LegacyRandomAccessIterator requirement.
         * The precise C++ type of the iterator is subject to change, so
         * C++ users should use \c auto (just like this declaration does).
         *
         * @return an iterator at the beginning of this list.
         */
        auto begin() const;
        /**
         * Returns an iterator beyond the end of this list of angle structures.
         *
         * In C++, the begin() and end() routines allow you to iterate through
         * all angle structures in this list using C++11 range-based \c for
         * loops.  In Python, an angle structure list can be treated as an
         * iterable object.
         *
         * See the begin() documentation for further details.
         *
         * @return an iterator beyond the end of this list.
         */
        auto end() const;

        /**
         * Determines whether any convex combination of the angle
         * structures in this list is a strict angle structure.
         * See AngleStructure::isStrict() for details on strict angle
         * structures.
         *
         * @return \c true if and only if a strict angle structure can
         * be produced.
         */
        bool spansStrict() const;
        /**
         * Determines whether any angle structure in this list is a
         * taut structure.  Because taut structures always appear as
         * vertices of the angle structure solution space, this routine
         * is equivalent to testing whether any convex combination of
         * the angle structures in this list is a taut structure.
         *
         * See AngleStructure::isTaut() for details on taut structures.
         *
         * @return \c true if and only if a taut structure can be produced.
         */
        bool spansTaut() const;

        /**
         * Deprecated routine to enumerate angle structures on a given
         * triangulation.
         *
         * This static routine is almost identical to calling the class
         * constructor with the given arguments.  The only difference is
         * that, unlike the class constructor, this routine will also insert
         * the angle structure list beneath \a owner in the packet tree.
         * If a progress tracker is passed (which means the enumeration runs
         * in a background thread), the tree insertion will not happen until
         * the enumeration has finished (and if the user cancels the operation,
         * the insertion will not happen at all).
         *
         * See the class constructor for details on how this routine
         * works and what the various arguments mean.
         *
         * \deprecated Just call the AngleStructures constructor.
         *
         * \ifacespython For this deprecated function, the progress tracker
         * argument is omitted.  It is still possible to enumerate in
         * the background with a progress tracker, but for that you will
         * need to call the class constructor instead.
         *
         * @param owner the triangulation for which the vertex
         * angle structures will be enumerated.
         * @param tautOnly \c true if only taut structures are to be
         * enuemrated, or \c false if we should enumerate all vertices
         * of the angle structure solution space; this defaults to \c false.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return the newly created angle structure list.
         */
        [[deprecated]] static AngleStructures* enumerate(
            Triangulation<3>& owner, bool tautOnly = false,
            ProgressTracker* tracker = nullptr);

        /**
         * Deprecated routine to enumerate all taut angle structures
         * on the given triangulation using the double description method.
         *
         * This static routine is almost identical to calling the class
         * constructor with the \c tautOnly argument set to \c true and
         * the \a algHints argument set to AS_ALG_DD.  The only difference is
         * that, unlike the class constructor, this routine will also insert
         * the angle structure list beneath \a owner in the packet tree.
         *
         * Note that default algorithm used by the class constructor for
         * taut angle structures is the tree traversal method, which is
         * based on linear programming, and which is typically \e much faster
         * than the double description method for larger triangulations.
         *
         * \warning This routine is slow, and users will not want to call it
         * unless they have some specialised need.
         *
         * \deprecated Just call the AngleStructures constructor.
         *
         * @param owner the triangulation for which the taut angle structures
         * will be enumerated.
         * @return the newly created angle structure list.
         */
        [[deprecated]] static AngleStructures* enumerateTautDD(
            Triangulation<3>& owner);

        virtual void writeTextShort(std::ostream& out) const override;
        virtual void writeTextLong(std::ostream& out) const override;

    protected:
        /**
         * Creates a new empty angle structure list.
         * All properties are marked as unknown.
         *
         * @param tautOnly \c true if only taut structures are to be
         * enuemrated (when the time comes for enumeration to be performed),
         * or \c false if we should enumerate all vertices of the angle
         * structure solution space.
         * @param algHints contains requests for which specific
         * enumeration algorithm should be used.
         * @param triangulation the triangulation on which the angle
         * structures will lie.
         */
        AngleStructures(bool tautOnly, AngleAlg algHints,
            const Triangulation<3>& triangulation);

        virtual Packet* internalClonePacket(Packet* parent) const override;
        virtual void writeXMLPacketData(std::ostream& out,
            FileFormat format, bool anon, PacketRefs& refs) const override;
        virtual void addPacketRefs(PacketRefs& refs) const override;

        /**
         * Calculate whether the convex span of this list includes a
         * strict angle structure.
         */
        void calculateSpanStrict() const;
        /**
         * Calculate whether the convex span of this list includes a
         * taut structure.
         */
        void calculateSpanTaut() const;

    private:
        /**
         * The main code that actually performs the angle structure enumeration.
         *
         * \pre This list is empty (i.e., contains no angle structures),
         * but all of its enumeration parameters have been set.
         *
         * @param tracker the progress tracker to use for progress
         * reporting and cancellation polling, or \c null if these
         * capabilities are not required.
         * @param treeParent the parent packet in the tree, if we should
         * insert the angle structure list into the packet tree once the
         * enumeration has finished, or \c null if we should not.
         */
        void enumerateInternal(ProgressTracker* tracker,
            Packet* treeParent);

    friend class regina::XMLAngleStructuresReader;
    friend class regina::XMLLegacyAngleStructuresReader;
};

/**
 * Generates the set of angle structure equations for the given triangulation.
 *
 * These are the angle equations that will be used when enumerating
 * angle structures on the given triangulation.
 *
 * Each equation will be represented as a row of the resulting matrix, and each
 * column will represent a coordinate in the underlying coordinate system
 * (which is described in the notes for AngleStructure::vector()).
 *
 * @param tri the triangulation upon which these angle structure
 * equations will be based.
 * @return the resulting set of angle structure equations.
 *
 * \ingroup angle
 */
MatrixInt makeAngleEquations(const Triangulation<3>& tri);

// Inline functions for AngleStructures

inline const Triangulation<3>& AngleStructures::triangulation() const {
    return *triangulation_;
}

inline bool AngleStructures::isTautOnly() const {
    return tautOnly_;
}

inline AngleAlg AngleStructures::algorithm() const {
    return algorithm_;
}

inline size_t AngleStructures::size() const {
    return structures_.size();
}

inline const AngleStructure& AngleStructures::structure(size_t index) const {
    return structures_[index];
}

inline auto AngleStructures::begin() const {
    return structures_.begin();
}

inline auto AngleStructures::end() const {
    return structures_.end();
}

inline bool AngleStructures::spansStrict() const {
    if (! doesSpanStrict_.has_value())
        calculateSpanStrict();
    return *doesSpanStrict_;
}

inline bool AngleStructures::spansTaut() const {
    if (! doesSpanTaut_.has_value())
        calculateSpanTaut();
    return *doesSpanTaut_;
}

inline AngleStructures::AngleStructures(bool tautOnly, AngleAlg algHints,
        const Triangulation<3>& triangulation) :
        triangulation_(triangulation), tautOnly_(tautOnly),
        algorithm_(algHints) {
}

} // namespace regina

#endif

