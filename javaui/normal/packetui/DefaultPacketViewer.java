
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
import javax.swing.*;
import normal.engine.Engine;
import normal.engine.packet.NPacket;

/**
 * A skeleton <tt>PacketUI</tt> that is only capable of viewing packets.
 * This interface will be an empty panel.  Note that routines
 * <tt>getPacket()</tt> and <tt>reflectPacket()</tt> are not
 * implemented.
 *
 * @see normal.packetui.PacketUI
 */
public abstract class DefaultPacketViewer extends JPanel
        implements PacketUI, PacketInfoUI {
    /**
     * Creates a new empty panel.
     */
    public DefaultPacketViewer() {
        super();
    }
    
    public void addUIHasChangesListener(UIHasChangesListener l) {
    }

    public void removeUIHasChangesListener(UIHasChangesListener l) {
    }

    public abstract NPacket getPacket();

    public Component getInterface() {
        return this;
    }

    public abstract void reflectPacket();

    public boolean isEditor() {
        return false;
    }

    public boolean hasChanges() {
        return false;
    }

    public void applyChanges(Engine engine) {
    }

    public void editingElsewhere() {
    }

    public void packetWasChanged(NPacket packet, PacketUI ui, Frame owner) {
        if (packet == getPacket())
            reflectPacket();
    }

    public void packetWasRenamed(NPacket packet, PacketUI ui, Frame owner) {
    }

    public void subtreeToBeDeleted(NPacket packet, PacketUI ui, Frame owner) {
    }

    public void subtreeWasInserted(NPacket subtree, PacketUI ui, Frame owner) {
    }

    public void subtreeHasChanged(NPacket subtree, PacketUI ui, Frame owner) {
    }

    public void cleanUp() {
    }
}

