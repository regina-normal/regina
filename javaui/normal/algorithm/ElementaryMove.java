
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.engine.triangulation.*;
import normal.packetui.triangulation.SkeletonTableFrame;
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
        super(shell, "Elementary Move...", KeyEvent.VK_E,
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
        if (dlg.selected == dlg.fourFour) {
            if (dlg.skelIndex >= 0)
                return tri.fourFourMove(tri.getEdge(dlg.skelIndex), 0);
            else
                return tri.fourFourMove(tri.getEdge(-(dlg.skelIndex + 1)), 1);
        }
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
        if (dlg.selected == dlg.collapseEdge)
            return tri.collapseEdge(tri.getEdge(dlg.skelIndex));

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
        private JButton ok;

        /**
         * Cancel button.
         */
        private JButton cancel;

        /**
         * View vertex details.
         */
        private JButton vertices;

        /**
         * View edge details.
         */
        private JButton edges;

        /**
         * View face details.
         */
        private JButton faces;

        /**
         * Button representing a 3-2 move.
         */
        private JRadioButton threeTwo;

        /**
         * Button representing a 2-3 move.
         */
        private JRadioButton twoThree;

        /**
         * Button representing a 4-4 move.
         */
        private JRadioButton fourFour;

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
         * Button representing a edge collapse move.
         */
        private JRadioButton collapseEdge;

        /**
         * Skeletal component about which to do a 3-2 move.
         */
        private JComboBox threeTwoComp;

        /**
         * Skeletal component about which to do a 2-3 move.
         */
        private JComboBox twoThreeComp;

        /**
         * Skeletal component about which to do a 4-4 move.
         */
        private JComboBox fourFourComp;

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
         * Skeletal component about which to do a edge collapse move.
         */
        private JComboBox collapseEdgeComp;

        /**
         * The triangulation which which we are working.
         */
        private NTriangulation tri;

        /**
         * A list of all frames for viewing skeleton details that are
         * currently open.
         */
        private Vector skeletonFrames = new Vector();

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
            fourFour = new JRadioButton("4-4");
            twoZeroEdge = new JRadioButton("2-0 (edge)");
            twoZeroVertex = new JRadioButton("2-0 (vertex)");
            twoOne = new JRadioButton("2-1");
            openBook = new JRadioButton("Open book");
            shellBoundary = new JRadioButton("Shell boundary");
            collapseEdge = new JRadioButton("Collapse Edge");
            type.add(threeTwo);
            type.add(twoThree);
            type.add(fourFour);
            type.add(twoZeroEdge);
            type.add(twoZeroVertex);
            type.add(twoOne);
            type.add(openBook);
            type.add(shellBoundary);
            // type.add(collapseEdge);

            threeTwoComp = new JComboBox();
            twoThreeComp = new JComboBox();
            fourFourComp = new JComboBox();
            twoZeroEdgeComp = new JComboBox();
            twoZeroVertexComp = new JComboBox();
            twoOneComp = new JComboBox();
            openBookComp = new JComboBox();
            shellBoundaryComp = new JComboBox();
            collapseEdgeComp = new JComboBox();

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
            typePanel.add(fourFour, cButton);
            typePanel.add(fourFourComp, cExtra);
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
            // typePanel.add(collapseEdge, cButton);
            // typePanel.add(collapseEdgeComp, cExtra);

            // Make the button panel.
            ok = new JButton("OK");
            cancel = new JButton("Cancel");

            JPanel buttonPanel = new JPanel();
            buttonPanel.setLayout(new FlowLayout());
            buttonPanel.add(ok);
            buttonPanel.add(cancel);

            // Make the skeleton viewer panel.
            vertices = new JButton("Vertices...");
            edges = new JButton("Edges...");
            faces = new JButton("Faces...");

            JPanel skelPanel = new JPanel();
            skelPanel.setLayout(new FlowLayout());
            skelPanel.add(vertices);
            skelPanel.add(edges);
            skelPanel.add(faces);
            skelPanel.setBorder(BorderFactory.createRaisedBevelBorder());

            // Insert everything into the main pane.
            getContentPane().setLayout(new BorderLayout());
            getContentPane().add(new PaddedPane(typePanel, 2, 2, 2, 2),
                BorderLayout.CENTER);
            JPanel allButtons = new JPanel();
            allButtons.setLayout(new GridLayout(2,1));
            allButtons.add(buttonPanel);
            allButtons.add(skelPanel);
            getContentPane().add(allButtons, BorderLayout.SOUTH);

            // Add event listeners.
            ok.addActionListener(this);
            cancel.addActionListener(this);
            vertices.addActionListener(this);
            edges.addActionListener(this);
            faces.addActionListener(this);

            addWindowListener(new WindowAdapter() {
                public void windowClosed(WindowEvent evt) {
                    closeAllFrames();
                }
            });

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
            fourFourComp.removeAllItems();
            twoZeroEdgeComp.removeAllItems();
            twoZeroVertexComp.removeAllItems();
            twoOneComp.removeAllItems();
            openBookComp.removeAllItems();
            shellBoundaryComp.removeAllItems();
            collapseEdgeComp.removeAllItems();

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
                if (tri.fourFourMove(e, 0, true, false))
                    fourFourComp.addItem(new ExtendedObject(
                        new Long(i), "Edge " + String.valueOf(i) +
                        " (axis 0)"));
                if (tri.fourFourMove(e, 1, true, false))
                    fourFourComp.addItem(new ExtendedObject(
                        new Long(-(i + 1)), "Edge " + String.valueOf(i) +
                        " (axis 1)"));
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
                if (tri.collapseEdge(e, true, false))
                    collapseEdgeComp.addItem(new ExtendedObject(
                        new Long(i), "Edge " + String.valueOf(i)));
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
            if (fourFourComp.getItemCount() == 0) {
                fourFourComp.setEnabled(false);
                fourFour.setEnabled(false);
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
            if (collapseEdgeComp.getItemCount() == 0) {
                collapseEdgeComp.setEnabled(false);
                collapseEdge.setEnabled(false);
            }
        }

        /**
         * Close all frames opened for viewing specific skeleton details.
         */
        public void closeAllFrames() {
            while (skeletonFrames.size() > 0) {
                Object e = skeletonFrames.elementAt(0);
                skeletonFrames.removeElement(e);
                ((SkeletonTableFrame)e).dispose();
            }
        }

        /**
         * For internal use only.
         * Called when a dialog button has been pressed.
         */
        public void actionPerformed(ActionEvent e) {
            Object src = e.getSource();
            if (src == ok) {
                if (threeTwo.isSelected()) {
                    selected = threeTwo;
                    skelIndex = ((Long)((ExtendedObject)threeTwoComp.
                        getSelectedItem()).getValue()).longValue();
                } else if (twoThree.isSelected()) {
                    selected = twoThree;
                    skelIndex = ((Long)((ExtendedObject)twoThreeComp.
                        getSelectedItem()).getValue()).longValue();
                } else if (fourFour.isSelected()) {
                    selected = fourFour;
                    skelIndex = ((Long)((ExtendedObject)fourFourComp.
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
                } else if (collapseEdge.isSelected()) {
                    selected = collapseEdge;
                    skelIndex = ((Long)((ExtendedObject)collapseEdgeComp.
                        getSelectedItem()).getValue()).longValue();
                } else {
                    shell.error("No elementary move has been selected.");
                    return;
                }
                dispose();
            } else if (src == cancel) {
                dispose();
            } else {
                int style;
                if (src == vertices)
                    style = SkeletonTableFrame.VERTICES;
                else if (src == edges)
                    style = SkeletonTableFrame.EDGES;
                else if (src == faces)
                    style = SkeletonTableFrame.FACES;
                else
                    return;

                SkeletonTableFrame f = new SkeletonTableFrame(this,
                    tri, style);
                skeletonFrames.addElement(f);
                f.addWindowListener(new WindowAdapter() {
                    public void windowClosed(WindowEvent evt) {
                        skeletonFrames.removeElement(evt.getSource());
                    }
                });
                f.show();
            }
        }
    }
}

