
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

package normal.imports;

import java.awt.*;
import java.io.*;
import javax.swing.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.options.NormalOptionSet;
import normal.packetfilter.*;
import normal.packetui.*;

/**
 * An interface through which the user can import a SnapPea
 * triangulation.
 * See <tt>PacketCreator</tt> for further details.
 *
 * @see normal.packetui.PacketCreator
 */
public class SnapPeaImporter implements PacketCreator {
    /**
     * The pane that makes up the interface.
     */
    private ImportFilePane pane;

    /**
     * Creates a new importer.
     *
     * @param shell the shell representing the entire program.
     */
    public SnapPeaImporter(Shell shell) {
        pane = new ImportFilePane(shell, null);
    }
     
    public Component getCreator() {
        return pane;
    }

    public String getDialogTitle() {
        return "Import SnapPea Triangulation";
    }

    public PacketFilter getPacketFilter() {
        return AllowAll.instance;
    }

    public String getSuggestedPacketLabel() {
        return "SnapPea Triangulation";
    }

    public NPacket makeNewPacket(Shell shell, NPacket parent,
            Dialog parentDialog) {
        String fileName = pane.getFileName().trim();
        if (fileName.length() == 0) {
            shell.error("No file has been selected.");
            return null;
        }
        File file = new File(fileName);
        try {
            if (! file.exists()) {
                shell.error("The file " + file.getAbsolutePath()
                    + " does not exist.");
                return null;
            }
        } catch (SecurityException e) {}
        NPacket packet =
            shell.getEngine().readSnapPea(file.getAbsolutePath());
        if (packet == null) {
            shell.error("An error occurred when attempting to import from "
                + file.getAbsolutePath() + ".");
            return null;
        }
        NormalOptionSet options = shell.getOptions();
        options.setStringOption("LastDir", file.getParent());
        options.writeToFile();
        return packet;
    }
}
 
