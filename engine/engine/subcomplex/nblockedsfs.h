
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
 *  \brief Supports Seifert fibred spaces that are triangulated using
 *  saturated blocks.
 */

#ifndef __NBLOCKEDSFS_H
#ifndef __DOXYGEN
#define __NBLOCKEDSFS_H
#endif

#include <vector>
#include "subcomplex/nsatblock.h"
#include "subcomplex/nstandardtri.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

struct NSatBlockSpec {
    NSatBlock* block;
    bool refVert;
    bool refHoriz;

    NSatBlockSpec();
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
            /**< True if no twists, or if twists correspond precisely to
             * orientation-reversing paths.  Note that reflector
             * boundaries are orientation-reversing but do not introduce
             * twists (thus their existence makes this false). */
        long shiftedAnnuli;
        unsigned long extraReflectors;

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

        /**
         * \pre We have a closed and connected triangulation.
         */
        void calculateBaseEuler();
};

/*@}*/

// Inline functions for NSatBlockSpec

inline NSatBlockSpec::NSatBlockSpec() {
}

inline NSatBlockSpec::NSatBlockSpec(NSatBlock* useBlock, bool useRefVert,
        bool useRefHoriz) : block(useBlock), refVert(useRefVert),
        refHoriz(useRefHoriz) {
}

// Inline functions for NBlockedSFS

inline NBlockedSFS::NBlockedSFS() {
}

} // namespace regina

#endif

