
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

package normal.packetui;

import java.awt.*;
import javax.swing.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.mainui.TopologyPane;
import normal.packetui.*;
import org.gjt.btools.gui.component.*;

/**
 * A packet viewer or editor containing a header and a tabbed pane of
 * sub-interface components for specific packet details.
 * The header is a label that will be displayed above the tabbed pane
 * and should contain general information regarding the packet in question.
 * Its value will be determined by calling the abstract method
 * <tt>getHeader()</tt>.
 * <p>
 * The sub-interfaces to be contained in the tabbed pane will be inserted
 * during a call to the abstract method <tt>fillWithInterfaces()</tt>.
 * Other than implementing this method, no further component
 * initialisation needs to be done by subclasses of this viewer/editor.
 * <p>
 * Note that in this case the interface is <b>not</b> this object
 * itself, but a panel containing both this object (which is a tabbed
 * pane) and the header.
 *
 * @see #getHeader
 * @see #fillWithInterfaces
 * @see normal.packetui.PacketUI
 * @see normal.packetui.PacketInfoUI
 */
public abstract class PacketTabbedEditor extends PacketInfoTabbedPane {
    /**
     * Displays the header.
     */
    private JLabel header;

    /**
     * The panel that forms the entire interface.
     */
    private JPanel panel;

    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * Create a new interface to work with a given packet.
     *
     * @param packet the packet to be associated with this interface.
     * @param shell the shell representing the entire program.
     * @param topPane the topology pane responsible for this interface.
     * @param editor <tt>true</tt> if this interface is to be used for
     * editing, or <tt>false</tt> if it is for viewing only.
     */
    public PacketTabbedEditor(NPacket packet, Shell shell,
            TopologyPane topPane, boolean editor) {
        super(packet, topPane, editor);
        this.shell = shell;
        init();
    }

    /**
     * Create all interface elements.
     */
    private void init() {
        panel = new JPanel();
        panel.setLayout(new BorderLayout());
        
        // Initialise the components.
        header = new JLabel("", JLabel.CENTER);
        fillWithInterfaces();
        update(false, null);
            
        // Insert the components.
        panel.add(new PaddedPane(header, 5), BorderLayout.NORTH);
        panel.add(this, BorderLayout.CENTER);
    }

    public Component getInterface() {
        return panel;
    }

    /**
     * Returns the shell representing the entire program.
     *
     * @return the shell representing the entire program.
     */
    public Shell getShell() {
        return shell;
    }

    /**
     * Determine a suitable header for this viewer based upon the
     * information stored in the underlying packet.  The header should
     * contain general information about the packet in question.
     * This routine will be used
     * to update the header label each time the packet is updated.
     * Note that the underlying packet can be accessed through the
     * protected data member <tt>packet</tt>.
     *
     * @return a suitable header for this viewer.
     */
    public abstract String getHeader();

    /**
     * Fills the tabbed pane with interfaces.
     * This routine should consist only of calls to
     * <tt>addUI()</tt> and will be used to fill the tabbed pane
     * during the initialisation of this viewer.
     * <p>
     * <b>Warning:</b> This routine will be called from the
     * <tt>PacketTabbedEditor</tt> constructor, so subclass data members
     * may not have been initialised.
     *
     * @see normal.packetui.PacketInfoTabbedPane#addUI
     */
    public abstract void fillWithInterfaces();

    public void update(boolean isEditing, PacketInfoUI source) {
        super.update(isEditing, source);
        if (isEditing)
            header.setText("Editing...");
        else
            header.setText(getHeader());
    }
}

