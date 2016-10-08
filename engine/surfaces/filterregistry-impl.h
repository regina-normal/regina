
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

/*! \file surfaces/filterregistry-impl.h
 *  \brief Contains the registry of all normal surface filter classes that can
 *  be used to filter lists of normal surfaces in 3-manifold triangulations.
 *
 * Each time a new filter is created, this registry must be updated to:
 *
 * - add a #include line for the corresponding filter class;
 * - add a corresponding case to each implementation of forFilter().
 *
 * See filterregistry.h for how other routines can use this registry.
 *
 * This file is automatically included from filterregistry.h; there
 * is no need for end users to include it explicitly.
 */

#ifndef __FILTERREGISTRY_IMPL_H
#ifndef __DOXYGEN
#define __FILTERREGISTRY_IMPL_H
#endif

#include "surfaces/surfacefilter.h"

namespace regina {

template <typename FunctionObject, typename... Args>
inline typename ReturnsTraits<FunctionObject>::ReturnType
forFilter(SurfaceFilterType filter, FunctionObject&& func,
        typename ReturnsTraits<FunctionObject>::ReturnType defaultReturn,
        Args&&... args) {
    switch (filter) {
        case NS_FILTER_DEFAULT : return
            func.template operator()<SurfaceFilterInfo<NS_FILTER_DEFAULT>>(
            std::forward<Args>(args)...);
        case NS_FILTER_PROPERTIES : return
            func.template operator()<SurfaceFilterInfo<NS_FILTER_PROPERTIES>>(
            std::forward<Args>(args)...);
        case NS_FILTER_COMBINATION : return
            func.template operator()<SurfaceFilterInfo<NS_FILTER_COMBINATION>>(
            std::forward<Args>(args)...);
        default: return defaultReturn;
    }
}

template <typename FunctionObject, typename... Args>
inline typename ReturnsTraits<FunctionObject>::Void
forFilter(SurfaceFilterType filter, FunctionObject&& func, Args&&... args) {
    switch (filter) {
        case NS_FILTER_DEFAULT :
            func.template operator()<SurfaceFilterInfo<NS_FILTER_DEFAULT>>(
            std::forward<Args>(args)...); break;
        case NS_FILTER_PROPERTIES :
            func.template operator()<SurfaceFilterInfo<NS_FILTER_PROPERTIES>>(
            std::forward<Args>(args)...); break;
        case NS_FILTER_COMBINATION :
            func.template operator()<SurfaceFilterInfo<NS_FILTER_COMBINATION>>(
            std::forward<Args>(args)...); break;
        default: break;
    }
}

} // namespace regina

#endif

