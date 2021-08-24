
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

/*! \file subcomplex/satblockstarter.h
 *  \brief Provides a hard-coded list of saturated blocks to use as starting
 *  points for recognising larger Seifert fibred spaces.
 */

#ifndef __REGINA_SATBLOCKSTARTER_H
#ifndef __DOXYGEN
#define __REGINA_SATBLOCKSTARTER_H
#endif

#include "regina-core.h"
#include "subcomplex/satblock.h"
#include "triangulation/dim3.h"
#include <list>

namespace regina {

class SatRegion;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Contains a triangulation of a saturated block along with the
 * accompanying saturated block description.  Different objects of this
 * class will correspond to different types of saturated block.
 *
 * This is a support class for SatBlockStarterSet, and as such it is a
 * read-only class to the rest of the world.
 *
 * This class is well-suited for subcomplex testing: if the
 * triangulation here is found to be a subcomplex of some larger
 * triangulation (see Triangulation<3>::isContainedIn()), then the
 * corresponding isomorphism can be used to copy this block structure
 * and transform it to describe the corresponding block in the larger
 * triangulation.
 *
 * As such, one of the core uses of this class is as a starting point for
 * identifying regions within triangulations that are formed by joining
 * saturated blocks together along their boundary annuli.  See the routines
 * SatRegion::findStarterBlocks() and SatRegion::expand() for implementations
 * of this.
 *
 * \ifacespython Not present.
 */
class SatBlockStarter {
    private:
        Triangulation<3> triangulation_;
            /**< The triangulation of the saturated block. */
        SatBlock* block_;
            /**< Structural details of the saturated block. */

    public:
        /**
         * Destroys both the internal triangulation and block structure.
         */
        ~SatBlockStarter();

        /**
         * Returns a reference to the triangulation of the saturated
         * block.
         *
         * @return the block triangulation.
         */
        const Triangulation<3>& triangulation() const;

        /**
         * Returns the structure of the saturated block.
         *
         * @return the block structure.
         */
        const SatBlock& block() const;

        // Mark this class as non-copyable.
        SatBlockStarter(const SatBlockStarter&) = delete;
        SatBlockStarter& operator = (const SatBlockStarter&) = delete;

    private:
        /**
         * Creates a new starter block.  The triangulation will be empty
         * and the block pointer set to null.
         *
         * The triangulation must be fleshed out and the block structure
         * created before this object can be used.
         */
        SatBlockStarter();

    friend class SatBlockStarterSet;
};

/**
 * Represents a set of starter blocks that can be used for identifying
 * triangulations of Seifert fibred spaces.
 *
 * This class provides a list of saturated blocks that can be used as
 * starting points for recognising triangulations; see the SatBlockStarter
 * class notes for details.  This list is hard-coded and global (it is
 * created only once, when the first object of this class is created).
 *
 * To iterate over starter blocks, create an object of this class (which is
 * lightweight) and either use it in a C++11 range-based \c for loop, or
 * else use the begin() and end() iterators manually.
 *
 * Be aware that this list makes no claims to be exhaustive; it is
 * expected to grow as future versions of Regina are released.
 *
 * This class is thread-safe.
 *
 * \ifacespython Not present.
 */
class SatBlockStarterSet {
    public:
        /**
         * An iterator over the starter blocks in this list.  This operates
         * as a forward iterator in a manner consistent with the standard C++
         * library.  It does not allow either the list or the blocks
         * that it contains to be changed.
         */
        typedef std::list<const SatBlockStarter*>::const_iterator iterator;

    private:
        static std::list<const SatBlockStarter*> blocks;
            /**< The hard-coded list of starter blocks. */

    public:
        /**
         * Creates a lightweight object for iterating through starter blocks.
         *
         * The underlying hard-coded list will be created if this has
         * not been done yet.
         *
         * This routine is thread-safe.
         */
        SatBlockStarterSet();

        /**
         * A default copy constructor.
         *
         * This constructor does nothing, since objects of this class
         * contain no data, and since calling this constructor implies
         * that the global initialisation work has already been done.
         */
        SatBlockStarterSet(const SatBlockStarterSet&) = default;

        /**
         * A default assignment operator.
         *
         * This operator does nothing, since objects of this class
         * contain no data, and since calling this operator implies
         * that the global initialisation work has already been done.
         */
        SatBlockStarterSet& operator = (const SatBlockStarterSet&) = default;

        /**
         * Returns an iterator pointing to the first block in the
         * hard-coded list.
         *
         * @return an iterator pointing to the first starter block.
         */
        iterator begin();

        /**
         * Returns an iterator pointing past the end of the hard-coded list
         * (i.e., just after the last item).
         *
         * @return a past-the-end iterator.
         */
        iterator end();
};

/*@}*/

// Inline functions for SatBlockStarter

inline SatBlockStarter::SatBlockStarter() : block_(nullptr) {
}

inline SatBlockStarter::~SatBlockStarter() {
    delete block_;
}

inline const Triangulation<3>& SatBlockStarter::triangulation() const {
    return triangulation_;
}

inline const SatBlock& SatBlockStarter::block() const {
    return *block_;
}

// Inline functions for SatBlockStarterSet

inline SatBlockStarterSet::iterator SatBlockStarterSet::begin() {
    return blocks.begin();
}

inline SatBlockStarterSet::iterator SatBlockStarterSet::end() {
    return blocks.end();
}

} // namespace regina

#endif

