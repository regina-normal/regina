
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

struct LinkView: View {
    @StateObject var wrapper: Wrapper<regina.SharedLink>
    
    @State private var selection: LinkTab = (LinkTab(rawValue: UserDefaults.standard.integer(forKey: "tabLink")) ?? .crossings)
    @Environment(\.horizontalSizeClass) var sizeClass
    
    var body: some View {
        let link = wrapper.readonly().heldCopy()
        
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
            TabView(selection: $selection) {
                LinkCrossingsView(wrapper: wrapper).tabItem {
                    Image(selection == .crossings ? "Tab-Crossings-Bold" : "Tab-Crossings").renderingMode(.template)
                    Text("Crossings")
                }.tag(LinkTab.crossings)
                LinkPolynomialsView(wrapper: wrapper).tabItem {
                    Image("Tab-Polynomials").renderingMode(.template)
                    Text("Polynomials")
                }.tag(LinkTab.polynomials)
                LinkAlgebraView(wrapper: wrapper).tabItem {
                    Image("Tab-Algebra").renderingMode(.template)
                    Text("Algebra")
                }.tag(LinkTab.algebra)
                LinkCodesView(wrapper: wrapper).tabItem {
                    Image("Tab-Codes").renderingMode(.template)
                    Text("Codes")
                }.tag(LinkTab.codes)
                LinkGraphsView(wrapper: wrapper).tabItem {
                    Image(selection == .graphs ? "Tab-Graphs-Bold" : "Tab-Graphs").renderingMode(.template)
                    Text("Graphs")
                }.tag(LinkTab.graphs)
            }.onChange(of: selection) { newValue in
                UserDefaults.standard.set(newValue.rawValue, forKey: "tabLink")
            }
        }
    }
}

enum LinkCrossingStyle: Int {
    case pictorial = 1, text = 2
}

struct LinkCrossingsView: View {
    @ObservedObject var wrapper: Wrapper<regina.SharedLink>
    @State private var style: LinkCrossingStyle = (LinkCrossingStyle(rawValue: UserDefaults.standard.integer(forKey: "linkCrossings")) ?? .pictorial)

    static private var posColour = Color("Positive")
    static private var negColour = Color("Negative")

    @AppStorage("displayUnicode") private var unicode = true

    /**
     * Returns an image depicting the given crossing, without the crossing index.
     */
    func iconFor(_ s: regina.StrandRefAlt) -> Image {
        // TODO: Should we preload these?
        if s.crossing().sign() > 0 {
            if s.strand() == 1 {
                return Image("Crossing+U")
            } else {
                return Image("Crossing+L")
            }
        } else {
            if s.strand() == 1 {
                return Image("Crossing-U")
            } else {
                return Image("Crossing-L")
            }
        }
    }
    
    func pictureFor(_ s: regina.StrandRefAlt) -> some View {
        let textHeight = fontSize(forTextStyle: .body)
        let iconSize = textHeight * 1.7
        return ZStack {
            iconFor(s).resizable().frame(width: iconSize, height: iconSize)
            // TODO: Work out the x offset properly, using the text width.
            Text("\(s.crossing().index())")
                .offset(x: textHeight, y: -textHeight)
        }
        // TODO: Choose the padding dynamically.
        .padding(.top, 10.0)
    }

    func textFor(_ s: regina.StrandRefAlt) -> Text {
        let c = s.crossing()
        let colour = (c.sign() > 0 ? LinkCrossingsView.posColour : LinkCrossingsView.negColour)
        
        if unicode {
            if c.sign() > 0 {
                if s.strand() == 1 {
                    return Text("\(c.index())⁺").foregroundColor(colour)
                } else {
                    return Text("\(c.index())₊").foregroundColor(colour)
                }
            } else {
                if s.strand() == 1 {
                    return Text("\(c.index())⁻").foregroundColor(colour)
                } else {
                    return Text("\(c.index())₋").foregroundColor(colour)
                }
            }
        } else {
            if c.sign() > 0 {
                if s.strand() == 1 {
                    return Text("\(c.index())^+").foregroundColor(colour)
                } else {
                    return Text("\(c.index())_+").foregroundColor(colour)
                }
            } else {
                if s.strand() == 1 {
                    return Text("\(c.index())^-").foregroundColor(colour)
                } else {
                    return Text("\(c.index())_-").foregroundColor(colour)
                }
            }
        }
    }
    
