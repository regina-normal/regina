
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
#if os(visionOS)
import RealityKit
#endif
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
        let link = packet.heldCopy()

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
    // TODO: Tie the radius and colour to the packet
    // TODO: Choose the radius properly.

    @State var packet: regina.SharedSpatialLink
    @State var radius: CGFloat = 0.2
    @State var colour = UIColor.systemTeal
    
    #if os(visionOS)
    @Environment(\.openWindow) private var openWindow
    #endif
    
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
                #if os(visionOS)
                Button("3-D", systemImage: "move.3d") {
                    openWindow(id: "spatiallink-volume")
                }
                #endif
            }.padding([.top, .trailing])
        }
    }
}

#if os(visionOS)
// TODO: How do we manage _closing_ the volume?
struct SpatialLinkVolume: View {
    // TODO: Tie the radius and colour to the packet

    @State var packet: regina.SharedSpatialLink
    @State var radius: Float = 0.2
    @State var colour = UIColor.systemTeal
    
    // TODO: Can we just apply the material once, to everything?
    func ball(_ p: regina.SpatialLink.Node, material: RealityKit.Material) -> ModelEntity {
        let sphere = MeshResource.generateSphere(radius: radius)
        let entity = ModelEntity(mesh: sphere, materials: [material])
        // TODO: Extend classes to make this easier
        entity.position = [Float(p.x), Float(p.y), Float(p.z)]
        return entity
    }
    
    func arc(_ a: regina.SpatialLink.Node, _ b: regina.SpatialLink.Node, material: RealityKit.Material) -> Entity {
        let len = Float(a.distance(b))
        let cylinder = MeshResource.generateCylinder(height: len, radius: radius)
        let entity = ModelEntity(mesh: cylinder, materials: [material])
        
        let m = a.midpoint(b)
        entity.position = [Float(m.x), Float(m.y), Float(m.z)]
        // Cylinders initially point along the y axis.
        // TODO: Clean this up a bit.
        entity.transform.rotation = simd_quatf(from: [0,1,0], to: [Float(a.x - b.x), Float(a.y - b.y), Float(a.z - b.z)] * (1 / len))
        return entity
    }

    func buildLink(material: RealityKit.Material) -> ModelEntity {
        let link = packet.heldCopy()
        let root = ModelEntity()
        
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
                    root.addChild(ball(n, material: material))
                    
                    if let p = prev {
                        root.addChild(arc(p, n, material: material))
                    }
                    prev = n
                }
                
                let n = link.__nodeUnsafe(i, 0).pointee
                root.addChild(arc(prev!, n, material: material))
            }
        }

        return root
    }

    var body: some View {
        ZStack(alignment: .bottom) {
            GeometryReader3D { geometry in
                RealityView { content in
                    let material = SimpleMaterial(color: .systemTeal, isMetallic: false)
                    let entity = buildLink(material: material)
                    content.add(entity)

                    // Get the smallest dimension of the volume.
                    let volumeBounds = content.convert(geometry.frame(in: .local), from: .local, to: content)
                    let minExtent = volumeBounds.extents.min()
                    
                    // Get the size of the entity that we are displaying.
                    let modelBounds = entity.visualBounds(relativeTo: nil)
                    
                    // Ensure that the model will fit inside the volume, with a little wiggle room to spare.
                    let factor = minExtent * 0.95 / modelBounds.extents.max()

                    entity.position = modelBounds.center * (-factor)
                    entity.scale = [factor, factor, factor]
                } update: { content in
                    // TODO: Fix the duplicated code
                    if let orig = content.entities.first {
                        content.remove(orig)
                    }
                    
                    let material = SimpleMaterial(color: .systemTeal, isMetallic: false)
                    let entity = buildLink(material: material)
                    content.add(entity)

                    // Get the smallest dimension of the volume.
                    let volumeBounds = content.convert(geometry.frame(in: .local), from: .local, to: content)
                    let minExtent = volumeBounds.extents.min()
                    
                    // Get the size of the entity that we are displaying.
                    let modelBounds = entity.visualBounds(relativeTo: nil)
                    
                    // Ensure that the model will fit inside the volume, with a little wiggle room to spare.
                    let factor = minExtent * 0.95 / modelBounds.extents.max()

                    entity.position = modelBounds.center * (-factor)
                    entity.scale = [factor, factor, factor]
                }
            }
        }
        // TODO: Zstack puts controls halfway back
        // TODO: Flashing on updates
        HStack {
            Button("Refine", systemImage: "point.topleft.down.to.point.bottomright.curvepath") {
                // TODO: This does not trigger an update
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
                // TODO: Remove this
                packet = regina.SharedSpatialLink(regina.ExampleLink.spatialTrefoil())
            }
        }
        //.buttonStyle(.borderless)
        .labelStyle(.iconOnly)
        .padding()
        .glassBackgroundEffect()
    }
}
#endif

struct SpatialLinkView_Previews: PreviewProvider {
    static var previews: some View {
        let link = regina.SharedSpatialLink(regina.ExampleLink.spatialTrefoil())
        SpatialLinkView(packet: link)
    }
}

#if os(visionOS)
struct SpatialLinkVolume_Previews: PreviewProvider {
    static var previews: some View {
        let link = regina.SharedSpatialLink(regina.ExampleLink.spatialTrefoil())
        SpatialLinkVolume(packet: link)
    }
}
#endif
