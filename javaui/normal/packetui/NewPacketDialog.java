
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

package normal.packetui;

import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.tree.*;
import btools.gui.*;
import btools.gui.component.*;
import btools.gui.dialog.*;
import normal.Shell;
import normal.mainui.*;
import normal.packetfilter.*;
import normal.engine.packet.NPacket;

/**
 * A dialog through which the user can create a new packet.
 * The dialog provides standard mechanisms to choose a parent for the
 * new packet in the tree structure as well as a label for the packet,
 * and provides <i>OK</i> and <i>Cancel</i> buttons to begin or abort
 * creating the new packet.  All other information gathering, as well
 * as the packet creation itself, is done through a
 * <tt>PacketCreator</tt> that is associated with this dialog.
 * <p>
 * In the <tt>newPacket()</tt> function, all of the work is done for you.
 * This function will create from scratch a new <tt>NewPacketDialog</tt>
 * with the requested details, process the user's input and return a new
 * packet pre-inserted into the tree if one was successfully created.
 *
 * @see normal.packetui.PacketCreator
 * @see #newPacket
 */
public class NewPacketDialog extends JDialog implements ActionListener {
    /**
     * The shell representing the entire program.
     * @serial
     */
    private Shell shell;
    
    /**
     * The <tt>PacketCreator</tt> that will collect user input and create
     * the new packet.
     * @serial
     */
    private PacketCreator ui;
    
    /**
     * The root of the visual packet tree corresponding to the internal
     * packet tree.
     * @serial
     */
    private PacketTreeNode rootNode;

    /**
     * The node in the visual packet tree chosen by the user to be the
     * parent for the new packet.
     * @serial
     */
    private PacketTreeNode parentNode = null;
    
    /**
     * The newly created packet.  This should be <tt>null</tt> unless
     * the new packet has been successfully created and the dialog closed.
     * @serial
     */
    private NPacket createdPacket = null;

    /**
     * The combo box that will contain the list of potential parents for
     * the new packet.
     * @serial
     */
    private JComboBox parentBox;
    
    /**
     * Text field for entering the new packet label.
     * @serial
     */
    private JTextField label = new JTextField();
    
    /**
     * OK button.
     * @serial
     */
    private JButton ok = new JButton();
    
    /**
     * Cancel button.
     * @serial
     */
    private JButton cancel = new JButton();

    /**
     * Create a new dialog and fully prepare all of its interface elements.
     *
     * @param shell the shell representing the entire program.
     * @param rootNode the root of the visual packet tree corresponding to
     * the internal packet tree with which we are working.
     * @param defaultParent the default offered to the user as the parent
     * packet for the new packet.  This can be <tt>null</tt> or a packet not
     * appearing in this dialog's list of potential parents;
     * in such cases, the default will be the first choice available.
     * @param ui the <tt>PacketCreator</tt> which will be responsible for
     * collecting information and actually creating the new packet.
     */
    private NewPacketDialog(Shell shell, PacketTreeNode rootNode,
            PacketTreeNode defaultParent, PacketCreator ui) {
        // Create a new modal dialog.
        super(shell.getPrimaryFrame(), ui.getDialogTitle(), true);
        
        // Initialise member variables.
        this.ui = ui;
        this.shell = shell;
        this.rootNode = rootNode;

        // Set up the combo box of potential parent packets.
        parentBox = new JComboBox(rootNode.findDescendants(
            ui.getPacketFilter()));
        parentBox.setRenderer(new PacketTreeNode.ListCellRenderer());

        // Build the dialog's interface elements.
        init();
        // Offer the user default selections.
        if (defaultParent != null)
            parentBox.setSelectedItem(defaultParent);
        label.setText(rootNode.getPacket().makeUniqueLabel(
            ui.getSuggestedPacketLabel()));
        pack();
        Positioner.centerOnScreen(this);
    }

