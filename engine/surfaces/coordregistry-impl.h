
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

/*! \file surfaces/coordregistry-impl.h
 *  \brief Contains the registry of all normal coordinate systems that can
 *  be used to create and store normal surfaces in 3-manifold triangulations.
 *
 *  Each time a new coordinate system is created, this registry must be
 *  updated to:
 *
 *  - add a #include line for the corresponding vector subclass;
 *  - add a corresponding case to each implementation of forCoords().
 *
 *  See coordregistry.h for how other routines can use this registry.
 *
 *  This file is automatically included from coordregistry.h; there
 *  is no need for end users to include it explicitly.
 */

#ifndef __COORDREGISTRY_IMPL_H
#ifndef __DOXYGEN
#define __COORDREGISTRY_IMPL_H
#endif

#include "surfaces/coordregistry.h"
#include "surfaces/nsvectorstandard.h"
#include "surfaces/nsvectoranstandard.h"
#include "surfaces/nsvectorquad.h"
#include "surfaces/nsvectorquadoct.h"
#include "surfaces/nsvectororiented.h"
#include "surfaces/nsvectororientedquad.h"

namespace regina {

template <typename FunctionObject, typename... Args>
inline typename ReturnsTraits<FunctionObject>::ReturnType
forCoords(NormalCoords coords, FunctionObject&& func,
        typename ReturnsTraits<FunctionObject>::ReturnType defaultReturn,
        Args&&... args) {
    switch (coords) {
        case NS_STANDARD : return
            func.template operator()<NormalInfo<NS_STANDARD>>(
            std::forward<Args>(args)...);
        case NS_AN_STANDARD : return
            func.template operator()<NormalInfo<NS_AN_STANDARD>>(
            std::forward<Args>(args)...);
        case NS_QUAD : return
            func.template operator()<NormalInfo<NS_QUAD>>(
            std::forward<Args>(args)...);
        case NS_AN_QUAD_OCT : return
            func.template operator()<NormalInfo<NS_AN_QUAD_OCT>>(
            std::forward<Args>(args)...);
        case NS_ORIENTED : return
            func.template operator()<NormalInfo<NS_ORIENTED>>(
            std::forward<Args>(args)...);
        case NS_ORIENTED_QUAD : return
            func.template operator()<NormalInfo<NS_ORIENTED_QUAD>>(
            std::forward<Args>(args)...);
        default: return defaultReturn;
    }
}

template <typename FunctionObject, typename... Args>
inline typename ReturnsTraits<FunctionObject>::Void
forCoords(NormalCoords coords, FunctionObject&& func, Args&&... args) {
    switch (coords) {
        case NS_STANDARD :
            func.template operator()<NormalInfo<NS_STANDARD>>(
            std::forward<Args>(args)...); break;
        case NS_AN_STANDARD :
            func.template operator()<NormalInfo<NS_AN_STANDARD>>(
            std::forward<Args>(args)...); break;
        case NS_QUAD :
            func.template operator()<NormalInfo<NS_QUAD>>(
            std::forward<Args>(args)...); break;
        case NS_AN_QUAD_OCT :
            func.template operator()<NormalInfo<NS_AN_QUAD_OCT>>(
            std::forward<Args>(args)...); break;
        case NS_ORIENTED :
            func.template operator()<NormalInfo<NS_ORIENTED>>(
            std::forward<Args>(args)...); break;
        case NS_ORIENTED_QUAD :
            func.template operator()<NormalInfo<NS_ORIENTED_QUAD>>(
            std::forward<Args>(args)...); break;
        default: break;
    }
}

} // namespace regina

#endif

