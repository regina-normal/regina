
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

package normal.algorithm;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.engine.triangulation.*;
import org.gjt.btools.gui.*;
import org.gjt.btools.gui.component.*;
import org.gjt.btools.utilities.*;

/**
 * A modification that performs elementary simplification moves on
 * a triangulation.
 */
public class ElementaryMove extends Modification {
    /**
     * Creates a new modification with the given specifications.
     *
     * @param shell the shell representing the entire program.
     */
    public ElementaryMove(Shell shell) {
        super(shell, "Elementary Move", KeyEvent.VK_E,
			"modified", NTriangulation.class);
    }

	public boolean perform(NPacket packet) {
		ElementaryMoveDialog dlg =
			new ElementaryMoveDialog((NTriangulation)packet);
		Positioner.centerOnScreen(dlg);
		dlg.show();

		// Perform the move.
		NTriangulation tri = (NTriangulation)packet;

		if (dlg.selected == dlg.threeTwo)
			return tri.threeTwoMove(tri.getEdge(dlg.skelIndex));
		if (dlg.selected == dlg.twoThree)
			return tri.twoThreeMove(tri.getFace(dlg.skelIndex));
		if (dlg.selected == dlg.twoZeroEdge)
			return tri.twoZeroMove(tri.getEdge(dlg.skelIndex));
		if (dlg.selected == dlg.twoZeroVertex)
			return tri.twoZeroMove(tri.getVertex(dlg.skelIndex));
		if (dlg.selected == dlg.twoOne) {
			if (dlg.skelIndex >= 0)
				return tri.twoOneMove(tri.getEdge(dlg.skelIndex), 0);
			else
				return tri.twoOneMove(tri.getEdge(-(dlg.skelIndex + 1)), 1);
		}
		if (dlg.selected == dlg.openBook)
			return tri.openBook(tri.getFace(dlg.skelIndex));
		if (dlg.selected == dlg.shellBoundary)
			return tri.shellBoundary(tri.getTetrahedron(dlg.skelIndex));

		// Nothing was done.
		return false;
	}

