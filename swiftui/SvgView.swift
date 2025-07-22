
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
import WebKit
import CxxStdlib

// TODO: The SVG is miniscule on iPhone.

// TODO: What to use as the base URL here?
// TODO: Work out how to centre the content.

// TODO: There is surely a better way of initialising the Data object. Also, do we need to manage the lifetime of cxxString?

#if os(macOS)
struct SvgView: NSViewRepresentable {
    var svg: Data
    @Environment(\.colorScheme) var colorScheme

    init(data: Data) {
        self.svg = data
    }
    
    init(cxxString: std.string) {
        self.svg = Data(String(cString: cxxString.__c_strUnsafe()).utf8)
    }

    func makeNSView(context: Context) -> WKWebView {
        var ans = WKWebView()
        ans.setValue(false, forKey: "drawsBackground")
        ans.allowsMagnification = true
        return ans
    }
    
    func updateNSView(_ webView: WKWebView, context: Context) {
        webView.load(svg, mimeType: "image/svg+xml", characterEncodingName: "utf-8", baseURL: URL(string: "file:///")!)
    }
}
#else
struct SvgView: UIViewRepresentable {
    var svg: Data
    @Environment(\.colorScheme) var colorScheme

    init(data: Data) {
        self.svg = data
    }
    
    init(cxxString: std.string) {
        self.svg = Data(String(cString: cxxString.__c_strUnsafe()).utf8)
    }

    func makeUIView(context: Context) -> WKWebView {
        // On iOS / iPadOS, magnification appears to be enabled by default.
        var ans = WKWebView()
        ans.isOpaque = false
        ans.backgroundColor = UIColor.clear
        return ans
    }
    
    func updateUIView(_ webView: WKWebView, context: Context) {
        webView.load(svg, mimeType: "image/svg+xml", characterEncodingName: "utf-8", baseURL: URL(string: "file:///")!)
    }
}
#endif
