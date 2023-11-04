//
//  SwiftUIView.swift
//  Regina for iOS
//
//  Created by Benjamin Burton on 4/11/2023.
//  Copyright © 2023 Regina Development Team. All rights reserved.
//

import SwiftUI
import ReginaEngine

struct TextView: View {
    let packet: regina.SharedText

    init(packet: regina.SharedText) {
        self.packet = packet
    }

    var body: some View {
        TextEditor(text: .constant(String(packet.text())))
    }
}

struct TextView_Previews: PreviewProvider {
    static var sampleText: regina.SharedText {
        var packet = regina.SharedText.make()
        packet.setText("Some sample text to play with.")
        return packet
    }

    static var previews: some View {
        TextView(packet: sampleText)
    }
}
