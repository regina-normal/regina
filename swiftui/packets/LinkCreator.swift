
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
import ReginaEngine

enum LinkType: Int {
    case example = 1, code = 2, torus = 3
}

struct LinkExample: Identifiable, Equatable, Hashable {
    // For now we use the string name as an identifier, since these are unique.
    // There is surely a better way that does not involve an unnecessary new enum.
    let name: String
    let creator: () -> regina.Link
    
    var id: String { name }
        
    init(_ name: String, _ creator: @escaping () -> regina.Link) {
        self.name = name
        self.creator = creator
    }
    
    func hash(into hasher: inout Hasher) {
        hasher.combine(name)
    }
    
    static func == (lhs: LinkExample, rhs: LinkExample) -> Bool {
        lhs.name == rhs.name
    }
    
    static let all: [LinkExample] = [
        .init("Borromean rings", regina.ExampleLink.borromean),
        .init("Conway knot", regina.ExampleLink.conway),
        .init("Figure eight knot", regina.ExampleLink.figureEight),
        .init("Gompf-Scharlemann-Thompson", regina.ExampleLink.gst),
        .init("Hopf link", regina.ExampleLink.hopf),
        .init("Kinoshita-Terasaka knot", regina.ExampleLink.kinoshitaTerasaka),
        .init("Trefoil (left)", regina.ExampleLink.trefoilLeft),
        .init("Trefoil (right)", regina.ExampleLink.trefoilRight),
        .init("Unknot (no crossings)", regina.ExampleLink.unknot),
        .init("Unknot (10-crossing monster)", regina.ExampleLink.monster),
        .init("Unknot (141-crossing Gordian)", regina.ExampleLink.gordian),
        .init("Whitehead link", regina.ExampleLink.whitehead)
    ]
}

// TODO: macOS: popup too small (torus link: width; text code: height)
// TODO: macOS: text code area needs a border or other visual indicator
// TODO: macOS: picker jumps around
struct LinkCreator: View {
    @State private var type: LinkType = (LinkType(rawValue: UserDefaults.standard.integer(forKey: "linkNew")) ?? .example)

    // TODO: Make this selection persistent
    @State private var inputExample: LinkExample = LinkExample.all.first!
    @State private var inputCode = ""
    @State private var inputTorusParams = ""
    @State private var error = false
    @State private var errorDetail: ReginaError?

    @Binding var createBeneath: PacketWrapper?
    @ObservedObject var display: DisplayState
    @Environment(\.dismiss) private var dismiss
    
    var body: some View {
        Form {
            Section {
                switch type {
                case .example:
                    Picker("Example", selection: $inputExample) {
                        ForEach(LinkExample.all) { example in
                            Text(example.name).tag(example)
                        }
                    }
                case .code:
                    TextEditor(text: $inputCode)
                        .font(.system(.body, design: .monospaced))
                        .autocorrectionDisabled()
                        .textInputAutocapitalization(.never)
                case .torus:
                    TextField("Parameters (𝑝, 𝑞)", text: $inputTorusParams)
                        #if !os(macOS)
                        .keyboardType(.numbersAndPunctuation)
                        #endif
                }
            } header: {
                HStack {
                    Spacer()
                    Picker("Type", selection: $type) {
                        Text("Example").tag(LinkType.example)
                        Text("Text code").tag(LinkType.code)
                        Text("Torus link").tag(LinkType.torus)
                    }
                    .pickerStyle(.segmented).fixedSize().labelsHidden()
                    .onChange(of: type) { newValue in
                        UserDefaults.standard.set(newValue.rawValue, forKey: "linkNew")
                    }
                    .textCase(nil)
                    Spacer()
                }
                .padding(.bottom)
            } footer: {
                switch type {
                // TODO: Should we support Jenkins' format also?
                case .code:
                    Text("""
                        Supported text codes:
                            • Knot/link signatures
                            • Gauss codes (oriented / classical)
                            • Dowker-Thistlethwaite notation
                            • Planar diagram codes
                        """)
                default:
                    EmptyView()
                }
            }
        }
        .onChange(of: createBeneath) { value in
            if let parent = createBeneath {
                let child = create()
                if !child.isNull() {
                    var p = parent.packet
                    p.append(child)
                    display.selectAndDisplay(packet: child)
                    dismiss()
                }
                createBeneath = nil
                // This triggers another call to this same onChange closure,
                // but this time with createBeneath as nil.
            }
        }
        .alert(isPresented: $error, error: errorDetail) { _ in
            Button("OK") {}
        } message: { error in
            if let detail = error.detail {
                Text(detail)
            }
        }
    }
    
    func create() -> regina.SharedPacket {
        switch type {
        case .example:
            var ans = regina.SharedLink(inputExample.creator()).asPacket()
            ans.setLabel(std.string(inputExample.name))
            return ans
        case .code:
            let code = regina.stripWhitespace(std.string(inputCode))
            if code.empty() {
                error = true
                errorDetail = .init("Empty text code", detail: "Please type a text code for the new link into the area provided.")
                return .init()
            }
            var ans = regina.SharedLink.make(code).asPacket()
            if ans.isNull() {
                error = true
                errorDetail = .init("Invalid text code", detail: "I could not interpret the given text code as representing a knot or link.")
                return .init()
            }
            ans.setLabel(code)
            return ans
        case .torus:
            if let match = inputTorusParams.wholeMatch(of: /[^0-9\-]*(\d+)[^0-9\-]+(\d+)[^0-9\-]*/) {
                if let p = Int32(match.1), let q = Int32(match.2) {
                    var ans = regina.SharedLink(regina.ExampleLink.torus(p, q)).asPacket()
                    ans.setLabel(std.string("Torus (\(p), \(q))"))
                    return ans
                } else {
                    error = true
                    errorDetail = .init("Parameters too large", detail: "The torus link parameters (p,q) must fit into 32-bit integers.")
                    return .init()
                }
            } else {
                error = true
                errorDetail = .init("Invalid torus link parameters", detail: "The torus link parameters (p,q) must be non-negative integers.")
                return .init()
            }
        }
    }
}

/*
 TODO: Fix broken previews
 #Preview {
    LinkCreator()
}
*/
