
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

template <typename FunctionObject, typename... Args>
inline typename ReturnsTraits<FunctionObject>::ReturnType
forPacket(PacketType packetType, FunctionObject&& func,
        typename ReturnsTraits<FunctionObject>::ReturnType defaultReturn,
        Args&&... args) {
    switch (packetType) {
        case PACKET_CONTAINER : return
            func.template operator()<PacketInfo<PACKET_CONTAINER>>(
            std::forward<Args>(args)...);
        case PACKET_TEXT : return
            func.template operator()<PacketInfo<PACKET_TEXT>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION>>(
            std::forward<Args>(args)...);
        case PACKET_NORMALSURFACELIST : return
            func.template operator()<PacketInfo<PACKET_NORMALSURFACELIST>>(
            std::forward<Args>(args)...);
        case PACKET_SCRIPT : return
            func.template operator()<PacketInfo<PACKET_SCRIPT>>(
            std::forward<Args>(args)...);
        case PACKET_SURFACEFILTER : return
            func.template operator()<PacketInfo<PACKET_SURFACEFILTER>>(
            std::forward<Args>(args)...);
        case PACKET_ANGLESTRUCTURELIST : return
            func.template operator()<PacketInfo<PACKET_ANGLESTRUCTURELIST>>(
            std::forward<Args>(args)...);
        case PACKET_PDF : return
            func.template operator()<PacketInfo<PACKET_PDF>>(
            std::forward<Args>(args)...);
        case PACKET_DIM2TRIANGULATION : return
            func.template operator()<PacketInfo<PACKET_DIM2TRIANGULATION>>(
            std::forward<Args>(args)...);
        case PACKET_SNAPPEATRIANGULATION : return
            func.template operator()<PacketInfo<PACKET_SNAPPEATRIANGULATION>>(
            std::forward<Args>(args)...);
        default: return defaultReturn;
    }
}

template <typename FunctionObject, typename... Args>
inline typename ReturnsTraits<FunctionObject>::Void
forPacket(PacketType packetType, FunctionObject&& func, Args&&... args) {
    switch (packetType) {
        case PACKET_CONTAINER :
            func.template operator()<PacketInfo<PACKET_CONTAINER>>(
            std::forward<Args>(args)...); break;
        case PACKET_TEXT :
            func.template operator()<PacketInfo<PACKET_TEXT>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION>>(
            std::forward<Args>(args)...); break;
        case PACKET_NORMALSURFACELIST :
            func.template operator()<PacketInfo<PACKET_NORMALSURFACELIST>>(
            std::forward<Args>(args)...); break;
        case PACKET_SCRIPT :
            func.template operator()<PacketInfo<PACKET_SCRIPT>>(
            std::forward<Args>(args)...); break;
        case PACKET_SURFACEFILTER :
            func.template operator()<PacketInfo<PACKET_SURFACEFILTER>>(
            std::forward<Args>(args)...); break;
        case PACKET_ANGLESTRUCTURELIST :
            func.template operator()<PacketInfo<PACKET_ANGLESTRUCTURELIST>>(
            std::forward<Args>(args)...); break;
        case PACKET_PDF :
            func.template operator()<PacketInfo<PACKET_PDF>>(
            std::forward<Args>(args)...); break;
        case PACKET_DIM2TRIANGULATION :
            func.template operator()<PacketInfo<PACKET_DIM2TRIANGULATION>>(
            std::forward<Args>(args)...); break;
        case PACKET_SNAPPEATRIANGULATION :
            func.template operator()<PacketInfo<PACKET_SNAPPEATRIANGULATION>>(
            std::forward<Args>(args)...); break;
        default: break;
    }
}

} // namespace regina

#endif

