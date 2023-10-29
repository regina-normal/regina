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

// TODO: Remember whether the data file was compressed when opened, and write it the same way.
class ReginaDocument: ReferenceFileDocument {
    // TODO: We need a way to observe changes to the packet and/or the packet tree.
    // This should happen via the ObservableObject protocol (which ReferenceFileDocument inherits).
    var root: regina.SharedPacket

    static var readableContentTypes: [UTType] { [.reginaData, .snapPeaTriangulation] }
    static var writableContentTypes: [UTType] { [.reginaData] }

    init(label: String = "New Regina data") {
        self.root = regina.SharedPacket(std.string(label))
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
