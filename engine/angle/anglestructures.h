
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

/*! \file angle/anglestructures.h
 *  \brief Contains a packet representing a collection of angle
 *  structures on a triangulation.
 */

#ifndef __ANGLESTRUCTURES_H
#ifndef __DOXYGEN
#define __ANGLESTRUCTURES_H
#endif

#include <algorithm>
#include <iterator>
#include <vector>
#include "regina-core.h"
#include "angle/anglestructure.h"
#include "packet/packet.h"
#include "utilities/memutils.h"
#include "utilities/property.h"

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
    inline static const char* name() {
        return "Angle Structure List";
    }
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
class REGINA_API AngleStructures : public Packet {
    REGINA_PACKET(AngleStructures, PACKET_ANGLESTRUCTURES)

    private:
        std::vector<AngleStructure*> structures;
            /**< Contains the angle structures stored in this packet. */
        bool tautOnly_;
            /**< Stores whether we are only interested in taut structures.
                 This is an option selected by the user before enumeration
                 takes place. */

        mutable Property<bool> doesSpanStrict;
            /**< Does the convex span of this list include a strict
                 angle structure?
                 This is determined by looking at the output angle structues
                 after enumeration has taken place. */
        mutable Property<bool> doesSpanTaut;
            /**< Does this list include a taut structure?
                 This is determined by looking at the output angle structues
                 after enumeration has taken place. */

    public:
        /**
         * Destroys this list and all the angle structures within.
         */
        virtual ~AngleStructures();

        /**
         * Returns the triangulation on which these angle structures lie.
         *
         * @return the corresponding triangulation.
         */
        Triangulation<3>* triangulation() const;

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
         * Returns the angle structure at the requested index in this
         * list.
         *
         * @param index the index of the requested angle structure in
         * this list; this must be between 0 and size()-1 inclusive.
         * @return the angle structure at the requested index.
         */
        const AngleStructure* structure(size_t index) const;

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
         * be reported, or 0 if no progress reporting is required.
         * @return the newly created angle structure list.  Note that if
         * a progress tracker is passed then this list may not be completely
         * filled when this routine returns.  If a progress tracker is
         * passed and a new thread could not be started, this routine
         * returns 0 (and no angle structure list is created).
         */
        static AngleStructures* enumerate(Triangulation<3>* owner,
            bool tautOnly = false, ProgressTracker* tracker = 0);

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
        static AngleStructures* enumerateTautDD(Triangulation<3>* owner);

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static XMLPacketReader* xmlReader(Packet* parent,
            XMLTreeResolver& resolver);
        virtual bool dependsOnParent() const;

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

        virtual Packet* internalClonePacket(Packet* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

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
         * AngleStructures.
         *
         * Objects of type <tt>AngleStructure*</tt> and
         * <tt>AngleStructureVector*</tt> can be assigned to this
         * iterator.  In the latter case, a surrounding AngleStructure
         * will be automatically created.
         */
        struct StructureInserter : public std::iterator<
                std::output_iterator_tag, AngleStructureVector*> {
            AngleStructures* list;
                /**< The list into which angle structures will be inserted. */
            Triangulation<3>* owner;
                /**< The triangulation on which the angle structures to
                 *   be inserted lie. */

            /**
             * Creates a new uninitialised output iterator.
             *
             * \warning This iterator must not be used until its
             * structure list and triangulation have been initialised.
             */
            StructureInserter();
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
                Triangulation<3>* newOwner);
            /**
             * Creates a new output iterator that is a clone of the
             * given iterator.
             *
             * @param cloneMe the output iterator to clone.
             */
            StructureInserter(const StructureInserter& cloneMe);

            /**
             * Sets this iterator to be a clone of the given output iterator.
             *
             * @param cloneMe the output iterator to clone.
             * @return this output iterator.
             */
            StructureInserter& operator =(const StructureInserter& cloneMe);

            /**
             * Appends an angle structure to the end of the appropriate
             * structure list.
             *
             * The given angle structure will be deallocated with the
             * other angle structures in this list when the list is
             * eventually destroyed.
             *
             * @param structure the angle structure to insert.
             * @return this output iterator.
             */
            StructureInserter& operator =(AngleStructure* structure);
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
            StructureInserter& operator =(AngleStructureVector* vector);

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
         * reporting and cancellation polling, or 0 if these
         * capabilities are not required.
         */
        void enumerateInternal(Triangulation<3>* triang,
            ProgressTracker* tracker = 0);

    friend class regina::XMLAngleStructuresReader;
};

/**
 * Creates a new set of angle structure equations for the given triangulation.
 *
 * Each equation will be represented as a row of the matrix, and
 * each column will represent a coordinate in the underlying
 * coordinate system (which is described in the AngleStructureVector
 * class notes).
 *
 * The returned matrix will be newly allocated and its destruction
 * will be the responsibility of the caller of this routine.
 *
 * This routine is identical to the static class method
 * AngleStructureVector::makeAngleEquations().  It is offered again here
 * as a global routine so that it is accessible to Python users (who cannot
 * access the AngleStructureVector class).
 *
 * @param tri the triangulation upon which these angle structure
 * equations will be based.
 * @return a newly allocated set of equations.
 */
REGINA_API MatrixInt* makeAngleEquations(const Triangulation<3>* tri);

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NAngleStructureList has now been renamed to
 * AngleStructures.
 */
REGINA_DEPRECATED typedef AngleStructures NAngleStructureList;

/*@}*/

// Inline functions for AngleStructures

inline AngleStructures::~AngleStructures() {
    for_each(structures.begin(), structures.end(),
        FuncDelete<AngleStructure>());
}

inline bool AngleStructures::isTautOnly() const {
    return tautOnly_;
}

inline size_t AngleStructures::size() const {
    return structures.size();
}

inline const AngleStructure* AngleStructures::structure(
        size_t index) const {
    return structures[index];
}

inline bool AngleStructures::spansStrict() const {
    if (! doesSpanStrict.known())
        calculateSpanStrict();
    return doesSpanStrict.value();
}

inline bool AngleStructures::spansTaut() const {
    if (! doesSpanTaut.known())
        calculateSpanTaut();
    return doesSpanTaut.value();
}

inline bool AngleStructures::dependsOnParent() const {
    return true;
}

inline AngleStructures::AngleStructures(bool tautOnly) :
        tautOnly_(tautOnly) {
}

inline AngleStructures::StructureInserter::StructureInserter() : list(0),
        owner(0) {
}

inline AngleStructures::StructureInserter::StructureInserter(
        AngleStructures& newList, Triangulation<3>* newOwner) :
        list(&newList), owner(newOwner) {
}

inline AngleStructures::StructureInserter::StructureInserter(
        const StructureInserter& cloneMe) : list(cloneMe.list),
        owner(cloneMe.owner) {
}

inline AngleStructures::StructureInserter&
        AngleStructures::StructureInserter::operator =(
        const StructureInserter& cloneMe) {
    list = cloneMe.list;
    owner = cloneMe.owner;
    return *this;
}

inline AngleStructures::StructureInserter&
        AngleStructures::StructureInserter::operator =(
        AngleStructure* structure) {
    list->structures.push_back(structure);
    return *this;
}

inline AngleStructures::StructureInserter&
        AngleStructures::StructureInserter::operator =(
        AngleStructureVector* vector) {
    list->structures.push_back(new AngleStructure(owner, vector));
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

inline MatrixInt* makeAngleEquations(const Triangulation<3>* tri) {
    return AngleStructureVector::makeAngleEquations(tri);
}

} // namespace regina

#endif

