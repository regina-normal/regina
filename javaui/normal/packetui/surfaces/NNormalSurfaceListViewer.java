
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

package normal.packetui.surfaces;

import normal.Shell;
import normal.engine.packet.*;
import normal.engine.surfaces.*;
import normal.mainui.TopologyPane;
import normal.packetui.*;

/**
 * An interface through which a user can view a normal surface list.
 * See <tt>PacketUI</tt> for more details.
 *
 * @see normal.packetui.PacketUI
 */
public class NNormalSurfaceListViewer extends PacketTabbedEditor {
    /**
     * Create a new interface to work with a given packet.
     *
     * @param packet the packet to be associated with this interface.
     * @param shell the shell representing the entire program.
     * @param topPane the topology pane responsible for this interface.
     */
    public NNormalSurfaceListViewer(NPacket packet, Shell shell,
            TopologyPane topPane) {
        super(packet, shell, topPane, false);
    }

    public String getHeader() {
        String type;
        if (((NNormalSurfaceList)getPacket()).isEmbeddedOnly())
            type = "embedded";
        else
            type = "embedded/immersed/singular";
        long n = ((NNormalSurfaceList)getPacket()).getNumberOfSurfaces();
        if (n == 0)
            return "No " + type + " normal surfaces";
        if (n == 1)
            return "1 " + type + " normal surface";
        return String.valueOf(n) + ' ' + type + " normal surfaces";
    }

    public void fillWithInterfaces() {
        NNormalSurfaceList list = (NNormalSurfaceList)getPacket();
        addUI(new CoordinateViewer(list, list, getShell(), getTopologyPane()),
            "Surface Coordinates");
        addUI(new MatchingEquationsViewer(list), "Matching Equations");
    }
}
