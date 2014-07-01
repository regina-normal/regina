
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

/*! \file packet/packetregistry-impl.h
 *  \brief Contains the registry of all packet types known to Regina.
 *
 * Each time a new packet type is created, this registry must be updated to:
 *
 * - add a #include line for the corresponding packet class;
 * - add a corresponding case to each implementation of forPacket().
 *
 * See packetregistry.h for how other routines can use this registry.
 */

#ifndef __PACKETREGISTRY_IMPL_H
#ifndef __DOXYGEN
#define __PACKETREGISTRY_IMPL_H
#endif

#include "packet/ncontainer.h"
#include "packet/ntext.h"
#include "triangulation/ntriangulation.h"
#include "surfaces/nnormalsurfacelist.h"
#include "packet/nscript.h"
#include "surfaces/nsurfacefilter.h"
#include "angle/nanglestructurelist.h"
#include "packet/npdf.h"
#include "dim2/dim2triangulation.h"
#include "snappea/nsnappeatriangulation.h"

namespace regina {

template <typename FunctionObject>
inline typename FunctionObject::ReturnType forPacket(
        PacketType packetType, FunctionObject func,
        typename FunctionObject::ReturnType defaultReturn) {
    switch (packetType) {
        case PACKET_CONTAINER :
            return func(PacketInfo<PACKET_CONTAINER>());
        case PACKET_TEXT :
            return func(PacketInfo<PACKET_TEXT>());
        case PACKET_TRIANGULATION :
            return func(PacketInfo<PACKET_TRIANGULATION>());
        case PACKET_NORMALSURFACELIST :
            return func(PacketInfo<PACKET_NORMALSURFACELIST>());
        case PACKET_SCRIPT :
            return func(PacketInfo<PACKET_SCRIPT>());
        case PACKET_SURFACEFILTER :
            return func(PacketInfo<PACKET_SURFACEFILTER>());
        case PACKET_ANGLESTRUCTURELIST :
            return func(PacketInfo<PACKET_ANGLESTRUCTURELIST>());
        case PACKET_PDF :
            return func(PacketInfo<PACKET_PDF>());
        case PACKET_DIM2TRIANGULATION :
            return func(PacketInfo<PACKET_DIM2TRIANGULATION>());
        case PACKET_SNAPPEATRIANGULATION :
            return func(PacketInfo<PACKET_SNAPPEATRIANGULATION>());
        default: return defaultReturn;
    }
}

template <typename VoidFunctionObject>
inline void forPacket(PacketType packetType, VoidFunctionObject func) {
    switch (packetType) {
        case PACKET_CONTAINER :
            func(PacketInfo<PACKET_CONTAINER>()); break;
        case PACKET_TEXT :
            func(PacketInfo<PACKET_TEXT>()); break;
        case PACKET_TRIANGULATION :
            func(PacketInfo<PACKET_TRIANGULATION>()); break;
        case PACKET_NORMALSURFACELIST :
            func(PacketInfo<PACKET_NORMALSURFACELIST>()); break;
        case PACKET_SCRIPT :
            func(PacketInfo<PACKET_SCRIPT>()); break;
        case PACKET_SURFACEFILTER :
            func(PacketInfo<PACKET_SURFACEFILTER>()); break;
        case PACKET_ANGLESTRUCTURELIST :
            func(PacketInfo<PACKET_ANGLESTRUCTURELIST>()); break;
        case PACKET_PDF :
            func(PacketInfo<PACKET_PDF>()); break;
        case PACKET_DIM2TRIANGULATION :
            func(PacketInfo<PACKET_DIM2TRIANGULATION>()); break;
        case PACKET_SNAPPEATRIANGULATION :
            func(PacketInfo<PACKET_SNAPPEATRIANGULATION>()); break;
        default: break;
    }
}

} // namespace regina

#endif

