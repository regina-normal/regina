
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file surfaces/hscoordregistry-impl.h
 *  \brief Contains the registry of all coordinate systems that can be used
 *  to create and store normal hypersurfaces in 4-manifold triangulations.
 *
 *  Each time a new coordinate system is created, this registry must be
 *  updated to:
 *
 *  - add a #include line for the corresponding vector subclass;
 *  - add a corresponding case to each implementation of forCoords().
 *
 *  See hscoordregistry.h for how other routines can use this registry.
 *
 *  This file is automatically included from hscoordregistry.h; there is
 *  no need for end users to include it explicitly.
 */

#ifndef __REGINA_HSCOORDREGISTRY_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_HSCOORDREGISTRY_IMPL_H
#endif

#include "hypersurface/hscoordregistry.h"
#include "hypersurface/hsvectorstandard.h"

namespace regina {

template <typename FunctionObject, typename ReturnType>
inline auto forCoords(HyperCoords coords, FunctionObject&& func,
        ReturnType&& defaultReturn) {
    typedef decltype(func(HyperInfo<HS_STANDARD>())) RealReturnType;
    switch (coords) {
        case HS_STANDARD : return func(HyperInfo<HS_STANDARD>());
        default: return static_cast<RealReturnType>(defaultReturn);
    }
}

template <typename FunctionObject>
inline auto forCoords(HyperCoords coords, FunctionObject&& func) {
    typedef decltype(func(HyperInfo<HS_STANDARD>())) RealReturnType;
    switch (coords) {
        case HS_STANDARD : return func(HyperInfo<HS_STANDARD>());
        default: return RealReturnType();
    }
}

/*@}*/

} // namespace regina

#endif

