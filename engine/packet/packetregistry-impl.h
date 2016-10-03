
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

/*! \file packet/packetregistry-impl.h
 *  \brief Contains the registry of all packet types known to Regina.
 *
 * Each time a new packet type is created, this registry must be updated to:
 *
 * - add a #include line for the corresponding packet class;
 * - add a corresponding case to each implementation of forPacket().
 *
 * See packetregistry.h for how other routines can use this registry.
 *
 * This file is automatically included from packetregistry.h; there
 * is no need for end users to include it explicitly.
 */

#ifndef __PACKETREGISTRY_IMPL_H
#ifndef __DOXYGEN
#define __PACKETREGISTRY_IMPL_H
#endif

#include "packet/container.h"
#include "packet/text.h"
#include "triangulation/ntriangulation.h"
#include "surfaces/nnormalsurfacelist.h"
#include "packet/script.h"
#include "surfaces/nsurfacefilter.h"
#include "angle/anglestructures.h"
#include "packet/pdf.h"
#include "dim2/dim2triangulation.h"
#include "dim4/dim4triangulation.h"
#include "hypersurface/nnormalhypersurfacelist.h"
#include "snappea/snappeatriangulation.h"
#include "generic/triangulation.h"

namespace regina {

// ----------------------------------------------------------------------
// Specialisations of PacketInfo<> for templated packet classes
// ----------------------------------------------------------------------
//
// These specialisations are not defined in the usual packet class headers,
// since this would have the undesirable side-effect of generating all
// possible instantiations of these templated packet classes every time the
// corresponding packet headers were included.
//
// By "quarantining" the specialisations of PacketInfo here in
// packetregistry-impl.h, we at least limit this flood of instantiations
// to those sections of code that genuinely need to iterate through all
// possible packet types.

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_TRIANGULATION5> {
    typedef Triangulation<5> Class;
    inline static const char* name() {
        return "5-Manifold Triangulation";
    }
};
template <>
struct PacketInfo<PACKET_TRIANGULATION6> {
    typedef Triangulation<6> Class;
    inline static const char* name() {
        return "6-Manifold Triangulation";
    }
};
template <>
struct PacketInfo<PACKET_TRIANGULATION7> {
    typedef Triangulation<7> Class;
    inline static const char* name() {
        return "7-Manifold Triangulation";
    }
};
template <>
struct PacketInfo<PACKET_TRIANGULATION8> {
    typedef Triangulation<8> Class;
    inline static const char* name() {
        return "8-Manifold Triangulation";
    }
};
template <>
struct PacketInfo<PACKET_TRIANGULATION9> {
    typedef Triangulation<9> Class;
    inline static const char* name() {
        return "9-Manifold Triangulation";
    }
};
template <>
struct PacketInfo<PACKET_TRIANGULATION10> {
    typedef Triangulation<10> Class;
    inline static const char* name() {
        return "10-Manifold Triangulation";
    }
};
template <>
struct PacketInfo<PACKET_TRIANGULATION11> {
    typedef Triangulation<11> Class;
    inline static const char* name() {
        return "11-Manifold Triangulation";
    }
};
template <>
struct PacketInfo<PACKET_TRIANGULATION12> {
    typedef Triangulation<12> Class;
    inline static const char* name() {
        return "12-Manifold Triangulation";
    }
};
template <>
struct PacketInfo<PACKET_TRIANGULATION13> {
    typedef Triangulation<13> Class;
    inline static const char* name() {
        return "13-Manifold Triangulation";
    }
};
template <>
struct PacketInfo<PACKET_TRIANGULATION14> {
    typedef Triangulation<14> Class;
    inline static const char* name() {
        return "14-Manifold Triangulation";
    }
};
template <>
struct PacketInfo<PACKET_TRIANGULATION15> {
    typedef Triangulation<15> Class;
    inline static const char* name() {
        return "15-Manifold Triangulation";
    }
};
#endif

// ----------------------------------------------------------------------
// Implementations of forPacket()
// ----------------------------------------------------------------------

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
        case PACKET_DIM4TRIANGULATION : return
            func.template operator()<PacketInfo<PACKET_DIM4TRIANGULATION>>(
            std::forward<Args>(args)...);
        case PACKET_NORMALHYPERSURFACELIST : return
            func.template operator()<PacketInfo<PACKET_NORMALHYPERSURFACELIST>>(
            std::forward<Args>(args)...);
        case PACKET_SNAPPEATRIANGULATION : return
            func.template operator()<PacketInfo<PACKET_SNAPPEATRIANGULATION>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION5 : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION5>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION6 : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION6>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION7 : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION7>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION8 : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION8>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION9 : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION9>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION10 : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION10>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION11 : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION11>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION12 : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION12>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION13 : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION13>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION14 : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION14>>(
            std::forward<Args>(args)...);
        case PACKET_TRIANGULATION15 : return
            func.template operator()<PacketInfo<PACKET_TRIANGULATION15>>(
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
        case PACKET_DIM4TRIANGULATION :
            func.template operator()<PacketInfo<PACKET_DIM4TRIANGULATION>>(
            std::forward<Args>(args)...); break;
        case PACKET_NORMALHYPERSURFACELIST :
            func.template operator()<PacketInfo<PACKET_NORMALHYPERSURFACELIST>>(
            std::forward<Args>(args)...); break;
        case PACKET_SNAPPEATRIANGULATION :
            func.template operator()<PacketInfo<PACKET_SNAPPEATRIANGULATION>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION5 :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION5>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION6 :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION6>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION7 :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION7>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION8 :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION8>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION9 :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION9>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION10 :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION10>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION11 :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION11>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION12 :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION12>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION13 :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION13>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION14 :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION14>>(
            std::forward<Args>(args)...); break;
        case PACKET_TRIANGULATION15 :
            func.template operator()<PacketInfo<PACKET_TRIANGULATION15>>(
            std::forward<Args>(args)...); break;
        default: break;
    }
}

} // namespace regina

#endif

