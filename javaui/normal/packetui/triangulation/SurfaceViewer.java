
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
import normal.engine.packet.NPacket;
import normal.engine.triangulation.*;
import normal.packetui.*;
import org.gjt.btools.gui.component.PaddedPane;

/**
 * An interface through which a user can view the normal surface
 * properties of a triangulation.
 */
public class SurfaceViewer extends DefaultPacketViewer
        implements PacketInfoUI {
    /**
     * The colour to use for green text.
     */
    public static final Color green = Color.green.darker().darker().darker();

    /**
     * The colour to use for red text.
     */
    public static final Color red = Color.red.darker().darker();

    /**
     * We will only calculate normal surface properties automatically
     * for triangulations with at most this many tetrahedra.
     */
    public static long maxAutoCalcTetrahedra = 6;

    /**
     * The triangulation under examination.
     */
    private NTriangulation triangulation;

    /**
     * Displays 0-efficiency.
     */
    private JLabel zeroEfficient;
    /**
     * Displays existance of a splitting surface.
     */
    private JLabel splittingSurface;
    /**
     * Button to calculate 0-efficiency.
     */
    private JButton btnZeroEfficient;
    /**
     * Button to calculate existance of a splitting surface.
     */
    private JButton btnSplittingSurface;

    /**
     * Components to disable when the packet is being edited.
     */
    private Vector componentsToDisable = new Vector();

    /**
     * Create a new interface to display the given surface properties.
     *
     * @param triangulation the triangulation whose properties we will
     * display.
     */
    public SurfaceViewer(NTriangulation triangulation) {
        super();
        this.triangulation = triangulation;
        init();
    }

    /**
     * Create the interface elements.
     */
    private void init() {
        // Set up the property pane.
        JPanel propPane = new JPanel();
        propPane.setLayout(new GridBagLayout());

        zeroEfficient = new JLabel();
        splittingSurface = new JLabel();
        componentsToDisable.addElement(zeroEfficient);
        componentsToDisable.addElement(splittingSurface);

        btnZeroEfficient = new JButton("Calculate...");
        btnSplittingSurface = new JButton("Calculate...");

        JLabel labelSurfaces = new JLabel("Normal Surface Properties");
        JLabel labelZero = new JLabel("Zero-efficient?");
        JLabel labelSplitting = new JLabel("Splitting surface?");
        componentsToDisable.addElement(labelSurfaces);
        componentsToDisable.addElement(labelZero);
        componentsToDisable.addElement(labelSplitting);

        GridBagConstraints cTitle = new GridBagConstraints();
        GridBagConstraints cLabel = new GridBagConstraints();
        GridBagConstraints cTotal = new GridBagConstraints();
        GridBagConstraints cButton = new GridBagConstraints();
        cTitle.anchor = cTitle.CENTER;
        cTitle.fill = cTitle.NONE;
        cTitle.gridx = 0;
        cTitle.gridwidth = cTitle.REMAINDER;
        cTitle.insets = new Insets(0, 0, 10, 0);
        cLabel.anchor = cLabel.WEST;
        cLabel.fill = cLabel.NONE;
        cLabel.gridx = 0;
        cTotal.anchor = cTotal.CENTER;
        cTotal.fill = cTotal.NONE;
        cTotal.gridx = 1;
        cTotal.insets = new Insets(0, 10, 0, 10);
        cButton.anchor = cButton.CENTER;
        cButton.fill = cButton.NONE;
        cButton.gridx = 2;
        cButton.insets = new Insets(2, 0, 2, 0);

        propPane.add(labelSurfaces, cTitle);
        propPane.add(labelZero, cLabel);
        propPane.add(zeroEfficient, cTotal);
        propPane.add(btnZeroEfficient, cButton);
        propPane.add(labelSplitting, cLabel);
        propPane.add(splittingSurface, cTotal);
        propPane.add(btnSplittingSurface, cButton);

        // Put everything together.
        setLayout(new BorderLayout());
        add(propPane, BorderLayout.CENTER);

        // Add action listeners.
        btnZeroEfficient.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                triangulation.isZeroEfficient();
                reflectPacket();
            }
        });
        btnSplittingSurface.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                triangulation.hasSplittingSurface();
                reflectPacket();
            }
        });
    }

    public NPacket getPacket() {
        return triangulation;
    }

    public void reflectPacket() {
        Enumeration e = componentsToDisable.elements();
        while (e.hasMoreElements())
            ((Component)e.nextElement()).setEnabled(true);

        long nTet = triangulation.getNumberOfTetrahedra();

        if ((! triangulation.knowsZeroEfficient()) &&
                nTet > maxAutoCalcTetrahedra) {
            zeroEfficient.setText("Unknown");
            zeroEfficient.setForeground(Color.black);
            btnZeroEfficient.setEnabled(true);
        } else if (triangulation.isZeroEfficient()) {
            zeroEfficient.setText("True");
            zeroEfficient.setForeground(green);
            btnZeroEfficient.setEnabled(false);
        } else {
            zeroEfficient.setText("False");
            zeroEfficient.setForeground(red);
            btnZeroEfficient.setEnabled(false);
        }

        if ((! triangulation.knowsSplittingSurface()) &&
                nTet > maxAutoCalcTetrahedra) {
            splittingSurface.setText("Unknown");
            splittingSurface.setForeground(Color.black);
            btnSplittingSurface.setEnabled(true);
        } else if (triangulation.hasSplittingSurface()) {
            splittingSurface.setText("True");
            splittingSurface.setForeground(green);
            btnSplittingSurface.setEnabled(false);
        } else {
            splittingSurface.setText("False");
            splittingSurface.setForeground(red);
            btnSplittingSurface.setEnabled(false);
        }
    }

    public void editingElsewhere() {
        Enumeration e = componentsToDisable.elements();
        while (e.hasMoreElements())
            ((Component)e.nextElement()).setEnabled(false);

        zeroEfficient.setText("");
        splittingSurface.setText("");

        btnZeroEfficient.setEnabled(false);
        btnSplittingSurface.setEnabled(false);
    }
}
