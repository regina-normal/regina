
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

package normal.packetui.angle;

import normal.Shell;
import normal.engine.packet.*;
import normal.engine.angle.*;
import normal.packetui.*;
import java.awt.*;
import javax.swing.*;
import org.gjt.btools.gui.component.*;

/**
 * An interface through which a user can view an angle structure list.
 * See <tt>PacketUI</tt> for more details.
 *
 * @see normal.packetui.PacketUI
 */
public class NAngleStructureListViewer extends DefaultPacketViewer {
    /**
     * Displays the header.
     */
    private JLabel header;

    /**
     * Displays the individual angle structures.
     */
    private AngleViewer angleViewer;

    /**
     * The angle structure list with which we are working.
     */
    private NAngleStructureList angles;

    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * Create a new interface to work with a given packet.
     *
     * @param packet the packet to be associated with this interface.
     * @param shell the shell representing the entire program.
     */
    public NAngleStructureListViewer(NPacket packet, Shell shell) {
        super();
        this.angles = (NAngleStructureList)packet;
        this.shell = shell;

        init();
        reflectPacket();
    }

    /**
     * Create all interface elements.
     */
    public void init() {
        setLayout(new BorderLayout());

        // Initialise the components.
        header = new JLabel("", JLabel.CENTER);
        angleViewer = new AngleViewer(angles);

        // Insert the components.
        add(new PaddedPane(header, 5), BorderLayout.NORTH);
        add(angleViewer, BorderLayout.CENTER);
    }

    public NPacket getPacket() {
        return angles;
    }

    public void reflectPacket() {
        header.setText(getHeader());
        angleViewer.reflectPacket();
    }

    /**
     * Returns an appropriate header string for the angle structure table.
     * The header should indicate the number of angle structures
     * presented.
     *
     * @return an appropriate header for the table.
     */
    public String getHeader() {
        long n = angles.getNumberOfStructures();
        if (n == 0)
            return "No vertex angle structures";
        if (n == 1)
            return "1 vertex angle structure";
        return String.valueOf(n) + " vertex angle structures";
    }
}
