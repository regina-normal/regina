
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

package normal.mainui;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import normal.Shell;
import normal.engine.packet.NPacket;
import org.gjt.btools.gui.Positioner;
import org.gjt.btools.gui.component.*;
import org.gjt.btools.gui.dialog.*;

/**
 * A modal dialog allowing packet tags to be viewed and edited by the user.
 */
public class TagEditor extends JDialog implements ActionListener {
    /**
     * Add a tag.
     */
    private JButton add;
    /**
     * Remove a tag.
     */
    private JButton remove;
    /**
     * Remove all tags.
     */
    private JButton clear;
    /**
     * Reread the tag list from the engine.
     */
    private JButton refresh;

    /**
     * Close button.
     */
    private JButton close;

    /**
     * The visual list of tags.
     */
    private JList tags;
    /**
     * The underlying model for the visual list of tags.
     */
    private DefaultListModel tagModel;

    /**
     * The topology pane in which we are working.
     */
    private TopologyPane pane;
    /**
     * The packet whose tags are to be edited.
     */
    private NPacket packet;

    /**
     * Creates a new tag editor.
     *
     * @param pane the topology pane in which we are working.
     * @param packet the packet whose tags are to be edited.
     */
    public TagEditor(TopologyPane pane, NPacket packet) {
        super(pane.getShell().getPrimaryFrame(), "Edit Packet Tags", true);
        this.pane = pane;
        this.packet = packet;
        init();
        pack();
        Positioner.centerOnScreen(this);
    }

    /**
     * Initialise the interface components.
     */
    private void init() {
        tagModel = new DefaultListModel();
        tags = new JList(tagModel);
        tags.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        tags.setVisibleRowCount(8);

        // Add the existing packet tags.
        // Note that they will be added in pre-sorted order.
        if (packet.hasTags()) {
            Enumeration e = packet.getTags();
            while (e.hasMoreElements())
                tagModel.addElement(e.nextElement());
        }

        add = new JButton("Add");
        remove = new JButton("Remove");
        clear = new JButton("Remove All");
        refresh = new JButton("Refresh");
        close = new JButton("Close");

        JScrollPane scroller = new JScrollPane(tags);

        // Create the action panel.
        JPanel actionPanel = new JPanel();
        actionPanel.setLayout(new FlowLayout());
        actionPanel.add(add);
        actionPanel.add(remove);
        actionPanel.add(clear);

        // Create the button panel.
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout());
        buttonPanel.add(refresh);
        buttonPanel.add(close);

        // Insert components.
        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(actionPanel, BorderLayout.NORTH);
        getContentPane().add(new PaddedPane(scroller, 5),
            BorderLayout.CENTER);
        getContentPane().add(buttonPanel, BorderLayout.SOUTH);
    
        // Add event listeners.    
        add.addActionListener(this);
        remove.addActionListener(this);
        clear.addActionListener(this);
        refresh.addActionListener(this);
        close.addActionListener(this);
        getRootPane().setDefaultButton(close);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
    }

    /**
     * Called when a button has been pressed.
     *
     * @param e the button pressing event.
     */
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == add) {
            String tag = InputBox.getInput(this, "Add Tag", "").trim();
            if (tag.length() == 0)
                pane.getShell().error("Tags cannot be empty.");
            else if (packet.addTag(tag)) {
                // Insert the tag in the alphabetically correct position.
                int n = tagModel.size();
                int pos, comp;
                for (pos = 0; pos < n; pos++) {
                    comp = tag.compareTo(tagModel.get(pos));
                    if (comp == 0) {
                        // The tag is already in this list!
                        // Make pos large enough that we'll notice.
                        pos = n + 1;
                        break;
                    } else if (comp < 0)
                        break;
                }
                if (pos <= n)
                    tagModel.add(pos, tag);

                setDirty();
            } else
                pane.getShell().error("Tag [" + tag +
                    "] is already associated with this packet.");
        } else if (e.getSource() == remove) {
            String tag = (String)tags.getSelectedValue();
            if (tag == null)
                pane.getShell().error("No tag is selected to remove.");
            else {
                tagModel.removeElement(tag);
                if (packet.removeTag(tag))
                    setDirty();
                else
                    pane.getShell().error("Tag [" + tag +
                        "] was not associated with this packet.");
            }
        } else if (e.getSource() == clear) {
            tagModel.clear();
            if (packet.hasTags()) {
                packet.removeAllTags();
                setDirty();
            }
        } else if (e.getSource() == refresh) {
            tagModel.removeAllElements();

            Enumeration en = packet.getTags();
            while (en.hasMoreElements())
                tagModel.addElement(en.nextElement());
        } else
            dispose();
    }

    /**
     * Called when packet tags are actually modified.
     * The topology pane in which we are working and associated
     * interface components will be updated accordingly.
     */
    private void setDirty() {
        pane.setDirty(true);
        pane.firePacketTagsChanged(packet);
    }
}

