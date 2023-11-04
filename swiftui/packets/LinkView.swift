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
    @State private var selection = 0
    
    init(packet: regina.SharedLink) {
        self.packet = packet
    }

    var body: some View {
        // TODO: Packet header
        // TODO: Ipad 11" portrait, tab icons jump around when selected??
        TabView(selection: $selection) {
            LinkCrossingsView(packet: packet).tabItem {
                Image(selection == 1 ? "Tab-Crossings-Bold" : "Tab-Crossings").renderingMode(.template)
                Text("Crossings")
            }.tag(1)
            LinkPolynomialsView(packet: packet).tabItem {
                Image("Tab-Polynomials").renderingMode(.template)
                Text("Polynomials")
            }.tag(2)
            LinkAlgebraView(packet: packet).tabItem {
                Image("Tab-Algebra").renderingMode(.template)
                Text("Algebra")
            }.tag(4)
            LinkCodesView(packet: packet).tabItem {
                Image("Tab-Codes").renderingMode(.template)
                Text("Codes")
            }.tag(4)
            LinkGraphsView(packet: packet).tabItem {
                Image(selection == 5 ? "Tab-Graphs-Bold" : "Tab-Graphs").renderingMode(.template)
                Text("Graphs")
            }.tag(5)
        }
    }
}

struct LinkCrossingsView: View {
    let packet: regina.SharedLink

    var body: some View {
        Text("Hello")
    }
}

struct LinkPolynomialsView: View {
    let packet: regina.SharedLink

    var body: some View {
        // TODO: How to make the vstack fill?
        VStack(alignment: .leading) {
            Text("Jones").font(.headline)
            // TODO: Get to Jones
            Text(String(packet.sharedPtr().pointee.jenkins()))
            Text("HOMFLY-PT").font(.headline)
            Text("Kauffman bracket").font(.headline)
        }
    }
}

struct LinkAlgebraView: View {
    let packet: regina.SharedLink

    var body: some View {
        Text("Very")
    }
}

struct LinkCodesView: View {
    let packet: regina.SharedLink

    var body: some View {
        Text("Nice")
    }
}

struct LinkGraphsView: View {
    let packet: regina.SharedLink

    var body: some View {
        Text("Day")
    }
}

struct LinkView_Previews: PreviewProvider {
    static var previews: some View {
        LinkView(packet: regina.SharedLink(regina.ExampleLink.whitehead()))
    }
}
