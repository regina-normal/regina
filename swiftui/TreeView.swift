
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

class TreeSelection: ObservableObject {
    @Published var current: PacketWrapper?
}

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

// TODO: When expanding/contracting and a child is displayed, select/deselect it in the list.
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

struct TreeDetail: View {
    @EnvironmentObject var selection: TreeSelection
    @Environment(\.horizontalSizeClass) var sizeClass

    var body: some View {
        // TODO: When transitioning from compact to non-compact,
        // the back button on the detail view seems to stay
        if let p = selection.current {
            if p.packet.type() == .Container {
                // TODO: Implement container views
            } else {
                p.viewer
                    .navigationTitle(swiftString(p.packet.humanLabel()))
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
}

struct TreeView: View {
    // @ObservedObject var document: ReginaDocument
    @ObservedObject private var root: PacketWrapper
    @StateObject private var selection = TreeSelection()
    // TODO: Should the title be a binding?
    let title: String

    @State private var listSelection: PacketWrapper?
    @State private var inputNewPacket = false
    @State private var inputNewPacketType: regina.PacketType = .None

    init(packet: regina.SharedPacket, title: String) {
        root = .init(packet: packet)
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
            List(root.children ?? [], selection: $listSelection) { item in
                PacketCell(wrapper: item, expanded: true)
            }
            .navigationTitle(title)
            .onChange(of: listSelection) { wrapper in
                // TODO: Ensure changes go in the right direction here.
                if let wrapper = wrapper {
                    if wrapper.packet.type() == .Container {
                        // TODO: Expand/collapse children
                        print("Container selected")
                    } else {
                        selection.current = wrapper
                    }
                } else {
                    selection.current = nil
                }
            }
            .toolbar {
                ToolbarItem(placement: .primaryAction) {
                    Menu {
                        // TODO: Should this be a static list?
                        let types: [regina.PacketType] = [
                            .Container,
                            .Triangulation2, .Triangulation3, .Triangulation4,
                            .NormalSurfaces, .NormalHypersurfaces, .AngleStructures,
                            .Link, .SpatialLink, .SnapPea, .SurfaceFilter,
                            .Text, .Script, .Attachment
                        ]
                        ForEach(types) { type in
                            Button {
                                inputNewPacketType = type
                                inputNewPacket = true
                            } label: {
                                Label(type.nameCreation, image: type.iconName)
                            }
                        }
                    } label: {
                        Label("New…", systemImage: "plus")
                    }
                }
            }
        } detail: {
            TreeDetail()
        }
        .environmentObject(selection)
        // TODO: On macOS we get the DocumentGroup's navigation title, not the packet's.
        #if !os(macOS)
        // Hide the DocumentGroup navigation bar, since we want the bar that
        // comes with the inner NavigationSplitView.
        .toolbar(.hidden, for: .navigationBar)
        #endif
        .sheet(isPresented: $inputNewPacket) { [inputNewPacketType] in
            // TODO: Implement packet creators for all types
            NavigationStack {
                inputNewPacketType.creator
                #if os(macOS)
                .padding()
                #endif
                .toolbar {
                    ToolbarItem(placement: .cancellationAction) {
                        Button("Cancel", role: .cancel) {
                            inputNewPacket = false
                        }
                    }
                    ToolbarItem(placement: .confirmationAction) {
                        Button("Create") {
                            // TODO: Do the creation
                            inputNewPacket = false
                        }
                    }
                }
                .navigationTitle("New \(inputNewPacketType.name)")
                .navigationBarBackButtonHidden()

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
        TreeView(packet: simpleTree, title: "Sample")
    }
}
