
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
#include "surfaces/normalsurfaces.h"
#include "packet/script.h"
#include "surfaces/surfacefilter.h"
#include "angle/anglestructures.h"
#include "packet/pdf.h"
#include "hypersurface/normalhypersurfaces.h"
#include "snappea/snappeatriangulation.h"
#include "link/link.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/generic.h"

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
#ifndef REGINA_LOWDIMONLY
template <>
struct PacketInfo<PACKET_TRIANGULATION5> {
    typedef Triangulation<5> Class;
    static constexpr const char* name = "5-Manifold Triangulation";
};
template <>
struct PacketInfo<PACKET_TRIANGULATION6> {
    typedef Triangulation<6> Class;
    static constexpr const char* name = "6-Manifold Triangulation";
};
template <>
struct PacketInfo<PACKET_TRIANGULATION7> {
    typedef Triangulation<7> Class;
    static constexpr const char* name = "7-Manifold Triangulation";
};
template <>
struct PacketInfo<PACKET_TRIANGULATION8> {
    typedef Triangulation<8> Class;
    static constexpr const char* name = "8-Manifold Triangulation";
};
template <>
struct PacketInfo<PACKET_TRIANGULATION9> {
    typedef Triangulation<9> Class;
    static constexpr const char* name = "9-Manifold Triangulation";
};
template <>
struct PacketInfo<PACKET_TRIANGULATION10> {
    typedef Triangulation<10> Class;
    static constexpr const char* name = "10-Manifold Triangulation";
};
template <>
struct PacketInfo<PACKET_TRIANGULATION11> {
    typedef Triangulation<11> Class;
    static constexpr const char* name = "11-Manifold Triangulation";
};
template <>
struct PacketInfo<PACKET_TRIANGULATION12> {
    typedef Triangulation<12> Class;
    static constexpr const char* name = "12-Manifold Triangulation";
};
template <>
struct PacketInfo<PACKET_TRIANGULATION13> {
    typedef Triangulation<13> Class;
    static constexpr const char* name = "13-Manifold Triangulation";
};
template <>
struct PacketInfo<PACKET_TRIANGULATION14> {
    typedef Triangulation<14> Class;
    static constexpr const char* name = "14-Manifold Triangulation";
};
template <>
struct PacketInfo<PACKET_TRIANGULATION15> {
    typedef Triangulation<15> Class;
    static constexpr const char* name = "15-Manifold Triangulation";
};
#endif /* ! REGINA_LOWDIMONLY */
#endif

// ----------------------------------------------------------------------
// Implementations of forPacket()
// ----------------------------------------------------------------------

template <typename FunctionObject, typename ReturnType>
inline auto forPacket(PacketType packetType, FunctionObject&& func,
        ReturnType&& defaultReturn) {
    typedef decltype(func(PacketInfo<PACKET_CONTAINER>())) RealReturnType;
    switch (packetType) {
        case PACKET_CONTAINER :
            return func(PacketInfo<PACKET_CONTAINER>());
        case PACKET_TEXT :
            return func(PacketInfo<PACKET_TEXT>());
        case PACKET_TRIANGULATION3 :
            return func(PacketInfo<PACKET_TRIANGULATION3>());
        case PACKET_NORMALSURFACES :
            return func(PacketInfo<PACKET_NORMALSURFACES>());
        case PACKET_SCRIPT :
            return func(PacketInfo<PACKET_SCRIPT>());
        case PACKET_SURFACEFILTER :
            return func(PacketInfo<PACKET_SURFACEFILTER>());
        case PACKET_ANGLESTRUCTURES :
            return func(PacketInfo<PACKET_ANGLESTRUCTURES>());
        case PACKET_PDF :
            return func(PacketInfo<PACKET_PDF>());
        case PACKET_TRIANGULATION2 :
            return func(PacketInfo<PACKET_TRIANGULATION2>());
        case PACKET_TRIANGULATION4 :
            return func(PacketInfo<PACKET_TRIANGULATION4>());
        case PACKET_NORMALHYPERSURFACES :
            return func(PacketInfo<PACKET_NORMALHYPERSURFACES>());
        case PACKET_SNAPPEATRIANGULATION :
            return func(PacketInfo<PACKET_SNAPPEATRIANGULATION>());
        case PACKET_LINK :
            return func(PacketInfo<PACKET_LINK>());
#ifndef REGINA_LOWDIMONLY
        case PACKET_TRIANGULATION5 :
            return func(PacketInfo<PACKET_TRIANGULATION5>());
        case PACKET_TRIANGULATION6 :
            return func(PacketInfo<PACKET_TRIANGULATION6>());
        case PACKET_TRIANGULATION7 :
            return func(PacketInfo<PACKET_TRIANGULATION7>());
        case PACKET_TRIANGULATION8 :
            return func(PacketInfo<PACKET_TRIANGULATION8>());
        case PACKET_TRIANGULATION9 :
            return func(PacketInfo<PACKET_TRIANGULATION9>());
        case PACKET_TRIANGULATION10 :
            return func(PacketInfo<PACKET_TRIANGULATION10>());
        case PACKET_TRIANGULATION11 :
            return func(PacketInfo<PACKET_TRIANGULATION11>());
        case PACKET_TRIANGULATION12 :
            return func(PacketInfo<PACKET_TRIANGULATION12>());
        case PACKET_TRIANGULATION13 :
            return func(PacketInfo<PACKET_TRIANGULATION13>());
        case PACKET_TRIANGULATION14 :
            return func(PacketInfo<PACKET_TRIANGULATION14>());
        case PACKET_TRIANGULATION15 :
            return func(PacketInfo<PACKET_TRIANGULATION15>());
#endif /* ! REGINA_LOWDIMONLY */
        default: return static_cast<RealReturnType>(defaultReturn);
    }
}

