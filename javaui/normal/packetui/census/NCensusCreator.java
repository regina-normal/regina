
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

package normal.packetui.census;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import normal.Shell;
import normal.engine.*;
import normal.engine.packet.*;
import normal.engine.progress.*;
import normal.engine.utilities.NBoolSet;
import normal.mainui.*;
import normal.packetfilter.*;
import normal.packetui.*;
import normal.progress.*;
import org.gjt.btools.ext.*;
import org.gjt.btools.gui.dialog.*;

/**
 * An interface through which the user can create a census of
 * triangulations.
 * See <tt>PacketCreator</tt> for further details.
 *
 * @see normal.packetui.PacketCreator
 */
public class NCensusCreator extends JPanel implements PacketCreator {
    /**
     * Number of tetrahedra in each triangulation.
     * @serial
     */
    private JTextField nTetrahedra;
    /**
     * Number of boundary faces in each triangulation.
     * @serial
     */
    private JTextField nBdryFaces;
    /**
     * A label for the number of boundary faces.
     * @serial
     */
    private JLabel nBdryFacesLabel;
    /**
     * Specifies whether to allow any number of boundary faces.
     * @serial
     */
    private JCheckBox anyBdryFaces;

    /**
     * Policy on finiteness.
     * @serial
     */
    private JComboBox finiteness;
    /**
     * Policy on orientability.
     * @serial
     */
    private JComboBox orientability;
    /**
     * Policy on boundary.
     * @serial
     */
    private JComboBox boundary;
    
    /**
     * Creates a new census creator.
     */
    public NCensusCreator() {
        init();
    }

