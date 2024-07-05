
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

enum LinkCode: Int {
    case gauss = 1, dt = 2, signature = 3, pd = 4, jenkins = 5
}

struct LinkCodesView: View {
    @ObservedObject var wrapper: Wrapper<regina.SharedLink>
    @State private var selected: LinkCode = (LinkCode(rawValue: UserDefaults.standard.integer(forKey: "linkCode")) ?? .gauss)

    // TODO: Split this into a separate view class.
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
    
    // TODO: Split this into a separate view class.
    @ViewBuilder func noSigs() -> some View {
        let detail = "Knot signatures are only available for links with fewer than 64 components."
        
        HStack {
            Spacer()
            if #available(macOS 14.0, iOS 17.0, *) {
                ContentUnavailableView {
                    Label {
                        Text("No knot signature")
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
            // TODO: The text here needs to be scrollable.
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
            
            let link = wrapper.packet.heldCopy()
            
            switch selected {
            case .gauss:
                if link.countComponents() == 1 {
                    Text("Classical: ").font(.headline) + Text(String(link.gauss()))
                    (Text("Oriented: ").font(.headline) + Text(String(link.orientedGauss()))).padding(.top)
                } else {
                    Spacer()
                    onlyKnots(code: "Gauss codes", plural: true)
                    Spacer()
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
                    Spacer()
                    onlyKnots(code: "Dowker-Thistlethwaite notation", plural: false)
                    Spacer()
                }
            case .signature:
                if link.countComponents() < 64 {
                    // TODO: Find a way to disable word wrap..?
                    // Not sure if SwiftUI makes this possible for Text.
                    Text(String(link.knotSig(true, true)))
                } else {
                    Spacer()
                    noSigs()
                    Spacer()
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

