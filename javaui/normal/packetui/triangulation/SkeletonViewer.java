
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import normal.Shell;
import normal.engine.packet.NPacket;
import normal.engine.triangulation.NTriangulation;
import normal.packetui.*;

/**
 * An interface through which a user can view the skeleton
 * of a triangulation.
 */
public class SkeletonViewer extends DefaultPacketViewer
        implements ActionListener, PacketInfoUI {
    /**
     * The triangulation whose skeleton we are displaying.
     */
    private NTriangulation triangulation;
        
    /**
     * Label displaying the number of vertices.
     */
    private JLabel nVertices;

    /**
     * Label displaying the number of edges.
     */
    private JLabel nEdges;

    /**
     * Label displaying the number of faces.
     */
    private JLabel nFaces;

    /**
     * Label displaying the number of tetrahedra.
     */
    private JLabel nTetrahedra;

    /**
     * Label displaying the number of components.
     */
    private JLabel nComponents;

    /**
     * Label displaying the number of boundary components.
     */
    private JLabel nBoundaryComponents;

    /**
     * Button for viewing vertex details.
     */
    private JButton viewVertices;
     
    /**
     * Button for viewing edge details.
     */
    private JButton viewEdges;
     
    /**
     * Button for viewing face details.
     */
    private JButton viewFaces;

    /**
     * Button for viewing tetrahedron details.
     */
    private JButton viewTetrahedra;

    /**
     * Button for viewing component details.
     */
    private JButton viewComponents;

    /**
     * Button for viewing boundary component details.
     */
    private JButton viewBoundaryComponents;

    /**
     * Label for vertices.
     */
    private JLabel labelVertices;

    /**
     * Label for edges.
     */
    private JLabel labelEdges;

    /**
     * Label for faces.
     */
    private JLabel labelFaces;

    /**
     * Label for tetrahedra.
     */
    private JLabel labelTetrahedra;

    /**
     * Label for components.
     */
    private JLabel labelComponents;

    /**
     * Label for boundary components.
     */
    private JLabel labelBoundaryComponents;

    /**
     * A list of all frames for viewing skeleton details
     * that are currently open.
     */
    private Vector skeletonFrames = new Vector();

    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * Create a new interface to display the given triangulation skeleton.
     *
     * @param triangulation the triangulation whose skeleton we will display.
     * @param shell the shell representing the entire program.
     */
    public SkeletonViewer(NTriangulation triangulation, Shell shell) {
        this.triangulation = triangulation;
        this.shell = shell;
        init();
    }

    public NPacket getPacket() {
        return triangulation;
    }

    /**
     * Create the interface elements.
     */
    private void init() {
        // Set the layout manager.
        setLayout(new GridBagLayout());
        
        // Initialise the components.
        nVertices = new JLabel();
        nEdges = new JLabel();
        nFaces = new JLabel();
        nTetrahedra = new JLabel();
        nComponents = new JLabel();
        nBoundaryComponents = new JLabel();
        viewVertices = new JButton("View");
        viewEdges = new JButton("View");
        viewFaces = new JButton("View");
        viewTetrahedra = new JButton("View");
        viewTetrahedra.setEnabled(false);
        viewComponents = new JButton("View");
        viewBoundaryComponents = new JButton("View");
        viewVertices.setBorder(BorderFactory.createEtchedBorder());
        viewEdges.setBorder(BorderFactory.createEtchedBorder());
        viewFaces.setBorder(BorderFactory.createEtchedBorder());
        viewTetrahedra.setBorder(BorderFactory.createEtchedBorder());
        viewComponents.setBorder(BorderFactory.createEtchedBorder());
        viewBoundaryComponents.setBorder(BorderFactory.createEtchedBorder());
        labelVertices = new JLabel("Vertices:");
        labelEdges = new JLabel("Edges:");
        labelFaces = new JLabel("Faces:");
        labelTetrahedra = new JLabel("Tetrahedra:");
        labelComponents = new JLabel("Components:");
        labelBoundaryComponents = new JLabel("Boundary Components:");
        GridBagConstraints cLabel = new GridBagConstraints();
        GridBagConstraints cTotal = new GridBagConstraints();
        GridBagConstraints cView = new GridBagConstraints();
        cLabel.anchor = cLabel.WEST;
        cLabel.fill = cLabel.NONE;
        cLabel.gridx = 0;
        cTotal.anchor = cTotal.CENTER;
        cTotal.fill = cTotal.NONE;
        cTotal.gridx = 1;
        cTotal.insets = new Insets(0, 5, 0, 5);
        cView.anchor = cView.CENTER;
        cView.fill = cView.NONE;
        cView.gridx = 2;
        cView.ipadx = 5;
        cView.ipady = 2;
        cView.insets = new Insets(0, 5, 0, 0);
        add(labelVertices, cLabel);
        add(nVertices, cTotal);
        add(viewVertices, cView);
        add(labelEdges, cLabel);
        add(nEdges, cTotal);
        add(viewEdges, cView);
        add(labelFaces, cLabel);
        add(nFaces, cTotal);
        add(viewFaces, cView);
        add(labelTetrahedra, cLabel);
        add(nTetrahedra, cTotal);
        add(viewTetrahedra, cView);
        add(labelComponents, cLabel);
        add(nComponents, cTotal);
        add(viewComponents, cView);
        add(labelBoundaryComponents, cLabel);
        add(nBoundaryComponents, cTotal);
        add(viewBoundaryComponents, cView);

        // Other miscellaneous tasks.
        setBorder(new EmptyBorder(5,5,5,5));
        
        // Set event listeners.
        viewVertices.addActionListener(this);
        viewEdges.addActionListener(this);
        viewFaces.addActionListener(this);
        viewComponents.addActionListener(this);
        viewBoundaryComponents.addActionListener(this);
    }

    /**
     * Called when a button has been pressed.
     *
     * @param e the corresponding event.
     */
    public void actionPerformed(ActionEvent e) {
        int style;

        Object src = e.getSource();
        if (src == viewVertices)
            style = SkeletonTableFrame.VERTICES;
        else if (src == viewEdges)
            style = SkeletonTableFrame.EDGES;
        else if (src == viewFaces)
            style = SkeletonTableFrame.FACES;
        else if (src == viewComponents)
            style = SkeletonTableFrame.COMPONENTS;
        else if (src == viewBoundaryComponents)
            style = SkeletonTableFrame.BOUNDARY_COMPONENTS;
        else
            return;

        showSkeletonDetails(style);
    }

    public void reflectPacket() {
        closeAllFrames();

        nVertices.setText(String.valueOf(triangulation.getNumberOfVertices()));
        nEdges.setText(String.valueOf(triangulation.getNumberOfEdges()));
        nFaces.setText(String.valueOf(triangulation.getNumberOfFaces()));
        nTetrahedra.setText(
            String.valueOf(triangulation.getNumberOfTetrahedra()));
        nComponents.setText(
            String.valueOf(triangulation.getNumberOfComponents()));
        nBoundaryComponents.setText(
            String.valueOf(triangulation.getNumberOfBoundaryComponents()));

        viewVertices.setEnabled(true);
        viewEdges.setEnabled(true);
        viewFaces.setEnabled(true);
        viewComponents.setEnabled(true);
        viewBoundaryComponents.setEnabled(true);
        labelVertices.setEnabled(true);
        labelEdges.setEnabled(true);
        labelFaces.setEnabled(true);
        labelTetrahedra.setEnabled(true);
        labelComponents.setEnabled(true);
        labelBoundaryComponents.setEnabled(true);
    }

    public void editingElsewhere() {
        closeAllFrames();

        nVertices.setText("");
        nEdges.setText("");
        nFaces.setText("");
        nTetrahedra.setText("");
        nComponents.setText("");
        nBoundaryComponents.setText("");

        viewVertices.setEnabled(false);
        viewEdges.setEnabled(false);
        viewFaces.setEnabled(false);
        viewComponents.setEnabled(false);
        viewBoundaryComponents.setEnabled(false);
        labelVertices.setEnabled(false);
        labelEdges.setEnabled(false);
        labelFaces.setEnabled(false);
        labelTetrahedra.setEnabled(false);
        labelComponents.setEnabled(false);
        labelBoundaryComponents.setEnabled(false);
    }

    /**
     * Close all frames opened for viewing specific skeleton details.
     */
    public void closeAllFrames() {
        while(skeletonFrames.size() > 0) {
            Object e = skeletonFrames.elementAt(0);
            skeletonFrames.removeElement(e);
            ((SkeletonTableFrame)e).dispose();
        }
    }

    /**
     * Bring up a frame showing particular details of the skeleton.
     *
     * @param style specifies whether to view details for vertices,
     * edges, faces or so on.  This must be one of the style constants
     * defined in class <tt>SkeletonTableFrame</tt>.
     *
     * @see normal.packetui.triangulation.SkeletonTableFrame
     */
    private void showSkeletonDetails(int style) {
        SkeletonTableFrame f = new SkeletonTableFrame(shell.getPrimaryFrame(),
            triangulation, style);
        skeletonFrames.addElement(f);
        f.addWindowListener(new WindowAdapter() {
            public void windowClosed(WindowEvent e) {
                skeletonFrames.removeElement(e.getSource());
            }
        });
        f.show();
    }
}
