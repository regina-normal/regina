
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Swift User Interface                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

struct GeneralSettingsView: View {
    @AppStorage("displayUnicode") private var displayUnicode = true
    @AppStorage("graphvizLabels") private var graphvizLabels = true

    var body: some View {
        Form {
            Toggle("Use unicode for mathematical symbols", isOn: $displayUnicode)
            Toggle("Labels on dual graphs and link graphs", isOn: $graphvizLabels)
        }
    }
}

struct PythonSettingsView: View {
    var body: some View {
        Form {
            Text("Python support is not yet available.")
        }
    }
}

struct SettingsView: View {
    var body: some View {
        TabView {
            GeneralSettingsView()
                .tabItem {
                    Label("General", systemImage: "gear")
                }
            PythonSettingsView()
            // TODO: apple.terminal in SFSymbols 5.. but see "terminal"
                .tabItem {
                    Label("Python", systemImage: "apple.terminal")
                }
        }
        .padding(.all)
        // TODO: What is a reasonable size here?
        .frame(width: 375.0, height: 150.0)
    }
}

#Preview {
    SettingsView()
}
