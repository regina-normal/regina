
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
import java.math.BigInteger;
import java.util.Enumeration;
import javax.swing.*;
import javax.swing.event.*;
import normal.Shell;
import normal.engine.*;
import normal.engine.packet.*;
import normal.engine.surfaces.*;
import normal.engine.utilities.*;
import normal.packetui.*;
import org.gjt.btools.ext.*;
import org.gjt.btools.gui.component.*;

/**
 * An interface through which a user can view or edit a surface filter.
 * See <tt>PacketUI</tt> for more details.
 *
 * @see normal.packetui.PacketUI
 */
public class NSFPropertiesEditor extends DefaultPacketEditor
        implements PacketInfoUI {
    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * The packet associated with this interface.
     */
    private NSurfaceFilterProperties packet;

    /**
     * The set of all allowable orientabilities.
     */
    private JComboBox orientability;
    /**
     * The set of all allowable compactness properties.
     */
    private JComboBox compactness;
    /**
     * The set of all has-real-boundary properties.
     */
    private JComboBox realBoundary;
    /**
     * Determines whether to restrict according to Euler characteristic.
     */
    private JCheckBox useEuler;
    /**
     * The list of Euler characteristics to allow.
     */
    private JList allowEuler;
    /**
     * The new Euler characteristic to add to the list.
     */
    private JTextField newEuler;
    /**
     * A button to add a new Euler characteristic to the list.
     */
    private JButton addEuler;
    /**
     * A button to remove the selected Euler characteristic from the list.
     */
    private JButton removeEuler;

    /**
     * Create a new interface to edit a given packet.
     *
     * @param packet the packet to be associated with this interface.
     * @param shell the shell representing the entire program.
     * @param editor <tt>true</tt> if this interface is to be used for
     * editing, or <tt>false</tt> if it is for viewing only.
     */
    public NSFPropertiesEditor(NPacket packet, Shell shell, boolean editor) {
        super(editor);
        this.packet = (NSurfaceFilterProperties)packet;
        this.shell = shell;
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
        String[] orientabilityStrings = { "No orientability restrictions",
            "Orientable only", "Non-orientable only" };
        orientability = new JComboBox(orientabilityStrings);
        String[] compactnessStrings = { "No compactness restrictions",
            "Compact only", "Non-compact only" };
        compactness = new JComboBox(compactnessStrings);
        String[] boundaryStrings = { "No real boundary restrictions",
            "With real boundary only", "Without real boundary only" };
        realBoundary = new JComboBox(boundaryStrings);
        orientability.setEnabled(editor);
        compactness.setEnabled(editor);
        realBoundary.setEnabled(editor);

        JPanel optionsPanel = new JPanel();
        optionsPanel.setLayout(new GridLayout(0, 1));
        optionsPanel.add(orientability);
        optionsPanel.add(compactness);
        optionsPanel.add(realBoundary);

        useEuler = new JCheckBox("Only allow Euler characteristics:", false);
        useEuler.setEnabled(editor);
        allowEuler = new JList(new DefaultListModel());
        allowEuler.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        newEuler = new JTextField(new IntegerDocument(), "", 4);
        newEuler.setMaximumSize(newEuler.getPreferredSize());
        newEuler.setAlignmentX(Component.CENTER_ALIGNMENT);
        addEuler = new JButton("Add:");
        addEuler.setMaximumSize(new Dimension(Short.MAX_VALUE,
            addEuler.getMaximumSize().height));
        addEuler.setAlignmentX(Component.CENTER_ALIGNMENT);
        removeEuler = new JButton("Remove");
        removeEuler.setMaximumSize(new Dimension(Short.MAX_VALUE,
            removeEuler.getMaximumSize().height));
        removeEuler.setAlignmentX(Component.CENTER_ALIGNMENT);

        JPanel eulerPanel = new JPanel();
        eulerPanel.setLayout(new BorderLayout());
        eulerPanel.add(useEuler, BorderLayout.NORTH);
        eulerPanel.add(new JScrollPane(allowEuler), BorderLayout.CENTER);
        Box eulerActionPanel = Box.createVerticalBox();
        eulerActionPanel.add(Box.createVerticalGlue());
        eulerActionPanel.add(addEuler);
        eulerActionPanel.add(newEuler);
        eulerActionPanel.add(Box.createVerticalStrut(10));
        eulerActionPanel.add(removeEuler);
        eulerActionPanel.add(Box.createVerticalGlue());
        eulerPanel.add(eulerActionPanel, BorderLayout.EAST);

        JPanel all = new JPanel();
        all.setLayout(new BorderLayout());
        all.add(optionsPanel, BorderLayout.NORTH);
        all.add(eulerPanel, BorderLayout.CENTER);
        add(new PaddedPane(all, 0, false), BorderLayout.CENTER);

        reflectPacket();
        
        if (editor) {
            // Add event listeners.
            ActionListener al = new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    setChanged(true);
                }
            };
            orientability.addActionListener(al);
            compactness.addActionListener(al);
            realBoundary.addActionListener(al);

            useEuler.addItemListener(new ItemListener() {
                public void itemStateChanged(ItemEvent e) {
                    boolean sel = useEuler.isSelected();
                    allowEuler.setEnabled(sel);
                    newEuler.setEnabled(sel);
                    addEuler.setEnabled(sel);
                    removeEuler.setEnabled(sel);
                    setChanged(true);
                }
            });

            addEuler.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    long ec;
                    try {
                        ec = ((IntegerDocument)newEuler.getDocument()).
                            getLongValue();
                    } catch (EmptyDocumentException exc) {
                        shell.error(
                            "No Euler characteristic is provided to add.");
                        return;
                    }
                    if (ec > 2) {
                        shell.error(
                            "No Euler characteristic will ever be larger " +
                            "than 2.");
                        return;
                    }
                    addECToList(BigInteger.valueOf(ec));
                }
            });

            removeEuler.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    int index = allowEuler.getSelectedIndex();
                    if (index >= 0) {
                        ((DefaultListModel)allowEuler.getModel()).
                            removeElementAt(index);
                        setChanged(true);
                    } else
                        shell.error(
                            "No Euler characteristic is selected to remove.");
                }
            });
        }
    }

    /**
     * Adds the given Euler characteristic to the list of those allowed.
     * The list will be kept sorted with unique elements.
     *
     * @param ec the Euler characteristic to add.
     */
    private void addECToList(BigInteger ec) {
        DefaultListModel m = (DefaultListModel)allowEuler.getModel();
        int size = m.size();
        if (size == 0) {
            m.addElement(ec);
            setChanged(true);
            return;
        }
        for (int i = 0; i < size; i++) {
            int cmp = ec.compareTo((BigInteger)m.elementAt(i));
            if (cmp == 0)
                return;
            else if (cmp > 0) {
                m.add(i, ec);
                setChanged(true);
                return;
            }
        }
        setChanged(true);
        m.add(size, ec);
    }

    public void reflectPacket() {
        boolean ed = isEditor();
        orientability.setEnabled(ed);
        compactness.setEnabled(ed);
        realBoundary.setEnabled(ed);
        useEuler.setEnabled(ed);

        ignoreSetChanged(true);
        orientability.setSelectedIndex(whichIndex(packet.getOrientability()));
        compactness.setSelectedIndex(whichIndex(packet.getCompactness()));
        realBoundary.setSelectedIndex(whichIndex(packet.getRealBoundary()));

        long nec = packet.getNumberOfECs();
        if (nec == 0) {
            useEuler.setSelected(false);
            allowEuler.setEnabled(false);
            newEuler.setEnabled(false);
            addEuler.setEnabled(false);
            removeEuler.setEnabled(false);
        } else {
            useEuler.setSelected(true);
            allowEuler.setEnabled(true);
            newEuler.setEnabled(ed);
            addEuler.setEnabled(ed);
            removeEuler.setEnabled(ed);

            DefaultListModel m = (DefaultListModel)allowEuler.getModel();
            m.removeAllElements();
            for (long i = nec - 1; i >= 0; i--)
                m.addElement(packet.getEC(i));
        }
        ignoreSetChanged(false);
        setChanged(false);
    }
    
    public void applyChanges(Engine engine) {
        if (useEuler.isSelected() &&
                ((DefaultListModel)allowEuler.getModel()).size() == 0) {
            useEuler.setSelected(false);
        }
        packet.setOrientability(whichBoolSet(orientability));
        packet.setCompactness(whichBoolSet(compactness));
        packet.setRealBoundary(whichBoolSet(realBoundary));

        packet.removeAllECs();
        if (useEuler.isSelected()) {
            Enumeration e =
                ((DefaultListModel)allowEuler.getModel()).elements();
            while (e.hasMoreElements())
                packet.addEC((BigInteger)e.nextElement());
        }
        setChanged(false);
    }

    public void editingElsewhere() {
        orientability.setEnabled(false);
        compactness.setEnabled(false);
        realBoundary.setEnabled(false);

        useEuler.setEnabled(false);
        allowEuler.setEnabled(false);
        newEuler.setEnabled(false);
        addEuler.setEnabled(false);
        removeEuler.setEnabled(false);
    }

    /**
     * Determines which boolean set has been selected in the given combo
     * box.
     *
     * @param comboBox the given combo box.
     * @return the selected boolean set.
     */
    private NBoolSet whichBoolSet(JComboBox comboBox) {
        switch (comboBox.getSelectedIndex()) {
            case 0 : return NBoolSet.sBoth;
            case 1 : return NBoolSet.sTrue;
            case 2 : return NBoolSet.sFalse;
        }
        return NBoolSet.sNone;
    }

    /**
     * Determines which combo box index corresponds to the given boolean
     * set.  If the empty set is passed, an index of -1 will be returned.
     *
     * @param boolSet the given boolean set.
     * @return the corresponding combo box index.
     */
    private int whichIndex(NBoolSet boolSet) {
        if (boolSet.sameSet(NBoolSet.sBoth))
            return 0;
        if (boolSet.hasTrue())
            return 1;
        if (boolSet.hasFalse())
            return 2;
        return -1;
    }
}
