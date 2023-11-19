
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

// TODO: Choose the default radius properly.

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
    
    #if os(macOS)
    static let compColours = [ NSColor.systemTeal, NSColor.systemYellow, NSColor.systemRed, NSColor.systemPurple, NSColor.systemGreen, NSColor.systemBrown, NSColor.systemBlue, NSColor.systemPink, NSColor.systemOrange, NSColor.systemIndigo ]
    #else
    static let compColours = [ UIColor.systemTeal, UIColor.systemYellow, UIColor.systemRed, UIColor.systemPurple, UIColor.systemGreen, UIColor.systemBrown, UIColor.systemBlue, UIColor.systemPink, UIColor.systemOrange, UIColor.systemIndigo ]
    #endif
    
    func arc(_ a: regina.SpatialLink.Node, _ b: regina.SpatialLink.Node, component: Int, radius: CGFloat, scene: SCNScene) -> SCNNode {
        let c = SCNCylinder(radius: radius, height: a.distance(b))
        // These cylinders are very thin; they do not need to be very smooth.
        c.radialSegmentCount = 12
        c.firstMaterial?.diffuse.contents = SpatialLink3D.compColours[component % SpatialLink3D.compColours.count]

        let node = SCNNode(geometry: c)
        node.position = SCNVector3(node: a.midpoint(b))
        node.look(at: SCNVector3(node: a), up: scene.rootNode.worldUp, localFront: node.worldUp)
        return node
    }
    
    func ball(_ p: regina.SpatialLink.Node, component: Int, radius: CGFloat, scene: SCNScene) -> SCNNode {
        let s = SCNSphere(radius: radius)
        s.segmentCount = 12
        s.firstMaterial?.diffuse.contents = SpatialLink3D.compColours[component % SpatialLink3D.compColours.count]

        let node = SCNNode(geometry: s)
        node.position = SCNVector3(node: p)
        return node
    }
    
    func fillScene(scene: SCNScene) {
        let link = wrapper.readonly().heldCopy()
        let radius = link.radius()

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
                    scene.rootNode.addChildNode(ball(n, component: i, radius: radius, scene: scene))
                    
                    if let p = prev {
                        scene.rootNode.addChildNode(arc(p, n, component: i, radius: radius, scene: scene))
                    }
                    prev = n
                }
                
                let n = link.__nodeUnsafe(i, 0).pointee
                scene.rootNode.addChildNode(arc(prev!, n, component: i, radius: radius, scene: scene))
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
            scene.rootNode.name = String(wrapper.id)
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
            scene.rootNode.name = String(wrapper.id)
        }
        return view
    }
    #endif

    // TODO: For a change in radius, just update existing elements.
    #if os(macOS)
    func updateNSView(_ nsView: NSViewType, context: Context) {
        if let scene = nsView.scene {
            if scene.rootNode.name == String(wrapper.id) {
                // There has been a change to the packet already being displayed.
                // Update the geometry of the scene but do not change the scene object, which controls the camera angle / zoom / etc.
                scene.rootNode.enumerateChildNodes { (node, stop) in
                    node.removeFromParentNode()
                }
                fillScene(scene: scene)
                return
            }
        }
        
        // We are reusing the same view to display a different spatial link.
        // Put in a completely new scene, which will recompute a new camera angle / zoom / etc.
        // to reflect the new geometry.
        let scene = SCNScene()
        fillScene(scene: scene)
        scene.rootNode.name = String(wrapper.id)
        nsView.scene = scene
    }
    #else
    func updateUIView(_ uiView: UIViewType, context: Context) {
        if let scene = uiView.scene {
            if scene.rootNode.name == String(wrapper.id) {
                // There has been a change to the packet already being displayed.
                // Update the geometry of the scene but do not change the scene object, which controls the camera angle / zoom / etc.
                scene.rootNode.enumerateChildNodes { (node, stop) in
                    node.removeFromParentNode()
                }
                fillScene(scene: scene)
                return
            }
        }
        
        // We are reusing the same view to display a different spatial link.
        // Put in a completely new scene, which will recompute a new camera angle / zoom / etc.
        // to reflect the new geometry.
        let scene = SCNScene()
        fillScene(scene: scene)
        scene.rootNode.name = String(wrapper.id)
        uiView.scene = scene
    }
    #endif
}

struct SpatialLinkView: View {
    let maxNodes = 500

    var wrapper: Wrapper<regina.SharedSpatialLink>
    
    @State var tooManyNodes = false

    var body: some View {
        // TODO: Make it fit the screen. (Look in particular at the trefoil example on iPhone.)
        // Note: it does seem that SceneKit is automatically scaling the image to fill the screen,
        // but on iPhone it fills vertically and overfills horizontally.
        // Note: the camera looks down from above (from high z value down onto the plane).
        SpatialLink3D(wrapper: wrapper)
        // TODO: When we have more buttons, start using (placement: ...).
        .toolbar {
            // TODO: Make these edits actually save the file.
            ToolbarItem {
                Button("Refine", systemImage: "point.bottomleft.forward.to.point.topright.scurvepath") {
                    if wrapper.readonly().size() * 2 > maxNodes {
                        tooManyNodes = true
                    } else {
                        var p = wrapper.modifying()
                        p.refine()
                    }
                }
                .alert("Too much detail", isPresented: $tooManyNodes) {
                    Button("OK") {}
                } message: {
                    Text("I am not brave enough to create a spatial link with more than \(maxNodes) nodes.")
                }
            }
            ToolbarItem {
                Button("Thinner", systemImage: "smallcircle.fill.circle") {
                    var p = wrapper.modifying()
                    p.setRadius(p.radius() / 1.2)
                }
            }
            ToolbarItem {
                Button("Thicker", systemImage: "largecircle.fill.circle") {
                    var p = wrapper.modifying()
                    p.setRadius(p.radius() * 1.2)
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
