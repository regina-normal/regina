
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
#include "subcomplex/nsatblockinst.h"
#include "subcomplex/nstandardtri.h"
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
};

class NSatBlockStarterSet : public NListOnCall<NSatBlockInstance> {
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

// Inline functions for NBlockedSFS

NBlockedSFS::NBlockedSFS() {
}

// Inline functions for NSatBlockStarterSet

inline NSatBlockStarterSet::NSatBlockStarterSet() {
}

inline NSatBlockStarterSet::iterator NSatBlockStarterSet::begin() {
    return blocks.begin();
}

inline NSatBlockStarterSet::iterator NSatBlockStarterSet::end() {
    return blocks.end();
}

} // namespace regina

#endif

