
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
import UniformTypeIdentifiers
import ReginaEngine

extension UTType {
    static let reginaData = UTType(exportedAs: "org.computop.regina-data")
    static let snapPeaTriangulation = UTType(exportedAs: "org.computop.snappea-triangulation")
}

// TODO: Remember whether the data file was compressed when opened, and write it the same way.
// Currently we are saving uncompressed, always.
class ReginaDocument: ReferenceFileDocument {
    // TODO: We need a way to observe changes to the packet and/or the packet tree.
    // This should happen via the ObservableObject protocol (which ReferenceFileDocument inherits),
    // and it will need to somehow communicate with Regina's packet listener interface.
    var root: regina.SharedPacket
    var title: String

    static var readableContentTypes: [UTType] { [.reginaData, .snapPeaTriangulation] }
    static var writableContentTypes: [UTType] { [.reginaData] }

    init() {
        // The root packet is not visible, and does not need a packet label.
        root = regina.SharedContainer.make().asPacket()
        
        // TODO: Add a helpful child text packet to explain what users should do.
        
        // This should never appear, since a new document gets saved and then opened under its real filename (e.g., "Untitled.rga").
        title = "New Document"
    }
    
    init(example: String, title: String) throws {
        // TODO: Work out how to make the example file read-only and the
        // document treated as a new document with an appropriate filename.
        guard let fileURL = Bundle.main.url(forResource: example, withExtension: "rga", subdirectory: "examples") else {
            throw CocoaError(.fileNoSuchFile)
        }
        self.title = title

        let data = try Data(contentsOf: fileURL)
        root = data.withUnsafeBytes { bytes in
            regina.SharedPacket.open(bytes.baseAddress, data.count)
        }
        if root.isNull() {
            throw CocoaError(.fileReadCorruptFile)
        }
    }
    
    required init(configuration: ReadConfiguration) throws {
        guard let data = configuration.file.regularFileContents else {
            throw CocoaError(.fileReadCorruptFile)
        }
        if let filename = configuration.file.filename {
            self.title = URL(fileURLWithPath: filename).deletingPathExtension().lastPathComponent
        } else {
            self.title = "Regina Document"
        }

        switch configuration.contentType {
        case .reginaData:
            root = data.withUnsafeBytes { bytes in
                regina.SharedPacket.open(bytes.baseAddress, data.count)
            }
        case .snapPeaTriangulation:
            // TODO: support reading SnapPea data files
            throw CocoaError(.fileReadUnsupportedScheme)
        default:
            // TODO: what *should* we be throwing here?
            throw CocoaError(.fileReadUnsupportedScheme)
        }

        if root.isNull() {
            throw CocoaError(.fileReadCorruptFile)
        }
    }
    
    func snapshot(contentType: UTType) throws -> std.string {
        return root.save()
    }
    
    func fileWrapper(snapshot: std.string, configuration: WriteConfiguration) throws -> FileWrapper {
        // TODO: Can we std::move() from snapshot into the new String?
        // TODO: Check exactly when String.data() can return null.
        // TODO: update file URL
        if let filename = configuration.existingFile?.filename {
            self.title = URL(fileURLWithPath: filename).deletingPathExtension().lastPathComponent
        }
        return .init(regularFileWithContents: String(snapshot).data(using: .utf8)!)
    }
}
