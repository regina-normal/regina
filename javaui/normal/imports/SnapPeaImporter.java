
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

package normal.imports;

import java.io.File;
import normal.Shell;
import normal.engine.packet.NPacket;

/**
 * An interface through which the user can import a SnapPea
 * triangulation.  See <tt>Importer</tt> for further details.
 *
 * @see normal.imports.Importer
 */
public class SnapPeaImporter extends Importer {
    /**
     * Creates a new importer.
     *
     * @param shell the shell representing the entire program.
     */
    public SnapPeaImporter(Shell shell) {
        super(shell, null);
    }
     
    public String getDialogTitle() {
        return "Import SnapPea Triangulation";
    }

    protected NPacket importData(Shell shell, File dataFile) {
        NPacket packet =
            shell.getEngine().readSnapPea(dataFile.getAbsolutePath());
        if (packet == null) {
            shell.error("An error occurred whilst attempting to import from "
                + dataFile.getAbsolutePath() + ".");
            return null;
        }
        return packet;
    }
}
 
