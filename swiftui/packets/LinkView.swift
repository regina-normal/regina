
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

extension regina.GroupExpression: Hashable {
    public func hash(into hasher: inout Hasher) {
        for i in 0..<countTerms() {
            hasher.combine(generator(i))
            hasher.combine(exponent(i))
        }
    }
}

/**
 * A class that supports refreshing a view even if the underlying link has not changed.
 *
 * An example of where this is useful is, for instance, when the Jones polynomial has been
 * computed (and so there is new information to display, even though the link itself did not change).
 */
class ObservedLink: ObservableObject {
    let packet: regina.SharedLink
    
    init(packet: regina.SharedLink) {
        self.packet = packet
    }
    
    func refresh() {
        objectWillChange.send()
    }
}

enum LinkTab {
    case Crossings, Polynomials, Algebra, Codes, Graphs
}

struct LinkView: View {
    let packet: regina.SharedLink
    @State private var selection: LinkTab = .Crossings
    @Environment(\.horizontalSizeClass) var sizeClass
    
    init(packet: regina.SharedLink) {
        self.packet = packet
    }

    var body: some View {
        let link = packet.held()
        
        VStack {
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
            
            // TODO: Ipad 11" portrait, tab icons jump around when selected??
            // TODO: Make a persistent default tab
            TabView(selection: $selection) {
                LinkCrossingsView(packet: packet).tabItem {
                    Image(selection == .Crossings ? "Tab-Crossings-Bold" : "Tab-Crossings").renderingMode(.template)
                    Text("Crossings")
                }.tag(LinkTab.Crossings)
                LinkPolynomialsView(packet: packet).tabItem {
                    Image("Tab-Polynomials").renderingMode(.template)
                    Text("Polynomials")
                }.tag(LinkTab.Polynomials)
                LinkAlgebraView(packet: packet).tabItem {
                    Image("Tab-Algebra").renderingMode(.template)
                    Text("Algebra")
                }.tag(LinkTab.Algebra)
                LinkCodesView(packet: packet).tabItem {
                    Image("Tab-Codes").renderingMode(.template)
                    Text("Codes")
                }.tag(LinkTab.Codes)
                LinkGraphsView(packet: packet).tabItem {
                    Image(selection == .Graphs ? "Tab-Graphs-Bold" : "Tab-Graphs").renderingMode(.template)
                    Text("Graphs")
                }.tag(LinkTab.Graphs)
            }
        }
    }
}

struct LinkCrossingsView: View {
    let packet: regina.SharedLink
    // TODO: Make a persistent default display type
    @State private var pictures: Bool = true

    var body: some View {
        VStack(alignment: .leading) {
            HStack {
                Spacer()
                Picker("Display crossings:", selection: $pictures) {
                    Text("Pictures").tag(true)
                    Text("Text").tag(false)
                }.fixedSize()
                Spacer()
            }
            .padding(.vertical)
            
            let link = packet.held()
            
            if (pictures) {
                // TODO: implement
            } else {
                // TODO: implement
            }
            Spacer()
        }.padding(.horizontal).textSelection(.enabled)
    }
}

enum HomflyStyle {
    case az, lm
}

struct LinkPolynomialsView: View {
    @ObservedObject var observed: ObservedLink
    // TODO: Make a persistent HOMFLY-PT selection
    @State private var homflyStyle: HomflyStyle = .az
    static let maxAuto = 6;

    init(packet: regina.SharedLink) {
        observed = ObservedLink(packet: packet)
    }
    
    var body: some View {
        let link = observed.packet.held()

        // TODO: Add a "copy plain text" option on long press
        // TODO: When computing, use progress trackers with cancellation
        // TODO: When computing, can we disable the buttons???
        VStack(alignment: .leading) {
            Text("Jones").font(.headline).padding(.vertical)
            if link.knowsJones() || link.size() <= LinkPolynomialsView.maxAuto {
                var jones = observed.packet.jones()
                // TODO: Make utf-8 configurable
                if jones.isZero() || jones.minExp() % 2 == 0 {
                    let _: Void = jones.scaleDown(2)
                    Text(String(jones.utf8("𝑡")))
                } else {
                    Text(String(jones.utf8("√𝑡")))
                }
            } else {
                Button("Compute…", systemImage: "gearshape") {
                    observed.packet.jones()
                    observed.refresh()
                }
            }
            HStack {
                Text("HOMFLY-PT").font(.headline).padding(.vertical)
                Spacer()
                Picker("HOMFLY-PT type", selection: $homflyStyle) {
                    Text("(𝛼, 𝑧)").tag(HomflyStyle.az)
                    Text("(ℓ, 𝑚)").tag(HomflyStyle.lm)
                }.pickerStyle(.segmented).fixedSize().labelsHidden()
            }
            if link.knowsHomfly() || link.size() <= LinkPolynomialsView.maxAuto {
                if homflyStyle == .az {
                    Text(String(observed.packet.homflyAZ().utf8("𝛼", "𝑧")))
                } else {
                    Text(String(observed.packet.homflyLM().utf8("ℓ", "𝑚")))
                }
            } else {
                Button("Compute…", systemImage: "gearshape") {
                    observed.packet.homflyAZ()
                    observed.refresh()
                }
            }
            Text("Kauffman bracket").font(.headline).padding(.vertical)
            if link.knowsBracket() || link.size() <= LinkPolynomialsView.maxAuto {
                Text(String(observed.packet.bracket().utf8("𝐴")))
            } else {
                Button("Compute…", systemImage: "gearshape") {
                    observed.packet.bracket()
                    observed.refresh()
                }
            }
            Spacer()
        }.padding(.horizontal).textSelection(.enabled)
    }
}

