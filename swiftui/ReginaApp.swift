
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

// TODO: Get rid of the DocumentKit import

import SwiftUI

// TODO: Accent colour does not show when the app opens to the file browser.
// TODO: Create a full set of dark mode assets.
// In particular, the attachment icon is almost invisible on visionOS.
// In dark mode, the crossing arrows need to be brighter to help visionOS.

// TODO: Go over compact tab icons and see if they need to be a little thicker.

// TODO: Possibly the toolbar actions could be a tiny bit heavier in stroke width.

@main
struct ReginaApp: App {
    var body: some Scene {
        /* TODO: Return to a plain old new document, and find a better way to open example files.
        DocumentGroup(newDocument: { ReginaDocument() }) { file in
            TreeView(packet: file.document.root).toolbarRole(.automatic)
        */
        DocumentGroup() { () -> ReginaDocument in
            do {
                return try ReginaDocument(example: "sample", title: "Introductory Examples")
            } catch {
                return ReginaDocument()
            }
        } editor: { file in
            MainView(document: file.document)
        }
        /*
        #if !os(macOS)
        .additionalNavigationBarButtonItems(
            leading: [],
            trailing: [ DocumentGroupToolbarItem(icon: UIImage(systemName: "books.vertical")) {
                // TODO: Here is where we offer the example data files.
                // This will _not_ work with macOS, which will need a menu item or some such instead.
            } ]
        )
        #endif
         */
        // Note: To support multiple document types, add additional DocumentGroup scenes.
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
            let link = regina.SharedSpatialLink(regina.ExampleLink.spatialTrefoil())
            // TODO: SpatialLinkVolume(packet: link)
            SpatialLinkVolume()
        }
        .windowStyle(.volumetric)
        .defaultSize(width: 0.7, height: 0.7, depth: 0.7, in: .meters)
        #endif
        #if os(macOS)
        Settings {
            SettingsView()
        }
        #endif
    }
}