    var body: some View {
        VStack(alignment: .leading) {
            HStack {
                Spacer()
                Picker("Display crossings:", selection: $style) {
                    Text("Pictures").tag(LinkCrossingStyle.pictorial)
                    Text("Text").tag(LinkCrossingStyle.text)
                }.fixedSize()
                    .onChange(of: style) { newValue in
                        UserDefaults.standard.set(newValue.rawValue, forKey: "linkCrossings")
                    }
                Spacer()
            }
            .padding(.vertical)

            List {
                ForEach(0..<wrapper.readonly().countComponents(), id: \.self) { i in
                    Section("Component \(i)") {
                        let strands = wrapper.readonly().strandsForComponent(index: i)
                        if strands.isEmpty {
                            Text("Unknot, no crossings")
                        } else if style == .pictorial {
                            // TODO: Fix sizes: 45 is about right for a 17-point font size
                            LazyVGrid(columns: [.init(.adaptive(minimum: 45, maximum: 45))]) {
                                ForEach(strands) { s in
                                    pictureFor(s)
                                }
                            }
                        } else {
                            // TODO: Fix sizes: 25 is about right for a 17-point font size with single digits.
                            LazyVGrid(columns: [.init(.adaptive(minimum: 25, maximum: 25))]) {
                                ForEach(strands) { s in
                                    textFor(s)
                                }
                            }
                        }
                    }
                }
            }.listStyle(.plain)

            Spacer()
        }.padding(.horizontal).textSelection(.enabled)
    }
}

enum HomflyStyle: Int {
    case az = 1, lm = 2
}

struct LinkPolynomialsView: View {
    static let maxAuto = 6;

    @ObservedObject var wrapper: Wrapper<regina.SharedLink>
    @State private var homflyStyle: HomflyStyle = (HomflyStyle(rawValue: UserDefaults.standard.integer(forKey: "linkHomfly")) ?? .az)
    @AppStorage("displayUnicode") private var unicode = true

    // TODO: The compute buttons use wrapper.modifying().
    // This refreshes the view, but will also have the side-effect of re-saving the file.
    // See if we can find a way around this.
    var body: some View {
        let link = wrapper.readonly().heldCopy()

        // TODO: Add a "copy plain text" option on long press
        // TODO: When computing, use progress trackers with cancellation
        // TODO: When computing, can we disable the buttons???
        VStack(alignment: .leading) {
            Text("Jones").font(.headline).padding(.vertical)
            if link.knowsJones() || link.size() <= LinkPolynomialsView.maxAuto {
                var jones = wrapper.readonly().jones()
                if jones.isZero() || jones.minExp() % 2 == 0 {
                    let _: Void = jones.scaleDown(2)
                    if unicode {
                        Text(swiftString(jones.utf8("𝑡")))
                    } else {
                        Text(swiftString(jones.str("t")))
                    }
                } else {
                    if unicode {
                        Text(swiftString(jones.utf8("√𝑡")))
                    } else {
                        Text(swiftString(jones.str("sqrt_t")))
                    }
                }
            } else {
                Button("Compute…", systemImage: "gearshape") {
                    wrapper.modifying().jones()
                }
            }
            HStack {
                Text("HOMFLY-PT").font(.headline).padding(.vertical)
                Spacer()
                Picker("HOMFLY-PT type", selection: $homflyStyle) {
                    Text("(𝛼, 𝑧)").tag(HomflyStyle.az)
                    Text("(ℓ, 𝑚)").tag(HomflyStyle.lm)
                }.pickerStyle(.segmented).fixedSize().labelsHidden()
                    .onChange(of: homflyStyle) { newValue in
                        UserDefaults.standard.set(newValue.rawValue, forKey: "linkHomfly")
                    }
            }
            if link.knowsHomfly() || link.size() <= LinkPolynomialsView.maxAuto {
                if homflyStyle == .az {
                    if unicode {
                        Text(swiftString(wrapper.readonly().homflyAZ().utf8("𝛼", "𝑧")))
                    } else {
                        Text(swiftString(wrapper.readonly().homflyAZ().str("a", "z")))
                    }
                } else {
                    if unicode {
                        Text(swiftString(wrapper.readonly().homflyLM().utf8("ℓ", "𝑚")))
                    } else {
                        Text(swiftString(wrapper.readonly().homflyLM().str("l", "m")))
                    }
                }
            } else {
                Button("Compute…", systemImage: "gearshape") {
                    wrapper.modifying().homflyAZ()
                }
            }
            Text("Kauffman bracket").font(.headline).padding(.vertical)
            if link.knowsBracket() || link.size() <= LinkPolynomialsView.maxAuto {
                if unicode {
                    Text(swiftString(wrapper.readonly().bracket().utf8("𝐴")))
                } else {
                    Text(swiftString(wrapper.readonly().bracket().str("A")))
                }
            } else {
                Button("Compute…", systemImage: "gearshape") {
                    wrapper.modifying().bracket()
                }
            }
            Spacer()
        }.padding(.horizontal).textSelection(.enabled)
    }
}

struct LinkAlgebraView: View {
    static let maxSimp = 50
    static let maxRecognise = 50

