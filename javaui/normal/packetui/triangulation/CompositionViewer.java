
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Java user interface                                                   *
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
import btools.gui.component.PaddedPane;

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
		long n, i;

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
				instance.add(new DefaultMutableTreeNode("Top level: tet " +
					String.valueOf(triangulation.getTetrahedronIndex(
					torus.getTopLevel()))));
				torus.destroy();
			}
		}

		// Look for snapped 3-balls.
		category = null;
		NSnappedBall ball;
		n = triangulation.getNumberOfTetrahedra();
		for (i = 0; i < n; i++) {
			ball = engine.isSnappedBall(triangulation.getTetrahedron(i));
			if (ball != null) {
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
				ball.destroy();
			}
		}

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
}
