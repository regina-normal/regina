
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
     * Displays various properties of the angle structure list.
     */
    private JLabel properties;

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
        properties = new JLabel("", JLabel.CENTER);
        angleViewer = new AngleViewer(angles);

        JPanel headerPane = new JPanel();
        headerPane.setLayout(new GridLayout(0, 1));
        headerPane.add(header);
        headerPane.add(properties);

        // Insert the components.
        add(new PaddedPane(headerPane, 5), BorderLayout.NORTH);
        add(angleViewer, BorderLayout.CENTER);
    }

    public NPacket getPacket() {
        return angles;
    }

    public void reflectPacket() {
        header.setText(getHeader());
        properties.setText(getListProperties());
        angleViewer.reflectPacket();
    }

    /**
     * Returns an appropriate header string for the angle structure table.
     * The header should indicate the number of angle structures
     * presented.
     *
     * @return an appropriate header for the table.
     */
    private String getHeader() {
        long n = angles.getNumberOfStructures();
        if (n == 0)
            return "No vertex angle structures";
        if (n == 1)
            return "1 vertex angle structure";
        return String.valueOf(n) + " vertex angle structures";
    }

    /**
     * Returns a human-readable string describing various properties
     * of the angle structure list.
     *
     * @return a string describing various list properties.
     */
    private String getListProperties() {
        StringBuffer ans = new StringBuffer("Span includes: ");

        if (! angles.allowsStrict())
            ans.append("NO ");
        ans.append("Strict, ");

        if (! angles.allowsTaut())
            ans.append("NO ");
        ans.append("Taut");

        return ans.toString();
    }
}