struct LinkAlgebraView: View {
    @ObservedObject var observed: ObservedLink
    @State var simplifiedGroup: regina.GroupPresentation?
    static let maxSimp = 50
    static let maxRecognise = 50

    init(packet: regina.SharedLink) {
        observed = ObservedLink(packet: packet)
    }

    var body: some View {
        let link = observed.packet.held()
        let autoSimp = (link.size() <= LinkAlgebraView.maxSimp)
        let group = simplifiedGroup ?? link.group(autoSimp)
        
        VStack(alignment: .leading) {
            // TODO: Allow toggling unicode support in settings
            HStack {
                Spacer()
                Text(link.countComponents() == 1 ? "Knot Group" : "Link Group").font(.headline).padding(.vertical)
                Spacer()
            }

            if !autoSimp {
                Text("Not automatically simplified").italic().padding(.bottom)
            }
            
            if group.countRelations() <= LinkAlgebraView.maxRecognise {
                let name = group.recogniseGroup(true /* utf8 */)
                if name.length() > 0 {
                    Text("Name: \(String(name))").padding(.bottom)
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
                // TODO: Should we be using a List?
                let rels = Array<regina.GroupExpression>(group.relations())
                List {
                    ForEach(rels, id: \.self) { rel in
                        Text(String(rel.utf8(alphabetic)))
                    }
                }
                .listStyle(.plain)
                //.padding(.horizontal)
                // TODO: Verify that the scrollable area works as it should
            }
            
            HStack {
                Spacer()
                Button("Try to simplify", systemImage: "rectangle.compress.vertical") {
                    var working = group
                    working.intelligentSimplify()
                    // TODO: If we could not simplify, inform the user and do not update
                    simplifiedGroup = working
                    // Currently Regina's links do not have a way to receive
                    // the simplified group, since link groups are not cached.
                }
                Spacer()
            }.padding(.vertical)
            
            Spacer()
        }.padding(.horizontal)
    }
}

enum LinkCode {
    case gauss, dt, signature, pd, jenkins
}

struct LinkCodesView: View {
    let packet: regina.SharedLink
    // TODO: Make a persistent default code
    @State private var selected: LinkCode = .gauss

    @ViewBuilder func onlyKnots(code: String, plural: Bool) -> some View {
        let capitalised = code.prefix(1).capitalized + code.dropFirst()
        let detail = "\(capitalised) \(plural ? "are" : "is") currently only available for knots, not multi-component links."
        
        if #available(macOS 14.0, iOS 17.0, *) {
            ContentUnavailableView {
                Label("No \(code)", systemImage: "link")
            } description: {
                Text(detail)
            }
        } else {
            Text(detail)
        }
    }
    
    var body: some View {
        VStack(alignment: .leading) {
            HStack {
                Spacer()
                Picker("Display code:", selection: $selected) {
                    Text("Gauss codes").tag(LinkCode.gauss)
                    Text("Dowker-Thistlethwaite").tag(LinkCode.dt)
                    Text("Knot signature").tag(LinkCode.signature)
                    Text("Planar diagram code").tag(LinkCode.pd)
                    Text("Jenkins format").tag(LinkCode.jenkins)
                }.fixedSize()
                Spacer()
            }
            .padding(.vertical)
            
            let link = packet.held()
            
            switch (selected) {
            case .gauss:
                if link.countComponents() == 1 {
                    Text("Classical: ").font(.headline) + Text((String(link.gauss())))
                    (Text("Oriented: ").font(.headline) + Text(String(link.orientedGauss()))).padding(.top)
                } else {
                    onlyKnots(code: "Gauss codes", plural: true)
                }
            case .dt:
                if link.countComponents() == 1 {
                    if link.size() > 26 {
                        Text(String(link.dt(false)))
                    } else {
                        Text(String(link.dt(true)))
                        Text(String(link.dt(false))).padding(.top)
                    }
                } else {
                    onlyKnots(code: "Dowker-Thistlethwaite notation", plural: false)
                }
            case .signature:
                if link.countComponents() == 1 {
                    Text(String(link.knotSig(true, true)))
                } else {
                    onlyKnots(code: "knot signatures", plural: true)
                }
           case .pd:
                Text(String(link.pd()))
                if link.countTrivialComponents() > 0 {
                    Text("This link includes one or more zero-crossing unknot components. These components are omitted entirely from the planar diagram code.").padding(.top)
                }
                if link.pdAmbiguous() {
                    Text("This link includes at least one component that consists entirely of over-crossings. The planar diagram code does not carry enough information to reconstruct the orientation of such a component.").padding(.top)
                }
            case .jenkins:
                Text(String(link.jenkins()))
            }
            Spacer()
        }.padding(.horizontal).textSelection(.enabled)
    }
}

struct LinkGraphsView: View {
    let packet: regina.SharedLink

    var body: some View {
        Text("Graphs are not yet implemented.")
    }
}

struct LinkView_Previews: PreviewProvider {
    static var previews: some View {
        LinkView(packet: regina.SharedLink(regina.ExampleLink.whitehead()))
    }
}
