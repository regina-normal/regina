
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

package normal.packetui.triangulation;

import java.awt.*;
import java.util.*;
import javax.swing.*;
import javax.swing.tree.*;
import normal.engine.Engine;
import normal.engine.subcomplex.*;
import normal.engine.packet.NPacket;
import normal.engine.triangulation.*;
import normal.packetui.*;
import org.gjt.btools.gui.component.PaddedPane;

/**
 * An interface through which a user can view details of the composition
 * of a triangulation.
 */
public class CompositionViewer extends DefaultPacketViewer
        implements PacketInfoUI {
    /**
     * The triangulation under examination.
     */
    private NTriangulation triangulation;
    /**
     * The underlying calculation engine.
     */
    private Engine engine;

    /**
     * Displays composition details.
     */
    private JTree composition;
    /**
     * Root node of the composition tree.
     */
    private DefaultMutableTreeNode rootNode;
    /**
     * Model for the composition tree.
     */
    private DefaultTreeModel model;

    /**
     * Components to disable when the packet is being edited.
     */
    private Vector componentsToDisable = new Vector();

    /**
     * Create a new interface to display the given algebraic information.
     *
     * @param triangulation the triangulation whose information we will
     * display.
     */
    public CompositionViewer(NTriangulation triangulation, Engine engine) {
        super();
        this.triangulation = triangulation;
        this.engine = engine;
        init();
    }

    /**
     * Create the interface elements.
     */
    private void init() {
        // Set up the tree.
        rootNode = new DefaultMutableTreeNode("Composition");
        model = new DefaultTreeModel(rootNode);
        composition = new JTree(model);
        composition.setShowsRootHandles(true);
        composition.setRootVisible(false);
        componentsToDisable.add(composition);

        // Put everything together.
        setLayout(new BorderLayout());
        add(new PaddedPane(new JScrollPane(composition), 5),
            BorderLayout.CENTER);
    }

    public NPacket getPacket() {
        return triangulation;
    }

    public void reflectPacket() {
        Enumeration e = componentsToDisable.elements();
        while (e.hasMoreElements())
            ((Component)e.nextElement()).setEnabled(true);

        rootNode.removeAllChildren();

        DefaultMutableTreeNode category;
        DefaultMutableTreeNode instance;
        long n, i, j, k;

        // Look for lens spaces.
        category = null;
        NLayeredLensSpace lens;
        NLayeredSolidTorus torus;
        n = triangulation.getNumberOfComponents();
        for (i = 0; i < n; i++) {
            lens = engine.isLayeredLensSpace(triangulation.getComponent(i));
            if (lens != null) {
                if (category == null) {
                    category = new DefaultMutableTreeNode("Lens Spaces");
                    rootNode.add(category);
                }
                instance = new DefaultMutableTreeNode("L(" +
                    String.valueOf(lens.getP()) + "," +
                    String.valueOf(lens.getQ()) + ")");
                category.add(instance);
                instance.add(new DefaultMutableTreeNode("Component " +
                    String.valueOf(i)));
                torus = lens.getTorus();
                instance.add(new DefaultMutableTreeNode("Layered " +
                    String.valueOf(torus.getMeridinalCuts(0)) + "-" +
                    String.valueOf(torus.getMeridinalCuts(1)) + "-" +
                    String.valueOf(torus.getMeridinalCuts(2)) +
                    " solid torus " + (lens.isSnapped() ? "snapped shut" :
                    "twisted shut")));
                lens.destroy();
            }
        }

        // Look for layered loops.
        category = null;
        NLayeredLoop loop;
        boolean twisted;
        n = triangulation.getNumberOfComponents();
        for (i = 0; i < n; i++) {
            loop = engine.isLayeredLoop(triangulation.getComponent(i));
            if (loop != null) {
                if (category == null) {
                    category = new DefaultMutableTreeNode("Layered Loops");
                    rootNode.add(category);
                }
                twisted = loop.isTwisted();
                instance = new DefaultMutableTreeNode((twisted ?
                    "Twisted" : "Not twisted") + ", index " +
                    String.valueOf(loop.getIndex()));
                category.add(instance);
                instance.add(new DefaultMutableTreeNode("Component " +
                    String.valueOf(i)));
                if (twisted)
                    instance.add(new DefaultMutableTreeNode("Hinge: edge " +
                        String.valueOf(triangulation.getEdgeIndex(
                        loop.getHinge(0)))));
                else
                    instance.add(new DefaultMutableTreeNode("Hinges: edges " +
                        String.valueOf(triangulation.getEdgeIndex(
                        loop.getHinge(0))) + ", " +
                        String.valueOf(triangulation.getEdgeIndex(
                        loop.getHinge(1)))));
                loop.destroy();
            }
        }

        // Look for augmented triangular solid tori.
        category = null;
        NAugTriSolidTorus aug;
        NTriSolidTorus tri;
        NSFS sfs;
        NLensSpace lensSpace;
        n = triangulation.getNumberOfComponents();
        for (i = 0; i < n; i++) {
            aug = engine.isAugTriSolidTorus(triangulation.getComponent(i));
            if (aug != null) {
                if (category == null) {
                    category = new DefaultMutableTreeNode(
                        "Augmented Triangular Solid Tori");
                    rootNode.add(category);
                }
                sfs = aug.getSeifertStructure();
                instance = new DefaultMutableTreeNode(sfs.toString());
                category.add(instance);
                lensSpace = sfs.isLensSpace();
                if (lensSpace != null) {
                    instance.add(new DefaultMutableTreeNode("Reduces to " +
                        lensSpace.toString()));
                    lensSpace.destroy();
                }
                instance.add(new DefaultMutableTreeNode("Component " +
                    String.valueOf(i)));
                tri = aug.getCore();
                instance.add(new DefaultMutableTreeNode("Core: tets " +
                    String.valueOf(triangulation.getTetrahedronIndex(
                    tri.getTetrahedron(0))) + ", " +
                    String.valueOf(triangulation.getTetrahedronIndex(
                    tri.getTetrahedron(1))) + ", " +
                    String.valueOf(triangulation.getTetrahedronIndex(
                    tri.getTetrahedron(2)))));
                if (aug.hasLayeredChain())
                    instance.add(new DefaultMutableTreeNode(
                        "Attached: layered chain + layered solid torus"));
                else
                    instance.add(new DefaultMutableTreeNode(
                        "Attached: 3 layered solid tori"));
                aug.destroy();
            }
        }

        // Look for layered solid tori.
        category = null;
        n = triangulation.getNumberOfTetrahedra();
        for (i = 0; i < n; i++) {
            torus = engine.isLayeredSolidTorusBase(
                triangulation.getTetrahedron(i));
            if (torus != null) {
                if (category == null) {
                    category = new DefaultMutableTreeNode("Layered Solid Tori");
                    rootNode.add(category);
                }
                instance = new DefaultMutableTreeNode(
                    String.valueOf(torus.getMeridinalCuts(0)) + "-" +
                    String.valueOf(torus.getMeridinalCuts(1)) + "-" +
                    String.valueOf(torus.getMeridinalCuts(2)));
                category.add(instance);
                instance.add(new DefaultMutableTreeNode("Base: tet " +
                    String.valueOf(triangulation.getTetrahedronIndex(
                    torus.getBase()))));
                long topIndex = triangulation.getTetrahedronIndex(
                    torus.getTopLevel());
                instance.add(new DefaultMutableTreeNode("Top level: tet " +
                    String.valueOf(topIndex)));
                String edgeCopy = edgeString(topIndex, torus.getTopEdge(0, 1));
                instance.add(new DefaultMutableTreeNode("Weight " +
                    String.valueOf(torus.getMeridinalCuts(0)) + " edge: " +
                    edgeString(topIndex, torus.getTopEdge(0, 0)) +
                    (edgeCopy == null ? "" : " = " + edgeCopy)));
                edgeCopy = edgeString(topIndex, torus.getTopEdge(1, 1));
                instance.add(new DefaultMutableTreeNode("Weight " +
                    String.valueOf(torus.getMeridinalCuts(1)) + " edge: " +
                    edgeString(topIndex, torus.getTopEdge(1, 0)) +
                    (edgeCopy == null ? "" : " = " + edgeCopy)));
                edgeCopy = edgeString(topIndex, torus.getTopEdge(2, 1));
                instance.add(new DefaultMutableTreeNode("Weight " +
                    String.valueOf(torus.getMeridinalCuts(2)) + " edge: " +
                    edgeString(topIndex, torus.getTopEdge(2, 0)) +
                    (edgeCopy == null ? "" : " = " + edgeCopy)));
                torus.destroy();
            }
        }

        // Look for triangular solid tori.
        // Make sure we find each triangular solid torus only once.
        category = null;
        n = triangulation.getNumberOfTetrahedra();
        long tetIndex[] = new long[3];
        NPerm p;
        NPerm roles[] = new NPerm[3];
        for (i = 0; i < (n - 2); i++)
            for (j = 0; j < 24; j++) {
                p = NPerm.allPermsS4[(int)j];
                if (p.imageOf(0) > p.imageOf(3))
                    continue;
                tri = engine.isTriSolidTorus(
                    triangulation.getTetrahedron(i), p);
                if (tri != null) {
                    for (k = 0; k < 3; k++) {
                        tetIndex[(int)k] = triangulation.getTetrahedronIndex(
                            tri.getTetrahedron((int)k));
                        roles[(int)k] = tri.getVertexRoles((int)k);
                    }
                    if (tetIndex[1] < tetIndex[0] ||
                            tetIndex[2] < tetIndex[0]) {
                        tri.destroy();
                        continue;
                    }

                    // We have the triangular solid torus in a unique
                    // canonical form.
                    if (category == null) {
                        category = new DefaultMutableTreeNode(
                            "Triangular Solid Tori");
                        rootNode.add(category);
                    }
                    instance = new DefaultMutableTreeNode("Tets " +
                        String.valueOf(tetIndex[0]) + ", " +
                        String.valueOf(tetIndex[1]) + ", " +
                        String.valueOf(tetIndex[2]));
                    category.add(instance);

                    instance.add(new DefaultMutableTreeNode("Axis edges: " +
                        edgeString(tetIndex[0], roles[0], 0, 3) + ", " +
                        edgeString(tetIndex[1], roles[1], 0, 3) + ", " +
                        edgeString(tetIndex[2], roles[2], 0, 3)));
                    instance.add(new DefaultMutableTreeNode("Major edge: " +
                        edgeString(tetIndex[0], roles[0], 1, 2) + " = " +
                        edgeString(tetIndex[1], roles[1], 0, 1) + " = " +
                        edgeString(tetIndex[2], roles[2], 2, 3)));
                    instance.add(new DefaultMutableTreeNode("Major edge: " +
                        edgeString(tetIndex[1], roles[1], 1, 2) + " = " +
                        edgeString(tetIndex[2], roles[2], 0, 1) + " = " +
                        edgeString(tetIndex[0], roles[0], 2, 3)));
                    instance.add(new DefaultMutableTreeNode("Major edge: " +
                        edgeString(tetIndex[2], roles[2], 1, 2) + " = " +
                        edgeString(tetIndex[0], roles[0], 0, 1) + " = " +
                        edgeString(tetIndex[1], roles[1], 2, 3)));
                    instance.add(new DefaultMutableTreeNode("Minor edge: " +
                        edgeString(tetIndex[1], roles[1], 1, 3) + " = " +
                        edgeString(tetIndex[2], roles[2], 0, 2)));
                    instance.add(new DefaultMutableTreeNode("Minor edge: " +
                        edgeString(tetIndex[2], roles[2], 1, 3) + " = " +
                        edgeString(tetIndex[0], roles[0], 0, 2)));
                    instance.add(new DefaultMutableTreeNode("Minor edge: " +
                        edgeString(tetIndex[0], roles[0], 1, 3) + " = " +
                        edgeString(tetIndex[1], roles[1], 0, 2)));

                    tri.destroy();
                }
            }

        // Look for snapped 3-balls.
        category = null;
        NSnappedBall ball;
        Vector balls = new Vector();
        n = triangulation.getNumberOfTetrahedra();
        for (i = 0; i < n; i++) {
            ball = engine.isSnappedBall(triangulation.getTetrahedron(i));
            if (ball != null) {
                balls.addElement(ball);
                if (category == null) {
                    category = new DefaultMutableTreeNode("Snapped 3-Balls");
                    rootNode.add(category);
                }
                instance = new DefaultMutableTreeNode(
                    "Tetrahedron " +
                    String.valueOf(triangulation.getTetrahedronIndex(
                    ball.getTetrahedron())));
                category.add(instance);
                instance.add(new DefaultMutableTreeNode("Equator: edge " +
                    String.valueOf(ball.getInternalFace(0)) +
                    String.valueOf(ball.getInternalFace(1))));
            }
        }

        // Look for snapped 2-spheres.
        category = null;
        NSnappedTwoSphere sphere;
        NSnappedBall ball2;
        n = balls.size();
        for (i = 0; i < n; i++) {
            ball = (NSnappedBall)balls.elementAt((int)i);
            for (j = i + 1; j < n; j++) {
                ball2 = (NSnappedBall)balls.elementAt((int)j);
                sphere = engine.formsSnappedTwoSphere(ball, ball2);
                if (sphere != null) {
                    if (category == null) {
                        category = new DefaultMutableTreeNode(
                            "Snapped 2-Spheres");
                        rootNode.add(category);
                    }
                    instance = new DefaultMutableTreeNode(
                        "Tetrahedra " +
                        String.valueOf(triangulation.getTetrahedronIndex(
                        ball.getTetrahedron())) + ", " +
                        String.valueOf(triangulation.getTetrahedronIndex(
                        ball2.getTetrahedron())));
                    category.add(instance);
                    instance.add(new DefaultMutableTreeNode("Equator: edge # " +
                        String.valueOf(triangulation.getEdgeIndex(
                        ball.getTetrahedron().getEdge(
                        ball.getEquatorEdge())))));
                    sphere.destroy();
                }
            }
        }

        // Look for pillow 2-spheres.
        category = null;
        NPillowTwoSphere pillowSphere;
        NFace f1, f2;
        n = triangulation.getNumberOfFaces();
        for (i = 0; i < n; i++) {
            f1 = triangulation.getFace(i);
            for (j = i + 1; j < n; j++) {
                f2 = triangulation.getFace(j);
                pillowSphere = engine.formsPillowTwoSphere(f1, f2);
                if (pillowSphere != null) {
                    if (category == null) {
                        category = new DefaultMutableTreeNode(
                            "Pillow 2-Spheres");
                        rootNode.add(category);
                    }
                    instance = new DefaultMutableTreeNode(
                        "Face #s " +
                        String.valueOf(triangulation.getFaceIndex(f1) + ", " +
                        String.valueOf(triangulation.getFaceIndex(f2))));
                    category.add(instance);
                    instance.add(new DefaultMutableTreeNode("Equator: edge #s " +
                        String.valueOf(triangulation.getEdgeIndex(
                        f1.getEdge(0))) + ", " +
                        String.valueOf(triangulation.getEdgeIndex(
                        f1.getEdge(1))) + ", " +
                        String.valueOf(triangulation.getEdgeIndex(
                        f1.getEdge(2)))));
                    pillowSphere.destroy();
                }
            }
        }

        // Destroy the cached 3-balls.
        Enumeration e1 = balls.elements();
        while (e1.hasMoreElements())
            ((NSnappedBall)e1.nextElement()).destroy();

        if (rootNode.isLeaf())
            rootNode.add(new DefaultMutableTreeNode(
                "(nothing recognised)"));
        model.nodeStructureChanged(rootNode);
    }

    public void editingElsewhere() {
        Enumeration e = componentsToDisable.elements();
        while (e.hasMoreElements())
            ((Component)e.nextElement()).setEnabled(false);

        rootNode.removeAllChildren();
        model.nodeStructureChanged(rootNode);
    }

    /**
     * Returns a string representation of the given edge.
     * If the given edge number is -1, <tt>null</tt> will be returned.
     *
     * @param tetIndex the index in the triangulation of a tetrahedron
     * containing the edge.
     * @param edgeNumber the number of the edge in the tetrahedron; this
     * should be between -1 and 5 inclusive.
     * @return a string representation of the given edge, or
     * <tt>null</tt> if <i>edgeNumber</i> was -1.
     */
    private static String edgeString(long tetIndex, int edgeNumber) {
        if (edgeNumber < 0)
            return null;
        return String.valueOf(tetIndex) + " (" +
            String.valueOf(NEdge.edgeStart[edgeNumber]) +
            String.valueOf(NEdge.edgeEnd[edgeNumber]) + ')';
    }

    /**
     * Returns a string representation of the given edge.
     *
     * @param tetIndex the index in the triangulation of a tetrahedron
     * containing the edge.
     * @param roles a permutation mapping 0, 1, 2 and 3 to the four
     * vertices of the tetrahedron.
     * @param start the preimage in <i>roles</i> of the starting vertex
     * of the edge.
     * @param start the preimage in <i>roles</i> of the end vertex
     * of the edge.
     * @return a string representation of the given edge.
     */
    private static String edgeString(long tetIndex, NPerm roles,
            int start, int end) {
        return String.valueOf(tetIndex) + " (" +
            String.valueOf(roles.imageOf(start)) +
            String.valueOf(roles.imageOf(end)) + ')';
    }
}
