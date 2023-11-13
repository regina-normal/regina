
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
        self.init(x: Float(n.x), y: Float(n.y), z: Float(n.z))
    }
}

struct SpatialLink3D: UIViewRepresentable {
    typealias UIViewType = SCNView
    
    @Binding var packet: regina.SharedSpatialLink
    @Binding var radius: CGFloat
    @Binding var colour: UIColor
    
    func arc(_ a: regina.SpatialLink.Node, _ b: regina.SpatialLink.Node, scene: SCNScene) -> SCNNode {
        let c = SCNCylinder(radius: radius, height: a.distance(b))
        // These cylinders are very thin; they do not need to be very smooth.
        c.radialSegmentCount = 12
        c.firstMaterial?.diffuse.contents = colour

        let node = SCNNode(geometry: c)
        node.position = SCNVector3(node: a.midpoint(b))
        node.look(at: SCNVector3(node: a), up: scene.rootNode.worldUp, localFront: node.worldUp)
        return node
    }
    
    func ball(_ p: regina.SpatialLink.Node, scene: SCNScene) -> SCNNode {
        let s = SCNSphere(radius: radius)
        s.segmentCount = 12
        s.firstMaterial?.diffuse.contents = colour
        
        let node = SCNNode(geometry: s)
        node.position = SCNVector3(node: p)
        return node
    }
    
    func fillScene(scene: SCNScene) {
        // I suspect this is some Swift incompatibility with ListView.
        // Perhaps audit all use of ListView and replace it with integer loops.
        let link = packet.heldCopy()

        // Since the Link functions obtain internal pointers into link, we need to ensure the lifespan of link.
        withExtendedLifetime(link) {
            // We use index-based loops here, since visionOS struggles with C++ bindings for regina::ListView and std::vector (though macOS and iOS seem fine).
            for i in 0..<link.countComponents() {
                let nodes = link.componentSize(i)
                if nodes == 0 {
                    continue
                }
                
                var prev: regina.SpatialLink.Node?
                
                for j in 0..<nodes {
                    let n = link.__nodeUnsafe(i, j).pointee
                    scene.rootNode.addChildNode(ball(n, scene: scene))
                    
                    if let p = prev {
                        scene.rootNode.addChildNode(arc(p, n, scene: scene))
                    }
                    prev = n
                }
                
                let n = link.__nodeUnsafe(i, 0).pointee
                scene.rootNode.addChildNode(arc(prev!, n, scene: scene))
            }
        }

    }
    
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
    
    func updateUIView(_ uiView: UIViewType, context: Context) {
        // TODO: For a change in radius or colour, just update existing elements.
        if let scene = uiView.scene {
            scene.rootNode.enumerateChildNodes { (node, stop) in
                node.removeFromParentNode()
            }
            fillScene(scene: scene)
        }
    }
}

struct SpatialLinkView: View {
    // TODO: Choose the radius properly.
    // TODO: Support custom colours in the data file

    @State var packet: regina.SharedSpatialLink
    @State var radius: CGFloat = 0.2
    @State var colour = UIColor.systemTeal
    
    var body: some View {
        // TODO: Make it fit the screen. (Look in particular at the trefoil example on iPhone.)
        // Note: it does seem that SceneKit is automatically scaling the image to fill the screen,
        // but on iPhone it fills vertically and overfills horizontally.
        // Note: the camera looks down from above (from high z value down onto the plane).
        ZStack(alignment: .topTrailing) {
            SpatialLink3D(packet: $packet, radius: $radius, colour: $colour)
            // TODO: Make this action panel pretty
            // TODO: Make these edits actually change the file.
            // TODO: RESPOND TO PACKET CHANGES
            VStack(alignment: .leading) {
                Button("Refine", systemImage: "point.topleft.down.to.point.bottomright.curvepath") {
                    packet.refine()
                    packet = packet.modified()
                }
                Button("Thinner", systemImage: "arrow.down.forward.and.arrow.up.backward") {
                    radius /= 1.2
                }
                Button("Thicker", systemImage: "arrow.up.backward.and.arrow.down.forward") {
                    radius *= 1.2
                }
                Button("Reset", systemImage: "smallcircle.filled.circle") {
                    radius = 0.2
                    colour = UIColor.systemTeal
                }
            }.padding([.top, .trailing])
        }
    }
}

struct SpatialLinkView_Previews: PreviewProvider {
    static var previews: some View {
        let link = regina.SharedSpatialLink(regina.ExampleLink.spatialTrefoil())
        SpatialLinkView(packet: link)
    }
}
