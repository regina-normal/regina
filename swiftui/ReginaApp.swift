
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

// What follows is a collected TODO list as the SwiftUI interface is put together.
// Current these TODOs are ancient, and any or all may no longer be relevant.
//
// TODO: Ancient notes:
// - Create a full set of dark mode assets.
//   + In particular, the attachment icon is almost invisible on visionOS.
//   + In dark mode, the crossing arrows need to be brighter to help visionOS.
// - Go over compact tab icons and see if they need to be a little thicker.
// - Possibly the toolbar actions could be a tiny bit heavier in stroke width.
//
// TODO: Modern notes:
// - Redo the launch/accent colours, since we are no longer tied to the brown app icon.
// - Have a very good look at info.plist and overhaul if necessary.
// - Documents from the file browser open the app but not the documents themselves.
//   .. but it works on macOS
//   .. and the same problem occurs even without DocumentGroupLaunchScene
// - Overhaul SvgView when we reach that point in the implementation.
// - Overhaul how we manage document icons that are registered with the OS.
// - Flesh out the settings bundle.

@main
struct ReginaApp: App {
    var body: some Scene {
        DocumentGroup(newDocument: { ReginaDocument() }) { file in
            MainView(document: file.document)
        }
        #if !os(macOS)
        DocumentGroupLaunchScene("Regina") {
            // These buttons cannot have images - otherwise this messes up the launch view.
            // Moreover, it looks like the system only allows two buttons before creating
            // a "More..." sub-menu that we'd rather not have.
            // Finally: it looks like these actions are not shown on visionOS at all. Sigh.
            NewDocumentButton("Create Document")
            Button("Open Example…") {
                // TODO: Support opening example files
            }
        } background: {
            Rectangle().fill(.launchBackground.gradient)
        }
        #if os(iOS)
        .commands {
            // TODO: Menus for iPadOS
        }
        #endif
        #endif
        
        // TODO: Properly implement spatiallink-volume for visionOS
        #if os(visionOS)
        /*
        WindowGroup(for: PacketWrapper.ID.self) { link in
            SpatialLinkVolume(packet: link)
        }
        .windowStyle(.volumetric)
        .defaultSize(width: 0.5, height: 0.5, depth: 0.5, in: .meters)
         */
        WindowGroup(id: "spatiallink-volume", for: Date.self) { value in
            // TODO: Bloody hell. There must be some way to get the actual link data into the new window.
            // We need to send _some_ data, and it appears this data needs to be different each time;
            // otherwise when opening a fresh window the packet does not get reset.
            // For the moment we pass the current date/time to ensure uniqueness.
            // let link = regina.SharedSpatialLink(regina.ExampleLink.spatialTrefoil())
            // TODO: SpatialLinkVolume(packet: link)
            SpatialLinkVolume()
        }
        .windowStyle(.volumetric)
        .defaultSize(width: 0.7, height: 0.7, depth: 0.7, in: .meters)
        #endif

        #if os(macOS)
        Settings {
            SettingsView()
                .frame(width: 400, height: 560)
        }
        .commands {
            // TODO: Menus for macOS
        }
        #else
        // TODO: Enforce just one settings window, and make it accessible.
        WindowGroup("Settings", id: "settings") {
            SettingsView()
                .navigationTitle("Settings")
                .handlesExternalEvents(preferring: [], allowing: [])
        }
        #if os(visionOS)
        .defaultSize(CGSize(width: 600, height: 720))
        #endif
        #endif
    }
}