    /**
     * Create the dialog's interface elements.
     */
    private void init() {
        // Create panels.
        JPanel parentPanel = new JPanel();
        JPanel buttonPanel = new JPanel();
        
        // Set layout managers.
        getContentPane().setLayout(new BorderLayout());
        parentPanel.setLayout(new GridBagLayout());
        buttonPanel.setLayout(new FlowLayout());

        // Set up the parent chooser panel.
        JLabel parentLabel = new JLabel("Derive from:");
        parentLabel.setHorizontalAlignment(SwingConstants.RIGHT);
        JLabel labelLabel = new JLabel("Label:");
        labelLabel.setHorizontalAlignment(SwingConstants.RIGHT);
        
        GridBagConstraints left = new GridBagConstraints();
        GridBagConstraints right = new GridBagConstraints();
        left.gridx = 0; right.gridx = 1;
        left.weightx = 0.0; right.weightx = 1.0;
        left.fill = left.HORIZONTAL; right.fill = right.HORIZONTAL;
        
        parentPanel.setBorder(BorderFactory.createRaisedBevelBorder());
        left.insets = new Insets(4,5,2,5);
        parentPanel.add(labelLabel, left.clone());
        right.insets = new Insets(4,2,2,5); right.ipady = 5;
        parentPanel.add(label, right.clone());
        left.insets = new Insets(2,5,4,5);
        parentPanel.add(parentLabel, left.clone());
        right.insets = new Insets(2,2,4,5); right.ipady = 0;
        parentPanel.add(parentBox, right.clone());

        // Set up the button panel.
        ok.setText("OK");
        cancel.setText("Cancel");
        
        buttonPanel.add(ok);
        buttonPanel.add(cancel);
        
        // Set up the main panel.
        getContentPane().add(parentPanel, BorderLayout.NORTH);
        getContentPane().add(new PaddedPane(ui.getCreator(), 5),
            BorderLayout.CENTER);
        getContentPane().add(buttonPanel, BorderLayout.SOUTH);

        // Add event listeners.    
        ok.addActionListener(this);
        cancel.addActionListener(this);
        getRootPane().setDefaultButton(ok);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
    }

    /**
     * For internal use only.
     * Called when a dialog button has been pressed.
     */
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == ok) {
            // OK was pressed - attempt to create the packet.
            
            // Find the proposed parent packet.
            parentNode = (PacketTreeNode)(parentBox.getSelectedItem());
            if (parentNode == null) {
                MessageBox.fgNote(this,
                    "You have not selected a packet from which to derive " +
                    "the new packet.");
                return;
            }
            
            // Find the proposed packet label.
            String newLabel = label.getText().trim();
            if (newLabel.length() == 0) {
                MessageBox.fgNote(this,
                    "You have not entered a label for the new packet.");
                return;
            }
            if (rootNode.getPacket().findPacketLabel(newLabel) != null) {
                MessageBox.fgNote(this, "The packet label [" + newLabel
                    + "] is already in use.  Please select another.");
                return;
            }
            
            // Attempt to create the packet.
            createdPacket = ui.makeNewPacket(shell, parentNode.getPacket(),
                this);
            if (createdPacket == null)
                return;
                
            // The packet was successfully created.
            // Label it and close the dialog.
            createdPacket.setPacketLabel(newLabel);
            dispose();
        }
        else if (e.getSource() == cancel) {
            // Cancel was pressed - close the dialog.
            dispose();
        }
    }

    /**
     * Creates a new dialog, collects user input and tidies up, returning
     * the new packet if one was created.  If the user wishes to create a
     * new packet, simply call this function and everything will be done
     * for you.  The new packet will be inserted into the internal
     * packet tree structure.  The system pane however will still need
     * to be notified that the internal tree structure has changed.
     * <p>
     * This routine will insert the newly created packet into the internal
     * packet tree structure if and only if this has not been done already.
     * <p>
     * Any errors whilst attempting to create the new packet will be
     * reported to the user.
     *
     * @param shell the shell representing the entire program.
     * @param rootNode the root of the visual packet tree corresponding to
     * the internal packet tree with which we are working.
     * @param defaultParent the default offered to the user as the parent
     * packet for the new packet.  This can be <tt>null</tt> or a packet not
     * appearing in this dialog's list of potential parents;
     * in such cases, the default will be the first choice available.
     * @param ui the <tt>PacketCreator</tt> which will be responsible for
     * collecting information and actually creating the new packet.
     * @return the newly created packet,
     * or <tt>null</tt> if no new packet was created.
     * @see normal.packetui.PacketCreator
     */
    public static NPacket newPacket(Shell shell, PacketTreeNode rootNode,
            PacketTreeNode defaultParent, PacketCreator ui) {
        // Create and display the new dialog.
        NewPacketDialog d = new NewPacketDialog(shell, rootNode,
            defaultParent, ui);
        d.show();
        
        // If the packet was successfully created, insert it into both trees.
        if (d.createdPacket != null) {
            if (d.createdPacket.getTreeParent() == null)
                d.parentNode.getPacket().insertChildLast(d.createdPacket);
            return d.createdPacket;
        }
        return null;
    }
}
