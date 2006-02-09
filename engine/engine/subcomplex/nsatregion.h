
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

/*! \file nsatregion.h
 *  \brief Supports connected regions of saturated blocks in triangulations
 *  of Seifert fibred spaces.
 */

#ifndef __NSATREGION_H
#ifndef __DOXYGEN
#define __NSATREGION_H
#endif

#include "subcomplex/nsatblock.h"
#include <vector>

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Describes how a single saturated block forms a part of a larger
 * saturated region.
 *
 * A saturated region consists of several saturated blocks joined
 * together along their boundary annuli.  This is a helper structure
 * containing a single saturated block along with details of its
 * orientation within a larger region.
 *
 * The ring of saturated annuli around the boundary of the block gives a
 * natural orientation to the block within the context of the base orbifold,
 * just as the ring of edges around a polygon would give a natural
 * orientation to that polygon within the context of a surrounding surface.
 * Again drawing an analogy with the orientation of polygons within a surface,
 * each block can be considered to have a correct or reflected orientation
 * according to whether this ring of annuli runs clockwise or anticlockwise
 * in the base orbifold.
 *
 * The precise orientation of a block is described using two booleans.
 * A block may be reflected \e horizontally, which preserves the
 * directions of Seifert fibres but which reverses the
 * clockwise/anticlockwise orientation as discussed above.  A block may
 * also be reflected \e vertically, which preserves the
 * clockwise/anticlockwise orientation but which reverses the directions
 * of the Seifert fibres.  A block may of course be reflected both
 * horizontally and vertically, or it may not be reflected at all.
 *
 * This helper structure is small, and can be copied by value if
 * necessary.  Be aware that when this structure is destroyed, the
 * internal block structure of type NSatBlock is \e not destroyed.
 */
struct NSatBlockSpec {
    NSatBlock* block;
        /**< Details of the saturated block structure. */
    bool refVert;
        /**< Indicates whether the block is reflected vertically within
             the larger region.  See the class notes for details. */
    bool refHoriz;
        /**< Indicates whether the block is reflected horizontally within
             the larger region.  See the class notes for details. */

    /**
     * Creates a new structure that is completely uninitialised.
     */
    NSatBlockSpec();
    /**
     * Creates a new structure that is initialised to the given set of
     * values.
     *
     * @param useBlock details of the saturated block structure.
     * @param useRefVert \c true if the block is reflected vertically
     * within the larger region, or \c false otherwise.
     * @param useRefHoriz \c true if the block is reflected horizontally
     * within the larger region, or \c false otherwise.
     */
    NSatBlockSpec(NSatBlock* useBlock, bool useRefVert, bool useRefHoriz);
};

/**
 * TODO: Document NSatRegion.
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

        unsigned long nBdryAnnuli_;

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

        unsigned long numberOfBoundaryAnnuli() const;
        /**
         * TODO:  This one is quit slow, since we search as we go.
         */
        const NSatAnnulus& boundaryAnnulus(unsigned long which) const;
        void boundaryAnnulus(unsigned long which,
            NSatBlock*& block, unsigned& annulus,
            bool& blockRefVert, bool& blockRefHoriz) const;

        void adjustSFS(NSFSpace& sfs, bool reflect) const;

        /**
         * stopIfBounded: true means we stop expanding as soon as we
         * find a boundary annulus that has some adjacent tetrahedron
         * (even if just on one face) but no corresponding adjacent block.
         * When we stop the structure will be in an inconsistent state;
         * it is assumed that it will be tossed away completely.
         *
         * Point out that this doesn't check for invalid edges.
         *
         * \pre Any block adjacencies are in this list.
         *
         * Returns false iff we passed stopIfBounded but it failed.
         */
        bool expand(NSatBlock::TetList& avoidTets, bool stopIfBounded = false);

        void calculateBaseEuler();

        void writeTextShort(std::ostream& out) const;
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

inline unsigned long NSatRegion::numberOfBoundaryAnnuli() const {
    return nBdryAnnuli_;
}

} // namespace regina

#endif

