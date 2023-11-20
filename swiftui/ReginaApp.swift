
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

// TODO: Accent colour does not show when the app opens to the file browser.
// TODO: Create a full set of dark mode assets.
// In particular, the attachment icon is almost invisible on visionOS.
// In dark mode, the crossing arrows need to be brighter to help visionOS.

// TODO: Go over compact tab icons and see if they need to be a little thicker.

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
            TreeView(packet: file.document.root, title: file.document.title).toolbarRole(.automatic)
        }
        // Note: To support multiple document types, add additional DocumentGroup scenes.
        #if os(macOS)
        Settings {
            SettingsView()
        }
        #endif
    }
}
