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

struct MainView: View {
    @StateObject var document: ReginaDocument
    
    @ScaledMetric private var gap = 20
    
    @ViewBuilder var body: some View {
        switch document.status {
        case .loading:
            VStack(spacing: gap) {
                // TODO: Possibly use a ProgressView here instead of an icon.
                Image(systemName: "TODO").font(.largeTitle)
                    .symbolEffect(.pulse)
                Text("Loading…")
            }
            .multilineTextAlignment(.center).padding(.all)
        case .open(let root):
            // TODO: What is the toolbar role for?
            // TODO: Pass a proper title.
            // TODO: We get two toolbars. The first is from the DocumentGroup and
            // the second is from TreeView.
            TreeView(packet: root, title: "Test Document")
                .toolbarRole(.editor)
        case .error(let filename):
            // TODO: Use the filename stem here.
            NavigationStack {
                VStack(spacing: gap) {
                    Image(systemName: "info.circle").font(.largeTitle)
                    // Text("I could not open the document \(filename).")
                    Text("I could not open the document.")
                    Text("Either this document is inaccessible, or it is not a Regina data file.")
                }
                .multilineTextAlignment(.center).padding(.all)
            }
        }
    }
}
