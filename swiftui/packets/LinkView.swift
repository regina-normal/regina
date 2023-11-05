
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

struct LinkView: View {
    let packet: regina.SharedLink
    @State private var selection = 0
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
                    Image(selection == 1 ? "Tab-Crossings-Bold" : "Tab-Crossings").renderingMode(.template)
                    Text("Crossings")
                }.tag(1)
                LinkPolynomialsView(packet: packet).tabItem {
                    Image("Tab-Polynomials").renderingMode(.template)
                    Text("Polynomials")
                }.tag(2)
                LinkAlgebraView(packet: packet).tabItem {
                    Image("Tab-Algebra").renderingMode(.template)
                    Text("Algebra")
                }.tag(3)
                LinkCodesView(packet: packet).tabItem {
                    Image("Tab-Codes").renderingMode(.template)
                    Text("Codes")
                }.tag(4)
                LinkGraphsView(packet: packet).tabItem {
                    Image(selection == 5 ? "Tab-Graphs-Bold" : "Tab-Graphs").renderingMode(.template)
                    Text("Graphs")
                }.tag(5)
            }
        }
    }
}

struct LinkCrossingsView: View {
    let packet: regina.SharedLink

    var body: some View {
        Text("Hello")
    }
}

struct LinkPolynomialsView: View {
    let packet: regina.SharedLink

    var body: some View {
        // TODO: How to make the vstack fill?
        VStack(alignment: .leading) {
            Text("Jones").font(.headline)
            // TODO: Get to Jones
            Text(String(packet.sharedPtr().pointee.jenkins()))
            Text("HOMFLY-PT").font(.headline)
            Text("Kauffman bracket").font(.headline)
        }
    }
}

struct LinkAlgebraView: View {
    let packet: regina.SharedLink

    var body: some View {
        Text("Very")
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
                }
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
        Text("Day")
    }
}

struct LinkView_Previews: PreviewProvider {
    static var previews: some View {
        LinkView(packet: regina.SharedLink(regina.ExampleLink.whitehead()))
    }
}
