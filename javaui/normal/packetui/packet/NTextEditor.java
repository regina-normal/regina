
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
import javax.swing.event.*;
import normal.engine.*;
import normal.engine.packet.*;
import normal.packetui.*;

/**
 * An interface through which a user can view or edit a text packet.
 * See <tt>PacketUI</tt> for more details.
 *
 * @see normal.packetui.PacketUI
 */
public class NTextEditor extends DefaultPacketEditor {
    /**
     * The packet associated with this interface.
     * @serial
     */
    private NText packet;
    
    /**
     * Text region containing the packet's data.
     * @serial
     */
    private JTextArea text = new JTextArea();

    /**
     * Create a new interface to edit a given packet.
     *
     * @param packet the packet to be associated with this interface.
     * @param editor <tt>true</tt> if this interface is to be used for
     * editing, or <tt>false</tt> if it is for viewing only.
     */
    public NTextEditor(NPacket packet, boolean editor) {
        super(editor);
        this.packet = (NText)packet;
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
        text.setLineWrap(true);
        text.setWrapStyleWord(true);
        text.setEditable(editor);

        reflectPacket();
        
        // Insert the components.
        add(new JScrollPane(text), BorderLayout.CENTER);
        
        if (editor) {
            // Add event listeners.
            text.getDocument().addDocumentListener(new DocumentListener() {
                public void changedUpdate(DocumentEvent e) {
                    setChanged(true);
                }
                public void insertUpdate(DocumentEvent e) {
                    setChanged(true);
                }
                public void removeUpdate(DocumentEvent e) {
                    setChanged(true);
                }
            });
        }
    }

    public void reflectPacket() {
        ignoreSetChanged(true);
        text.setText(packet.getText());
        ignoreSetChanged(false);
        setChanged(false);
    }
    
    public void applyChanges(Engine engine) {
        packet.setText(text.getText());
        setChanged(false);
    }
}