	/**
	 * A dialog allowing the user to select an elementary move to make.
	 */
	private class ElementaryMoveDialog extends JDialog
			implements ActionListener {
	    /**
	     * The radio button that the user selected.
	     */
	    private JRadioButton selected;

	    /**
	     * The index of the skeletal object about which to perform the move.
	     */
	    private long skelIndex;

		/**
		 * OK button.
		 */
		private JButton ok = new JButton();

		/**
		 * Cancel button.
		 */
		private JButton cancel = new JButton();

		/**
		 * Button representing a 3-2 move.
		 */
		private JRadioButton threeTwo;

		/**
		 * Button representing a 2-3 move.
		 */
		private JRadioButton twoThree;

		/**
		 * Button representing a 2-0 move about an edge.
		 */
		private JRadioButton twoZeroEdge;

		/**
		 * Button representing a 2-0 move about a vertex.
		 */
		private JRadioButton twoZeroVertex;

		/**
		 * Button representing a 2-1 move.
		 */
		private JRadioButton twoOne;

		/**
		 * Button representing a book opening move.
		 */
		private JRadioButton openBook;

		/**
		 * Button representing a boundary shelling move.
		 */
		private JRadioButton shellBoundary;

		/**
		 * Skeletal component about which to do a 3-2 move.
		 */
		private JComboBox threeTwoComp;

		/**
		 * Skeletal component about which to do a 2-3 move.
		 */
		private JComboBox twoThreeComp;

		/**
		 * Skeletal component about which to do a 2-0 (edge) move.
		 */
		private JComboBox twoZeroEdgeComp;

		/**
		 * Skeletal component about which to do a 2-0 (vertex) move.
		 */
		private JComboBox twoZeroVertexComp;

		/**
		 * Skeletal component about which to do a 2-1 move.
		 */
		private JComboBox twoOneComp;

		/**
		 * Skeletal component about which to do a book opening move.
		 */
		private JComboBox openBookComp;

		/**
		 * Skeletal component about which to do a boundary shelling move.
		 */
		private JComboBox shellBoundaryComp;

		/**
		 * The triangulation which which we are working.
		 */
		private NTriangulation tri;

		/**
		 * Create a new dialog.
		 */
		public ElementaryMoveDialog(NTriangulation tri) {
			super(shell.getPrimaryFrame(), "Elementary Move", true);
			this.tri = tri;
			this.selected = null;
			init();
			pack();
		}

		/**
		 * Initialise the interface components.
		 */
		private void init() {
			// Make the move panel.
			ButtonGroup type = new ButtonGroup();
			threeTwo = new JRadioButton("3-2");
			twoThree = new JRadioButton("2-3");
			twoZeroEdge = new JRadioButton("2-0 (edge)");
			twoZeroVertex = new JRadioButton("2-0 (vertex)");
			twoOne = new JRadioButton("2-1");
			openBook = new JRadioButton("Open book");
			shellBoundary = new JRadioButton("Shell boundary");
			type.add(threeTwo);
			type.add(twoThree);
			type.add(twoZeroEdge);
			type.add(twoZeroVertex);
			type.add(twoOne);
			type.add(openBook);
			type.add(shellBoundary);

			threeTwoComp = new JComboBox();
			twoThreeComp = new JComboBox();
			twoZeroEdgeComp = new JComboBox();
			twoZeroVertexComp = new JComboBox();
			twoOneComp = new JComboBox();
			openBookComp = new JComboBox();
			shellBoundaryComp = new JComboBox();

			fillMoves();

			JPanel typePanel = new JPanel();
			typePanel.setLayout(new GridBagLayout());
			GridBagConstraints cButton = new GridBagConstraints();
			GridBagConstraints cExtra = new GridBagConstraints();
			cButton.anchor = cButton.WEST;
			cButton.fill = cButton.NONE;
			cButton.gridx = 0;
			cExtra.anchor = cExtra.WEST;
			cExtra.fill = cExtra.HORIZONTAL;
			cExtra.gridx = 1;

			typePanel.add(threeTwo, cButton);
			typePanel.add(threeTwoComp, cExtra);
			typePanel.add(twoThree, cButton);
			typePanel.add(twoThreeComp, cExtra);
			typePanel.add(twoZeroEdge, cButton);
			typePanel.add(twoZeroEdgeComp, cExtra);
			typePanel.add(twoZeroVertex, cButton);
			typePanel.add(twoZeroVertexComp, cExtra);
			typePanel.add(twoOne, cButton);
			typePanel.add(twoOneComp, cExtra);
			typePanel.add(openBook, cButton);
			typePanel.add(openBookComp, cExtra);
			typePanel.add(shellBoundary, cButton);
			typePanel.add(shellBoundaryComp, cExtra);

			//Make the button panel.
			ok.setText("OK");
			cancel.setText("Cancel");

			JPanel buttonPanel = new JPanel();
			buttonPanel.setLayout(new FlowLayout());
			buttonPanel.add(ok);
			buttonPanel.add(cancel);

			// Insert everything into the main pane.
			getContentPane().setLayout(new BorderLayout());
			getContentPane().add(new PaddedPane(typePanel, 2, 2, 2, 2),
				BorderLayout.CENTER);
			getContentPane().add(buttonPanel, BorderLayout.SOUTH);

			// Add action listeners.
			ok.addActionListener(this);
			cancel.addActionListener(this);

			// Tidy up.
			this.getRootPane().setDefaultButton(ok);
			setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		}

		/**
		 * Fills the various combo boxes with the elementary moves that
		 * can be made upon this triangulation.
		 */
		private void fillMoves() {
			threeTwoComp.removeAllItems();
			twoThreeComp.removeAllItems();
			twoZeroEdgeComp.removeAllItems();
			twoZeroVertexComp.removeAllItems();
			twoOneComp.removeAllItems();
			openBookComp.removeAllItems();
			shellBoundaryComp.removeAllItems();

			long nVertices = tri.getNumberOfVertices();
			long nEdges = tri.getNumberOfEdges();
			long nFaces = tri.getNumberOfFaces();
			long nTet = tri.getNumberOfTetrahedra();
			long i;

			for (i = 0; i < nVertices; i++)
				if (tri.twoZeroMove(tri.getVertex(i), true, false))
					twoZeroVertexComp.addItem(new ExtendedObject(
						new Long(i), "Vertex " + String.valueOf(i)));

			NEdge e;
			for (i = 0; i < nEdges; i++) {
				e = tri.getEdge(i);
				if (tri.threeTwoMove(e, true, false))
					threeTwoComp.addItem(new ExtendedObject(
						new Long(i), "Edge " + String.valueOf(i)));
				if (tri.twoZeroMove(e, true, false))
					twoZeroEdgeComp.addItem(new ExtendedObject(
						new Long(i), "Edge " + String.valueOf(i)));
				if (tri.twoOneMove(e, 0, true, false))
					twoOneComp.addItem(new ExtendedObject(
						new Long(i), "Edge " + String.valueOf(i) +
						" (end 0)"));
				if (tri.twoOneMove(e, 1, true, false))
					twoOneComp.addItem(new ExtendedObject(
						new Long(-(i + 1)), "Edge " + String.valueOf(i) +
						" (end 1)"));
			}

			NFace f;
			for (i = 0; i < nFaces; i++) {
				f = tri.getFace(i);
				if (tri.twoThreeMove(f, true, false))
					twoThreeComp.addItem(new ExtendedObject(
						new Long(i), "Face " + String.valueOf(i)));
				if (tri.openBook(f, true, false))
					openBookComp.addItem(new ExtendedObject(
						new Long(i), "Face " + String.valueOf(i)));
			}

			for (i = 0; i < nTet; i++)
				if (tri.shellBoundary(tri.getTetrahedron(i), true, false))
					shellBoundaryComp.addItem(new ExtendedObject(
						new Long(i), "Tet " + String.valueOf(i)));
			
			// Disable any empty combo boxes.
			if (threeTwoComp.getItemCount() == 0) {
				threeTwoComp.setEnabled(false);
				threeTwo.setEnabled(false);
			}
			if (twoThreeComp.getItemCount() == 0) {
				twoThreeComp.setEnabled(false);
				twoThree.setEnabled(false);
			}
			if (twoZeroEdgeComp.getItemCount() == 0) {
				twoZeroEdgeComp.setEnabled(false);
				twoZeroEdge.setEnabled(false);
			}
			if (twoZeroVertexComp.getItemCount() == 0) {
				twoZeroVertexComp.setEnabled(false);
				twoZeroVertex.setEnabled(false);
			}
			if (twoOneComp.getItemCount() == 0) {
				twoOneComp.setEnabled(false);
				twoOne.setEnabled(false);
			}
			if (openBookComp.getItemCount() == 0) {
				openBookComp.setEnabled(false);
				openBook.setEnabled(false);
			}
			if (shellBoundaryComp.getItemCount() == 0) {
				shellBoundaryComp.setEnabled(false);
				shellBoundary.setEnabled(false);
			}
		}

		/**
		 * For internal use only.
		 * Called when a dialog button has been pressed.
		 */
		public void actionPerformed(ActionEvent e) {
			if (e.getSource() == ok) {
				if (threeTwo.isSelected()) {
					selected = threeTwo;
					skelIndex = ((Long)((ExtendedObject)threeTwoComp.
						getSelectedItem()).getValue()).longValue();
				} else if (twoThree.isSelected()) {
					selected = twoThree;
					skelIndex = ((Long)((ExtendedObject)twoThreeComp.
						getSelectedItem()).getValue()).longValue();
				} else if (twoZeroEdge.isSelected()) {
					selected = twoZeroEdge;
					skelIndex = ((Long)((ExtendedObject)twoZeroEdgeComp.
						getSelectedItem()).getValue()).longValue();
				} else if (twoZeroVertex.isSelected()) {
					selected = twoZeroVertex;
					skelIndex = ((Long)((ExtendedObject)twoZeroVertexComp.
						getSelectedItem()).getValue()).longValue();
				} else if (twoOne.isSelected()) {
					selected = twoOne;
					skelIndex = ((Long)((ExtendedObject)twoOneComp.
						getSelectedItem()).getValue()).longValue();
				} else if (openBook.isSelected()) {
					selected = openBook;
					skelIndex = ((Long)((ExtendedObject)openBookComp.
						getSelectedItem()).getValue()).longValue();
				} else if (shellBoundary.isSelected()) {
					selected = shellBoundary;
					skelIndex = ((Long)((ExtendedObject)shellBoundaryComp.
						getSelectedItem()).getValue()).longValue();
				} else {
					shell.error("No elementary move has been selected.");
					return;
				}
			}
			dispose();
		}
	}
}

