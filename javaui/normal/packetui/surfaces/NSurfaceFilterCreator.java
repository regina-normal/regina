
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

package normal.packetui.surfaces;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.engine.surfaces.*;
import normal.packetfilter.*;
import normal.packetui.*;

/**
 * An interface through which a user can create a new surface filter packet.
 * See <tt>PacketCreator</tt> for more details.
 *
 * @see normal.packetui.PacketCreator
 */
public class NSurfaceFilterCreator extends JPanel implements PacketCreator {
    /**
     * Button representing a filter by properties.
     */
    private JRadioButton filterProperties;

    /**
     * Button representing a combination filter.
     */
    private JRadioButton filterCombination;

    /**
     * Combo box representing the type of combination filter.
     */
    private JComboBox combinationType;

    /**
     * Create the new interface.
     */
    public NSurfaceFilterCreator() {
        init();
    }

    /**
     * Create the interface's internal components.
     */
    private void init() {
        // Set the layout manager.
        setLayout(new GridBagLayout());

        // Initialise the components.
        ButtonGroup type = new ButtonGroup();
        filterProperties = new JRadioButton("Filter by properties", true);
        filterCombination = new JRadioButton("Combination filter:");
        type.add(filterProperties);
        type.add(filterCombination);

        JPanel comboPane = new JPanel();
        comboPane.setLayout(new FlowLayout(FlowLayout.LEFT, 5, 0));
        String[] types = { "AND", "OR" };
        combinationType = new JComboBox(types);
        combinationType.setSelectedIndex(0);
        comboPane.add(new JLabel("Combine using"));
        comboPane.add(combinationType);
        combinationType.setEnabled(false);

        // Insert the components.
        GridBagConstraints cIcon = new GridBagConstraints();
        GridBagConstraints cButton = new GridBagConstraints();
        GridBagConstraints cExtra = new GridBagConstraints();
        cIcon.anchor = cIcon.WEST;
        cIcon.fill = cIcon.NONE;
        cIcon.gridx = 0;
        cIcon.insets = new Insets(0, 0, 0, 5);
        cButton.anchor = cButton.WEST;
        cButton.fill = cButton.NONE;
        cButton.gridx = 1;
        cExtra.anchor = cExtra.WEST;
        cExtra.fill = cExtra.NONE;
        cExtra.gridx = 2;
        add(new JLabel(PacketUIManager.filterPropertiesIcon), cIcon);
        add(filterProperties, cButton);
        add(new JLabel(PacketUIManager.filterCombinationIcon), cIcon);
        add(filterCombination, cButton);
        cExtra.gridy = 1;
        add(comboPane, cExtra);

        // Add selection listeners.
        filterCombination.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                combinationType.setEnabled(
                    e.getStateChange() == ItemEvent.SELECTED);
            }
        });
    }

    public NPacket makeNewPacket(Shell shell, NPacket parent,
            Dialog parentDialog) {
        if (filterProperties.isSelected())
            return shell.getEngine().newNSurfaceFilterProperties();
        else {
            NSurfaceFilterCombination ans =
                shell.getEngine().newNSurfaceFilterCombination();
            ans.setUsesAnd(combinationType.getSelectedIndex() == 0);
            return ans;
        }
    }

    public Component getCreator() {
        return this;
    }

    public String getDialogTitle() {
        return "New Surface Filter";
    }

    public String getSuggestedPacketLabel() {
        return "Filter";
    }

    public PacketFilter getPacketFilter() {
        return AllowAll.instance;
    }
}