    @ObservedObject var wrapper: Wrapper<regina.SharedLink>
    @State var simplifiedGroup: regina.GroupPresentation?
    @State var didSimplify = false
    @State var couldNotSimplify = false
    @AppStorage("displayUnicode") private var unicode = true

    var body: some View {
        let link = wrapper.readonly().heldCopy()
        let autoSimp = (link.size() <= LinkAlgebraView.maxSimp)
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
            
            if group.countRelations() <= LinkAlgebraView.maxRecognise {
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
                Button("Try to simplify", systemImage: "rectangle.compress.vertical") {
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
                        couldNotSimplify = true
                    }
                    #endif
                }
                .alert("Could not simplify", isPresented: $couldNotSimplify) {
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

enum LinkCode: Int {
    case gauss = 1, dt = 2, signature = 3, pd = 4, jenkins = 5
}

struct LinkCodesView: View {
    @ObservedObject var wrapper: Wrapper<regina.SharedLink>
    @State private var selected: LinkCode = (LinkCode(rawValue: UserDefaults.standard.integer(forKey: "linkCode")) ?? .gauss)

    @ViewBuilder func onlyKnots(code: String, plural: Bool) -> some View {
        let capitalised = code.prefix(1).capitalized + code.dropFirst()
        let detail = "\(capitalised) \(plural ? "are" : "is") currently only available for knots, not multi-component links."
        
        HStack {
            Spacer()
            if #available(macOS 14.0, iOS 17.0, *) {
                ContentUnavailableView {
                    Label {
                        Text("No \(code)")
                    } icon: {
                        // For now we use the native size of this icon (64pt).
                        // Probably this is reasonable.
                        Image("Link-Large").renderingMode(.template)
                    }
                } description: {
                    Text(detail)
                }
            } else {
                Text(detail)
            }
            Spacer()
        }
    }
    
    var body: some View {
        VStack(alignment: .leading) {
            // TODO: It's possible the text here needs to be scrollable.
            HStack {
                Spacer()
                Picker("Display code:", selection: $selected) {
                    Text("Gauss codes").tag(LinkCode.gauss)
                    Text("Dowker-Thistlethwaite").tag(LinkCode.dt)
                    Text("Knot signature").tag(LinkCode.signature)
                    Text("Planar diagram code").tag(LinkCode.pd)
                    Text("Jenkins format").tag(LinkCode.jenkins)
                }.fixedSize()
                    .onChange(of: selected) { newValue in
                        UserDefaults.standard.set(newValue.rawValue, forKey: "linkCode")
                    }
                Spacer()
            }
            .padding(.vertical)
            
            let link = wrapper.readonly().heldCopy()
            
            switch (selected) {
            case .gauss:
                if link.countComponents() == 1 {
                    Text("Classical: ").font(.headline) + Text((swiftString(link.gauss())))
                    (Text("Oriented: ").font(.headline) + Text(swiftString(link.orientedGauss()))).padding(.top)
                } else {
                    Spacer()
                    onlyKnots(code: "Gauss codes", plural: true)
                    Spacer()
                }
            case .dt:
                if link.countComponents() == 1 {
                    if link.size() > 26 {
                        Text(swiftString(link.dt(false)))
                    } else {
                        Text(swiftString(link.dt(true)))
                        Text(swiftString(link.dt(false))).padding(.top)
                    }
                } else {
                    Spacer()
                    onlyKnots(code: "Dowker-Thistlethwaite notation", plural: false)
                    Spacer()
                }
            case .signature:
                if link.countComponents() == 1 {
                    // TODO: Find a way to disable word wrap..?
                    // Not sure if SwiftUI makes this possible for Text.
                    Text(swiftString(link.knotSig(true, true)))
                } else {
                    Spacer()
                    onlyKnots(code: "knot signatures", plural: true)
                    Spacer()
                }
           case .pd:
                Text(swiftString(link.pd()))
                if link.countTrivialComponents() > 0 {
                    Text("This link includes one or more zero-crossing unknot components. These components are omitted entirely from the planar diagram code.").padding(.top)
                }
                if link.pdAmbiguous() {
                    Text("This link includes at least one component that consists entirely of over-crossings. The planar diagram code does not carry enough information to reconstruct the orientation of such a component.").padding(.top)
                }
            case .jenkins:
                Text(swiftString(link.jenkins()))
            }
            Spacer()
        }.padding(.horizontal).textSelection(.enabled)
    }
}

enum LinkGraph: Int {
    case tree = 1, nice = 2
}

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
            
            // TODO: Give a "working on it" message, and build the graph in the background (maybe only when it's large)
            
            var tree = regina.TreeDecomposition(wrapper.readonly().heldCopy(), .Upper)
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

struct LinkView_Previews: PreviewProvider {
    static var previews: some View {
        let link = regina.SharedLink(regina.ExampleLink.whitehead())
        LinkView(wrapper: Wrapper<regina.SharedLink>(packet: link))
    }
}
