
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

package normal.algorithm;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.tree.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.mainui.*;
import normal.packetfilter.*;
import org.gjt.btools.gui.*;
import org.gjt.btools.gui.component.*;

/**
 * An algorithm that the user can perform on a packet.
 * <p>
 * To fill a menu with algorithms, the static method <tt>makeMenu()</tt>
 * should be extended to include all desired algorithms and should be
 * called from the appropriate visual component initialisation routine.
 * <p>
 * See the <tt>Algorithm</tt> constructor for details regarding how to
 * specify a specific algorithm.
 *
 * @see #Algorithm
 */
public abstract class Algorithm extends JMenuItem implements ActionListener {
    /**
     * Filters which packets this algorithm may be performed upon.
     */
    protected PacketFilter filter;

    /**
     * The shell representing the entire program.
     */
    protected Shell shell;

    /**
     * The topology pane in which we are working.
     */
    protected TopologyPane system;

    /**
     * Is it possible that this algorithm might edit the packet upon
     * which it is performed?
     */
    protected boolean mightEdit;

    /**
     * The name of this algorithm as it appears on a menu.
     */
    protected String name;

    /**
     * The text to add to the end of a packet label if the packet is
     * cloned to perform this algorithm.
     */
    protected String packetPostLabel;

    /**
     * Creates a new algorithm with the given specifications.
     * This algorithm object will also be registered as an action
     * listener for itself.
     *
     * @param shell the shell representing the entire program.
     * @param menuLabel the text to display in the menu item
     * corresponding to this algorithm.
     * @param packetPostLabel the text to add to the end of a packet
     * label if the packet is cloned to perform this algorithm; this
     * text should not begin with any spaces.
     * @param filter specifies which packets we are allowed to perform
     * this algorithm upon.
     * @param mightEdit <tt>true</tt> if and only if there is a chance
     * that this algorithm will alter the contents of the packet upon
     * which it is performed.
     * @see normal.engine.packet.NPacket
     */
    public Algorithm(Shell shell, String menuLabel,
            String packetPostLabel, PacketFilter filter, boolean mightEdit) {
        super(menuLabel);
        this.shell = shell;
        this.filter = filter;
        this.mightEdit = mightEdit;
        this.name = menuLabel;
        this.packetPostLabel = packetPostLabel;
        addActionListener(this);
    }

    /**
     * Creates a new algorithm with the given specifications.
     * This algorithm object will also be registered as an action
     * listener for itself.
     *
     * @param shell the shell representing the entire program.
     * @param menuLabel the text to display in the menu item
     * corresponding to this algorithm.
	 * @param mnemonic the keyboard mnemonic to associate with the
	 * corresponding menu item.
     * @param packetPostLabel the text to add to the end of a packet
     * label if the packet is cloned to perform this algorithm; this
     * text should not begin with any spaces.
     * @param filter specifies which packets we are allowed to perform
     * this algorithm upon.
     * @param mightEdit <tt>true</tt> if and only if there is a chance
     * that this algorithm will alter the contents of the packet upon
     * which it is performed.
     * @see normal.engine.packet.NPacket
     */
    public Algorithm(Shell shell, String menuLabel, int mnemonic,
            String packetPostLabel, PacketFilter filter, boolean mightEdit) {
        super(menuLabel);
        this.shell = shell;
        this.filter = filter;
        this.mightEdit = mightEdit;
        this.name = menuLabel;
        this.packetPostLabel = packetPostLabel;
		setMnemonic(mnemonic);
        addActionListener(this);
    }

    /**
     * Called when an algorithm menu item is selected.
     * Brings up a dialog to run the appropriate algorithm
     * and then refreshes the display if necessary.
     *
     * @param event contains details of the event that occurred.
     */
    public void actionPerformed(ActionEvent event) {
        system = shell.getNormalFrame().getCurrentTopologyPane();
        if (system == null)
            return;
        PacketTreeNode node = system.getSelectedNode();
        PacketTreeNode rootNode = (PacketTreeNode)system.getPacketTree().
            getModel().getRoot();
        
        AlgorithmDialog dlg = new AlgorithmDialog(node, rootNode, shell);
        Positioner.centerOnScreen(dlg);
        dlg.show();
    }

