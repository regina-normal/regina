
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

package normal.packetui;

import java.awt.*;
import javax.swing.*;
import normal.engine.packet.NPacket;

/**
 * An interface through which a user can view a packet of unknown type.
 * See <tt>PacketUI</tt> for more details.
 *
 * @see normal.packetui.PacketUI
 */
public class UnknownPacketViewer extends DefaultPacketViewer {
    /**
     * The packet associated with this interface.
     */
    private NPacket packet;
    
    /**
     * The label contained in this panel.
     */
    private JLabel message;

    /**
     * Create a new interface to work with a given packet.
     *
     * @param packet the packet to be associated with this interface.
     */
    public UnknownPacketViewer(NPacket packet) {
        super();
        this.packet = packet;

        message = new JLabel();
        message.setHorizontalAlignment(message.CENTER);

        setLayout(new BorderLayout());
        add(message, BorderLayout.CENTER);

        reflectPacket();
    }

    public NPacket getPacket() {
        return packet;
    }

    public void reflectPacket() {
        message.setText(packet.toString());
    }
}
