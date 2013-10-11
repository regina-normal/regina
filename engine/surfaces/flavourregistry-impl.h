
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

/*! \file surfaces/flavourregistry-impl.h
 *  \brief Contains the registry of all normal coordinate systems that can
 *  be used to create and store normal surfaces in 3-manifold triangulations.
 *
 *  Each time a new flavour is created, this registry must be updated to:
 *
 *  - add a #include line for the corresponding vector subclass;
 *  - add a corresponding case to each implementation of forFlavour().
 *
 *  See flavourregistry.h for how other routines can use this registry.
 */

#ifndef __FLAVOURREGISTRY_IMPL_H
#ifndef __DOXYGEN
#define __FLAVOURREGISTRY_IMPL_H
#endif

#include "surfaces/flavourregistry.h"
#include "surfaces/nsstandard.h"
#include "surfaces/nsanstandard.h"
#include "surfaces/nsquad.h"
#include "surfaces/nsquadoct.h"
#include "surfaces/nsoriented.h"
#include "surfaces/nsorientedquad.h"

namespace regina {

template <typename FunctionObject>
inline typename FunctionObject::ReturnType forFlavour(
        NormalCoords flavour, FunctionObject func,
        typename FunctionObject::ReturnType defaultReturn) {
    switch (flavour) {
        case NS_STANDARD : return func(NormalInfo<NS_STANDARD>());
        case NS_AN_STANDARD : return func(NormalInfo<NS_AN_STANDARD>());
        case NS_QUAD : return func(NormalInfo<NS_QUAD>());
        case NS_AN_QUAD_OCT : return func(NormalInfo<NS_AN_QUAD_OCT>());
        case NS_ORIENTED : return func(NormalInfo<NS_ORIENTED>());
        case NS_ORIENTED_QUAD : return func(NormalInfo<NS_ORIENTED_QUAD>());
        default: return defaultReturn;
    }
}

template <typename VoidFunctionObject>
inline void forFlavour(NormalCoords flavour, VoidFunctionObject func) {
    switch (flavour) {
        case NS_STANDARD : func(NormalInfo<NS_STANDARD>()); break;
        case NS_AN_STANDARD : func(NormalInfo<NS_AN_STANDARD>()); break;
        case NS_QUAD : func(NormalInfo<NS_QUAD>()); break;
        case NS_AN_QUAD_OCT : func(NormalInfo<NS_AN_QUAD_OCT>()); break;
        case NS_ORIENTED : func(NormalInfo<NS_ORIENTED>()); break;
        case NS_ORIENTED_QUAD : func(NormalInfo<NS_ORIENTED_QUAD>()); break;
        default: break;
    }
}

/*@}*/

} // namespace regina

#endif // include guard __FLAVOURREGISTRY_IMPL_H

