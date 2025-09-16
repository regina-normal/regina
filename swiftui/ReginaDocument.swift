
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
import UniformTypeIdentifiers
import ReginaEngine

extension UTType {
    static let reginaData = UTType(exportedAs: "org.computop.regina-data")
    static let snapPeaTriangulation = UTType(exportedAs: "org.computop.snappea-triangulation")
}

enum ReginaDocumentError: Error {
    /// Indicates that the document is still in a loading state.
    case documentLoading
    /// Indicates that the document tried to load but failed.
    case documentHasError
    /// Indicates that the C++ engine returned a null pointer where data was expected.
    case noData
}

// TODO: We need a way to observe changes to the packet and/or the packet tree.
// This should happen via the ObservableObject protocol (which ReferenceFileDocument inherits),
// and it will need to somehow communicate with Regina's packet listener interface.

// TODO: Remember whether the data file was compressed when opened, and write it the same way.
// Currently we are saving uncompressed, always.
final class ReginaDocument: ReferenceFileDocument {
    static var readableContentTypes: [UTType] { [.reginaData, .snapPeaTriangulation] }
    static var writableContentTypes: [UTType] { [.reginaData] }

    enum Origin {
        case url
        case example(title: String)
    }
    
    enum Status {
        /**
         * The file is currently loading.
         */
        case loading
        /**
         * The file has been successfully loaded (which means it should be visible to the user).
         */
        case open(root: regina.SharedPacket)
        /**
         * There was an error attempting to open the file.
         */
        case error
    }
    
    let origin: Origin
    @Published var status: Status
    
    var isOpen: Bool {
        if case .open = status {
            return true
        } else {
            return false
        }
    }
    
    init() {
        // The root packet is not visible, and does not need a packet label.
        // TODO: Add a helpful child text packet to explain what users should do.
        origin = .url
        status = .open(root: regina.SharedContainer.make().asPacket())
    }
    
    init(example: String, title: String) throws {
        // TODO: Work out how to make the example file read-only.
        guard let fileURL = Bundle.main.url(forResource: example, withExtension: "rga", subdirectory: "examples") else {
            throw CocoaError(.fileNoSuchFile)
        }
        origin = .example(title: title)
        status = .loading

        // We load this in the background, much like how we do it
        // in the ReadConfiguration initialiser below.
        Task { [weak self] in
            let data = try Data(contentsOf: fileURL)
            let root = data.withUnsafeBytes { bytes in
                regina.SharedPacket.open(bytes.baseAddress, data.count)
            }
            guard let self else {
                print("EXAMPLE LOAD CANCELLED (#1)")
                return
            }
            Task { @MainActor [weak self] in
                guard let self else {
                    print("EXAMPLE LOAD CANCELLED (#2)")
                    return
                }
                if root.isNull() {
                    self.status = .error
                } else {
                    self.status = .open(root: root)
                }
            }
        }
    }
    
    required init(configuration: ReadConfiguration) throws {
        guard let data = configuration.file.regularFileContents else {
            throw CocoaError(.fileReadUnknown)
        }
        origin = .url
        status = .loading
        let filename = configuration.file.filename

        switch configuration.contentType {
        case .reginaData:
            // We will load the document in a background thread, since
            // files could be large.
            // To cancel a load, the user can just close the window.
            // The task will finish and then throw the result away.
            Task { [weak self] in
                let root = data.withUnsafeBytes { bytes in
                    regina.SharedPacket.open(bytes.baseAddress, data.count)
                }
                guard let self else {
                    print("DOCUMENT LOAD CANCELLED (#1)")
                    return
                }
                Task { @MainActor [weak self] in
                    guard let self else {
                        print("DOCUMENT LOAD CANCELLED (#2)")
                        return
                    }
                    if root.isNull() {
                        self.status = .error
                    } else {
                        self.status = .open(root: root)
                    }
                }
            }
        case .snapPeaTriangulation:
            // TODO: support reading SnapPea data files
            throw CocoaError(.fileReadUnsupportedScheme)
        default:
            throw CocoaError(.fileReadUnsupportedScheme)
        }
    }
    
    func snapshot(contentType: UTType) throws -> Data {
        print("DOCUMENT SNAPSHOT")
        switch status {
        case .loading:
            throw ReginaDocumentError.documentLoading
        case .error:
            throw ReginaDocumentError.documentHasError
        case .open(let root):
            let save = root.save()
            guard let bytes = save.__dataUnsafe() else {
                throw ReginaDocumentError.noData
            }
            return Data(bytes: bytes, count: save.length())
        }
    }
    
    func fileWrapper(snapshot: Data, configuration: WriteConfiguration) throws -> FileWrapper {
        print("DOCUMENT WRITE: \(configuration.existingFile?.filename ?? "New file")")
        return .init(regularFileWithContents: snapshot)
    }
}
