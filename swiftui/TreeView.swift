
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

// TODO: Support renames, additions, and deletions.

struct PacketLabel: View {
    static let iconSize = fontSize(forTextStyle: .body) * 1.5
    let wrapper: PacketWrapper

    var body: some View {
        if let icon = wrapper.icon {
            Label {
                Text(swiftString(wrapper.packet.humanLabel()))
            } icon: {
                icon.resizable().frame(width: Self.iconSize, height: Self.iconSize)
            }
        } else {
            Text(swiftString(wrapper.packet.humanLabel()))
        }
    }
}

struct PacketCell: View {
    @ObservedObject var wrapper: PacketWrapper
    @State var expanded = false

    var body: some View {
        if let children = wrapper.children {
            DisclosureGroup(isExpanded: $expanded) {
                ForEach(children) { child in
                    PacketCell(wrapper: child, expanded: false)
                }
            } label: {
                PacketLabel(wrapper: wrapper)
            }
        } else {
            PacketLabel(wrapper: wrapper)
        }
    }
}

struct TreeView: View {
    // @ObservedObject var document: ReginaDocument
    @ObservedObject var root: PacketWrapper
    // TODO: Should the title be a binding?
    let title: String

    @State private var selected: PacketWrapper?
    @Environment(\.horizontalSizeClass) var sizeClass

    init(packet: regina.SharedPacket, title: String) {
        root = PacketWrapper(packet: packet)
        self.title = title
    }

    var body: some View {
        // TODO: When first opening a file, we should ensure the packet list (iPad portrait).
        // See the columnVisibility parameter for NavigationSplitView.
        // TODO: macOS: remember the split position
        NavigationSplitView {
            // We should not display the root packet.
            // Instead start directly with the list of top-level children.
            // TODO: What to do if there are no child packets at all?
            // TODO: Disclosure groups with inner disclosure groups do not animate nicely at all on iPad.
            List(root.children ?? [], selection: $selected) { item in
                PacketCell(wrapper: item, expanded: true)
            }
            .navigationTitle(title)
            .onChange(of: selected) { selection in
                if let selection = selection {
                    if selection.packet.type() == .Container {
                        // TODO: Expand/collapse children
                        print("Container selected")
                    }
                }
            }
            .refreshable {
                // TODO: Right now, reload the tree.
                // Eventually, get rid of this and use PacketListener instead.
            }
        } detail: {
            // TODO: When transitioning from compact to non-compact,
            // the back button on the detail view seems to stay
            if let s = selected {
                if s.packet.type() == .Container {
                    // TODO: Implement container views
                } else {
                    s.packetViewer
                        .navigationTitle(swiftString(s.packet.humanLabel()))
                        .navigationBarBackButtonHidden(sizeClass != .compact)
                }
            } else {
                // TODO: Something for the case of no selection.
                // TODO: Do we want a navigation title also?
                // Perhaps just in the case of no selection?
                Text("No packet selected")
                    .navigationBarBackButtonHidden(sizeClass != .compact)
            }
        }
        // TODO: On macOS we get the DocumentGroup's navigation title, not the packet's.
        #if !os(macOS)
        // Hide the DocumentGroup navigation bar, since we want the bar that
        // comes with the inner NavigationSplitView.
        .toolbar(.hidden, for: .navigationBar)
        #endif
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
        TreeView(packet: simpleTree, title: "Sample")
    }
}
