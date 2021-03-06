
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
#include "utilities/listview.h"

namespace regina {

class AngleStructures;
class ProgressTracker;
class XMLPacketReader;
class XMLAngleStructuresReader;

/**
 * \weakgroup angle
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_ANGLESTRUCTURES> {
    typedef AngleStructures Class;
    static constexpr const char* name = "Angle Structure List";
};
#endif

/**
 * A packet representing a collection of angle structures on a triangulation.
 * Such a packet must always be a child packet of the triangulation on
 * which the angle structures lie.  If this triangulation changes, the
 * information contained in this packet will become invalid.
 *
 * Angle structure lists should be created using the routine enumerate(),
 * which is new as of Regina 3.95.
 */
class AngleStructures : public Packet {
    REGINA_PACKET(AngleStructures, PACKET_ANGLESTRUCTURES)

    private:
        std::vector<AngleStructure> structures_;
            /**< Contains the angle structures stored in this packet. */
        bool tautOnly_;
            /**< Stores whether we are only interested in taut structures.
                 This is an option selected by the user before enumeration
                 takes place. */

        mutable std::optional<bool> doesSpanStrict_;
            /**< Does the convex span of this list include a strict
                 angle structure?
                 This is determined by looking at the output angle structues
                 after enumeration has taken place. */
        mutable std::optional<bool> doesSpanTaut_;
            /**< Does this list include a taut structure?
                 This is determined by looking at the output angle structues
                 after enumeration has taken place. */

    public:
        /**
         * Returns the triangulation on which these angle structures lie.
         *
         * The triangulation is also accessible via the packet tree as
         * parent(); this routine simply adds the convenience of casting
         * down to the correct triangulation class.
         *
         * If you need non-const access to the triangulation (e.g., to
         * rename the packet), use parent(); however, remember that a
         * triangulation that owns angle structures or normal surfaces
         * must \e not change its tetrahedra or their gluings.
         *
         * @return a reference to the corresponding triangulation.
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
         * Returns the number of angle structures stored in this list.
         *
         * @return the number of angle structures.
         */
        size_t size() const;
        /**
         * Returns an object that allows iteration through and random access
         * to all angle structures in this list.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * C++11 range-based \c for loops.  Note that the elements of the list
         * will be pointers, so your code might look like:
         *
         * \code{.cpp}
         * for (const AngleStructure* s : list.structures()) { ... }
         * \endcode
         *
         * The object that is returned will remain valid only for as
         * long as this angle structure list exists.
         *
         * \ifacespython This routine returns a Python list.
         *
         * @return access to the list of all angle structures.
         */
        auto structures() const;
        /**
         * Returns the angle structure at the requested index in this
         * list.
         *
         * @param index the index of the requested angle structure in
         * this list; this must be between 0 and size()-1 inclusive.
         * @return the angle structure at the requested index.
         */
        const AngleStructure& structure(size_t index) const;

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
         * See AngleStructure::isTaut() for details on taut
         * structures.
         *
         * @return \c true if and only if a taut structure can be produced.
         */
        bool spansTaut() const;

        /**
         * Enumerates all angle structures on the given triangulation.
         * A list containing all vertices of the angle structure solution
         * space will be returned.
         *
         * The option is offered to find only taut structures (which are
         * considerably faster to enumerate) instead of enumerating all
         * vertex angle structures.  See the \a tautOnly argument below.
         *
         * The angle structure list that is created will be inserted as the
         * last child of the given triangulation.  This triangulation \b must
         * remain the parent of this angle structure list, and must not
         * change while this angle structure list remains in existence.
         *
         * If a progress tracker is passed, the angle structure
         * enumeration will take place in a new thread and this routine
         * will return immediately.  If the user cancels the operation
         * from another thread, then the angle structure list will \e not
         * be inserted into the packet tree (but the caller of this
         * routine will still need to delete it).  Regarding progress tracking,
         * this routine will declare and work through a series of stages
         * whose combined weights sum to 1; typically this means that the
         * given tracker must not have been used before.
         *
         * If no progress tracker is passed, the enumeration will run
         * in the current thread and this routine will return only when
         * the enumeration is complete.  Note that this enumeration can
         * be extremely slow for larger triangulations.
         *
         * @param owner the triangulation for which the vertex
         * angle structures will be enumerated.
         * @param tautOnly \c true if only taut structures are to be
         * enuemrated, or \c false if we should enumerate all vertices
         * of the angle structure solution space; this defaults to \c false.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return the newly created angle structure list.  Note that if
         * a progress tracker is passed then this list may not be completely
         * filled when this routine returns.  If a progress tracker is
         * passed and a new thread could not be started, this routine
         * returns \c null (and no angle structure list is created).
         */
        static AngleStructures* enumerate(Triangulation<3>& owner,
            bool tautOnly = false, ProgressTracker* tracker = nullptr);

        /**
         * A slower, alternative method to enumerate all taut angle structures
         * on the given triangulation.  A list containing all taut angle
         * structures will be returned.
         *
         * The algorithm used by this routine is based on the double
         * description method.  The algorithm used by enumerate() (the
         * preferred method) is instead based on linear programming, and is
         * typically \e much faster than this routine for larger triangulations.
         *
         * The angle structure list that is created will be inserted as the
         * last child of the given triangulation.  This triangulation \b must
         * remain the parent of this angle structure list, and must not
         * change while this angle structure list remains in existence.
         *
         * \warning This routine is slow, and users will not want to call it
         * unless they have some specialised need.
         *
         * @param owner the triangulation for which the taut angle structures
         * will be enumerated.
         * @return the newly created angle structure list.
         */
        static AngleStructures* enumerateTautDD(Triangulation<3>& owner);

