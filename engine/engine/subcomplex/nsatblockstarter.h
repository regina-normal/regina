
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

struct NSatBlockStarter {
    NTriangulation triangulation;
    NSatBlock* block;

    NSatBlockStarter();
    ~NSatBlockStarter();
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

inline NSatBlockStarter::NSatBlockStarter() : block(0) {
}

inline NSatBlockStarter::~NSatBlockStarter() {
    if (block)
        delete block;
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

