
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

package normal.packetui.packet;

import java.awt.*;
import javax.swing.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.packetfilter.*;
import normal.packetui.*;

/**
 * An interface through which a user can create a new text packet.
 * See <tt>PacketCreator</tt> for more details.
 *
 * @see normal.packetui.PacketCreator
 */
public class NTextCreator extends JPanel implements PacketCreator {
    /**
     * The text to store in the new packet.
     * @serial
     */
    private JTextArea text = new JTextArea();

    /**
     * Create the new interface.
     */
    public NTextCreator() {
        init();
    }

    /**
     * Create the interface's internal components.
     */
    private void init() {
        // Set the layout manager.
        setLayout(new BorderLayout());

        // Initialise the components.
        text.setLineWrap(true);
        text.setWrapStyleWord(true);
        text.setColumns(30);
        text.setRows(5);
        
        // Place the text field in a scrolling pane.
        JScrollPane scroller = new JScrollPane(text);
//        scroller.setBorder(BorderFactory.createEtchedBorder());

        // Insert the components.
        add(scroller, BorderLayout.CENTER);
        add(new JLabel("New text:"), BorderLayout.NORTH);
    }

    public NPacket makeNewPacket(Shell shell, NPacket parent,
            Dialog parentDialog) {
        return shell.getEngine().newNText(text.getText());
    }

    public Component getCreator() {
        return this;
    }

    public String getDialogTitle() {
        return "New Text";
    }

    public String getSuggestedPacketLabel() {
        return "New Text";
    }

    public PacketFilter getPacketFilter() {
        return AllowAll.instance;
    }
}
