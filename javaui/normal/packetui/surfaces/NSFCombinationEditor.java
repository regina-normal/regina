
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

package normal.packetui.surfaces;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import normal.engine.*;
import normal.engine.packet.*;
import normal.engine.surfaces.*;
import normal.engine.utilities.*;
import normal.packetui.*;
import org.gjt.btools.gui.component.*;

/**
 * An interface through which a user can view or edit a surface filter.
 * See <tt>PacketUI</tt> for more details.
 *
 * @see normal.packetui.PacketUI
 */
public class NSFCombinationEditor extends DefaultPacketEditor
        implements PacketInfoUI {
    /**
     * The packet associated with this interface.
     */
    private NSurfaceFilterCombination packet;

    /**
     * The type of filter combination.
     */
    private JComboBox comboType;

    /**
     * A list of the filters that are combined by this filter.
     */
    private JList childFilters;

    /**
     * Create a new interface to edit a given packet.
     *
     * @param packet the packet to be associated with this interface.
     * @param editor <tt>true</tt> if this interface is to be used for
     * editing, or <tt>false</tt> if it is for viewing only.
     */
    public NSFCombinationEditor(NPacket packet, boolean editor) {
        super(editor);
        this.packet = (NSurfaceFilterCombination)packet;
        init(editor);
    }

    public NPacket getPacket() {
        return packet;
    }

    /**
     * Create the interface's interface elements.
     *
     * @param editor <tt>true</tt> if this interface is to be used for
     * editing, or <tt>false</tt> if it is for viewing only.
     */
    private void init(boolean editor) {
        // Set the layout manager.
        setLayout(new BorderLayout());
        
        // Initialise the components.
        String[] typeStrings = { "AND", "OR" };
        comboType = new JComboBox(typeStrings);
        comboType.setEnabled(editor);
        childFilters = new JList(new DefaultListModel());
        childFilters.setCellRenderer(new PacketListCellRenderer() {
            // Make it look as if nothing is ever selected.
            public Component getListCellRendererComponent(JList l,
                    Object v, int i, boolean selected, boolean focus) {
                return super.getListCellRendererComponent(l, v, i, false,
                    false);
            }
        });
        refreshChildFilters();

        JPanel typePane = new JPanel();
        typePane.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
        typePane.add(new JLabel("Combine using"));
        typePane.add(comboType);

        JPanel childPane = new JPanel();
        childPane.setLayout(new BorderLayout());
        childPane.add(new JLabel("Filters combined:"),
            BorderLayout.NORTH);
        childPane.add(new JScrollPane(childFilters), BorderLayout.CENTER);
        childPane.add(new JLabel(
            "All immediate child filters in the tree are combined.",
            SwingConstants.CENTER), BorderLayout.SOUTH);

        JPanel all = new JPanel();
        all.setLayout(new BorderLayout());
        all.add(typePane, BorderLayout.NORTH);
        all.add(childPane, BorderLayout.CENTER);
        add(new PaddedPane(all, 0, false), BorderLayout.CENTER);

        reflectPacket();
        
        if (editor) {
            // Add event listeners.
            ActionListener al = new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    setChanged(true);
                }
            };
            comboType.addActionListener(al);
        }
    }

    /**
     * Refreshes the contents of the list of immediate child filters.
     */
    private void refreshChildFilters() {
        DefaultListModel m = (DefaultListModel)childFilters.getModel();
        m.removeAllElements();
        NPacket child = packet.getFirstTreeChild();
        while (child != null) {
            if (child instanceof NSurfaceFilter)
                m.addElement(child);
            child = child.getNextTreeSibling();
        }
    }

    public void reflectPacket() {
        comboType.setEnabled(isEditor());

        ignoreSetChanged(true);
        comboType.setSelectedIndex(
            packet.getUsesAnd() ? 0 : 1);
        ignoreSetChanged(false);
        setChanged(false);
    }
    
    public void applyChanges(Engine engine) {
        packet.setUsesAnd(
            comboType.getSelectedIndex() == 0 ? true : false);
        setChanged(false);
    }

    public void editingElsewhere() {
        comboType.setEnabled(false);
    }

    public void packetWasRenamed(NPacket ren, PacketUI ui, Frame owner) {
        super.packetWasRenamed(ren, ui, owner);
        NPacket parent = ren.getTreeParent();
        if (parent != null && parent.sameObject(packet))
            childFilters.repaint();
    }

    public void subtreeToBeDeleted(NPacket subtree, PacketUI ui, Frame owner) {
        super.subtreeToBeDeleted(subtree, ui, owner);
        if (subtree.getTreeParent().sameObject(packet))
            ((DefaultListModel)childFilters.getModel()).removeElement(subtree);
    }

    public void subtreeWasInserted(NPacket subtree, PacketUI ui, Frame owner) {
        super.subtreeWasInserted(subtree, ui, owner);
        if (subtree.getTreeParent().sameObject(packet))
            refreshChildFilters();
    }

    public void subtreeHasChanged(NPacket subtree, PacketUI ui, Frame owner) {
        super.subtreeHasChanged(subtree, ui, owner);
        if (subtree.isGrandparentOf(packet) ||
                subtree.getTreeParent().sameObject(packet))
            refreshChildFilters();
    }
}
