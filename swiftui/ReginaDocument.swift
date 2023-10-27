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
}

struct ReginaDocument: FileDocument {
    var text: String

    init(text: String = "Hello, world!") {
        let version = String(cString: regina.versionString())
        self.text = text + " - version \(version)"
    }

    static var readableContentTypes: [UTType] { [.reginaData] }

    init(configuration: ReadConfiguration) throws {
        guard let data = configuration.file.regularFileContents,
              let string = String(data: data, encoding: .utf8)
        else {
            throw CocoaError(.fileReadCorruptFile)
        }
        text = string
    }
    
    func fileWrapper(configuration: WriteConfiguration) throws -> FileWrapper {
        let data = text.data(using: .utf8)!
        return .init(regularFileWithContents: data)
    }
    
    // TODO: I guess we need a write() function?
}
