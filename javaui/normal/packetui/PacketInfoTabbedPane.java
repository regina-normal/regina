
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
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

import java.awt.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.JTextComponent;
import normal.engine.Engine;
import normal.engine.packet.NPacket;
import org.gjt.btools.gui.dialog.MessageBox;

/**
 * A tabbed pane consisting entirely of <tt>PacketInfoUI</tt> tabs.
 * Only components implementing the <tt>PacketInfoUI</tt> interface
 * should be added to this tabbed pane.
 * This interface is capable of both viewing and editing packets.
 * <p>
 * The interface reported to the rest of the world as responsible for
 * any event, packet change or the like will always be this interface
 * and not one of its sub-interfaces.
 * <p>
 * If any additional behaviour is required of this main interface when
 * editing begins or ends in a sub-interface, the routine
 * <tt>update()</tt> should be overwritten.
 * However, <tt>super.update()</tt> should always be called to ensure
 * the notification is properly passed to all other sub-interfaces.
 *
 * @see normal.packetui.PacketInfoUI
 */
public class PacketInfoTabbedPane extends JTabbedPane
        implements ChangeListener, PacketUI, UIHasChangesListener {
    /**
     * The packet associated with this interface.
     */
    private NPacket packet;

    /**
     * Is this interface allowed to edit packets?
     */
    private boolean editor;

    /**
     * The list of listeners that will receive
     * <tt>UIHasChangesEvent</tt> events fired by this interface.
     */
    private Vector UIHasChangesListeners = new Vector();

    /**
     * A list of the tab components and the UIs to which they
     * correspond.
     */
    private HashMap uis = new HashMap();

    /**
     * The sub-interface that is currently editing the packet, or
     * <tt>null</tt> if no sub-interface is editing the packet.
     */
    private PacketInfoUI currentEditor;

    /**
     * Creates a new tabbed pane with the tabs at the top.
     *
     * @param packet the packet associated with this interface.
     * @param editor <tt>true</tt> if this interface is to be used for
     * editing, or <tt>false</tt> if it is for viewing only.
     */
    public PacketInfoTabbedPane(NPacket packet, boolean editor) {
        super(TOP);
        this.packet = packet;
        this.editor = editor;
        this.currentEditor = null;
        addChangeListener(this);
    }

    /**
     * Adds a new sub-interface to the tabbed pane.
     * This should be the <b>only</b> way in which any new tab is added.
     * None of the standard <tt>JTabbedPane.add()</tt> or
     * <tt>JTabbedPane.addTab()</tt> routines should be used.
     * <p>
     * <b>Precondition:</b> If this tabbed pane was created as a viewer
     * only, then any sub-interface added should also be a viewer only.
     *
     * @param ui the sub-interface to add.  The component actually inserted
     * into the tabbed pane will be that returned by
     * <tt>ui.getInterface()</tt>.
     * @param title the text to appear on the new tab.
     */
    public void addUI(PacketInfoUI ui, String title) {
        Component c = ui.getInterface();
        addTab(title, c);
        uis.put(c, new UIDetails(ui));
        if (editor)
            ui.addUIHasChangesListener(this);
    }

    /**
     * Reports that the sub-interfaces are to be updated.
     * Each time a new update is announced via this routine, all past
     * such announcements will be forgotten.
     * <p>
     * This routine should be called whenever the underlying packet
     * changes or editing begins or finishes.  The individual sub-interfaces
     * will only be updated when they become visible or if they are
     * already visible.
     * <p>
     * If any extra behaviour is required of this main interface when
     * editing begins or ends, this routine should be overwritten.
     * However, <tt>super.update()</tt> should always be called to
     * ensure the notification is properly passed to all sub-interfaces.
     *
     * @param isEditing <tt>true</tt> if editing on the packet has begun
     * and all other sub-interfaces should declare all information unknown,
     * or <tt>false</tt> if all sub-interfaces should update themselves
     * according to the contents of the underlying packet in the engine.
     * @param source the sub-interface responsible for the update;
     * this particular sub-interface will <i>not</i> be updated, since it is
     * the source of the change.  If <tt>isEditing</tt> is
     * <tt>true</tt>, this parameter <b>must</b> be the sub-interface
     * that has begun editing.  If <tt>isEditing</tt> is <tt>false</tt>,
     * this parameter may be <tt>null</tt> if desired.
     */
    public void update(boolean isEditing, PacketInfoUI source) {
        if (isEditing)
            currentEditor = source;
        else
            currentEditor = null;

        Iterator it = uis.values().iterator();
        UIDetails d;
        while (it.hasNext()) {
            d = ((UIDetails)it.next());
            d.updated = (d.ui == source);
        }
        stateChanged(null);
    }

    /**
     * Called when the tab selection changes.
     * The currently selected tab will be updated if required.
     *
     * @param e the event corresponding to the tab selection change.
     */
    public void stateChanged(ChangeEvent e) {
        UIDetails d = (UIDetails)uis.get(getSelectedComponent());
        if (d != null)
            if (! d.updated) {
                if (currentEditor == null)
                    d.ui.reflectPacket();
                else if (d.ui != currentEditor)
                    d.ui.editingElsewhere();
                d.updated = true;
            }
    }

    public void addUIHasChangesListener(UIHasChangesListener l) {
        UIHasChangesListeners.add(l);
    }

    public void removeUIHasChangesListener(UIHasChangesListener l) {
        UIHasChangesListeners.remove(l);
    }

    /**
     * Sends a <tt>UIHasChangesEvent</tt> event to all listeners.
     */
    private void fireUIHasChangesEvent() {
        UIHasChangesEvent event = new UIHasChangesEvent(this);
        Enumeration e = UIHasChangesListeners.elements();
        while (e.hasMoreElements())
            ((UIHasChangesListener)e.nextElement()).UIStatusChanged(event);
    }

    /**
     * Handles a <tt>UIStatusChangesEvent</tt> event.
     * If the event was generated by a sub-interface of this interface,
     * all sub-interfaces are informed and a new event is fired to the
     * listeners of this interface with this interface as the source.
     *
     * @param event the event to be handled.
     */
    public void UIStatusChanged(UIHasChangesEvent event) {
        PacketUI source = (PacketUI)event.getSource();
        if (uis.containsKey(source.getInterface())) {
            // The event source is a sub-interface.
            if (currentEditor == null) {
                // This interface currently thinks it's not editing
                // anything.
                if (event.hasChanges()) {
                    update(true, (PacketInfoUI)source);
                    fireUIHasChangesEvent();
                }
            } else {
                // This interface is already being edited.
                update(event.hasChanges(), (PacketInfoUI)source);
                fireUIHasChangesEvent();
            }
        }
    }

    public NPacket getPacket() {
        return packet;
    }

    public Component getInterface() {
        return this;
    }

    public JTextComponent getPrimaryTextComponent() {
        return null;
    }

    public void reflectPacket() {
        update(false, null);
    }
    
    public boolean isEditor() {
        return editor;
    }

    public boolean hasChanges() {
        if (currentEditor == null)
            return false;
        else
            return currentEditor.hasChanges();
    }

    public void applyChanges(Engine engine) {
        if (currentEditor != null)
            currentEditor.applyChanges(engine);
    }

    public void packetWasChanged(NPacket packet, PacketUI ui, Frame owner) {
        // If this interface or a sub-interface is responsible for the
        // change, we will assume everyone already knows all about it.
        if (ui == this)
            return;

        // If it's the same packet, either update everything or stick
        // our head in the sand.
        if (packet.sameObject(this.packet)) {
            if (hasChanges())
                if (! MessageBox.confirm(owner,
                        "Do you wish to replace your unapplied working " +
                        "copy of packet " + packet.getPacketLabel() +
                        " with the most up to date confirmed copy?"))
                    return;
            reflectPacket();
            return;
        }

        // It's a different packet and a different interface.
        // Let the sub-interfaces deal with the change individually.
        int n = getTabCount();
        for (int i=0; i<n; i++)
            ((UIDetails)uis.get(getComponent(i))).ui.
                packetWasChanged(packet, ui, owner);
    }

    public void packetWasRenamed(NPacket packet, PacketUI ui, Frame owner) {
        int n = getTabCount();
        for (int i=0; i<n; i++)
            ((UIDetails)uis.get(getComponent(i))).ui.
                packetWasRenamed(packet, ui, owner);
    }

    public void subtreeToBeDeleted(NPacket subtree, PacketUI ui, Frame owner) {
        int n = getTabCount();
        for (int i=0; i<n; i++)
            ((UIDetails)uis.get(getComponent(i))).ui.
                subtreeToBeDeleted(subtree, ui, owner);
    }

    public void subtreeWasDeleted(NPacket parent, PacketUI ui, Frame owner) {
        int n = getTabCount();
        for (int i=0; i<n; i++)
            ((UIDetails)uis.get(getComponent(i))).ui.
                subtreeWasDeleted(parent, ui, owner);
    }

    public void subtreeWasInserted(NPacket subtree, PacketUI ui, Frame owner) {
        int n = getTabCount();
        for (int i=0; i<n; i++)
            ((UIDetails)uis.get(getComponent(i))).ui.
                subtreeWasInserted(subtree, ui, owner);
    }

    public void subtreeHasChanged(NPacket subtree, PacketUI ui, Frame owner) {
        int n = getTabCount();
        for (int i=0; i<n; i++)
            ((UIDetails)uis.get(getComponent(i))).ui.
                subtreeHasChanged(subtree, ui, owner);
    }

    public void cleanUp() {
        int n = getTabCount();
        for (int i=0; i<n; i++)
            ((UIDetails)uis.get(getComponent(i))).ui.cleanUp();
    }

    /**
     * Stores an interface and whether it has been updated.
     */
    private class UIDetails {
        /**
         * The interface stored.
         */
        public PacketInfoUI ui;

        /**
         * Whether the interface has been updated.
         * This is <tt>false</tt> if and only if the interface needs to
         * be updated before it is used.
         */
        public boolean updated;

        /**
         * Creates a new <tt>UIDetails</tt> containing the given
         * interface.  The <tt>updated</tt> field will be set to
         * <tt>false</tt>.
         *
         * @param ui the interface to store.
         */
        public UIDetails(PacketInfoUI ui) {
            this.ui = ui;
            this.updated = false;
        }
    }
}

