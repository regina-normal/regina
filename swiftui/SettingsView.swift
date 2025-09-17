
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

import SwiftUI

// TODO: Flesh this out with proper settings as we need them.
// TODO: Remember the last visited settings page in app settings.
// TODO: Provide a way for non-mac platforms to access settings.

struct SettingsView: View {
    #if !os(macOS)
    enum AppIcon {
        case classic
        case reverse
        
        var name: String? {
            switch self {
            case .classic: "Classic"
            case .reverse: nil
            }
        }

        static var current: AppIcon {
            if let name = UIApplication.shared.alternateIconName {
                if name == "Classic" {
                    return .classic
                }
            }
            return .reverse
        }
    }
    #endif
    
    @AppStorage("displayUnicode") private var displayUnicode = true
    @AppStorage("warnNonEmbedded") private var warnNonEmbedded = true
    @AppStorage("graphvizLabels") private var graphvizLabels = true
    @AppStorage("pythonAutoIndent") private var autoIndent = true
    @AppStorage("pythonSpacesPerTab") private var spacesPerTab = 4
    
    #if !os(macOS)
    @State var appIcon = AppIcon.current
    #endif

    var body: some View {
        Form {
            Section("General") {
                Toggle("Use unicode for mathematical symbols", isOn: $displayUnicode)
                Toggle("Warn before generating immersed and/or non-singular surfaces", isOn: $warnNonEmbedded)
                Toggle("Labels on dual graphs and link graphs", isOn: $graphvizLabels)
                #if !os(macOS)
                Picker("App icon", selection: $appIcon) {
                    // Users can't see these icons, but as soon as they change the icon
                    // the system will show them the new one that they chose.
                    // So I think this is okay.
                    Text("Inverted").tag(AppIcon.reverse)
                    Text("Classic").tag(AppIcon.classic)
                }
                .onChange(of: appIcon) { _, newValue in
                    UIApplication.shared.setAlternateIconName(newValue.name)
                }
                #endif
            }
            Section("Python") {
                Toggle("Auto-indent", isOn: $autoIndent)
                Stepper("Spaces per tab: \(spacesPerTab)", value: $spacesPerTab, in: 1...16)
            }
        }
        #if os(macOS)
        .formStyle(.grouped)
        #else
        .background(Color(.systemGroupedBackground))
        #endif
        .navigationTitle("Regina Settings")
    }
}

#Preview {
    SettingsView()
}
