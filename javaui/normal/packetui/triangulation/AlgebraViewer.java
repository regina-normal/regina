
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
import javax.swing.*;
import normal.engine.packet.NPacket;
import normal.engine.triangulation.*;
import normal.packetui.*;

/**
 * An interface through which a user can view the algebraic details
 * of a triangulation.
 */
public class AlgebraViewer extends DefaultPacketViewer
        implements PacketInfoUI {
    /**
     * The triangulation under examination.
     * @serial
     */
    private NTriangulation triangulation;

    /**
     * Displays the first homology group.
     * @serial
     */
    private JLabel H1;
    /**
     * Displays the first homology group relative to the boundary.
     * @serial
     */
    private JLabel H1Rel;
    /**
     * Displays the first homology group of the boundary.
     * @serial
     */
    private JLabel H1Bdry;
    /**
     * Displays the second homology group.
     * @serial
     */
    private JLabel H2;
    /**
     * Displays the second homology group with coefficients in Z_2.
     * @serial
     */
    private JLabel H2Z2;

    /**
     * Label for the first homology group.
     * @serial
     */
    private JLabel labelH1;
    /**
     * Label for the first homology group relative to the boundary.
     * @serial
     */
    private JLabel labelH1Rel;
    /**
     * Label for the first homology group of the boundary.
     * @serial
     */
    private JLabel labelH1Bdry;
    /**
     * Label for the second homology group.
     * @serial
     */
    private JLabel labelH2;
    /**
     * Label for the second homology group with coefficients in Z_2.
     * @serial
     */
    private JLabel labelH2Z2;

    /**
     * Create a new interface to display the given algebraic information.
     *
     * @param triangulation the triangulation whose information we will
     * display.
     */
    public AlgebraViewer(NTriangulation triangulation) {
        super();
        this.triangulation = triangulation;
        init();
    }

    /**
     * Create the interface elements.
     */
    private void init() {
        // Set the layout manager.
        setLayout(new GridBagLayout());
        
        // Initialise the components.
        H1 = new JLabel();
        H1Rel = new JLabel();
        H1Bdry = new JLabel();
        H2 = new JLabel();
        H2Z2 = new JLabel();

        labelH1 = new JLabel("H1(M)");
        labelH1Rel = new JLabel("H1(M, Bdry M)");
        labelH1Bdry = new JLabel("H1(Bdry M)");
        labelH2 = new JLabel("H2(M)");
        labelH2Z2 = new JLabel("H2(M ; Z_2)");

        GridBagConstraints cLabel = new GridBagConstraints();
        GridBagConstraints cTotal = new GridBagConstraints();
        cLabel.anchor = cLabel.WEST;
        cLabel.fill = cLabel.NONE;
        cLabel.gridx = 0;
        cTotal.anchor = cTotal.CENTER;
        cTotal.fill = cTotal.NONE;
        cTotal.gridx = 1;
        cTotal.insets = new Insets(0, 5, 0, 5);

        // Insert the components.
        add(labelH1, cLabel);
        add(H1, cTotal);
        add(labelH1Rel, cLabel);
        add(H1Rel, cTotal);
        add(labelH1Bdry, cLabel);
        add(H1Bdry, cTotal);
        add(labelH2, cLabel);
        add(H2, cTotal);
        add(labelH2Z2, cLabel);
        add(H2Z2, cTotal);
    }

    public NPacket getPacket() {
        return triangulation;
    }

    public void reflectPacket() {
        H1.setEnabled(true);
        H1Rel.setEnabled(true);
        H1Bdry.setEnabled(true);
        H2.setEnabled(true);
        H2Z2.setEnabled(true);
        labelH1.setEnabled(true);
        labelH1Rel.setEnabled(true);
        labelH1Bdry.setEnabled(true);
        labelH2.setEnabled(true);
        labelH2Z2.setEnabled(true);

        H1.setText(triangulation.getHomologyH1().toString());

        if (triangulation.isValid()) {
            H1Rel.setText(triangulation.getHomologyH1Rel().toString());
            H1Bdry.setText(triangulation.getHomologyH1Bdry().toString());
            H2.setText(triangulation.getHomologyH2().toString());
            long coeffZ2 = triangulation.getHomologyH2Z2();
            if (coeffZ2 == 0)
                H2Z2.setText("0");
            else if (coeffZ2 == 1)
                H2Z2.setText("Z_2");
            else
                H2Z2.setText(String.valueOf(coeffZ2) + " Z_2");
        } else {
            H1Rel.setText("Invalid Triangulation");
            H1Bdry.setText("Invalid Triangulation");
            H2.setText("Invalid Triangulation");
            H2Z2.setText("Invalid Triangulation");
        }
    }

    public void editingElsewhere() {
        H1.setEnabled(false);
        H1Rel.setEnabled(false);
        H1Bdry.setEnabled(false);
        H2.setEnabled(false);
        H2Z2.setEnabled(false);
        labelH1.setEnabled(false);
        labelH1Rel.setEnabled(false);
        labelH1Bdry.setEnabled(false);
        labelH2.setEnabled(false);
        labelH2Z2.setEnabled(false);

        H1.setText("");
        H1Rel.setText("");
        H1Bdry.setText("");
        H2.setText("");
        H2Z2.setText("");
    }
}
