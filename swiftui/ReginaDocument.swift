
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
import UniformTypeIdentifiers
import ReginaEngine

extension UTType {
    static let reginaData = UTType(exportedAs: "org.computop.regina-data")
    static let snapPeaTriangulation = UTType(exportedAs: "org.computop.snappea-triangulation")
}

// TODO: Remember whether the data file was compressed when opened, and write it the same way.
class ReginaDocument: ReferenceFileDocument {
    // TODO: We need a way to observe changes to the packet and/or the packet tree.
    // This should happen via the ObservableObject protocol (which ReferenceFileDocument inherits).
    var root: regina.SharedPacket

    static var readableContentTypes: [UTType] { [.reginaData, .snapPeaTriangulation] }
    static var writableContentTypes: [UTType] { [.reginaData] }

    init(label: String = "New Regina data") {
        root = regina.SharedPacket(std.string(label))
    }
    
    // TODO: Add an action that opens sample-misc

    required init(configuration: ReadConfiguration) throws {
        guard let data = configuration.file.regularFileContents
        else {
            throw CocoaError(.fileReadCorruptFile)
        }

        switch configuration.contentType {
        case .reginaData:
            root = data.withUnsafeBytes { bytes in
                regina.SharedPacket.open(bytes.baseAddress, data.count)
            }
        case .snapPeaTriangulation:
            // TODO: support this
            throw CocoaError(.fileReadUnsupportedScheme)
        default:
            // TODO: what *should* we be throwing here?
            throw CocoaError(.fileReadUnsupportedScheme)
        }

        // TODO: use bool conversion
        if root.isNull() {
            throw CocoaError(.fileReadCorruptFile)
        }
    }
    
    typealias Snapshot = std.string
    
    func snapshot(contentType: UTType) throws -> Snapshot {
        return root.save()
    }
    
    func fileWrapper(snapshot: Snapshot, configuration: WriteConfiguration) throws -> FileWrapper {
        // TODO: We are doing several deep copies here.
        // TODO: Support compression
        return .init(regularFileWithContents: String(snapshot).data(using: .utf8)!)
    }
}