    /**
     * Create the interface's internal components.
     */
    private void init() {
        // Set the layout manager.
        setLayout(new BorderLayout());

        // Initialise the components.
        nTetrahedra = new JTextField(new NonNegativeIntegerDocument(), "", 3);
        nBdryFaces = new JTextField(new NonNegativeIntegerDocument(), "", 3);
        nBdryFacesLabel = new JLabel("Number of boundary faces:");
        anyBdryFaces = new JCheckBox("Any number of boundary faces", true);
        String[] finitenessStrings = { "No finiteness restrictions",
            "Finite only", "Ideal only" };
        finiteness = new JComboBox(finitenessStrings);
        finiteness.setSelectedIndex(0);
        String[] orientabilityStrings = { "No orientability restrictions",
            "Orientable only", "Non-orientable only" };
        orientability = new JComboBox(orientabilityStrings);
        orientability.setSelectedIndex(0);
        String[] boundaryStrings = { "No boundary restrictions",
            "Has boundary faces", "All faces internal" };
        boundary = new JComboBox(boundaryStrings);
        boundary.setSelectedIndex(2);
        nBdryFaces.setEnabled(false);
        nBdryFacesLabel.setEnabled(false);
        anyBdryFaces.setEnabled(false);

        // Insert the components.
        JPanel tetPanel = new JPanel();
        tetPanel.setLayout(new GridBagLayout());
        GridBagConstraints cLeft = new GridBagConstraints();
        GridBagConstraints cRight = new GridBagConstraints();
        GridBagConstraints cBoth = new GridBagConstraints();
        cLeft.weightx = 1; cRight.weightx = 0; cBoth.weightx = 1;
        cLeft.gridx = 0; cRight.gridx = 1; cBoth.gridx = 0;
        cLeft.fill = cLeft.NONE; cRight.fill = cRight.NONE;
            cBoth.fill = cBoth.NONE;
        cLeft.anchor = cLeft.WEST; cBoth.anchor = cBoth.WEST;
        cBoth.gridwidth = 2;
        tetPanel.add(new JLabel("Number of tetrahedra:"), cLeft);
        tetPanel.add(nTetrahedra, cRight);
        tetPanel.add(nBdryFacesLabel, cLeft);
        tetPanel.add(nBdryFaces, cRight);
        tetPanel.add(anyBdryFaces, cBoth);
        JPanel optionPanel = new JPanel();
        optionPanel.setLayout(new GridLayout(0, 1));
        optionPanel.add(finiteness);
        optionPanel.add(orientability);
        optionPanel.add(boundary);
        add(tetPanel, BorderLayout.NORTH);
        add(optionPanel, BorderLayout.CENTER);

        boundary.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                if (e.getStateChange() == e.SELECTED) {
                    if (boundary.getSelectedIndex() == 2) {
                        anyBdryFaces.setEnabled(false);
                        nBdryFaces.setEnabled(false);
                        nBdryFacesLabel.setEnabled(false);
                    } else {
                        anyBdryFaces.setEnabled(true);
                        nBdryFaces.setEnabled(! anyBdryFaces.isSelected());
                        nBdryFacesLabel.setEnabled(! anyBdryFaces.isSelected());
                    }
                }
            }
        });
        anyBdryFaces.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                if (boundary.getSelectedIndex() == 2) {
                    anyBdryFaces.setEnabled(false);
                    nBdryFaces.setEnabled(false);
                    nBdryFacesLabel.setEnabled(false);
                } else {
                    anyBdryFaces.setEnabled(true);
                    nBdryFaces.setEnabled(e.getStateChange() == e.DESELECTED);
                    nBdryFacesLabel.setEnabled(e.getStateChange() ==
                        e.DESELECTED);
                }
            }
        });
    }

    public Component getCreator() {
        return this;
    }

    public String getDialogTitle() {
        return "New Census";
    }

    public PacketFilter getPacketFilter() {
        return AllowAll.instance;
    }

    public String getSuggestedPacketLabel() {
        return "Census";
    }

    /**
     * Determines which boolean set has been selected in the given combo box.
     *
     * @param comboBox the given combo box.
     * @return the selected boolean set.
     */
    public NBoolSet whichBoolSet(JComboBox comboBox) {
        switch(comboBox.getSelectedIndex()) {
            case 0: return NBoolSet.sBoth;
            case 1: return NBoolSet.sTrue;
            case 2: return NBoolSet.sFalse;
        }
        return NBoolSet.sNone;
    }

    public NPacket makeNewPacket(Shell shell, NPacket parent,
            Dialog parentDialog) {
        // Check value of NTetrahedra.
        int tets = ((NonNegativeIntegerDocument)nTetrahedra.getDocument()).
            getIntValue();
        int bdryFaces = ((NonNegativeIntegerDocument)nBdryFaces.getDocument()).
            getIntValue();
        if (tets <= 0) {
            MessageBox.fgNote(parentDialog,
                "The number of tetrahedra must be positive.");
            return null;
        }
        if (whichBoolSet(boundary).hasTrue() &&
                (! anyBdryFaces.isSelected())) {
            if (nBdryFaces.getText().length() == 0) {
                MessageBox.fgNote(parentDialog,
                    "You must either specify the number of boundary faces " +
                    "or check the box that allows any number of boundary " +
                    "faces.");
                return null;
            }
            if (bdryFaces % 2 == 1) {
                MessageBox.fgNote(parentDialog,
                    "The number of boundary faces must be even.");
                return null;
            } else if (bdryFaces > 2 * tets + 2) {
                MessageBox.fgNote(parentDialog,
                    "No connected triangulation with " +
                    String.valueOf(tets) +
                    (tets == 1 ? " tetrahedron" : " tetrahedra") +
                    " has more than "
                    + String.valueOf(2 * tets + 2) + " boundary faces.");
                return null;
            } else if (bdryFaces == 0 && ! whichBoolSet(boundary).hasFalse()) {
                MessageBox.fgNote(parentDialog,
                    "You cannot request 0 boundary faces for only " +
                    "bounded triangulations.");
                return null;
            }
        } else
            bdryFaces = -1;

        if (tets > 4) {
            if (! MessageBox.confirm(parentDialog,
                    "This routine is designed for a small census only.  " +
                    "Using more than four tetrahedra will be very slow " +
                    "and might result in a program crash.  " +
                    "Are you sure you wish to continue?"))
                return null;
        }
        
        Engine engine = shell.getEngine();
        NPacket censusBase = engine.newNContainer();
        // Place the census base in the tree now so unique labels for
        // child triangulations can be assured.
        parent.insertChildLast(censusBase);

        NProgressManager manager = engine.newNProgressManager();
        engine.formCensus(censusBase, tets,
            whichBoolSet(finiteness), whichBoolSet(orientability),
            whichBoolSet(boundary), bdryFaces, manager);
        new ProgressDialog(parentDialog, "Forming census:", manager).
            runLifeCycle();

        return censusBase;
    }
}
 
