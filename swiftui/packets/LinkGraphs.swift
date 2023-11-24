
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

enum LinkGraph: Int {
    case tree = 1, nice = 2
}

// TODO: These take some time to load.. is there a way to give feedback?
struct LinkGraphsView: View {
    @ObservedObject var wrapper: Wrapper<regina.SharedLink>
    @State private var selected: LinkGraph = (LinkGraph(rawValue: UserDefaults.standard.integer(forKey: "linkGraph")) ?? .tree)

    @Environment(\.colorScheme) var colorScheme
    
    var body: some View {
        VStack {
            HStack {
                Spacer()
                Picker("Display graph:", selection: $selected) {
                    Text("Tree decomposition").tag(LinkGraph.tree)
                    Text("Nice tree decomposition").tag(LinkGraph.nice)
                }.fixedSize()
                    .onChange(of: selected) { newValue in
                        UserDefaults.standard.set(newValue.rawValue, forKey: "linkGraph")
                    }
                Spacer()
            }
            .padding(.vertical)
            
            // TODO: Refuse for enormous links
            // "This link contains over 500 crossings.\nRegina does not display graphs for such large links."
            
            // TODO: Give a "working on it" message, and build the graph in the background (maybe only when it's large)
            
            var tree = regina.TreeDecomposition(wrapper.packet.heldCopy(), .Upper)
            if selected == .nice {
                let _ = tree.makeNice(nil)
            }
            if tree.size() == 1 {
                Text("1 bag, width \(tree.width())").padding(.bottom)
            } else {
                Text("\(tree.size()) bags, width \(tree.width())").padding(.bottom)
            }

            SvgView(cxxString: regina.svgUsingDot(tree.dot(colorScheme == .dark)))
            Spacer()
        }.padding(.horizontal).textSelection(.enabled)
   }
}

