
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "surfaces/nsurfacesubset.h"
#include "surfaces/nsurfacefilter.h"

namespace regina {

NSurfaceSubset::NSurfaceSubset(const NSurfaceSet& set,
        const NSurfaceFilter& filter) : source(set) {
    unsigned long n = set.getNumberOfSurfaces();
    NNormalSurface* s;
    for (unsigned long i = 0; i < n; i++) {
        s = const_cast<NNormalSurface*>(set.getSurface(i));
        if (filter.accept(*s))
            surfaces.push_back(s);
    }
}

} // namespace regina

