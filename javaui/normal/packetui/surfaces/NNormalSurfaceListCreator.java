
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

package normal.packetui.surfaces;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.engine.triangulation.*;
import normal.engine.surfaces.*;
import normal.packetui.*;
import normal.packetfilter.*;
import org.gjt.btools.gui.dialog.*;

/**
 * An interface through which a user can create a new normal surface
 * list from a triangulation.
 * See <tt>PacketCreator</tt> for more details.
 *
 * @see normal.packetui.PacketCreator
 */
public class NNormalSurfaceListCreator extends JPanel
        implements PacketCreator {
    /**
     * Whether or not only embedded surfaces are required.
     */
    private JCheckBox embeddedOnly;

    /**
     * The flavour of coordinate system to use.
     */
    private JComboBox flavourBox;

    /**
     * Create the new interface.
     */
    public NNormalSurfaceListCreator() {
        init();
    }

    /**
     * Create the interface's internal components.
     */
    private void init() {
        // Set up the default options.
        flavourBox = new JComboBox(Coordinates.allCreators());
        embeddedOnly = new JCheckBox("Embedded surfaces only", null, true);

        setLayout(new GridBagLayout());
        GridBagConstraints left = new GridBagConstraints();
        GridBagConstraints right = new GridBagConstraints();
        GridBagConstraints all = new GridBagConstraints();
        left.gridx = 0; right.gridx = 1; all.gridx = 0;
        all.gridwidth = 2;
        left.weightx = 0.0; right.weightx = 1.0; all.weightx = 1.0;
        left.fill = left.NONE; right.fill = right.HORIZONTAL;
            all.fill = all.NONE;
        left.insets = new Insets(0,0,2,2);
        right.insets = new Insets(0,2,2,0);
        all.insets = new Insets(2,0,0,0);
        
        add(new JLabel("Coordinates:"), left);
        add(flavourBox, right);
        add(embeddedOnly, all);
    }

    public NPacket makeNewPacket(Shell shell, NPacket parent,
            Dialog parentDialog) {
        Coordinates flavour = (Coordinates)flavourBox.getSelectedItem();
        if (flavour == null) {
            MessageBox.fgNote(parentDialog,
                "A coordinate system must be selected.");
            return null;
        }
        return shell.getEngine().newNNormalSurfaceList((NTriangulation)parent,
            flavour.getFlavour(), embeddedOnly.isSelected());
    }

    public Component getCreator() {
        return this;
    }

    public String getDialogTitle() {
        return "New Normal Surface List";
    }

    public String getSuggestedPacketLabel() {
        return "Normal Surfaces";
    }

    public PacketFilter getPacketFilter() {
        return new SingleClass(NTriangulation.class);
    }
}

