
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

package normal.packetui.triangulation;

import java.awt.*;
import javax.swing.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.engine.triangulation.*;
import normal.mainui.TopologyPane;
import normal.packetui.*;

/**
 * An interface through which a user can view or edit a triangulation.
 * See <tt>PacketUI</tt> for more details regarding user interfaces.
 *
 * @see normal.packetui.PacketUI
 */
public class NTriangulationEditor extends PacketTabbedEditor {
    /**
     * The sub-interface that views skeleton information.
     */
    private SkeletonViewer skeletonViewer;

    /**
     * Create a new interface to work with a given packet.
     *
     * @param packet the packet to be associated with this interface.
     * @param shell the shell representing the entire program.
     * @param topPane the topology pane responsible for this interface.
     * @param editor <tt>true</tt> if this interface is to be an
     * editor, or <tt>false</tt> if it is to be a viewer.
     */
    public NTriangulationEditor(NPacket packet, Shell shell,
            TopologyPane topPane, boolean editor) {
        super(packet, shell, topPane, editor);
    }

    public String getHeader() {
        NTriangulation packet = (NTriangulation)getPacket();
        if (packet.getNumberOfTetrahedra() == 0)
            return "Empty";
        if (! packet.isValid())
            return "INVALID TRIANGULATION!";
        StringBuffer ans = new StringBuffer();
        if (packet.isClosed())
            ans.append("Closed, ");
        else {
            if (packet.isIdeal())
                ans.append("Ideal, ");
            if (packet.hasBoundaryFaces())
                ans.append("Real Bdry, ");
        }
        ans.append(packet.isOrientable() ? "Orientable, " :
            "Non-orientable, ");
        ans.append(packet.isConnected() ? "Connected" : "Disconnected");
        return ans.toString();
    }

    public void fillWithInterfaces() {
        NTriangulation packet = (NTriangulation)getPacket();

        addUI(new GluingEditor(packet, getShell(), isEditor()), "Gluings");
        skeletonViewer = new SkeletonViewer(packet, getShell());
        addUI(skeletonViewer, "Skeleton");
        addUI(new AlgebraViewer(packet), "Algebra");
        addUI(new CompositionViewer(packet, getShell().getEngine()),
            "Composition");
        addUI(new SurfaceViewer(packet), "Surfaces");
    }

    public void update(boolean isEditing, PacketInfoUI source) {
        skeletonViewer.closeAllFrames();
        super.update(isEditing, source);
    }

    public void cleanUp() {
        skeletonViewer.closeAllFrames();
        super.cleanUp();
    }
}
