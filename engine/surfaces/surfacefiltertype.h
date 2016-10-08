
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

/*! \file surfaces/surfacefiltertype.h
 *  \brief Defines constants for normal surface filter types.
 */

#ifndef __SURFACEFILTERTYPE_H
#ifndef __DOXYGEN
#define __SURFACEFILTERTYPE_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * Represents different types of filter classes that can be used to filter
 * lists of normal surfaces in 3-manifold triangulations.
 *
 * IDs 0-9999 are reserved for future use by Regina.  If you are extending
 * Regina to include your own filter class, you should choose an ID >= 10000.
 */
enum SurfaceFilterType {
    /**
     * Represents the SurfaceFilter class: a do-nothing filter that
     * accepts any normal surface.
     */
    NS_FILTER_DEFAULT = 0,
    /**
     * Represents the SurfaceFilterProperties subclass: a filter that
     * examines simple properties of a normal surface.
     */
    NS_FILTER_PROPERTIES = 1,
    /**
     * Represents the SurfaceFilterCombination subclass: a filter that
     * combines other filters using boolean AND or OR.
     */
    NS_FILTER_COMBINATION = 2
};

/*@}*/

} // namespace regina

#endif

