
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
                p.packetViewer
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
    @State private var inputNewLinkType = 0

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
            .refreshable {
                // TODO: Right now, reload the tree.
                // Eventually, get rid of this and use PacketListener instead.
            }
            .toolbar {
                ToolbarItem(placement: .primaryAction) {
                    Menu {
                        Button {
                            inputNewPacketType = .Container
                            inputNewPacket = true
                        } label: {
                            Label("Container", image: "Container")
                        }
                        Button {
                            inputNewPacketType = .Triangulation2
                            inputNewPacket = true
                        } label: {
                            Label("2-D Triangulation", image: "Triangulation2")
                        }
                        Button {
                            inputNewPacketType = .Triangulation3
                            inputNewPacket = true
                        } label: {
                            Label("3-D Triangulation", image: "Triangulation3")
                        }
                        Button {
                            inputNewPacketType = .Triangulation4
                            inputNewPacket = true
                        } label: {
                            Label("4-D Triangulation", image: "Triangulation4")
                        }
                        Button {
                            inputNewPacketType = .NormalSurfaces
                            inputNewPacket = true
                        } label: {
                            Label("Normal Surfaces (3-D)", image: "Surfaces")
                        }
                        Button {
                            inputNewPacketType = .NormalHypersurfaces
                            inputNewPacket = true
                        } label: {
                            Label("Normal Hypersurfaces (4-D)", image: "Hypersurfaces")
                        }
                        Button {
                            inputNewPacketType = .AngleStructures
                            inputNewPacket = true
                        } label: {
                            Label("Angle Structures", image: "Angles")
                        }
                        Button {
                            inputNewPacketType = .Link
                            inputNewPacket = true
                        } label: {
                            Label("Knot or Link", image: "Link")
                        }
                        Button {
                            inputNewPacketType = .SpatialLink
                            inputNewPacket = true
                        } label: {
                            Label("Spatial Link", image: "SpatialLink")
                        }
                        Button {
                            inputNewPacketType = .SnapPea
                            inputNewPacket = true
                        } label: {
                            Label("SnapPea Triangulation", image: "SnapPea")
                        }
                        Button {
                            inputNewPacketType = .SurfaceFilter
                            inputNewPacket = true
                        } label: {
                            Label("Filter", image: "Filter")
                        }
                        Button {
                            inputNewPacketType = .Text
                            inputNewPacket = true
                        } label: {
                            Label("Text", image: "Text")
                        }
                        Button {
                            inputNewPacketType = .Script
                            inputNewPacket = true
                        } label: {
                            Label("Script", image: "Script")
                        }
                        Button {
                            inputNewPacketType = .Attachment
                            inputNewPacket = true
                        } label: {
                            Label("Attachment", image: "Attachment")
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
        .sheet(isPresented: $inputNewPacket) {
            // TODO: Implement packet creators for all types
            VStack {
                switch inputNewPacketType {
                case .Container:
                    // TODO: Implement
                    Text("TODO: Container")
                case .Text:
                    // TODO: Implement
                    Text("TODO: Text")
                case .NormalSurfaces:
                    // TODO: Implement
                    Text("TODO: Surfaces")
                case .Script:
                    // TODO: Implement
                    Text("TODO: Script")
                case .SurfaceFilter:
                    // TODO: Implement
                    Text("TODO: Filter")
                case .AngleStructures:
                    // TODO: Implement
                    Text("TODO: Angles")
                case .Attachment:
                    // TODO: Implement
                    Text("TODO: Attachment")
                case .NormalHypersurfaces:
                    // TODO: Implement
                    Text("TODO: Hypersurfaces")
                case .SnapPea:
                    // TODO: Implement
                    Text("TODO: SnapPea")
                case .Link:
                    // TODO: Implement
                    Text("New Knot / Link").font(.headline).padding(.bottom)
                    Picker("Type", selection: $inputNewLinkType) {
                        Text("Example")
                        Text("Text code")
                        Text("Torus link")
                    }.pickerStyle(.segmented).fixedSize()
                case .SpatialLink:
                    // TODO: Implement
                    Text("TODO: Spatial Link")
                case .Triangulation2:
                    // TODO: Implement
                    Text("TODO: Dim2")
                case .Triangulation3:
                    // TODO: Implement
                    Text("TODO: Dim3")
                case .Triangulation4:
                    // TODO: Implement
                    Text("TODO: Dim4")
                default:
                    // TODO: Implement
                    Text("TODO: Default")
                }
                Spacer()
                HStack {
                    Spacer()
                    Button("OK") {
                        inputNewPacket = false
                    }
                    Spacer()
                    Button("Cancel") {
                        inputNewPacket = false
                    }
                    Spacer()
                }
            }.padding()
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
