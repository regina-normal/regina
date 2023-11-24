
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
        let colour = (c.sign() > 0 ? Self.posColour : Self.negColour)
        
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

            // TODO: This needs to be scrollable.
            //List {
                ForEach(0..<wrapper.packet.countComponents(), id: \.self) { i in
                    Section("Component \(i)") {
                        let strands = wrapper.packet.strandsForComponent(index: i)
                        if strands.isEmpty {
                            Text("Unknot, no crossings")
                        } else if style == .pictorial {
                            // TODO: Fix sizes: 45 is about right for a 17-point font size
                            LazyVGrid(columns: [.init(.adaptive(minimum: 45, maximum: 45))]) {
                                ForEach(strands) { s in
                                    // TODO: Context menu grabs the wrong size box.
                                    pictureFor(s)
                                        .contextMenu {
                                            Group {
                                                Button("Change crossing \(s.crossingIndex())") {
                                                    var packet = wrapper.packet
                                                    packet.change(s)
                                                }
                                                Button("Resolve crossing \(s.crossingIndex())") {
                                                    var packet = wrapper.packet
                                                    packet.resolve(s)
                                                }
                                            }
                                        }
                                }
                            }
                        } else {
                            // TODO: Fix sizes: 25 is about right for a 17-point font size with single digits.
                            LazyVGrid(columns: [.init(.adaptive(minimum: 25, maximum: 25))]) {
                                ForEach(strands) { s in
                                    textFor(s)
                                    // TODO: Context menu
                                }
                            }
                        }
                    }
                }
            //}.listStyle(.plain)

            Spacer()
        }.padding(.horizontal).textSelection(.enabled)
    }
}

