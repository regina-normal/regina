//
//  ContentView.swift
//  Regina
//
//  Created by Ben Burton on 23/10/2023.
//  Copyright © 2023 Regina Development Team. All rights reserved.
//

import SwiftUI

struct ContentView: View {
    @Binding var document: ReginaDocument

    var body: some View {
        Label(String(document.root.label()), systemImage: /*@START_MENU_TOKEN@*/"42.circle"/*@END_MENU_TOKEN@*/)
        // TextEditor(text: $document.text)
        // Label(title: document.data.label())
    }
}

#Preview {
    ContentView(document: .constant(ReginaDocument()))
}
