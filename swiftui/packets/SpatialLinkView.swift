
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Swift User Interface                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

import SwiftUI
import SceneKit
#if os(visionOS)
import RealityKit
#endif
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
        c.firstMaterial?.diffuse.contents = Self.compColours[component % Self.compColours.count]

        let node = SCNNode(geometry: c)
        node.position = SCNVector3(node: a.midpoint(b))
        node.look(at: SCNVector3(node: a), up: scene.rootNode.worldUp, localFront: node.worldUp)
        return node
    }
    
    func ball(_ p: regina.SpatialLink.Node, component: Int, radius: CGFloat, scene: SCNScene) -> SCNNode {
        let s = SCNSphere(radius: radius)
        s.segmentCount = 12
        s.firstMaterial?.diffuse.contents = Self.compColours[component % Self.compColours.count]

        let node = SCNNode(geometry: s)
        node.position = SCNVector3(node: p)
        return node
    }
    
    func fillScene(scene: SCNScene) {
        let link = wrapper.packet.heldCopy()
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
    static let maxNodes = 500

    var wrapper: Wrapper<regina.SharedSpatialLink>
    
    @State private var errTooManyNodes = false

    #if os(visionOS)
    @Environment(\.openWindow) private var openWindow
    #endif
    
    var body: some View {
        // TODO: Make it fit the screen. (Look in particular at the trefoil example on iPhone.)
        // Note: it does seem that SceneKit is automatically scaling the image to fill the screen,
        // but on iPhone it fills vertically and overfills horizontally.
        // Note: the camera looks down from above (from high z value down onto the plane).
        #if os(visionOS)
        // TODO: We are not actually passing in the data here.
        let view = SpatialLinkVolume()
        #else
        let view = SpatialLink3D(wrapper: wrapper)
        #endif
        // TODO: When we have more buttons, start using (placement: ...).
        view.toolbar {
            // TODO: Make these edits actually save the file.
            ToolbarItem {
                // TODO: Curves need a different name for SFSymbols <5.
                Button("Refine", systemImage: "point.bottomleft.forward.to.point.topright.scurvepath") {
                    if wrapper.packet.size() * 2 > Self.maxNodes {
                        errTooManyNodes = true
                    } else {
                        var p = wrapper.packet
                        p.refine()
                    }
                }
                .alert("Too much detail", isPresented: $errTooManyNodes) {
                    Button("OK") {}
                } message: {
                    Text("I am not brave enough to create a spatial link with more than \(Self.maxNodes) nodes.")
                }
            }
            ToolbarItem {
                Button("Thinner", systemImage: "smallcircle.fill.circle") {
                    var p = wrapper.packet
                    p.setRadius(p.radius() / 1.2)
                }
            }
            ToolbarItem {
                Button("Thicker", systemImage: "largecircle.fill.circle") {
                    var p = wrapper.packet
                    p.setRadius(p.radius() * 1.2)
                }
                #if os(visionOS)
                Button("3-D", systemImage: "move.3d") {
                    // TODO: open next to, not in front of
                    openWindow(id: "spatiallink-volume", value: Date.now)
                }
                #endif
            }
        }
    }
}

#if os(visionOS)
// TODO: How do we manage _closing_ the volume?
struct SpatialLinkVolume: View {
    // TODO: Tie the radius and colour to the packet

    // TODO: Use an example packet until we figure out how to get the real data in.
    @State var packet = regina.SharedSpatialLink(regina.ExampleLink.spatialTrefoil())
    // @State var packet: regina.SharedSpatialLink
    @State var radius: Float = 0.2
    @State var colour = UIColor.systemTeal
    // TODO: Rotation3D.zero is deprecated.
    @State var rotation: Rotation3D = Rotation3D.zero
    @State var rotationInProgress: Rotation3D = Rotation3D.zero
    
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
        
        root.generateCollisionShapes(recursive: true)
        root.components.set(InputTargetComponent())

        return root
    }

    var body: some View {
        let material = SimpleMaterial(color: .systemTeal, isMetallic: false)
        let entity = buildLink(material: material)

        ZStack(alignment: .bottom) {
            GeometryReader3D { geometry in
                RealityView { content in
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
                    if let root = content.entities.first {
                        // root.orientation = simd_quatf(rotation * rotationInProgress)
                        root.orientation = simd_quatf(rotationInProgress * rotation)
                    }

                    // TODO: Fix this update.
                    /*
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
                     */
                    
                }.gesture(RotateGesture3D()
                          // TODO: Rotations are falling out of the volume bounds
                          // TODO: Pinch to scale?
                          // TODO: The rotation angles and origin are all wrong.
                          // Possibly we need to change coordinate system, and also update entity.position.
                    .targetedToAnyEntity()
                    .onChanged { value in
                        rotationInProgress = value.rotation
                    }
                    .onEnded { value in
                        rotationInProgress = Rotation3D.zero
                        // rotation = rotation * value.rotation
                        rotation = value.rotation * rotation
                    }
                )
            }
            /*
            // TODO: Fix flashing on updates
            HStack {
             // TODO: Change systemImage names to match iOS.
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
             */
        }
    }
}
#endif

struct SpatialLinkView_Previews: PreviewProvider {
    static var previews: some View {
        let link = regina.SharedSpatialLink(regina.ExampleLink.spatialBorromean())
        SpatialLinkView(wrapper: Wrapper<regina.SharedSpatialLink>(packet: link))
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
