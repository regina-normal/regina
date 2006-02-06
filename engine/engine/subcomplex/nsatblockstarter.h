
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nsatblockstarter.h
 *  \brief Provides a hard-coded list of saturated blocks to use as starting
 *  points for recognising larger Seifert fibred spaces.
 */

#ifndef __NSATBLOCKSTARTER_H
#ifndef __DOXYGEN
#define __NSATBLOCKSTARTER_H
#endif

#include "subcomplex/nsatblock.h"
#include "triangulation/ntriangulation.h"
#include "utilities/nlistoncall.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Contains a triangulation of a saturated block along with the
 * accompanying saturated block description.  Different objects of this
 * class will correspond to different types of saturated block.
 *
 * This is a support class for NSatBlockStarterSet, and as such it is a
 * read-only class to the rest of the world.
 *
 * This class is well-suited for subcomplex testing: if the
 * triangulation here is found to be a subcomplex of some larger
 * triangulation (see NTriangulation::isContainedIn()), then the
 * corresponding isomorphism can be used to copy this block structure
 * and transform it to describe the corresponding block in the larger
 * triangulation.
 *
 * As such, one of the core uses of this class is as a starting point
 * for identifying triangulations that are formed by joining saturated
 * blocks together along their boundary annuli.  See the NBlockedSFS
 * class for details.
 */
class NSatBlockStarter : regina::boost::noncopyable {
    private:
        NTriangulation triangulation_;
            /**< The triangulation of the saturated block. */
        NSatBlock* block_;
            /**< Structural details of the saturated block. */

    public:
        /**
         * Destroys both the internal triangulation and block structure.
         */
        ~NSatBlockStarter();

        /**
         * Returns a reference to the triangulation of the saturated
         * block.
         *
         * @return the block triangulation.
         */
        const NTriangulation& triangulation() const;

        /**
         * Returns details that describe the structure of the saturated
         * block.
         *
         * @return the block structure.
         */
        const NSatBlock* block() const;

    private:
        /**
         * Creates a new starter block.  The triangulation will be empty
         * and the block pointer set to null.
         *
         * The triangulation must be fleshed out and the block structure
         * created before this object can be used.
         */
        NSatBlockStarter();

    friend class NSatBlockStarterSet;
};

class NSatBlockStarterSet : private NListOnCall<NSatBlockStarter> {
    public:
        typedef NListOnCall<NSatBlockStarter>::iterator iterator;

    private:
        static const NSatBlockStarterSet blocks;

    public:
        static iterator begin();
        static iterator end();

    protected:
        void initialise();

    private:
        NSatBlockStarterSet();
};

/*@}*/

// Inline functions for NSatBlockStarter

inline NSatBlockStarter::NSatBlockStarter() : block_(0) {
}

inline NSatBlockStarter::~NSatBlockStarter() {
    if (block_)
        delete block_;
}

inline const NTriangulation& NSatBlockStarter::triangulation() const {
    return triangulation_;
}

inline const NSatBlock* NSatBlockStarter::block() const {
    return block_;
}

// Inline functions for NSatBlockStarterSet

inline NSatBlockStarterSet::NSatBlockStarterSet() {
}

inline NSatBlockStarterSet::iterator NSatBlockStarterSet::begin() {
    return blocks.NListOnCall<NSatBlockStarter>::begin();
}

inline NSatBlockStarterSet::iterator NSatBlockStarterSet::end() {
    return blocks.NListOnCall<NSatBlockStarter>::end();
}

} // namespace regina

#endif

