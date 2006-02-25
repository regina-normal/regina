
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

#include "subcomplex/nstandardtri.h"

namespace regina {

class NSatRegion;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a blocked Seifert fibred space.  This is a particular type of
 * triangulation of a closed Seifert fibred space, where this triangulation
 * is formed from a single closed saturated region.  A saturated region is
 * in turn formed from saturated blocks by identifying their boundary annuli
 * in pairs; see the NSatRegion class for details.
 *
 * Note that the routines writeName() and writeTeXName() do \e not
 * offer enough information to uniquely identify the triangulation,
 * since this essentially requires a 2-dimensional assembling of
 * saturated blocks.  For full detail, writeTextLong() may be used instead.
 *
 * The optional NStandardTriangulation routine getManifold() is
 * implemented for this class, but getHomologyH1() is not.
 */
class NBlockedSFS : public NStandardTriangulation {
    private:
        NSatRegion* region_;
            /**< The single saturated region that describes this entire
                 triangulation. */

    public:
        /**
         * Destroys this structure and its constituent components.
         */
        ~NBlockedSFS();

        /**
         * Returns details of the single closed saturated region that
         * fills this triangulation.
         *
         * @return the single saturated region.
         */
        const NSatRegion& region() const;

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        /**
         * Determines if the given triangulation is a blocked Seifert
         * fibred space.
         *
         * @param tri the triangulation to examine.
         * @return a newly created structure containing details of the
         * blocked Seifert fibred space, or \c null if the given
         * triangulation is not a blocked Seifert fibred space.
         */
        static NBlockedSFS* isBlockedSFS(NTriangulation* tri);

    private:
        /**
         * Constructs a new blocked Seifert fibred space, as described by
         * the given saturated region.  The new object will take ownership
         * of the given region.
         *
         * @param region the region describing this entire triangulation.
         */
        NBlockedSFS(NSatRegion* region);
};

/*@}*/

// Inline functions for NBlockedSFS

inline NBlockedSFS::NBlockedSFS(NSatRegion* region) : region_(region) {
}

inline const NSatRegion& NBlockedSFS::region() const {
    return *region_;
}

} // namespace regina

#endif

