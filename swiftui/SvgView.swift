
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
import WebKit
import CxxStdlib

struct SvgView: UIViewRepresentable {
    var svg: Data
    
    init(data: Data) {
        self.svg = data
    }
    
    init(cxxString: std.string) {
        // TODO: There is surely a better way of initialising Data here.
        // TODO: Do we need to manage the lifetime of cxxString?
        // self.svg = Data(String(cString: cxxString.__c_strUnsafe()).utf8)
        self.svg = Data(String(cxxString).utf8)
    }

    func makeUIView(context: Context) -> WKWebView {
        return WKWebView()
    }
    
    func updateUIView(_ webView: WKWebView, context: Context) {
        // TODO: What to use as the base URL here?
        // TODO: Work out how to centre the content.
        // TODO: Can we make it zoomable? Try webView.allowsMagnification = true
        // TODO: Do we want to set WKWebView's scalesPageToFit property to yes?
        webView.load(svg, mimeType: "image/svg+xml", characterEncodingName: "utf-8", baseURL: URL(string: "file:///")!)
    }
}
