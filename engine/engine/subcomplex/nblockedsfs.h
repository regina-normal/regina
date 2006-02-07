
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
 * TODO.
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

// Inline functions for NBlockedSFS

inline NBlockedSFS::NBlockedSFS(NSatRegion* region) : region_(region) {
}

} // namespace regina

#endif

