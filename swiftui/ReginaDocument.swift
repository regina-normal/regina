//
//  ReginaDocument.swift
//  Regina
//
//  Created by Ben Burton on 23/10/2023.
//  Copyright © 2023 Regina Development Team. All rights reserved.
//

import SwiftUI
import UniformTypeIdentifiers
import ReginaEngine

extension UTType {
    static let reginaData = UTType(exportedAs: "org.computop.regina-data")
    static let snapPeaTriangulation = UTType(exportedAs: "org.computop.snappea-triangulation")
}

// TODO: Should we be using class ReferenceFileDocument instead?
struct ReginaDocument: FileDocument {
    // TODO: We need a way to observe changes to the packet and/or the packet tree
    var root: regina.SharedPacket

    static var readableContentTypes: [UTType] { [.reginaData, .snapPeaTriangulation] }
    static var writableContentTypes: [UTType] { [.reginaData] }

    init(label: String = "New Regina data") {
        self.root = regina.SharedPacket(std.string(label))
    }
    
    // TODO: Add an action that opens sample-misc

    init(configuration: ReadConfiguration) throws {
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
    
    func fileWrapper(configuration: WriteConfiguration) throws -> FileWrapper {
        // TODO: We need a FileWrapper with a custom write()
        // TODO: ensure that writing is done in a threadsafe manner
//        let data = text.data(using: .utf8)!
//        return .init(regularFileWithContents: data)
        throw CocoaError(.fileWriteUnsupportedScheme)
    }
}
