
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

struct ReginaError: LocalizedError {
    let title: String
    let detail: String?
    
    var errorDescription: String? { title }
    var failureReason: String? { detail }
    
    init(_ title: String) {
        self.title = title
        self.detail = nil
    }

    init(_ title: String, detail: String) {
        self.title = title
        self.detail = detail
    }
}

// These functions are here because, as of Xcode 15.1-beta2, the conversion between
// Swift and C++ strings on visionOS is broken (it _does_ work on iOS/iPadOS/macOS).
//
// Once this is fixed:
// - all occurrences of swiftString(s) should be replaced with String(s);
// - all occurrences of cxxString(s) should be replaced with s;
// - these functions can be removed entirely (for all platforms).
#if os(visionOS)
import ReginaEngine
func swiftString(_ s: std.string) -> String {
    return String(cString: s.__c_strUnsafe())
}
func cxxString(_ s: String) -> std.string {
    return s.withCString { c in
        std.string(c)
    }
}
extension std.string {
    func swiftString() -> String {
        return String(cString: s.__c_strUnsafe())
    }
}
#else
func swiftString(_ s: std.string) -> String {
    return String(s)
}
func cxxString(_ s: String) -> std.string {
    return std.string(s)
}
extension std.string {
    func swiftString() -> String {
        return String(self)
    }
}
#endif

#if os(macOS)
func fontSize(forTextStyle style: NSFont.TextStyle) -> CGFloat {
    return NSFontDescriptor.preferredFontDescriptor(forTextStyle: style).pointSize
}
#else
func fontSize(forTextStyle style: UIFont.TextStyle) -> CGFloat {
    return UIFontDescriptor.preferredFontDescriptor(withTextStyle: style).pointSize
}
#endif