template <typename FunctionObject>
inline auto forPacket(PacketType packetType, FunctionObject&& func) {
    typedef decltype(func(PacketInfo<PACKET_CONTAINER>())) RealReturnType;
    switch (packetType) {
        case PACKET_CONTAINER :
            return func(PacketInfo<PACKET_CONTAINER>());
        case PACKET_TEXT :
            return func(PacketInfo<PACKET_TEXT>());
        case PACKET_TRIANGULATION3 :
            return func(PacketInfo<PACKET_TRIANGULATION3>());
        case PACKET_NORMALSURFACES :
            return func(PacketInfo<PACKET_NORMALSURFACES>());
        case PACKET_SCRIPT :
            return func(PacketInfo<PACKET_SCRIPT>());
        case PACKET_SURFACEFILTER :
            return func(PacketInfo<PACKET_SURFACEFILTER>());
        case PACKET_ANGLESTRUCTURES :
            return func(PacketInfo<PACKET_ANGLESTRUCTURES>());
        case PACKET_PDF :
            return func(PacketInfo<PACKET_PDF>());
        case PACKET_TRIANGULATION2 :
            return func(PacketInfo<PACKET_TRIANGULATION2>());
        case PACKET_TRIANGULATION4 :
            return func(PacketInfo<PACKET_TRIANGULATION4>());
        case PACKET_NORMALHYPERSURFACES :
            return func(PacketInfo<PACKET_NORMALHYPERSURFACES>());
        case PACKET_SNAPPEATRIANGULATION :
            return func(PacketInfo<PACKET_SNAPPEATRIANGULATION>());
        case PACKET_LINK :
            return func(PacketInfo<PACKET_LINK>());
#ifndef REGINA_LOWDIMONLY
        case PACKET_TRIANGULATION5 :
            return func(PacketInfo<PACKET_TRIANGULATION5>());
        case PACKET_TRIANGULATION6 :
            return func(PacketInfo<PACKET_TRIANGULATION6>());
        case PACKET_TRIANGULATION7 :
            return func(PacketInfo<PACKET_TRIANGULATION7>());
        case PACKET_TRIANGULATION8 :
            return func(PacketInfo<PACKET_TRIANGULATION8>());
        case PACKET_TRIANGULATION9 :
            return func(PacketInfo<PACKET_TRIANGULATION9>());
        case PACKET_TRIANGULATION10 :
            return func(PacketInfo<PACKET_TRIANGULATION10>());
        case PACKET_TRIANGULATION11 :
            return func(PacketInfo<PACKET_TRIANGULATION11>());
        case PACKET_TRIANGULATION12 :
            return func(PacketInfo<PACKET_TRIANGULATION12>());
        case PACKET_TRIANGULATION13 :
            return func(PacketInfo<PACKET_TRIANGULATION13>());
        case PACKET_TRIANGULATION14 :
            return func(PacketInfo<PACKET_TRIANGULATION14>());
        case PACKET_TRIANGULATION15 :
            return func(PacketInfo<PACKET_TRIANGULATION15>());
#endif /* ! REGINA_LOWDIMONLY */
        default: return RealReturnType();
    }
}

} // namespace regina

#endif

