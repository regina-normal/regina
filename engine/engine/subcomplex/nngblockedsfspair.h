
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

/*! \file nngblockedsfspair.h
 *  \brief Deals with non-geometric pairs of Seifert fibred spaces joined
 *  along a single annulus.
 */

#ifndef __NNGBLOCKEDSFSPAIR_H
#ifndef __DOXYGEN
#define __NNGBLOCKEDSFSPAIR_H
#endif

#include "subcomplex/nstandardtri.h"

namespace regina {

class NSatRegion;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * TODO.
 */
class NNGBlockedSFSPair : public NStandardTriangulation {
    private:
        NSatRegion* region_[2];
        bool horizontal_;
            /**< Do we match vertical to horizontal, or to diagonal? */
        bool firstRegionReflected_;

    public:
        /**
         * Destroys this structure and its constituent components.
         */
        ~NNGBlockedSFSPair();

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        static NNGBlockedSFSPair* isNGBlockedSFSPair(NTriangulation* tri);

    private:
        NNGBlockedSFSPair(NSatRegion* region0, NSatRegion* region1,
            bool horizontal, bool firstRegionReflected);
};

/*@}*/

// Inline functions for NNGBlockedSFSPair

inline NNGBlockedSFSPair::NNGBlockedSFSPair(NSatRegion* region0,
        NSatRegion* region1, bool horizontal, bool firstRegionReflected) :
        horizontal_(horizontal), firstRegionReflected_(firstRegionReflected) {
    region_[0] = region0;
    region_[1] = region1;
}

} // namespace regina

#endif

