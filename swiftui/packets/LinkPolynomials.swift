
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
        let link = wrapper.packet.heldCopy()

        // TODO: Add a "copy plain text" option on long press
        // TODO: When computing, use progress trackers with cancellation
        // TODO: When computing, can we disable the buttons???
        VStack(alignment: .leading) {
            Text("Jones").font(.headline).padding(.vertical)
            if link.knowsJones() || link.size() <= Self.maxAuto {
                var jones = wrapper.packet.jones()
                if jones.isZero() || jones.minExp() % 2 == 0 {
                    let _: Void = jones.scaleDown(2)
                    if unicode {
                        Text(String(jones.utf8("𝑡")))
                    } else {
                        Text(String(jones.str("t")))
                    }
                } else {
                    if unicode {
                        Text(String(jones.utf8("√𝑡")))
                    } else {
                        Text(String(jones.str("sqrt_t")))
                    }
                }
            } else {
                Button("Compute…", systemImage: "gearshape") {
                    wrapper.packet.jones()
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
            if link.knowsHomfly() || link.size() <= Self.maxAuto {
                if homflyStyle == .az {
                    if unicode {
                        Text(String(wrapper.packet.homflyAZ().utf8("𝛼", "𝑧")))
                    } else {
                        Text(String(wrapper.packet.homflyAZ().str("a", "z")))
                    }
                } else {
                    if unicode {
                        Text(String(wrapper.packet.homflyLM().utf8("ℓ", "𝑚")))
                    } else {
                        Text(String(wrapper.packet.homflyLM().str("l", "m")))
                    }
                }
            } else {
                Button("Compute…", systemImage: "gearshape") {
                    wrapper.packet.homflyAZ()
                }
            }
            Text("Kauffman bracket").font(.headline).padding(.vertical)
            if link.knowsBracket() || link.size() <= Self.maxAuto {
                if unicode {
                    Text(String(wrapper.packet.bracket().utf8("𝐴")))
                } else {
                    Text(String(wrapper.packet.bracket().str("A")))
                }
            } else {
                Button("Compute…", systemImage: "gearshape") {
                    wrapper.packet.bracket()
                }
            }
            Spacer()
        }.padding(.horizontal).textSelection(.enabled)
    }
}

