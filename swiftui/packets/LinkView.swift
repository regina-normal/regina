
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

// TODO: We need to BAN heldCopy() for shared packets. Deep copies are leading to dangling pointers/references.

extension regina.StrandRefAlt: Identifiable {
    public var id: Int { id() }
}

extension regina.SharedLink {
    func strandsForComponent(index: Int) -> [regina.StrandRefAlt] {
        let start = component(index)

        if start.isNull() {
            return []
        }
        
        var ans = [regina.StrandRefAlt]()
        var s = start
        repeat {
            ans.append(s)
            s = s.next()
        } while !(s == start)
        return ans
    }
}

enum LinkTab: Int {
    case crossings = 1, polynomials = 2, algebra = 3, codes = 4, graphs = 5
}

// TODO: How to synchronise these integer values with regina.Framing?
enum LinkFraming: Int {
    case seifert = 1, blackboard = 2
}

struct LinkView: View {
    @ObservedObject var wrapper: Wrapper<regina.SharedLink>
    @State private var selection: LinkTab = (LinkTab(rawValue: UserDefaults.standard.integer(forKey: "tabLink")) ?? .crossings)

    @EnvironmentObject var display: DisplayState
    @Environment(\.horizontalSizeClass) var sizeClass
    
    @State private var errorGeneral = false
    @State private var errorCables = false
    @State private var errorDetail: ReginaError?
    @State private var popoverCables = false
    @State private var inputCables: Int?
    @State private var inputFraming: LinkFraming = (LinkFraming(rawValue: UserDefaults.standard.integer(forKey: "linkFraming")) ?? .seifert)
    
    static let maxCables = 50
    