        virtual void writeTextShort(std::ostream& out) const override;
        virtual void writeTextLong(std::ostream& out) const override;
        static XMLPacketReader* xmlReader(Packet* parent,
            XMLTreeResolver& resolver);
        virtual bool dependsOnParent() const override;

    protected:
        /**
         * Creates a new empty angle structure list.
         * All properties are marked as unknown.
         *
         * @param tautOnly \c true if only taut structures are to be
         * enuemrated (when the time comes for enumeration to be performed),
         * or \c false if we should enumerate all vertices of the angle
         * structure solution space.
         */
        AngleStructures(bool tautOnly);

        virtual Packet* internalClonePacket(Packet* parent) const override;
        virtual void writeXMLPacketData(std::ostream& out) const override;

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

        /**
         * An output iterator used to insert angle structures into an
         * AngleStructures list.
         *
         * Objects of type <tt>VectorInt*</tt> can be assigned to this
         * iterator, whereupon a surrounding AngleStructure will be
         * automatically created.
         */
        struct StructureInserter : public std::iterator<
                std::output_iterator_tag, VectorInt*> {
            AngleStructures* list;
                /**< The list into which angle structures will be inserted. */
            const Triangulation<3>& owner;
                /**< The triangulation on which the angle structures to
                 *   be inserted lie. */

            /**
             * Creates a new output iterator.  The member variables of
             * this iterator will be initialised according to the
             * parameters passed to this constructor.
             *
             * @param newList the list into which angle structures will
             * be inserted.
             * @param newOwner the triangulation on which the structures
             * to be inserted lie.
             */
            StructureInserter(AngleStructures& newList,
                const Triangulation<3>& newOwner);
            /**
             * Creates a new output iterator that is a clone of the
             * given iterator.
             *
             * @param cloneMe the output iterator to clone.
             */
            StructureInserter(const StructureInserter& cloneMe) = default;

            /**
             * Appends the angle structure corresponding to the given
             * vector to the end of the appropriate structure list.
             *
             * The given vector will be owned by the newly created
             * angle structure and will be deallocated with the
             * other angle structures in this list when the list is
             * eventually destroyed.
             *
             * @param vector the vector of the angle structure to insert.
             * @return this output iterator.
             */
            StructureInserter& operator =(VectorInt* vector);

            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            StructureInserter& operator *();
            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            StructureInserter& operator ++();
            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            StructureInserter& operator ++(int);

            StructureInserter& operator =(const StructureInserter& cloneMe)
                = delete;
        };

    private:
        /**
         * The main code that actually performs the angle structure
         * enumeration.
         *
         * \pre This list is empty (i.e., contains no angle structures),
         * but all of its enumeration parameters have been set.
         * \pre This list has not yet been inserted into the packet tree.
         *
         * @param triang the triangulation upon which this angle
         * structure list will be based.
         * @param tracker the progress tracker to use for progress
         * reporting and cancellation polling, or \c null if these
         * capabilities are not required.
         */
        void enumerateInternal(Triangulation<3>& triang,
            ProgressTracker* tracker = nullptr);

    friend class regina::XMLAngleStructuresReader;
};

/**
 * Generates the set of angle structure equations for the given triangulation.
 *
 * Each equation will be represented as a row of the resulting matrix, and each
 * column will represent a coordinate in the underlying coordinate system
 * (which is described in the notes for AngleStructure::vector()).
 *
 * @param tri the triangulation upon which these angle structure
 * equations will be based.
 * @return the resulting set of angle structure equations.
 */
MatrixInt makeAngleEquations(const Triangulation<3>& tri);

/*@}*/

// Inline functions for AngleStructures

inline bool AngleStructures::isTautOnly() const {
    return tautOnly_;
}

inline size_t AngleStructures::size() const {
    return structures_.size();
}

inline auto AngleStructures::structures() const {
    return ListView(structures_);
}

inline const AngleStructure& AngleStructures::structure(
        size_t index) const {
    return structures_[index];
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

inline bool AngleStructures::dependsOnParent() const {
    return true;
}

inline AngleStructures::AngleStructures(bool tautOnly) :
        tautOnly_(tautOnly) {
}

inline AngleStructures::StructureInserter::StructureInserter(
        AngleStructures& newList, const Triangulation<3>& newOwner) :
        list(&newList), owner(newOwner) {
}

inline AngleStructures::StructureInserter&
        AngleStructures::StructureInserter::operator =(
        VectorInt* vector) {
    list->structures_.push_back(AngleStructure(owner, vector));
    return *this;
}

inline AngleStructures::StructureInserter&
        AngleStructures::StructureInserter::operator *() {
    return *this;
}

inline AngleStructures::StructureInserter&
        AngleStructures::StructureInserter::operator ++() {
    return *this;
}

inline AngleStructures::StructureInserter&
        AngleStructures::StructureInserter::operator ++(int) {
    return *this;
}

} // namespace regina

#endif

