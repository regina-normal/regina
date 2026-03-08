
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

/*! \file packet/packettype.h
 *  \brief Defines constants for the various packet types known to Regina.
 */

#ifndef __REGINA_PACKETTYPE_H
#ifndef __DOXYGEN
#define __REGINA_PACKETTYPE_H
#endif

#include "regina-core.h"
#include "triangulation/forward.h"

namespace regina {

/**
 * Represents the different types of packet that are available in Regina.
 *
 * \ingroup packet
 */
enum class PacketType {
    /**
     * Does not represent any of Regina's packet types.
     *
     * This can be used (for example) for initialising a PacketType variable.
     */
    None = 0,
    /**
     * Represents a container packet, of class Container.
     */
    Container = 1,
    /**
     * Represents a text packet, of class Text.
     */
    Text = 2,
    /**
     * Represents a normal surface list, of class NormalSurfaces.
     */
    NormalSurfaces = 6,
    /**
     * Represents a script packet, of class Script.
     */
    Script = 7,
    /**
     * Represents a normal surface filter, of class SurfaceFilter or
     * one of its descendant classes.
     */
    SurfaceFilter = 8,
    /**
     * Represents an angle structure list, of class AngleStructures.
     */
    AngleStructures = 9,
    /**
     * Represents an arbitrary file attachment, of class Attachment.
     */
    Attachment = 10,
    /**
     * Represents a normal hypersurface list, of class NormalHypersurfaces.
     */
    NormalHypersurfaces = 13,
    /**
     * Represents a triangulation in the embedded SnapPea kernel, of
     * class SnapPeaTriangulation.
     */
    SnapPea = 16,
    /**
     * Represents a combinatorial diagram of a knot or link in the 3-sphere,
     * of class Link.
     */
    Link = 17,
    /**
     * Represents a specific embedding of a knot or link in 3-dimensional space,
     * of class SpatialLink.
     */
    SpatialLink = 18,
    /**
     * Represents a 2-dimensional triangulation, of class Triangulation<2>.
     */
    Triangulation2 = 15,
    /**
     * Represents a 3-dimensional triangulation, of class Triangulation<3>.
     */
    Triangulation3 = 3,
    /**
     * Represents a 4-dimensional triangulation, of class Triangulation<4>.
     */
    Triangulation4 = 11,
    /**
     * Represents a 5-dimensional triangulation, of class Triangulation<5>.
     */
    Triangulation5 = 105,
    /**
     * Represents a 6-dimensional triangulation, of class Triangulation<6>.
     */
    Triangulation6 = 106,
    /**
     * Represents a 7-dimensional triangulation, of class Triangulation<7>.
     */
    Triangulation7 = 107,
    /**
     * Represents a 8-dimensional triangulation, of class Triangulation<8>.
     */
    Triangulation8 = 108,
#ifdef REGINA_HIGHDIM
    /**
     * Represents a 9-dimensional triangulation, of class Triangulation<9>.
     */
    Triangulation9 = 109,
    /**
     * Represents a 10-dimensional triangulation, of class Triangulation<10>.
     */
    Triangulation10 = 110,
    /**
     * Represents a 11-dimensional triangulation, of class Triangulation<11>.
     */
    Triangulation11 = 111,
    /**
     * Represents a 12-dimensional triangulation, of class Triangulation<12>.
     */
    Triangulation12 = 112,
    /**
     * Represents a 13-dimensional triangulation, of class Triangulation<13>.
     */
    Triangulation13 = 113,
    /**
     * Represents a 14-dimensional triangulation, of class Triangulation<14>.
     */
    Triangulation14 = 114,
    /**
     * Represents a 15-dimensional triangulation, of class Triangulation<15>.
     */
    Triangulation15 = 115
#endif /* REGINA_HIGHDIM */
};

/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::None.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_NONE = PacketType::None;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Container.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_CONTAINER =
    PacketType::Container;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Text.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TEXT = PacketType::Text;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::NormalSurfaces.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_NORMALSURFACES =
    PacketType::NormalSurfaces;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Script.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_SCRIPT = PacketType::Script;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::SurfaceFilter.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_SURFACEFILTER =
    PacketType::SurfaceFilter;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::AngleStructures.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_ANGLESTRUCTURES =
    PacketType::AngleStructures;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Attachment.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_ATTACHMENT = PacketType::Attachment;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::NormalHypersurfaces.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_NORMALHYPERSURFACES =
    PacketType::NormalHypersurfaces;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::SnapPea.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_SNAPPEATRIANGULATION =
    PacketType::SnapPea;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Link.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_LINK = PacketType::Link;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation2.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION2 =
    PacketType::Triangulation2;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation3.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION3 =
    PacketType::Triangulation3;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation4.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION4 =
    PacketType::Triangulation4;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation5.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION5 =
    PacketType::Triangulation5;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation6.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION6 =
    PacketType::Triangulation6;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation7.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION7 =
    PacketType::Triangulation7;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation8.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION8 =
    PacketType::Triangulation8;
#ifdef REGINA_HIGHDIM
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation9.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION9 =
    PacketType::Triangulation9;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation10.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION10 =
    PacketType::Triangulation10;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation11.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION11 =
    PacketType::Triangulation11;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation12.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION12 =
    PacketType::Triangulation12;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation13.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION13 =
    PacketType::Triangulation13;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation14.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION14 =
    PacketType::Triangulation14;
/**
 * A deprecated constant representing one of the different types of packet.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * PacketType::Triangulation15.
 *
 * \ingroup packet
 */
[[deprecated]] inline constexpr PacketType PACKET_TRIANGULATION15 =
    PacketType::Triangulation15;
#endif /* REGINA_HIGHDIM */

/**
 * A class used to query general information about different packet types.
 *
 * This class has become much simpler since Regina 7.0.  It is no longer
 * templated, and instead just offers a name() function that is accessible at
 * both compile time and runtime.
 *
 * \ingroup packet
 */
class PacketInfo {
    public:
        /**
         * Returns a human-readable name for the given packet type.
         *
         * The first letter of the returned string will be upper-case,
         * and all subsequent letters will be lower-case (except for
         * special words such as "PDF" and "SnapPea", which will retain
         * their internal upper-case letters where required).
         *
         * This routine is guaranteed to return a non-null string, even
         * if \a packetType is not one of the PacketType enum values.
         *
         * \param packetType the packet type being queried.
         * \return the name of the given packet type.
         */
        constexpr static const char* name(PacketType packetType) {
            switch (packetType) {
                case PacketType::Container:
                    return "Container";
                case PacketType::Text:
                    return "Text";
                case PacketType::NormalSurfaces:
                    return "Normal surface list";
                case PacketType::Script:
                    return "Script";
                case PacketType::SurfaceFilter:
                    return "Surface filter";
                case PacketType::AngleStructures:
                    return "Angle structure list";
                case PacketType::Attachment:
                    return "Attachment";
                case PacketType::NormalHypersurfaces:
                    return "Normal hypersurface list";
                case PacketType::SnapPea:
                    return "SnapPea triangulation";
                case PacketType::Link:
                    return "Link";
                case PacketType::SpatialLink:
                    return "Spatial Link";
                case PacketType::Triangulation2:
                    return "2-D triangulation";
                case PacketType::Triangulation3:
                    return "3-D triangulation";
                case PacketType::Triangulation4:
                    return "4-D triangulation";
                case PacketType::Triangulation5:
                    return "5-D triangulation";
                case PacketType::Triangulation6:
                    return "6-D triangulation";
                case PacketType::Triangulation7:
                    return "7-D triangulation";
                case PacketType::Triangulation8:
                    return "8-D triangulation";
            #ifdef REGINA_HIGHDIM
                case PacketType::Triangulation9:
                    return "9-D triangulation";
                case PacketType::Triangulation10:
                    return "10-D triangulation";
                case PacketType::Triangulation11:
                    return "11-D triangulation";
                case PacketType::Triangulation12:
                    return "12-D triangulation";
                case PacketType::Triangulation13:
                    return "13-D triangulation";
                case PacketType::Triangulation14:
                    return "14-D triangulation";
                case PacketType::Triangulation15:
                    return "15-D triangulation";
            #endif /* REGINA_HIGHDIM */
                default:
                    return "Unknown";
            }
        }
};

/**
 * The packet type constant for a packet wrapping an object of type \a Held.
 *
 * This variable is only meaningful when \a Held is not itself a packet type,
 * but instead is a standalone type that can (if desired) be wrapped in a
 * packet of type PacketOf<Held>.  Examples of such types include Link and
 * Triangulation<dim>.
 *
 * In all other cases, this variable will be PacketType::None.
 *
 * In particular, if \a Held is a full packet type itself (such as Container,
 * Script, or PacketOf<...>), then this variable will be PacketType::None.
 *
 * \nopython
 *
 * \ingroup packet
 */
template <typename Held>
static constexpr PacketType packetTypeHolds = PacketType::None;

/**
 * Identifies whether the given packet type represents a native Regina
 * triangulation, and if so, returns the dimension.
 *
 * Note that this routine does _not_ recognise SnapPea triangulations:
 * calling `isTriangulation(PacketType::SnapPea)` will return 0.
 *
 * This routine is intended to help with generic code, where one might need to
 * query a packet type at runtime and from this extract a compile-time
 * dimension constant for use as a template parameter.  For example:
 *
 * \code{.cpp}
 * if (int d = isTriangulation(packet.type())) {
 *      // Our packet holds a triangulation of dimension d.
 *      select_constexpr<2, regina::maxDim() + 1, void>(d, [&](auto dim) {
 *          // The runtime variable d is now a compile-time constant dim.
 *          do_something<dim>(packet);
 *      });
 * }
 * \endcode
 *
 * \param type a packet type constant.
 * \return the dimension of triangulation that \a type represents, or 0 if
 * \a type does not represent a native Regina triangulation.
 *
 * \ingroup packet
 */
constexpr int isTriangulation(PacketType type) {
    switch (type) {
        case PacketType::Triangulation2: return 2;
        case PacketType::Triangulation3: return 3;
        case PacketType::Triangulation4: return 4;
        default:
            // Dimensions 5-15 use constants 105-115.
            int value = static_cast<int>(type);
            if (value >= 105 && value <= 100 + maxDim())
                return value - 100;
            else
                return 0;
    }
}

#ifndef __DOXYGEN
// Don't confuse doxygen with specialisations.

class AngleStructures;
class Link;
class SpatialLink;
class NormalHypersurfaces;
class NormalSurfaces;
class SnapPeaTriangulation;
template <int dim> requires (supportedDim(dim)) class Triangulation;

template <>
inline constexpr PacketType packetTypeHolds<Link> = PacketType::Link;

template <>
inline constexpr PacketType packetTypeHolds<SpatialLink> =
    PacketType::SpatialLink;

template <>
inline constexpr PacketType packetTypeHolds<SnapPeaTriangulation> =
    PacketType::SnapPea;

template <>
inline constexpr PacketType packetTypeHolds<Triangulation<2>> =
    PacketType::Triangulation2;

template <>
inline constexpr PacketType packetTypeHolds<Triangulation<3>> =
    PacketType::Triangulation3;

template <>
inline constexpr PacketType packetTypeHolds<Triangulation<4>> =
    PacketType::Triangulation4;

template <int dim> requires (supportedDim(dim))
inline constexpr PacketType packetTypeHolds<Triangulation<dim>> =
    PacketType(100 + dim); // dimensions 5-15 use constants 105-115

template <>
inline constexpr PacketType packetTypeHolds<AngleStructures> =
    PacketType::AngleStructures;

template <>
inline constexpr PacketType packetTypeHolds<NormalSurfaces> =
    PacketType::NormalSurfaces;

template <>
inline constexpr PacketType packetTypeHolds<NormalHypersurfaces> =
    PacketType::NormalHypersurfaces;

#endif // __DOXYGEN

} // namespace regina

#endif

