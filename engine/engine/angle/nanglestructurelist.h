
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nanglestructurelist.h
 *  \brief Contains a packet representing a collection of angle
 *  structures on a triangulation.
 */

#ifndef __NANGLESTRUCTURELIST_H
#ifndef __DOXYGEN
#define __NANGLESTRUCTURELIST_H
#endif

#include <vector>
#include "angle/nanglestructure.h"
#include "packet/npacket.h"
#include "utilities/nmiscutils.h"

/**
 * A packet representing a collection of angle structures on a triangulation.
 * Such a packet must always be a child packet of the triangulation on
 * which the angle structures lie.  If this triangulation changes, the
 * information contained in this packet will become invalid.
 */
class NAngleStructureList : public NPacket, public NPropertyHolder {
    public:
        static const int packetType;

    private:
        std::vector<NAngleStructure*> structures;
            /**< Contains the angle structures stored in this packet. */

        bool doesAllowStrict;
            /**< Does the convex span of this list include a strict
             *   angle structure? */
        bool calculatedAllowStrict;
            /**< Have we calculated \a doesAllowStrict? */
        bool doesAllowTaut;
            /**< Does the convex span of this list include a taut structure? */
        bool calculatedAllowTaut;
            /**< Have we calculated \a doesAllowTaut? */

    public:
        /**
         * Creates a new list of angle structures on the given
         * triangulation.  The angle structures produced will be
         * the vertices of the angle structure solution space.
         *
         * This constructor will insert this angle structure list as a
         * child of the given triangulation.  This triangulation \b must
         * remain the parent of this angle structure list, and must not
         * change while this angle structure list is alive.
         */
        NAngleStructureList(NTriangulation* owner);
        /**
         * Destroys this list and all the angle structures within.
         */
        virtual ~NAngleStructureList();

        /**
         * Returns the triangulation on which these angle structures
         * lie.
         *
         * @return the corresponding triangulation.
         */
        NTriangulation* getTriangulation() const;

        /**
         * Returns the number of angle structures stored in this list.
         *
         * @return the number of angle structures.
         */
        unsigned long getNumberOfStructures() const;
        /**
         * Returns the angle structure at the requested index in this
         * list.
         *
         * @param index the index of the requested angle structure in
         * this list; this must be between 0 and
         * getNumberOfStructures()-1 inclusive.
         * @return the angle structure at the requested index.
         */
        const NAngleStructure* getStructure(unsigned long index) const;

        /**
         * Determines whether any convex combination of the angle
         * structures in this list is a strict angle structure.
         * See NAngleStructure::isStrict() for details on strict angle
         * structures.
         *
         * @return \c true if and only if a strict angle structure can
         * be produced.
         */
        bool allowsStrict();
        /**
         * Determines whether any convex combination of the angle
         * structures in this list is a taut structure.
         * See NAngleStructure::isTaut() for details on taut
         * structures.
         *
         * @return \c true if and only if a taut structure can be produced.
         */
        bool allowsTaut();

        virtual int getPacketType() const;
        virtual NString getPacketName() const;
        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        virtual void writePacket(NFile& out) const;
        static NAngleStructureList* readPacket(NFile& in, NPacket* parent);
        virtual bool dependsOnParent() const;

    protected:
        /**
         * Creates a new angle structure list performing no
         * initialisation whatsoever other than property initialisation.
         */
        NAngleStructureList();

        virtual NPacket* internalClonePacket(NPacket* parent) const;

        virtual void readIndividualProperty(NFile& infile, unsigned propType);
        virtual void initialiseAllProperties();

        /**
         * Calculate whether the convex span of this list includes a
         * strict angle structure.
         */
        void calculateAllowStrict();
        /**
         * Calculate whether the convex span of this list includes a
         * taut structure.
         */
        void calculateAllowTaut();

        /**
         * An output iterator used to insert angle structures into an
         * NAngleStructureList.
         *
         * Objects of type <tt>NAngleStructure*</tt> and
         * <tt>NAngleStructureVector*</tt> can be assigned to this
         * iterator.  In the latter case, a surrounding NAngleStructure
         * will be automatically created.
         */
        friend struct StructureInserter {
            NAngleStructureList* list;
                /**< The list into which angle structures will be inserted. */
            NTriangulation* owner;
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
            StructureInserter(NAngleStructureList& newList,
                NTriangulation* newOwner);
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
             * other angle structures in this list.
             *
             * @param structure the angle structure to insert.
             * @return this output iterator.
             */
            StructureInserter& operator =(NAngleStructure* structure);
            /**
             * Appends the angle structure corresponding to the given
             * vector to the end of the appropriate structure list.
             *
             * The given vector will be owned by the newly created
             * angle structure and will be deallocated with the
             * other angle structures in this list.
             *
             * @param vector the vector of the angle structure to insert.
             * @return this output iterator.
             */
            StructureInserter& operator =(NAngleStructureVector* vector);

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
};

// Inline functions for NAngleStructureList

inline NAngleStructureList::NAngleStructureList() {
    NAngleStructureList::initialiseAllProperties();
}

inline NAngleStructureList::~NAngleStructureList() {
    for_each(structures.begin(), structures.end(),
        FuncDelete<NAngleStructure>());
}

inline NTriangulation* NAngleStructureList::getTriangulation() const {
    return (NTriangulation*)getTreeParent();
}

inline unsigned long NAngleStructureList::getNumberOfStructures() const {
    return structures.size();
}

inline const NAngleStructure* NAngleStructureList::getStructure(
        unsigned long index) const {
    return structures[index];
}

inline bool NAngleStructureList::allowsStrict() {
    if (! calculatedAllowStrict)
        calculateAllowStrict();
    return doesAllowStrict;
}

inline bool NAngleStructureList::allowsTaut() {
    if (! calculatedAllowTaut)
        calculateAllowTaut();
    return doesAllowTaut;
}

inline bool NAngleStructureList::dependsOnParent() const {
    return true;
}

inline NAngleStructureList::StructureInserter::StructureInserter() : list(0),
        owner(0) {
}

inline NAngleStructureList::StructureInserter::StructureInserter(
        NAngleStructureList& newList, NTriangulation* newOwner) :
        list(&newList), owner(newOwner) {
}

inline NAngleStructureList::StructureInserter::StructureInserter(
        const StructureInserter& cloneMe) : list(cloneMe.list),
        owner(cloneMe.owner) {
}

inline NAngleStructureList::StructureInserter&
        NAngleStructureList::StructureInserter::operator =(
        const StructureInserter& cloneMe) {
    list = cloneMe.list;
    owner = cloneMe.owner;
    return *this;
}

inline NAngleStructureList::StructureInserter&
        NAngleStructureList::StructureInserter::operator =(
        NAngleStructure* structure) {
    list->structures.push_back(structure);
    return *this;
}

inline NAngleStructureList::StructureInserter&
        NAngleStructureList::StructureInserter::operator =(
        NAngleStructureVector* vector) {
    list->structures.push_back(new NAngleStructure(owner, vector));
    return *this;
}

inline NAngleStructureList::StructureInserter&
        NAngleStructureList::StructureInserter::operator *() {
    return *this;
}

inline NAngleStructureList::StructureInserter&
        NAngleStructureList::StructureInserter::operator ++() {
    return *this;
}

inline NAngleStructureList::StructureInserter&
        NAngleStructureList::StructureInserter::operator ++(int) {
    return *this;
}

#endif

