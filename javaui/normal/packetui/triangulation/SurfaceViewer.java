
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
     * The triangulation under examination.
     */
    private NTriangulation triangulation;

    /**
     * Displays 0-efficiency.
     */
    private JLabel zeroEfficient;
    /**
     * Displays existance of a central surface.
     */
    private JLabel centralSurface;

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
        centralSurface = new JLabel();
        componentsToDisable.addElement(zeroEfficient);
        componentsToDisable.addElement(centralSurface);

        JLabel labelSurfaces = new JLabel("Normal Surface Properties");
        JLabel labelZero = new JLabel("Zero-efficient?");
        JLabel labelCentral = new JLabel("Central surface?");
        componentsToDisable.addElement(labelSurfaces);
        componentsToDisable.addElement(labelZero);
        componentsToDisable.addElement(labelCentral);

        GridBagConstraints cTitle = new GridBagConstraints();
        GridBagConstraints cLabel = new GridBagConstraints();
        GridBagConstraints cTotal = new GridBagConstraints();
        cTitle.anchor = cTitle.CENTER;
        cTitle.fill = cTitle.NONE;
        cTitle.gridx = 0;
        cTitle.gridwidth = cTitle.REMAINDER;
        cTitle.insets = new Insets(0, 0, 5, 0);
        cLabel.anchor = cLabel.WEST;
        cLabel.fill = cLabel.NONE;
        cLabel.gridx = 0;
        cTotal.anchor = cTotal.CENTER;
        cTotal.fill = cTotal.NONE;
        cTotal.gridx = 1;
        cTotal.insets = new Insets(0, 10, 0, 5);

        propPane.add(labelSurfaces, cTitle);
        propPane.add(labelZero, cLabel);
        propPane.add(zeroEfficient, cTotal);
        propPane.add(labelCentral, cLabel);
        propPane.add(centralSurface, cTotal);

        // Put everything together.
        setLayout(new BorderLayout());
        add(propPane, BorderLayout.CENTER);
    }

    public NPacket getPacket() {
        return triangulation;
    }

    public void reflectPacket() {
        Enumeration e = componentsToDisable.elements();
        while (e.hasMoreElements())
            ((Component)e.nextElement()).setEnabled(true);

        if (triangulation.isZeroEfficient()) {
            zeroEfficient.setText("True");
            zeroEfficient.setForeground(green);
        } else {
            zeroEfficient.setText("False");
            zeroEfficient.setForeground(red);
        }

        if (triangulation.hasCentralSurface()) {
            centralSurface.setText("True");
            centralSurface.setForeground(green);
        } else {
            centralSurface.setText("False");
            centralSurface.setForeground(red);
        }
    }

    public void editingElsewhere() {
        Enumeration e = componentsToDisable.elements();
        while (e.hasMoreElements())
            ((Component)e.nextElement()).setEnabled(false);

        zeroEfficient.setText("");
        centralSurface.setText("");
    }
}