    var body: some View {
        let link = wrapper.packet.heldCopy()
        
        VStack {
            // TODO: Somehow improve the look of this header.
            // In particular, distinguish it from the navigation title.
            switch link.size() {
            case 0:
                switch link.countComponents() {
                case 0:
                    Text("Empty link")
                case 1:
                    if sizeClass == .compact {
                        Text("Unknot")
                        Text("No crossings")
                    } else {
                        Text("Unknot with no crossings")
                    }
                default:
                    if sizeClass == .compact {
                        Text("Unlink")
                        Text("\(link.countComponents()) components")
                        Text("No crossings")
                    } else {
                        Text("Unlink with \(link.countComponents()) components, no crossings")
                    }
                }
            case 1:
                // This must be alternating, and must have ≥1 component.
                let signs = (link.writhe() > 0 ? Text("+")
                    .foregroundColor(Color("Positive")) : Text("−")
                    .foregroundColor(Color("Negative")))
                if link.countComponents() == 1 {
                    if sizeClass == .compact {
                        Text("Alternating knot")
                        Text("1 crossing (\(signs))")
                    } else {
                        Text("Alternating knot with 1 crossing (\(signs))")
                    }
                } else {
                    if sizeClass == .compact {
                        Text("Alternating link")
                        Text("\(link.countComponents()) components")
                        Text("1 crossing (\(signs))")
                    } else {
                        Text("Alternating link with \(link.countComponents()) components, 1 crossing (\(signs))")
                    }
                }
            default:
                let pos = (link.writhe() + link.size()) / 2
                let neg = link.size() - pos
                let signs = (neg == 0 ? Text("all +").foregroundColor(Color("Positive")) : pos == 0 ? Text("all −").foregroundColor(Color("Negative")) : Text("\(pos) +").foregroundColor(Color("Positive")) + Text(", ") + Text("\(neg) −").foregroundColor(Color("Negative")))
    
                let alt = (link.isAlternating() ? "Alternating" : "Non-alternating")
                if link.countComponents() == 1 {
                    if sizeClass == .compact {
                        Text("\(alt) knot")
                        Text("\(link.size()) crossings (\(signs))")
                    } else {
                        Text("\(alt) knot with \(link.size()) crossings (\(signs))")
                    }
                } else {
                    if sizeClass == .compact {
                        Text("\(alt) link")
                        Text("\(link.countComponents()) components")
                        Text("\(link.size()) crossings (\(signs))")
                    } else {
                        Text("\(alt) link with \(link.countComponents()) components, \(link.size()) crossings (\(signs))")
                    }
                }
            }
            
            TabView(selection: $selection) {
                LinkCrossingsView(wrapper: wrapper).tabItem {
                    Image("Tab-Crossings")
                    Text("Crossings")
                }.tag(LinkTab.crossings)
                LinkPolynomialsView(wrapper: wrapper).tabItem {
                    Image("Tab-Polynomials")
                    Text("Polynomials")
                }.tag(LinkTab.polynomials)
                LinkAlgebraView(wrapper: wrapper).tabItem {
                    Image("Tab-Algebra")
                    Text("Algebra")
                }.tag(LinkTab.algebra)
                LinkCodesView(wrapper: wrapper).tabItem {
                    Image("Tab-Codes")
                    Text("Codes")
                }.tag(LinkTab.codes)
                LinkGraphsView(wrapper: wrapper).tabItem {
                    Image("Tab-Graph")
                    Text("Graphs")
                }.tag(LinkTab.graphs)
            }.onChange(of: selection) { newValue in
                UserDefaults.standard.set(newValue.rawValue, forKey: "tabLink")
            }
            #if os(macOS)
            .padding(.top)
            #endif
            .toolbar {
                // TODO: Fix groupings.
                // TODO: On macOS, primary and secondary are all clumped together.
                ToolbarItemGroup(placement: .primaryAction) {
                    Button {
                        var p = wrapper.packet
                        if !p.intelligentSimplify() {
                            // TODO: Offer a "try harder" option here.
                            errorGeneral = true
                            errorDetail = .init("Could not simplify", detail: "I could not simplify the link diagram any further.")
                        }
                    } label: {
                        Label("Simplify", systemImage: "rectangle.compress.vertical")
                    }
                    Button {
                        // TODO: Perform Reidemeister moves.
                    } label: {
                        // TODO: This is SFSymbols 5.0
                        Label("Moves", systemImage: "slider.horizontal.2.square")
                    }
                }
                ToolbarItemGroup(placement: .primaryAction) {
                    Button {
                        var p = wrapper.packet.asPacket()
                        var c = wrapper.packet.complement()
                        c.setLabel("Complement")
                        p.append(c)
                        display.selectAndDisplay(packet: c)
                    } label: {
                        Label("Complement", image: "Act-Complement")
                    }
                    Button {
                        if wrapper.packet.isEmpty() {
                            errorGeneral = true
                            errorDetail = .init("Empty link", detail: "The SnapPea kernel cannot triangulate the complement of an empty link.")
                        } else {
                            var p = wrapper.packet.asPacket()
                            var c = wrapper.packet.snapPea()
                            c.setLabel("Complement")
                            p.append(c)
                            display.selectAndDisplay(packet: c)
                        }
                    } label: {
                        Label("SnapPea", image: "Act-SnapPea")
                    }
                }
                ToolbarItemGroup(placement: .secondaryAction) {
                    Button {
                        var p = wrapper.packet
                        p.reflect()
                    } label: {
                        Label("Reflect", image: "Act-Reflect")
                    }
                    Button {
                        var p = wrapper.packet
                        p.rotate()
                    } label: {
                        Label("Rotate", image: "Act-Rotate")
                    }
                    Button {
                        var p = wrapper.packet
                        p.reverse()
                    } label: {
                        Label("Reverse", image: "Act-Reverse")
                    }
                }
                ToolbarItemGroup(placement: .secondaryAction) {
                    Button {
                        var p = wrapper.packet
                        if !p.makeAlternating() {
                            // TODO: Notify this in the Qt UI also.
                            errorGeneral = true
                            errorDetail = .init("Already alternating", detail: "This link diagram is already alternating.")
                        }
                    } label: {
                        Label("Make Alternating", image: "Act-Alternating")
                    }
                    Button {
                        var p = wrapper.packet
                        if !p.selfFrame() {
                            // TODO: Notify this in the Qt UI also.
                            errorGeneral = true
                            errorDetail = .init("Already self-framed", detail: "Every component already has zero writhe.")
                        }
                    } label: {
                        Label("Self-Frame", image: "Act-SelfFrame")
                    }
                    Button {
                        popoverCables = true
                    } label: {
                        Label("Cables", image: "Act-Parallel")
                    }
                    // TODO: Compose with
                }
                #if os(visionOS)
                .padding()
                .background(.regularMaterial, in: .rect(cornerRadius: 12))
                #endif
            }
            .alert(isPresented: $errorGeneral, error: errorDetail) { _ in
                Button("OK") {}
            } message: { error in
                if let detail = error.detail {
                    Text(detail)
                }
            }
        }
        #if os(macOS)
        .padding(.top)
        #endif
        .sheet(isPresented: $popoverCables) {
            NavigationStack {
                Form {
                    TextField("Number of cables", value: $inputCables, format: .number)
                        #if !os(macOS)
                        .keyboardType(.numberPad)
                        #endif
                    Picker("Framing", selection: $inputFraming) {
                        Text("Seifert").tag(LinkFraming.seifert)
                        Text("Blackboard").tag(LinkFraming.blackboard)
                    }//.fixedSize()
                    .onChange(of: inputFraming) { newValue in
                        UserDefaults.standard.set(newValue.rawValue, forKey: "linkFraming")
                    }
                }
                #if os(macOS)
                .padding()
                #endif
                .toolbar {
                    ToolbarItem(placement: .cancellationAction) {
                        Button("Cancel", role: .cancel) {
                            popoverCables = false
                        }
                    }
                    ToolbarItem(placement: .confirmationAction) {
                        Button("Convert") {
                            if let cables = inputCables {
                                if cables < 2 {
                                    errorCables = true
                                    errorDetail = .init("Too few cables", detail: "The number of parallel cables should be at least 2.")
                                } else if cables > Self.maxCables {
                                    errorCables = true
                                    errorDetail = .init("Too many cables", detail: "I am not brave enough to try more than \(Self.maxCables) parallel cables.")
                                } else {
                                    var p = wrapper.packet
                                    switch inputFraming {
                                    case .seifert:
                                        p.makeParallel(cables, .Blackboard)
                                    case .blackboard:
                                        p.makeParallel(cables, .Seifert)
                                    }
                                    popoverCables = false
                                }
                            } else {
                                errorCables = true
                                errorDetail = .init("Number of cables required", detail: "Please enter the number of parallel cables that should be used.")
                            }
                        }
                    }
                }
                .navigationTitle("Parallel Cables")
                .navigationBarBackButtonHidden()
            }
            .presentationDetents([.medium])
            .alert(isPresented: $errorCables, error: errorDetail) { _ in
                Button("OK") {}
            } message: { error in
                if let detail = error.detail {
                    Text(detail)
                }
            }
        }
    }
}

struct LinkView_Previews: PreviewProvider {
    static var previews: some View {
        let link = regina.SharedLink(regina.ExampleLink.whitehead())
        LinkView(wrapper: Wrapper<regina.SharedLink>(packet: link))
    }
}
