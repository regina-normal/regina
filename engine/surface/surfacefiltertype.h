
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file surface/surfacefiltertype.h
 *  \brief Defines constants for normal surface filter types.
 */

#ifndef __REGINA_SURFACEFILTERTYPE_H
#ifndef __DOXYGEN
#define __REGINA_SURFACEFILTERTYPE_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * Represents different types of filter classes that can be used to filter
 * lists of normal surfaces in 3-manifold triangulations.
 *
 * \ingroup surface
 */
enum class SurfaceFilterType {
    /**
     * A legacy constant representing a do-nothing filter that
     * accepts any normal surface.
     *
     * This type of filter could technically appear in a second-generation
     * Regina data file, though it is unlikely that this feature was ever
     * used in practice (in particular, filters of this type could not be
     * created through the GUI).
     */
    LegacyDefault = 0,
    /**
     * Represents the SurfaceFilterProperties subclass: a filter that
     * examines simple properties of a normal surface.
     */
    Properties = 1,
    /**
     * Represents the SurfaceFilterCombination subclass: a filter that
     * combines other filters using boolean AND or OR.
     */
    Combination = 2
};

/**
 * A deprecated constant representing one of the different types of
 * normal surface filter.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * SurfaceFilterType::LegacyDefault.
 */
[[deprecated]] inline constexpr SurfaceFilterType NS_FILTER_LEGACY_DEFAULT =
    SurfaceFilterType::LegacyDefault;
/**
 * A deprecated constant representing one of the different types of
 * normal surface filter.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * SurfaceFilterType::Properties.
 */
[[deprecated]] inline constexpr SurfaceFilterType NS_FILTER_PROPERTIES =
    SurfaceFilterType::Properties;
/**
 * A deprecated constant representing one of the different types of
 * normal surface filter.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * SurfaceFilterType::Combination.
 */
[[deprecated]] inline constexpr SurfaceFilterType NS_FILTER_COMBINATION =
    SurfaceFilterType::Combination;

} // namespace regina

#endif

