
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
    // TODO: Use a deep copy here?
    let packet: regina.SharedSpatialLink
    // TODO: Choose the radius properly.
    static let radius = 0.2
    // TODO: Support custom colours in the data file
    static let colour = UIColor.blue
    
    func arc(_ a: regina.SpatialLink.Node, _ b: regina.SpatialLink.Node, scene: SCNScene) -> SCNNode {
        let diff = regina.SpatialLink.Node(b.x - a.x, b.y - a.y, b.z - a.z)
        let length = CGFloat(sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z))

        let c = SCNCylinder(radius: SpatialLink3D.radius, height: length)
        // These cylinders are very thin; they do not need to be very smooth.
        c.radialSegmentCount = 12
        c.firstMaterial?.diffuse.contents = SpatialLink3D.colour

        let node = SCNNode(geometry: c)
        node.position = SCNVector3(node: regina.SpatialLink.Node((a.x + b.x) / 2, (a.y + b.y) / 2, (a.z + b.z) / 2))
        node.look(at: SCNVector3(node: a), up: scene.rootNode.worldUp, localFront: node.worldUp)
        return node
    }
    
    func ball(_ p: regina.SpatialLink.Node, scene: SCNScene) -> SCNNode {
        let s = SCNSphere(radius: SpatialLink3D.radius)
        s.segmentCount = 12
        s.firstMaterial?.diffuse.contents = SpatialLink3D.colour
        
        let node = SCNNode(geometry: s)
        node.position = SCNVector3(node: p)
        return node
    }
    
    func makeUIView(context: Context) -> some UIView {
        // TODO: Background is still white in dark mode.
        // TODO: Change foreground colour in dark mode also.
        let view = SCNView()
        view.scene = SCNScene()
        view.allowsCameraControl = true
        view.autoenablesDefaultLighting = true

        let link = packet.held()
        
        for c in link.components() {
            if (c.isEmpty) {
                continue
            }

            var prev: regina.SpatialLink.Node?
            
            for n in c {
                view.scene?.rootNode.addChildNode(ball(n, scene: view.scene!))
                
                if let p = prev {
                    // TODO: There are lots of bangs here.
                    view.scene?.rootNode.addChildNode(arc(p, n, scene: view.scene!))
                }
                prev = n
            }
            
            let n = c[0]
            // TODO: There are lots of bangs here.
            view.scene?.rootNode.addChildNode(arc(prev!, n, scene: view.scene!))
        }

        return view
    }
    
    func updateUIView(_ uiView: UIViewType, context: Context) {
        // TODO: what to put here?
    }
}

struct SpatialLinkView: View {
    let packet: regina.SharedSpatialLink
    
    init(packet: regina.SharedSpatialLink) {
        self.packet = packet
    }

    var body: some View {
        // TODO: Make it fit the screen. (Look in particular at the trefoil example on iPhone.)
        // TODO: Check that handedness is actually preserved.
        SpatialLink3D(packet: packet)
    }
}
/*
struct SpatialLinkView_Previews: PreviewProvider {
    static var previews: some View {
        // TODO: How to get data for the preview?
        SpatialLinkView()
    }
}
*/
