
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
import normal.engine.packet.NPacket;
import org.gjt.btools.utilities.NullObject;

/**
 * A renderer for packets that can be used in lists.
 * Each packet will have a cute icon placed next to it according to the
 * packet type.
 * A <tt>null</tt> packet or a <tt>NullObject</tt> (from package
 * <tt>btools.utilities</tt>) will be rendered by
 * displaying the message <tt>&lt;None&gt;</tt>.
 *
 * <b>Precondition:</b> Only objects derived from <tt>NPacket</tt> are
 * rendered by this renderer, aside from the exceptions mentioned above.
 */
public class PacketListCellRenderer extends DefaultListCellRenderer {
    /**
     * Creates a new renderer.
     */
    public PacketListCellRenderer() {
    }

    /**
     * Returns the component used for rendering.
     *
     * @param list the list containing the cell to be rendered.
     * @param value the value of the cell to be rendered.
     * @param index the index of the cell to be rendered.
     * @param isSelected whether or not the cell in question is selected.
     * @param cellHasFocus whether or not the cell in question has the
     * focus.
     * @return the component used for rendering.
     */
    public Component getListCellRendererComponent(JList list,
            Object value, int index, boolean isSelected,
            boolean cellHasFocus) {
        if (value == null || value instanceof NullObject)
            return super.getListCellRendererComponent(list,
                "<None>", index, isSelected, cellHasFocus);

        Component ans = super.getListCellRendererComponent(list,
            ((NPacket)value).getPacketLabel(),
            index, isSelected, cellHasFocus);
        ((JLabel)ans).setIcon(PacketUIManager.packetIcon(
            (NPacket)value));
        return ans;
    }
}
