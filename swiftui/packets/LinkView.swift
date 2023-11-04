//
//  LinkView.swift
//  Regina for iOS
//
//  Created by Benjamin Burton on 5/11/2023.
//  Copyright © 2023 Regina Development Team. All rights reserved.
//

import SwiftUI
import ReginaEngine

struct LinkView: View {
    let packet: regina.SharedLink
    
    init(packet: regina.SharedLink) {
        self.packet = packet
    }

    var body: some View {
        Text(/*@START_MENU_TOKEN@*/"Hello, World!"/*@END_MENU_TOKEN@*/)
    }
}

struct LinkView_Previews: PreviewProvider {
    static var previews: some View {
        LinkView(packet: regina.SharedLink(regina.ExampleLink.whitehead()))
    }
}
