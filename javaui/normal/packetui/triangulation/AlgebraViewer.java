
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
import java.util.*;
import javax.swing.*;
import normal.engine.algebra.*;
import normal.engine.packet.NPacket;
import normal.engine.triangulation.*;
import normal.packetui.*;
import org.gjt.btools.gui.component.PaddedPane;

/**
 * An interface through which a user can view the algebraic details
 * of a triangulation.
 */
public class AlgebraViewer extends DefaultPacketViewer
        implements PacketInfoUI {
    /**
     * The triangulation under examination.
     */
    private NTriangulation triangulation;

    /**
     * Displays the first homology group.
     */
    private JLabel H1;
    /**
     * Displays the first homology group relative to the boundary.
     */
    private JLabel H1Rel;
    /**
     * Displays the first homology group of the boundary.
     */
    private JLabel H1Bdry;
    /**
     * Displays the second homology group.
     */
    private JLabel H2;
    /**
     * Displays the second homology group with coefficients in Z_2.
     */
    private JLabel H2Z2;

    /**
     * The short name of the fundamental group.
     */
    private JLabel fgpName;
    /**
     * The generators of the fundamental group.
     */
    private JLabel fgpGenerators;
    /**
     * The relations in the fundamental group.
     */
    private JList fgpRelations;
    /**
     * The label for the fundamental group relations list.
     */
    private JLabel labelFgpRelations;
    /**
     * The list model for the fundamental group relations.
     */
    private DefaultListModel fgpRelModel;
    
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
    public AlgebraViewer(NTriangulation triangulation) {
        super();
        this.triangulation = triangulation;
        init();
    }

    /**
     * Create the interface elements.
     */
    private void init() {
        // Set up the homology pane.
        JPanel homologyPane = new JPanel();
        homologyPane.setLayout(new GridBagLayout());

        H1 = new JLabel();
        H1Rel = new JLabel();
        H1Bdry = new JLabel();
        H2 = new JLabel();
        H2Z2 = new JLabel();
        componentsToDisable.addElement(H1);
        componentsToDisable.addElement(H1Rel);
        componentsToDisable.addElement(H1Bdry);
        componentsToDisable.addElement(H2);
        componentsToDisable.addElement(H2Z2);

        JLabel labelHom = new JLabel("Homology");
        JLabel labelH1 = new JLabel("H1(M)");
        JLabel labelH1Rel = new JLabel("H1(M, Bdry M)");
        JLabel labelH1Bdry = new JLabel("H1(Bdry M)");
        JLabel labelH2 = new JLabel("H2(M)");
        JLabel labelH2Z2 = new JLabel("H2(M ; Z_2)");
        componentsToDisable.addElement(labelHom);
        componentsToDisable.addElement(labelH1);
        componentsToDisable.addElement(labelH1Rel);
        componentsToDisable.addElement(labelH1Bdry);
        componentsToDisable.addElement(labelH2);
        componentsToDisable.addElement(labelH2Z2);

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

        homologyPane.add(labelHom, cTitle);
        homologyPane.add(labelH1, cLabel);
        homologyPane.add(H1, cTotal);
        homologyPane.add(labelH1Rel, cLabel);
        homologyPane.add(H1Rel, cTotal);
        homologyPane.add(labelH1Bdry, cLabel);
        homologyPane.add(H1Bdry, cTotal);
        homologyPane.add(labelH2, cLabel);
        homologyPane.add(H2, cTotal);
        homologyPane.add(labelH2Z2, cLabel);
        homologyPane.add(H2Z2, cTotal);

        // Set up the fundamental group pane.
        JPanel fgpPane = new JPanel();
        fgpPane.setLayout(new GridBagLayout());

        fgpName = new JLabel();
        fgpGenerators = new JLabel();
        fgpRelModel = new DefaultListModel();
        fgpRelations = new JList(fgpRelModel);
        fgpRelations.setVisibleRowCount(6);
        componentsToDisable.addElement(fgpName);
        componentsToDisable.addElement(fgpGenerators);
        componentsToDisable.addElement(fgpRelations);

        JLabel labelFgpName = new JLabel("Fundamental Group");
        labelFgpRelations = new JLabel("Relations:");
        componentsToDisable.add(labelFgpName);
        componentsToDisable.add(labelFgpRelations);

        GridBagConstraints cLabel2 = new GridBagConstraints();
        GridBagConstraints cTotal2 = new GridBagConstraints();
        GridBagConstraints cList = new GridBagConstraints();
        cLabel2.anchor = cLabel.CENTER;
        cLabel2.fill = cLabel.NONE;
        cLabel2.gridx = 0;
        cTotal2.anchor = cTotal.CENTER;
        cTotal2.fill = cTotal.NONE;
        cTotal2.gridx = 0;
        cTotal2.insets = new Insets(0, 0, 5, 0);
        cList.anchor = cList.CENTER;
        cList.fill = cList.BOTH;
        cList.gridx = 0;
        cList.weightx = 1;
        cList.weighty = 1;

        fgpPane.add(labelFgpName, cTitle);
        fgpPane.add(fgpName, cTotal2);
        fgpPane.add(fgpGenerators, cLabel2);
        fgpPane.add(labelFgpRelations, cLabel2);
        fgpPane.add(new JScrollPane(fgpRelations), cList);

        // Put everything together.
        JSplitPane all = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
            homologyPane, new PaddedPane(fgpPane, 5));
        all.setResizeWeight(0.5);
        setLayout(new BorderLayout());
        add(all, BorderLayout.CENTER);
    }

    public NPacket getPacket() {
        return triangulation;
    }

    public void reflectPacket() {
        Enumeration e = componentsToDisable.elements();
        while (e.hasMoreElements())
            ((Component)e.nextElement()).setEnabled(true);

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

        if (triangulation.getNumberOfComponents() <= 1) {
            NGroupPresentation fgp = triangulation.getFundamentalGroup();
            String name = fgp.recogniseGroup();
            if (name.length() == 0)
                fgpName.setText("Not recognised");
            else
                fgpName.setText(name);

            long n = fgp.getNumberOfGenerators();
            if (n == 0)
                fgpGenerators.setText("Generators: (none)");
            else if (n == 1)
                fgpGenerators.setText("Generator: g0");
            else if (n == 2)
                fgpGenerators.setText("Generators: g0, g1");
            else
                fgpGenerators.setText("Generators: g0 .. g" +
                    String.valueOf(n - 1));

            n = fgp.getNumberOfRelations();
            for (long i =0; i < n; i++)
                fgpRelModel.addElement("1 = " + fgp.getRelation(i).toString());
        } else {
            fgpName.setText("Cannot calculate");
            fgpGenerators.setText("(disconnected manifold)");
            fgpRelModel.removeAllElements();
            labelFgpRelations.setEnabled(false);
            fgpRelations.setEnabled(false);
        }
    }

    public void editingElsewhere() {
        Enumeration e = componentsToDisable.elements();
        while (e.hasMoreElements())
            ((Component)e.nextElement()).setEnabled(false);

        H1.setText("");
        H1Rel.setText("");
        H1Bdry.setText("");
        H2.setText("");
        H2Z2.setText("");

        fgpName.setText("");
        fgpGenerators.setText("Generators:");
        fgpRelModel.removeAllElements();
    }
}
