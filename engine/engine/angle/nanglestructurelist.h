
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

#include "packet/npacket.h"
#include "angle/nanglestructure.h"
#include "utilities/ndynamicarray.h"

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
        NDynamicArray<NAngleStructure*> structures;
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

        typedef NDynamicArrayIterator<NAngleStructure*> StructureIterator;
            /**< An iterator class used to run through the individual
             *   angle structures in this list. */

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
        virtual void writeTextShort(ostream& out) const;
        virtual void writeTextLong(ostream& out) const;
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
};

// Inline functions for NAngleStructureList

inline NAngleStructureList::NAngleStructureList() {
    NAngleStructureList::initialiseAllProperties();
}

inline NAngleStructureList::~NAngleStructureList() {
    structures.flushAndDelete();
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

#endif

