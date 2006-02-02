
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

/*! \file nblockedsfs.h
 *  \brief Provides Seifert fibred spaces that are triangulated using
 *  saturated blocks.
 */

#ifndef __NBLOCKEDSFS_H
#ifndef __DOXYGEN
#define __NBLOCKEDSFS_H
#endif

#include <vector>
#include "subcomplex/nsatblock.h"
#include "subcomplex/nstandardtri.h"
#include "triangulation/ntriangulation.h"
#include "utilities/nlistoncall.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

struct NSatBlockSpec {
    NSatBlock* block;
    bool refVert;
    bool refHoriz;

    NSatBlockSpec(NSatBlock* useBlock, bool useRefVert, bool useRefHoriz);
};

class NBlockedSFS : public NStandardTriangulation {
    private:
        typedef std::vector<NSatBlockSpec> BlockSet;

        BlockSet blocks;
        long baseEuler;
        bool baseOrbl;
        bool hasTwist;
        bool twistsMatchOrientation;

    public:
        ~NBlockedSFS();

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        static NBlockedSFS* isBlockedSFS(NTriangulation* tri);

    private:
        NBlockedSFS();

        /**
         * \pre tri is a closed and connected triangulation.
         */
        static NBlockedSFS* hunt(NSatBlock* starter,
            NSatBlock::TetList& avoidTets);
};

struct NSatBlockStarter {
    NTriangulation triangulation;
    NSatBlock* block;

    NSatBlockStarter();
    ~NSatBlockStarter();
};

class NSatBlockStarterSet : public NListOnCall<NSatBlockStarter> {
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

// Inline functions for NSatBlockSpec

inline NSatBlockSpec::NSatBlockSpec(NSatBlock* useBlock, bool useRefVert,
        bool useRefHoriz) : block(useBlock), refVert(useRefVert),
        refHoriz(useRefHoriz) {
}

// Inline functions for NBlockedSFS

inline NBlockedSFS::NBlockedSFS() {
}

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

