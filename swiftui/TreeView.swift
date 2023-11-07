
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
    static let iconSize = fontSize(forTextStyle: .body) * 1.5

    var wrapper: PacketWrapper
    
    @State var opened = false

    var body: some View {
        HStack {
            if let icon = wrapper.icon {
                icon.resizable().frame(width: PacketCell.iconSize, height: PacketCell.iconSize)
            }
            VStack(alignment: .leading) {
                Text(swiftString(wrapper.packet.humanLabel()))
                // TODO: Should we display child counts or not?
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

struct TreeView: View {
    // @ObservedObject var document: ReginaDocument
    var wrapper: PacketWrapper

    @State private var selected: PacketWrapper?

    init(packet: regina.SharedPacket) {
        wrapper = PacketWrapper(packet: packet)
    }

    var body: some View {
        // TODO: Merge the DocumentGroup and NavigationSplitView toolbars
        // If absolutely necessary, we can change to a three-column view and use fileImporter to open a file.
        // TODO: When first opening a file, we should ensure the packet list
        // is visible instead of being hidden away (iPad portrait).
        // See the columnVisibility parameter for NavigationSplitView.
        // TODO: iPad portrait: enormous gap at the top of each side of the split view
        // TODO: iPad landscape: two back arrows (one on each side of the split), which seems weird
        NavigationSplitView {
            // We should not display the root packet.
            // Instead start directly with the list of top-level children.
            // TODO: What to do if there are no child packets at all?
            // TODO: This list does not animate nicely at all on iPad.
            List(wrapper.children ?? [], children: \.children, selection: $selected) { item in
                // TODO: If this is a container, expand/collapse on selection.
                PacketCell(wrapper: item)
            }
            // TODO: Choose a better navigation title
            .navigationTitle("Packets")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarBackButtonHidden()
        } detail: {
            // TODO: We need to hide the back button on a non-compact layout.
            if let s = selected {
                s.packetViewer
                    .navigationTitle(swiftString(s.packet.humanLabel()))
                    .navigationBarTitleDisplayMode(.inline)
            } else {
                // TODO: Something for the case of no selection.
                // TODO: Do we want a navigation title also?
                // Perhaps just in the case of no selection?
                Text("No packet selected")
                    .navigationBarTitleDisplayMode(.inline)
            }
        }
    }
}

struct TreeView_Previews: PreviewProvider {
    static var simpleTree: regina.SharedPacket {
        var root = regina.SharedContainer.make().asPacket()

        var x = regina.SharedContainer.make().asPacket()
        x.setLabel(cxxString("First child"))
        root.append(x)

        var y = regina.SharedContainer.make().asPacket()
        y.setLabel(cxxString("Second child"))
        root.append(y)

        var y1 = regina.SharedContainer.make().asPacket()
        y1.setLabel(cxxString("Grandchild"))
        y.append(y1)

        let z = regina.SharedContainer.make().asPacket()
        root.append(z)

        return root
    }
    
    static var previews: some View {
        TreeView(packet: simpleTree)
    }
}
