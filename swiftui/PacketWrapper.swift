
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

/**
 * A lightweight Swift wrapper around a packet pointer (which may be `null`).
 *
 * Specifically: this is a value-based Swift object that wraps a value-based C++ `std::shared_ptr`,
 * which in turn wraps a raw packet pointer.
 */
struct PacketWrapper: Identifiable, Equatable, Hashable {
    typealias ID = PacketWrapper
    
    /// A lightweight value-based C++ wrapper around the underlying packet.
    let packet: regina.SharedPacket

    /**
     * Creates a null packet wrapper.
     */
    init() {
        self.packet = regina.SharedPacket()
    }

    /**
     * Creates a wrapper around the given C++ packet in Regina's calculation engine.
     *
     * This is safe to use even if _packet_ holds a null pointer.
     */
    init(packet: regina.SharedPacket) {
        self.packet = packet
    }

    /**
     * Identifies whether the two given wrappers refer to the same underlying packet.
     *
     * All null packets are considerd equal to each other (but to nothing else).
     */
    static func == (lhs: PacketWrapper, rhs: PacketWrapper) -> Bool {
        // By comparing IDs, we are effectively comparing the raw C++ Packet pointers.
        return (lhs.packet.id() == rhs.packet.id())
    }
    
    /**
     * Returns a list of all immediate child packets of this packet.
     *
     * If this packet has no children or if this is a null packet, then this returns `nil`.
     */
    var children: [PacketWrapper]? {
        if packet.isNull() || packet.firstChild().isNull() {
            return nil
        }

        var ans: [PacketWrapper] = []
        var p = packet.firstChild()
        while !p.isNull() {
            ans.append(PacketWrapper(packet: p))
            p = p.nextSibling()
        }
        return ans
    }
    
    /**
     * Returns a 32-point image representing the type of the given packet.
     *
     * If this packet has an unknown type or if this is a null packet, then this returns `nil`.
     */
    var icon: Image? {
        if packet.isNull() {
            return nil
        }

        switch packet.type() {
        case .AngleStructures:
            return Image("Angles")
        case .Attachment:
            return Image("Attachment")
        case .Container:
            return Image("Container")
        case .Link:
            return Image("Link")
        case .NormalHypersurfaces:
            return Image("Hypersurfaces")
        case .NormalSurfaces:
            return Image("Surfaces")
        case .Script:
            return Image("Script")
        case .SnapPea:
            return Image("SnapPea")
        case .SurfaceFilter:
            return Image("Filter")
        case .Text:
            return Image("Text")
        case .Triangulation2:
            return Image("Triangulation2")
        case .Triangulation3:
            return Image("Triangulation3")
        case .Triangulation4:
            return Image("Triangulation4")
        case .Triangulation5:
            return Image("Triangulation5")
        case .Triangulation6:
            return Image("Triangulation6")
        case .Triangulation7:
            return Image("Triangulation7")
        case .Triangulation8:
            return Image("Triangulation8")
#if REGINA_HIGHDIM
        case .Triangulation9:
            return Image("Triangulation9")
        case .Triangulation10:
            return Image("Triangulation10")
        case .Triangulation11:
            return Image("Triangulation11")
        case .Triangulation12:
            return Image("Triangulation12")
        case .Triangulation13:
            return Image("Triangulation13")
        case .Triangulation14:
            return Image("Triangulation14")
        case .Triangulation15:
            return Image("Triangulation15")
#endif
        default:
            return nil
        }
    }
    
    @ViewBuilder var packetViewer: some View {
        if packet.isNull() {
            // TODO: What do we return here? Probably this view should contain nothing.
            VStack {
                Spacer()
                Text("Null packet")
                Spacer()
            }
        } else {
            switch (packet.type()) {
            case .Link:
                LinkView(packet: regina.SharedLink(packet))
            case .Text:
                TextView(packet: regina.SharedText(packet))
            default:
                // TODO: What to put here?
                VStack {
                    Spacer()
                    Text("Unknown or unimplemented packet type")
                    Spacer()
                }
            }
        }
    }
    
    /**
     * Returns a hashable ID that uniquely identifies the underlying packet in Regina's calculation engine.
     *
     * This is safe to use for null packets (and all null packets will receive equal IDs).
     */
    var id: ID { self }

    /**
     * Hashes this packet wrapper.
     * Different wrappers around the same underlying packet in Regina's calculation engine are guaranteed to receive the same hash.
     *
     * This is safe to use for null packets (and all null packets will receive the same hash value).
     */
    func hash(into hasher: inout Hasher) {
        hasher.combine(packet.id())
    }
}
