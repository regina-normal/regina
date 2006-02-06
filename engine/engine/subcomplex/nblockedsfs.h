
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

/**
 * TODO
 *
 * For example, might correspond to one component of the JSJ
 * decomposition.
 */
class NSatRegion : public ShareableObject {
    private:
        typedef std::vector<NSatBlockSpec> BlockSet;

        BlockSet blocks_;
        long baseEuler_;
        bool baseOrbl_;
        bool hasTwist_;
        bool twistsMatchOrientation_;
            /**< True if no twists, or if twists correspond precisely to
             * orientation-reversing paths.  Note that reflector
             * boundaries are orientation-reversing but do not introduce
             * twists (thus their existence makes this false). */
        long shiftedAnnuli_;
        unsigned long extraReflectors_;

    public:
        /**
         * \pre No adjacencies for the given block.
         */
        NSatRegion(NSatBlock* starter);

        /**
         * Destroys internal blocks also.
         */
        virtual ~NSatRegion();

        long baseEuler() const;
        bool baseOrientable() const;
        bool hasTwist() const;
        bool twistsMatchOrientation() const;

        void adjustSFS(NSFSpace& sfs, bool reflect) const;

        /**
         * stopIfBounded: true means we stop expanding as soon as we
         * find a boundary annulus that has some adjacent tetrahedron
         * (even if just on one face) but no corresponding adjacent block.
         * When we stop the structure will be in an inconsistent state;
         * it is assumed that it will be tossed away completely.
         *
         * \pre Any block adjacencies are in this list.
         *
         * Returns false iff we passed stopIfBounded but it failed.
         */
        bool expand(NSatBlock::TetList& avoidTets, bool stopIfBounded);

        void calculateBaseEuler();

        void writeTextShort(std::ostream& out) const;
};

/**
 * Big big TODO.
 */
class NBlockedSFS : public NStandardTriangulation {
    private:
        NSatRegion* region_;

    public:
        ~NBlockedSFS();

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        static NBlockedSFS* isBlockedSFS(NTriangulation* tri);

    private:
        NBlockedSFS(NSatRegion* region);
};

/*@}*/

// Inline functions for NSatBlockSpec

inline NSatBlockSpec::NSatBlockSpec() {
}

inline NSatBlockSpec::NSatBlockSpec(NSatBlock* useBlock, bool useRefVert,
        bool useRefHoriz) : block(useBlock), refVert(useRefVert),
        refHoriz(useRefHoriz) {
}

// Inline functions for NSatRegion

inline long NSatRegion::baseEuler() const {
    return baseEuler_;
}

inline bool NSatRegion::baseOrientable() const {
    return baseOrbl_;
}

inline bool NSatRegion::hasTwist() const {
    return hasTwist_;
}

inline bool NSatRegion::twistsMatchOrientation() const {
    return twistsMatchOrientation_;
}

inline void NSatRegion::writeTextShort(std::ostream& out) const {
    // TODO: Plural
    out << "Saturated region with " << blocks_.size() << " block(s)";
}

// Inline functions for NBlockedSFS

inline NBlockedSFS::NBlockedSFS(NSatRegion* region) : region_(region) {
}

inline NBlockedSFS::~NBlockedSFS() {
    if (region_)
        delete region_;
}

} // namespace regina

#endif

