
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
import normal.engine.Engine;
import normal.engine.packet.*;
import normal.engine.surfaces.*;
import normal.packetui.*;
import normal.packetfilter.*;
import org.gjt.btools.gui.component.*;

/**
 * An interface through which a user can view or edit a normal surface
 * filter.
 * See <tt>PacketUI</tt> for more details regarding user interfaces.
 *
 * @see normal.packetui.PacketUI
 */
public class NSurfaceFilterEditor extends PacketInfoTabbedPane
        implements ItemListener {
    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * The panel that forms the entire interface.
     */
    private JPanel panel;

    /**
     * The coordinate viewer that presents the filtered list of surfaces.
     */
    private CoordinateViewer coordinates;

    /**
     * The normal surface list to which this filter should be applied.
     */
    private PacketComboBox applyTo;

    /**
     * The list of surfaces that we wish to run this filter through.
     */
    private NSurfaceSet surfaceSource;

    /**
     * The list of surfaces after filtering.
     */
    private NSurfaceSubset filteredSurfaces;
    
    /**
     * Create a new interface to work with a given packet.
     *
     * @param packet the packet to be associated with this interface.
     * @param shell the shell representing the entire program.
     * @param editor <tt>true</tt> if this interface is to be an
     * editor, or <tt>false</tt> if it is to be a viewer.
     */
    public NSurfaceFilterEditor(NPacket packet, Shell shell, boolean editor) {
        super(packet, editor);
        this.shell = shell;
        this.surfaceSource = null;
        this.filteredSurfaces = null;
        init(packet);

        // Set up the list of filtered surfaces.
        applyFilter();
    }

    /**
     * Create all interface elements.
     *
     * @param packet the packet we are editing.
     */
    private void init(NPacket packet) {
        panel = new JPanel();
        panel.setLayout(new BorderLayout());

        // Initialise the apply-to box.
        // Select the parent packet if possible.
        applyTo = new PacketComboBox(packet.getTreeMatriarch(),
            new SingleClass(NNormalSurfaceList.class), true);
        NPacket parent = packet.getTreeParent();
        if (parent != null && parent instanceof NNormalSurfaceList)
            applyTo.setSelectedItem(parent);
        applyTo.addItemListener(this);

        // Initialise other components.
        fillWithInterfaces();
        update(false, null);

        // Insert the components.
        JPanel headerPane = new JPanel();
        headerPane.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
        headerPane.add(new JLabel("Apply to:"));
        headerPane.add(applyTo);
        panel.add(headerPane, BorderLayout.NORTH);
        panel.add(this, BorderLayout.CENTER);
    }

    public Component getInterface() {
        return panel;
    }

    public void fillWithInterfaces() {
        NSurfaceFilter packet = (NSurfaceFilter)getPacket();

        if (packet instanceof NSurfaceFilterProperties)
            addUI(new NSFPropertiesEditor(packet, shell, isEditor()),
                "Details");
        else if (packet instanceof NSurfaceFilterCombination)
            addUI(new NSFCombinationEditor(packet, isEditor()), "Details");
        else
            addUI(new UnknownPacketViewer(packet), "Description");

        coordinates = new CoordinateViewer(null, packet);
        addUI(coordinates, "Surfaces");
    }

    /**
     * For internal use only.
     * Called when the selected item in the apply-to box is changed.
     */
    public void itemStateChanged(ItemEvent e) {
        if (e.getStateChange() == ItemEvent.SELECTED)
            applyFilter();
    }

    /**
     * Refreshes the set of filtered surfaces according to the current
     * apply-to selection.
     */
    private void applyFilter() {
        if (filteredSurfaces != null) {
            filteredSurfaces.destroy();
            filteredSurfaces = null;
            surfaceSource = null;
        }

        NPacket sel = applyTo.getSelectedPacket();
        if (sel != null && sel instanceof NNormalSurfaceList) {
            surfaceSource = (NNormalSurfaceList)sel;
            filteredSurfaces = shell.getEngine().newNSurfaceSubset(
                surfaceSource, (NSurfaceFilter)getPacket());
        }

        coordinates.updateSet(filteredSurfaces);
    }

    public void reflectPacket() {
        applyFilter();
        super.reflectPacket();
    }

    public void applyChanges(Engine engine) {
        super.applyChanges(engine);
        applyFilter();
    }

    public void packetWasChanged(NPacket packet, PacketUI ui, Frame owner) {
        super.packetWasChanged(packet, ui, owner);
        if (surfaceSource != null)
            if (packet.sameObject(surfaceSource.getShareableObject()))
                applyFilter();
    }

    public void packetWasRenamed(NPacket packet, PacketUI ui, Frame owner) {
        super.packetWasRenamed(packet, ui, owner);
        // Remove and reinsert the item to force the combo box to resize
        // itself.
        Object selected = applyTo.getSelectedItem();

        DefaultComboBoxModel m = (DefaultComboBoxModel)applyTo.getModel();
        int index = m.getIndexOf(packet);
        if (index >= 0) {
            m.removeElementAt(index);
            m.insertElementAt(packet, index);
            applyTo.setSelectedItem(selected);
        }
    }

    public void subtreeToBeDeleted(NPacket subtree, PacketUI ui, Frame owner) {
        super.subtreeToBeDeleted(subtree, ui, owner);
        NPacket p = subtree;
        while (p != null && subtree.isGrandparentOf(p)) {
            applyTo.removeItem(p);
            p = p.nextTreePacket();
        }
    }

    public void subtreeWasInserted(NPacket subtree, PacketUI ui, Frame owner) {
        super.subtreeWasInserted(subtree, ui, owner);
        applyTo.updateContents();
    }

    public void subtreeHasChanged(NPacket subtree, PacketUI ui, Frame owner) {
        super.subtreeHasChanged(subtree, ui, owner);
        applyTo.updateContents();
    }

    public void cleanUp() {
        if (filteredSurfaces != null)
            filteredSurfaces.destroy();
        super.cleanUp();
    }
}