    /**
     * Inserts this algorithm into the given menu and vector.
     *
     * @param menu the menu into which the algorithm will be inserted.
     * @param vector the vector into which the algorithm will be
     * inserted.
     */
    public void insert(JMenu menu, Vector vector) {
        menu.add(this);
        vector.addElement(this);
    }

    /**
     * Performs this algorithm upon the given packet.  It can be
     * guaranteed that the packet passed satisfies the packet filter
     * passed to the algorithm constructor.  If
     * <tt>mightEdit</tt> was set to <tt>true</tt> in the algorithm
     * constructor, it can also be guaranteed that the given packet will
     * allow itself to be edited.
     * <p>
     * No updating of the visual display is required.  No packet other than
     * the packet passed may be edited.
     *
     * @param packet the packet upon which to perform this algorithm.
     * @return <tt>true</tt> if and only if the packet contents were
     * actually changed.
     * @see #Algorithm
     */
    public abstract boolean perform(NPacket packet);

    private class AlgorithmDialog extends JDialog implements ActionListener,
            ItemListener {
        /**
         * The node in the visual packet tree to be operated upon.
         */
        private PacketTreeNode selectedNode = null;

        /**
         * The combo box that will contain the list of potential packets
         * to operate upon.
         */
        private JComboBox packetBox;

        /**
         * Button asking to perform the algorithm in place.
         */
        private JRadioButton performOnMe;

        /**
         * Button asking to perform the algorithm on a clone.
         */
        private JRadioButton performOnClone;

        /**
         * OK button.
         */
        private JButton ok = new JButton();

        /**
         * Cancel button.
         */
        private JButton cancel = new JButton();

        /**
         * Create a new dialog and fully prepare all of its interface
         * elements.
         *
         * @param defaultNode the default offered to the user as the
         * packet upon which to operate.  This can be <tt>null</tt> or a
         * packet not appearing in this dialog's list of potential
         * packets; in such cases, the default will be the first choice
         * available.
         * @param rootNode the root node of the visual packet tree within
         * which we are operating.
         * @param shell the shell representing the entire program.
         */
        public AlgorithmDialog(PacketTreeNode defaultNode,
                PacketTreeNode rootNode, Shell shell) {
            // Create a new modal dialog.
            super(shell.getPrimaryFrame(), true);
            setTitle(name);

            // Set up the combo box of potential packets to use.
            packetBox = new JComboBox((rootNode.findDescendants(filter)));
            packetBox.setRenderer(new PacketTreeNode.ListCellRenderer());

            // Build the dialog's interface elements.
            init();

            // Offer the user a default selection.
            if (defaultNode != null)
                packetBox.setSelectedItem(defaultNode);

            pack();
        }

        /**
         * Create the interface's internal components.
         */
        private void init() {
            ButtonGroup performOnWho = new ButtonGroup();
            performOnMe = new JRadioButton("Modify directly", true);
            performOnClone = new JRadioButton("Clone first");
            performOnWho.add(performOnMe);
            performOnWho.add(performOnClone);
            if (mightEdit)
                rectifyWhoToModify();
            else
                performOnClone.setEnabled(false);
            ok.setText("OK");
            cancel.setText("Cancel");

            JPanel packetPanel = new JPanel();
            packetPanel.setLayout(new GridLayout(1,0));
            packetPanel.add(new JLabel("Operate upon: "));
            packetPanel.add(packetBox);
            JPanel packetPadding = new PaddedPane(packetPanel, 5);
            packetPadding.setBorder(BorderFactory.createRaisedBevelBorder());
            
            JPanel choicePanel = new JPanel();
            choicePanel.setLayout(new GridLayout(0,1));
            choicePanel.add(performOnMe);
            choicePanel.add(performOnClone);

            JPanel buttonPanel = new JPanel();
            buttonPanel.setLayout(new FlowLayout());
            buttonPanel.add(ok);
            buttonPanel.add(cancel);
            
            getContentPane().setLayout(new GridBagLayout());
            GridBagConstraints outer = new GridBagConstraints();
            GridBagConstraints middle = new GridBagConstraints();
            outer.gridx = 0; middle.gridx = 0;
            outer.weightx = 1.0; middle.weightx = 0.0;
            outer.weighty = 0.0; middle.weighty = 1.0;
            outer.fill = outer.HORIZONTAL; middle.fill = middle.NONE;
            middle.insets = new Insets(5,5,5,5);
            getContentPane().add(packetPadding, outer);
            getContentPane().add(choicePanel, middle);
            getContentPane().add(buttonPanel, outer);

            ok.addActionListener(this);
            cancel.addActionListener(this);
            packetBox.addItemListener(this);
            this.getRootPane().setDefaultButton(ok);
            setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        }

        /**
         * Enable or disable the who-to-modify buttons according to the
         * currently selected packet.  This routine will only do
         * anything if this algorithm might actually modify the packet.
         */
        private void rectifyWhoToModify() {
            if (! mightEdit)
                return;
            PacketTreeNode n = (PacketTreeNode)packetBox.getSelectedItem();
            if (n == null)
                return;
            if (n.getPacket().isPacketEditable())
                performOnMe.setEnabled(true);
            else {
                performOnMe.setSelected(false);
                performOnClone.setSelected(true);
                performOnMe.setEnabled(false);
            }
        }

        /**
         * For internal use only.
         * Called when the selected item in the packet box is changed.
         */
        public void itemStateChanged(ItemEvent e) {
            if (e.getStateChange() == e.SELECTED)
                rectifyWhoToModify();
        }

        /**
         * For internal use only.
         * Called when a dialog button has been pressed.
         */
        public void actionPerformed(ActionEvent e) {
            if (e.getSource() == ok) {
                PacketTreeNode n = (PacketTreeNode)packetBox.getSelectedItem();
                if (n == null) {
                    shell.error("No packet is currently selected.");
                    return;
                }
                if (! (performOnMe.isSelected() ||
                        performOnClone.isSelected())) {
                    shell.error("Neither of the two modification "
                        + "options has been selected.");
                    return;
                }
                NPacket packet = n.getPacket();
                if (! packet.isPacketEditable())
                    if (performOnMe.isSelected()) {
                        shell.error("At least one of its " +
                            "children forbids this packet from being edited.  "
                            + "Try selecting the cloning option instead.");
                        return;
                    }
                
                // Actually perform the algorithm.
                boolean changed;
                boolean cloned = false;
                if (performOnClone.isSelected()) {
                    String label = packet.getPacketLabel();
                    packet = packet.clonePacket(false, false);
                    if (packet == null) {
                        shell.error("The matriarch of the packet " +
                            "tree cannot be cloned.");
                        return;
                    }
                    changed = perform(packet);
                    if (changed) {
                        cloned = true;
                        packet.setPacketLabel(
                            packet.makeUniqueLabel(label + " - " +
                            packetPostLabel));
                    } else {
                        packet.makeOrphan();
                        packet.destroy();
                    }
                } else
                    changed = perform(packet);

                // Do away with the dialog at this point.
                dispose();

                if (changed) {
                    system.setDirty(true);
                    if (cloned)
                        system.fireSubtreeWasInserted(packet, null, true);
                    else {
                        system.firePacketWasChanged(packet, null);
                        system.fireSubtreeHasChanged(packet, null, true);
                        system.ensureViewingPacket(packet);
                    }
                } else if (mightEdit)
                    shell.error("No changes to the selected "
                        + "packet were necessary.");
            } else if (e.getSource() == cancel)
                dispose();
        }
    }
}

