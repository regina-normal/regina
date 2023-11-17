
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
import SceneKit
import ReginaEngine

extension SCNVector3 {
    init(node n: regina.SpatialLink.Node) {
        #if os(macOS)
        self.init(x: CGFloat(n.x), y: CGFloat(n.y), z: CGFloat(n.z))
        #else
        self.init(x: Float(n.x), y: Float(n.y), z: Float(n.z))
        #endif
    }
}

#if os(macOS)
typealias ViewRepresentable = NSViewRepresentable
#else
typealias ViewRepresentable = UIViewRepresentable
#endif

struct SpatialLink3D: ViewRepresentable {
    typealias UIViewType = SCNView
    
    @ObservedObject var wrapper: Wrapper<regina.SharedSpatialLink>
    @Binding var radius: CGFloat
    #if os(macOS)
    @Binding var colour: [NSColor]
    #else
    @Binding var colour: [UIColor]
    #endif
    
    func arc(_ a: regina.SpatialLink.Node, _ b: regina.SpatialLink.Node, component: Int, scene: SCNScene) -> SCNNode {
        let c = SCNCylinder(radius: radius, height: a.distance(b))
        // These cylinders are very thin; they do not need to be very smooth.
        c.radialSegmentCount = 12
        c.firstMaterial?.diffuse.contents = colour[component % colour.count]

        let node = SCNNode(geometry: c)
        node.position = SCNVector3(node: a.midpoint(b))
        node.look(at: SCNVector3(node: a), up: scene.rootNode.worldUp, localFront: node.worldUp)
        return node
    }
    
    func ball(_ p: regina.SpatialLink.Node, component: Int, scene: SCNScene) -> SCNNode {
        let s = SCNSphere(radius: radius)
        s.segmentCount = 12
        s.firstMaterial?.diffuse.contents = colour[component % colour.count]

        let node = SCNNode(geometry: s)
        node.position = SCNVector3(node: p)
        return node
    }
    
    func fillScene(scene: SCNScene) {
        let link = wrapper.readonly().heldCopy()

        // Since the Link functions obtain internal pointers into link, we need to ensure the lifespan of link.
        withExtendedLifetime(link) {
            // We use index-based loops here, since visionOS struggles with C++ bindings for regina::ListView and std::vector (though macOS and iOS seem fine).
            // TODO: Audit all use of ListView
            for i in 0..<link.countComponents() {
                let nodes = link.componentSize(i)
                if nodes == 0 {
                    continue
                }
                
                var prev: regina.SpatialLink.Node?
                
                for j in 0..<nodes {
                    let n = link.__nodeUnsafe(i, j).pointee
                    scene.rootNode.addChildNode(ball(n, component: i, scene: scene))
                    
                    if let p = prev {
                        scene.rootNode.addChildNode(arc(p, n, component: i, scene: scene))
                    }
                    prev = n
                }
                
                let n = link.__nodeUnsafe(i, 0).pointee
                scene.rootNode.addChildNode(arc(prev!, n, component: i, scene: scene))
            }
        }
    }
    
    #if os(macOS)
    func makeNSView(context: Context) -> SCNView {
        let view = SCNView()
        view.scene = SCNScene()
        view.allowsCameraControl = true
        view.autoenablesDefaultLighting = true
        view.backgroundColor = NSColor.clear

        if let scene = view.scene {
            fillScene(scene: scene)
        }
        return view
    }
    #else
    func makeUIView(context: Context) -> SCNView {
        let view = SCNView()
        view.scene = SCNScene()
        view.allowsCameraControl = true
        view.autoenablesDefaultLighting = true
        view.backgroundColor = UIColor.clear

        if let scene = view.scene {
            fillScene(scene: scene)
        }
        return view
    }
    #endif

    // TODO: For a change in radius or colour, just update existing elements.
    #if os(macOS)
    func updateNSView(_ nsView: NSViewType, context: Context) {
        if let scene = nsView.scene {
            scene.rootNode.enumerateChildNodes { (node, stop) in
                node.removeFromParentNode()
            }
            fillScene(scene: scene)
        }
    }
    #else
    func updateUIView(_ uiView: UIViewType, context: Context) {
        if let scene = uiView.scene {
            scene.rootNode.enumerateChildNodes { (node, stop) in
                node.removeFromParentNode()
            }
            fillScene(scene: scene)
        }
    }
    #endif
}

struct SpatialLinkView: View {
    // TODO: Tie the radius and colour to the packet
    // TODO: Choose the radius properly.

    @StateObject var wrapper: Wrapper<regina.SharedSpatialLink>
    @State var radius: CGFloat = 0.2
    #if os(macOS)
    @State var colour = [ NSColor.systemTeal, NSColor.systemYellow, NSColor.systemRed ]
    #else
    @State var colour = [ UIColor.systemTeal, UIColor.systemYellow, UIColor.systemRed ]
    #endif
    
    var body: some View {
        // TODO: Make it fit the screen. (Look in particular at the trefoil example on iPhone.)
        // Note: it does seem that SceneKit is automatically scaling the image to fill the screen,
        // but on iPhone it fills vertically and overfills horizontally.
        // Note: the camera looks down from above (from high z value down onto the plane).
        SpatialLink3D(wrapper: wrapper, radius: $radius, colour: $colour)
        .toolbar {
            // TODO: On the toolbar, buttons are far apart. Is this correct?
            // TODO: Make these edits actually save the file.
            ToolbarItem {
                Button("Refine", systemImage: "point.bottomleft.forward.to.point.topright.scurvepath") {
                    var p = wrapper.modifying()
                    p.refine()
                }
            }
            ToolbarItem {
                Button("Thinner", systemImage: "smallcircle.fill.circle") {
                    radius /= 1.2
                }
            }
            ToolbarItem {
                Button("Thicker", systemImage: "largecircle.fill.circle") {
                    radius *= 1.2
                }
            }
        }
    }
}

struct SpatialLinkView_Previews: PreviewProvider {
    static var previews: some View {
        let link = regina.SharedSpatialLink(regina.ExampleLink.spatialBorromean())
        SpatialLinkView(wrapper: Wrapper<regina.SharedSpatialLink>(packet: link))
    }
}
