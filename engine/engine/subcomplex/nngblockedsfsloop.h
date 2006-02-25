
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

/*! \file nngblockedsfsloop.h
 *  \brief Deals with non-geometric spaces formed by joining a single
 *  saturated region to itself along an annulus.
 */

#ifndef __NNGBLOCKEDSFSLOOP_H
#ifndef __DOXYGEN
#define __NNGBLOCKEDSFSLOOP_H
#endif

#include "subcomplex/nstandardtri.h"
#include "triangulation/nperm.h"

namespace regina {

class NSatRegion;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * TODO: Document NNGBlockedSFSLoop.
 *
 * TODO: When documenting this one, don't forget to talk about the two
 * possibilities for the two boundary annuli.
 *
 * Only deal with annuli for now.
 */
class NNGBlockedSFSLoop : public NStandardTriangulation {
    private:
        NSatRegion* region_;
        bool bdryRefVert_[2];
        bool bdryRefHoriz_[2];
        bool swapFaces_;
        NPerm facePerm_;
            /**< Maps 0/1/2 markings of the region's first boundary annulus
                 to the second. */

    public:
        /**
         * Destroys this structure and its constituent components.
         */
        ~NNGBlockedSFSLoop();

        const NSatRegion& region() const;

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        static NNGBlockedSFSLoop* isNGBlockedSFSLoop(NTriangulation* tri);

    private:
        NNGBlockedSFSLoop(NSatRegion* region,
            bool bdryRefVert0, bool bdryRefHoriz0,
            bool bdryRefVert1, bool bdryRefHoriz1,
            bool swapFaces, NPerm facePerm);
};

/*@}*/

// Inline functions for NNGBlockedSFSLoop

inline NNGBlockedSFSLoop::NNGBlockedSFSLoop(NSatRegion* region,
        bool bdryRefVert0, bool bdryRefHoriz0,
        bool bdryRefVert1, bool bdryRefHoriz1,
        bool swapFaces, NPerm facePerm) :
        region_(region), swapFaces_(swapFaces), facePerm_(facePerm) {
    bdryRefVert_[0] = bdryRefVert0;
    bdryRefVert_[1] = bdryRefVert1;
    bdryRefHoriz_[0] = bdryRefHoriz0;
    bdryRefHoriz_[1] = bdryRefHoriz1;
}

inline const NSatRegion& NNGBlockedSFSLoop::region() const {
    return *region_;
}

} // namespace regina

#endif

