
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

package normal.packetui.packet;

import java.awt.*;
import javax.swing.*;
import normal.Images;
import normal.engine.packet.*;
import normal.packetui.*;
import btools.gui.component.*;

/**
 * An interface through which a user can view a container.
 * See <tt>PacketUI</tt> for more details.
 *
 * @see normal.packetui.PacketUI
 */
public class NContainerViewer extends DefaultPacketViewer {
    /**
     * The packet associated with this interface.
     */
    private NContainer packet;
    
    /**
     * Create a new interface to work with a given packet.
     *
     * @param packet the packet to be associated with this interface.
     */
    public NContainerViewer(NPacket packet) {
        super();
        this.packet = (NContainer)packet;
        setLayout(new BorderLayout());
        JLabel icon = new JLabel(Images.mainLargeIcon.image());
        icon.setBorder(BorderFactory.createLoweredBevelBorder());
        icon.setHorizontalAlignment(icon.CENTER);
        add(new PaddedPane(icon, 0, false), BorderLayout.CENTER);
    }

    public NPacket getPacket() {
        return packet;
    }

    public void reflectPacket() {
    }
}
