
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

import java.awt.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.JTextComponent;
import normal.engine.Engine;
import normal.engine.packet.NPacket;
import org.gjt.btools.gui.dialog.*;

/**
 * A skeleton <tt>PacketUI</tt> that is capable of both viewing and
 * editing packets.  The decision as to whether packets are allowed to
 * be edited must be made when the constructor is called.
 * <p>
 * This interface will be an empty panel.  Note that some routines from
 * the <tt>PacketUI</tt> interface are not implemented.
 *
 * @see normal.packetui.PacketUI
 */
public abstract class DefaultPacketEditor extends JPanel implements PacketUI {
    /**
     * Is this interface allowed to edit packets?
     */
    private boolean editor;

    /**
     * Have there been changes to the interface that have not yet been
     * applied to the underlying packet in the engine?
     */
    private boolean hasChanged;

    /**
     * Should we ignore all calls to <tt>setChanged()</tt>?
     */
    private boolean shouldIgnoreSetChanged;

    /**
     * The list of listeners that will receive
     * <tt>UIHasChangesEvent</tt> events fired by this interface.
     */
    private Vector UIHasChangesListeners = new Vector();

    /**
     * Creates a new empty panel.
     *
     * @param editor <tt>true</tt> if this interface is to be used for
     * editing, or <tt>false</tt> if it is for viewing only.
     */
    public DefaultPacketEditor(boolean editor) {
        super();
        this.editor = editor;
        this.hasChanged = false;
        this.shouldIgnoreSetChanged = false;
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

    public abstract NPacket getPacket();

    public Component getInterface() {
        return this;
    }

	public JTextComponent getPrimaryTextComponent() {
		return null;
	}

    public abstract void reflectPacket();

    public boolean isEditor() {
        return editor;
    }

    /**
     * Sets whether there have been changes to the interface that have
     * not yet been applied to the underlying packet in the engine.
     * The internal flag that stores this value is set to <tt>false</tt>
     * when this interface is initially created.
     * <p>
     * This routine should be called whenever this status changes;
     * this will handle any necessary consequences such as notifying
     * other components that the status has changed.  Note in particular
     * that it will probably need to be called from both
     * <tt>reflectPacket()</tt> and <tt>applyChanges()</tt>.
     * <p>
     * If <tt>ignoreSetChanged(true)</tt> is called, subsequent calls to
     * this routine will do nothing and will be ignored completely
     * until <tt>ignoreSetChanged(false)</tt> is called.  This feature
     * is useful when a component is being updated and various event
     * listeners will call <tt>setChanged()</tt> when you don't want
     * them to.  By default calls to this routine will be attended to.
     *
     * @param hasChanged <tt>true</tt> if there are now unapplied
     * changes or <tt>false</tt> otherwise.
     * @see #ignoreSetChanged
     */
    public void setChanged(boolean hasChanged) {
        if (shouldIgnoreSetChanged)
            return;
        if ((this.hasChanged && hasChanged) ||
                (! (this.hasChanged || hasChanged)))
            return;

        // The status has actually changed and we are supposed to care.
        this.hasChanged = hasChanged;
        fireUIHasChangesEvent();
    }

    public boolean hasChanges() {
        return hasChanged;
    }

    /**
     * Sets whether to ignore all calls to <tt>setChanged()</tt>
     * By default such calls will be attended to.
     *
     * @param shouldIgnore <tt>true</tt> if calls to
     * <tt>setChanged()</tt> should be ignored or <tt>false</tt> if they
     * should be attended to.
     * @see #setChanged
     * @see #isIgnoringSetChanged
     */
    public void ignoreSetChanged(boolean shouldIgnore) {
        shouldIgnoreSetChanged = shouldIgnore;
    }

    /**
     * Determines if all calls to <tt>setChanged()</tt> are being
     * ignored.
     *
     * @return <tt>true</tt> if and only if such calls are being
     * ignored.
     * @see #ignoreSetChanged
     */
    public boolean isIgnoringSetChanged() {
        return shouldIgnoreSetChanged;
    }

    public abstract void applyChanges(Engine engine);

    public void packetWasChanged(NPacket packet, PacketUI ui, Frame owner) {
        if (this != ui)
            if (packet.sameObject(getPacket())) {
                if (hasChanges())
                    if (! MessageBox.confirm(owner,
                            "Do you wish to replace your unapplied working " +
                            "copy of packet " + getPacket().getPacketLabel() +
                            " with the most up to date confirmed copy?"))
                        return;
                reflectPacket();
            }
    }

    public void packetWasRenamed(NPacket packet, PacketUI ui, Frame owner) {
    }

    public void subtreeToBeDeleted(NPacket packet, PacketUI ui, Frame owner) {
    }

    public void subtreeWasDeleted(NPacket packet, PacketUI ui, Frame owner) {
    }

    public void subtreeWasInserted(NPacket subtree, PacketUI ui, Frame owner) {
    }

    public void subtreeHasChanged(NPacket subtree, PacketUI ui, Frame owner) {
    }

    public void cleanUp() {
    }
}

