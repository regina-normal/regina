
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Swift User Interface                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

import SwiftUI
import ReginaEngine

struct PacketManager {
    /**
     * Returns a 32-point image representing the type of the given packet.
     */
    static func iconFor(_ packet: regina.SharedPacket) -> Image {
        switch packet.type() {
        case regina.PacketType.AngleStructures:
            return Image("Angles")
        case regina.PacketType.Attachment:
            return Image("Attachment")
        case regina.PacketType.Container:
            return Image("Container")
        case regina.PacketType.Link:
            return Image("Link")
        case regina.PacketType.NormalHypersurfaces:
            return Image("Hypersurfaces")
        case regina.PacketType.NormalSurfaces:
            return Image("Surfaces")
        case regina.PacketType.Script:
            return Image("Script")
        case regina.PacketType.SnapPea:
            return Image("SnapPea")
        case regina.PacketType.SurfaceFilter:
            return Image("Filter")
        case regina.PacketType.Text:
            return Image("Text")
        case regina.PacketType.Triangulation2:
            return Image("Triangulation2")
        case regina.PacketType.Triangulation3:
            return Image("Triangulation3")
        case regina.PacketType.Triangulation4:
            return Image("Triangulation4")
        case regina.PacketType.Triangulation5:
            return Image("Triangulation5")
        case regina.PacketType.Triangulation6:
            return Image("Triangulation6")
        case regina.PacketType.Triangulation7:
            return Image("Triangulation7")
        case regina.PacketType.Triangulation8:
            return Image("Triangulation8")
        // TODO: Cases 9..15, conditionally compiled
        default:
            // TODO: What do we return here?
            return Image("")
        }
    }
}
