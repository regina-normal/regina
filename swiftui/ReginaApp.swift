//
//  ReginaApp.swift
//  Regina
//
//  Created by Ben Burton on 23/10/2023.
//  Copyright © 2023 Regina Development Team. All rights reserved.
//

import SwiftUI

@main
struct ReginaApp: App {
    var body: some Scene {
        DocumentGroup(newDocument: ReginaDocument()) { file in
            ContentView(document: file.$document)
        }
    }
}
