
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

struct LinkAlgebraView: View {
    static let maxSimp = 50
    static let maxRecognise = 50

    @ObservedObject var wrapper: Wrapper<regina.SharedLink>
    // TODO: Not state.
    @State var simplifiedGroup: regina.GroupPresentation?

    @State var didSimplify = false
    @State private var errCouldNotSimplify = false

    @AppStorage("displayUnicode") private var unicode = true

    var body: some View {
        let link = wrapper.packet.heldCopy()
        let autoSimp = (link.size() <= Self.maxSimp)
        let group = simplifiedGroup ?? link.group(autoSimp)
        
        VStack(alignment: .leading) {
            HStack {
                Spacer()
                Text(link.countComponents() == 1 ? "Knot Group" : "Link Group").font(.headline).padding(.vertical)
                Spacer()
            }

            if !(autoSimp || didSimplify) {
                Text("Not automatically simplified").italic().padding(.bottom)
            }
            
            if group.countRelations() <= Self.maxRecognise {
                let name = group.recogniseGroup(unicode)
                if name.length() > 0 {
                    Text("Name: \(swiftString(name))").padding(.bottom)
                }
            }

            // TODO: Do we want to headline the line headings here?
            let nGen = group.countGenerators()
            let alphabetic = (nGen <= 26)
            if nGen == 0 {
                Text("No generators").padding(.bottom)
            } else if nGen == 1 {
                Text("1 generator: a").padding(.bottom)
            } else if nGen == 2 {
                Text("2 generators: a, b").padding(.bottom)
            } else if alphabetic {
                let lastGen = String(UnicodeScalar(96 + Int(nGen))!)
                Text("\(nGen) generators: a … \(lastGen)").padding(.bottom)
            } else {
                Text("\(nGen) generators: g0 … g\(nGen - 1)").padding(.bottom)
            }
            
            let nRel = group.countRelations()
            if nRel == 0 {
                Text("No relations").padding(.bottom)
            } else {
                Text(nRel == 1 ? "1 relation:" : "\(nRel) relations:").padding(.bottom)
                // TODO: Should we put the relations inside a visible frame?
                List {
                    // We are using internal pointers within group, so ensure that
                    // group survives this entire block:
                    withExtendedLifetime(group) {
                        ForEach(0..<group.countRelations(), id: \.self) { i in
                            let rel = group.__relationUnsafe(i).pointee
                            if unicode {
                                Text(swiftString(rel.utf8(alphabetic)))
                            } else {
                                Text(swiftString(rel.str(alphabetic)))
                            }
                        }
                    }
                }
                .listStyle(.plain)
            }
            
            HStack {
                Spacer()
                Button("Simplify presentation") {
                    // TODO: Use a cancellable progress box (maybe only when it's large).
                    var working = group
                    let hom = working.intelligentSimplify()
                    // TODO: Find a way to work around the broken std::optional <-> Swift binding in visionOS.
                    #if !os(visionOS)
                    if hom.hasValue {
                        simplifiedGroup = working
                        didSimplify = true
                        // Currently Regina's links do not have a way to receive
                        // the simplified group, since link groups are not cached.
                    } else {
                        errCouldNotSimplify = true
                    }
                    #endif
                }
                .alert("Could not simplify", isPresented: $errCouldNotSimplify) {
                    Button("OK") {}
                } message: {
                    Text("I could not simplify the group presentation any further.")
                }
                Spacer()
            }.padding(.vertical)
            
            Spacer()
        }.padding(.horizontal)
    }
}

