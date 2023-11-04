
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

struct PacketCell: View {
    static let iconSize = 36.0 // TODO: Choose the icon size properly
    var wrapper: PacketWrapper
    
    @State var opened = false

    var body: some View {
        HStack {
            wrapper.icon.resizable().frame(width: PacketCell.iconSize, height: PacketCell.iconSize)
            VStack(alignment: .leading) {
                Text(String(wrapper.packet.humanLabel()))
                let count = wrapper.packet.countChildren()
                if (count == 1) {
                    Text("1 subpacket").font(.footnote)
                } else if (count > 1) {
                    Text("\(count) subpackets").font(.footnote)
                }
            }
        }
    }
}

struct SubtreeCell: View {
    var wrapper: PacketWrapper
    
    var body: some View {
        HStack {
            Image("Subtree").resizable().frame(width: PacketCell.iconSize, height: PacketCell.iconSize)
            Text("Browse subpackets")
        }
    }
}

struct TreeView: View {
    // @ObservedObject var document: ReginaDocument
    var wrapper: PacketWrapper

    @State private var selected: Int64?

    init(packet: regina.SharedPacket) {
        wrapper = PacketWrapper(packet: packet)
    }

    var body: some View {
        NavigationSplitView {
            List(wrapper.children, selection: $selected) { child in
                PacketCell(wrapper: child)
                // TODO: .listRowSeparator(.visible, edges: .bottom)
            }
            // TODO: Find a sensible list style to use here.
            // .listStyle(.grouped)
            .navigationTitle(String(wrapper.packet.humanLabel())) // TODO: Use filename?
        } detail: {
            // TODO: push another TreeView(packet: child)
            Text("Detail")
        }
    }
}

struct TreeView_Previews: PreviewProvider {
    static var simpleTree: regina.SharedPacket {
        var root = regina.SharedPacket.makeContainer()
        var x = regina.SharedPacket.makeContainer()
        x.setLabel("First child")
        var y = regina.SharedPacket.makeContainer()
        y.setLabel("Second child")
        var z = regina.SharedPacket.makeContainer()
        z.setLabel("Grandchild")
        root.append(x)
        root.append(y)
        y.append(z)
        return root
    }
    
    static var previews: some View {
        TreeView(packet: simpleTree)
    }
}
