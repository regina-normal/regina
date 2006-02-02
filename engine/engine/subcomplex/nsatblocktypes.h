
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

/*! \file nsatblocktypes.h
 *  \brief Describes several types of saturated blocks within Seifert fibred
 *  space triangulations.
 */

#ifndef __NSATBLOCKTYPES_H
#ifndef __DOXYGEN
#define __NSATBLOCKTYPES_H
#endif

#include "subcomplex/nsatblock.h"

namespace regina {

class NLayeredSolidTorus;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * A saturated block that is a layered solid torus.  See the
 * NLayeredSolidTorus class for details.
 *
 * The three boundary edges of the layered solid torus are attached to
 * the vertical, horizontal and diagonal edges of the boundary annulus;
 * see the NSatAnnulus class notes for details on precisely what
 * vertical, horizontal and diagonal mean.
 */
class NSatLST : public NSatBlock {
    private:
        NLayeredSolidTorus* lst_;
            /**< Contains details of the layered solid torus that this
                 block represents. */
        NPerm roles_;
            /**< Describes how the layered solid torus is attached to the
                 boundary annulus.  In particular, edge groups \a roles_[0],
                 \a roles_[1] and \a roles_[2] of the layered solid torus are
                 attached to the vertical, horizontal and diagonal edges of
                 the annulus respectively. */

    public:
        /**
         * Returns a clone of the given block structure.
         *
         * @param cloneMe the block structure to clone.
         */
        NSatLST(const NSatLST& cloneMe);
        /**
         * Destroys this structure and its internal components.
         */
        ~NSatLST();

        virtual NSatBlock* clone() const;
        virtual void adjustSFS(NSFSpace& sfs, bool reflect) const;
        // TODO: transform()
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Determines whether the given annulus is a boundary annulus for
         * a block of this type (layered solid torus).  This routine is
         * a specific case of NSatBlock::isBlock(); see that routine for
         * further details.
         *
         * @param annulus the proposed boundary annulus that should form
         * part of the new saturated block.
         * @param avoidTets the list of tetrahedra that should not be
         * considered, and to which any new tetrahedra will be added.
         * @return details of the saturated block if one was found, or
         * \c null if none was found.
         */
        static NSatLST* isBlockLST(const NSatAnnulus& annulus,
            TetList& avoidTets);

    private:
        /**
         * Constructs a partially initialised block.  The boundary
         * annulus will remain uninitialised, and must be initialised
         * before this block can be used.
         *
         * @param lst details of the layered solid torus.
         * @param roles describes how the layered solid torus is
         * attached to the boundary annulus, as explained in the
         * \a roles_ data member documentation.
         */
        NSatLST(NLayeredSolidTorus* lst, NPerm roles);
};

/**
 * A saturated block that is a three-tetrahedron triangular prism.
 *
 * Such a prism may be of major type or of minor type.  In a \e major
 * type prism, the horizontal edges of the boundary annuli are all
 * major (degree three) edges of the prism.  Likewise, in a \e minor
 * type prism, the horizontal boundary edges are all minor (degree two)
 * edges of the prism.  See the NSatAnnulus class notes for a definition
 * of "horizontal" and the NTriSolidTorus class notes for further
 * details regarding "major" and "minor".
 */
class NSatTriPrism : public NSatBlock {
    protected:
        bool major_;
            /**< Is this prism of major type or of minor type? */

    public:
        /**
         * Returns a clone of the given block structure.
         *
         * @param cloneMe the block structure to clone.
         */
        NSatTriPrism(const NSatTriPrism& cloneMe);

        virtual NSatBlock* clone() const;
        virtual void adjustSFS(NSFSpace& sfs, bool reflect) const;
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Determines whether the given annulus is a boundary annulus for
         * a block of this type (triangular prism).  This routine is
         * a specific case of NSatBlock::isBlock(); see that routine for
         * further details.
         *
         * @param annulus the proposed boundary annulus that should form
         * part of the new saturated block.
         * @param avoidTets the list of tetrahedra that should not be
         * considered, and to which any new tetrahedra will be added.
         * @return details of the saturated block if one was found, or
         * \c null if none was found.
         */
        static NSatTriPrism* isBlockTriPrism(const NSatAnnulus& annulus,
            TetList& avoidTets);

        /**
         * Inserts a new copy of a triangular prism block into the given
         * triangulation, and returns the corresponding block structure.
         *
         * The given triangulation will not be emptied before the new
         * tetrahedra are inserted.
         *
         * @param tri the triangulation into which the new block should
         * be inserted.
         * @param \c true if a block of major type should be inserted,
         * or \c false if a block of minor type should be inserted.
         * @return structural details of the newly inserted block.
         */
        static NSatTriPrism* insertBlock(NTriangulation& tri, bool major);

    protected:
        /**
         * Constructs a partially initialised block.  The boundary
         * annuli will remain uninitialised, and must be initialised
         * before this block can be used.
         *
         * @param major \c true if this block is of major type, or
         * \c false if it is of minor type.
         */
        NSatTriPrism(bool major);

    private:
        /**
         * Implements a special case of isBlockTriPrism() to search for
         * a block of major type.  See isBlockTriPrism() for further details.
         *
         * @param annulus the proposed boundary annulus that should form
         * part of the new saturated block.
         * @param avoidTets the list of tetrahedra that should not be
         * considered, and to which any new tetrahedra will be added.
         * @return details of the saturated block if one was found, or
         * \c null if none was found.
         */
        static NSatTriPrism* isBlockTriPrismMajor(const NSatAnnulus& annulus,
            TetList& avoidTets);
};

/*
class NSatMobius : public NSatBlock {
};

class NSatCube : public NSatBlock {
};

class NSatReflector : public NSatBlock {
};

class NSatLayering : public NSatBlock {
};
*/

/*@}*/

// Inline functions for NSatLST

inline NSatLST::NSatLST(NLayeredSolidTorus* lst, NPerm roles) : NSatBlock(1),
        lst_(lst), roles_(roles) {
}

inline NSatBlock* NSatLST::clone() const {
    return new NSatLST(*this);
}

inline void NSatLST::writeTextShort(std::ostream& out) const {
    out << "Saturated layered solid torus";
}

// Inline functions for NSatTriPrism

inline NSatTriPrism::NSatTriPrism(const NSatTriPrism& cloneMe) :
        NSatBlock(cloneMe), major_(cloneMe.major_) {
}

inline NSatTriPrism::NSatTriPrism(bool major) : NSatBlock(3), major_(major) {
}

inline NSatBlock* NSatTriPrism::clone() const {
    return new NSatTriPrism(*this);
}

inline void NSatTriPrism::writeTextShort(std::ostream& out) const {
    out << "Saturated triangular prism";
}

} // namespace regina

#endif

