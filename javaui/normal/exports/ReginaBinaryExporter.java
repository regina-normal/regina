
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

package normal.exports;

import java.io.File;
import normal.Shell;
import normal.engine.packet.NPacket;
import normal.packetfilter.*;

/**
 * An exporter that exports to an old-style binary Regina data file.
 *
 * @see normal.exports.Exporter
 */
public class ReginaBinaryExporter extends Exporter {
    /**
     * Creates a new exporter.
     *
     * @param shell the shell representing the entire program.
     * @param menuLabel the text to display in the menu item
     * corresponding to this export algorithm.
     * @see normal.algorithm.Algorithm
     */
    public ReginaBinaryExporter(Shell shell, String menuLabel) {
        super(shell, menuLabel, Standalone.instance);
    }

    /**
     * Creates a new exporter.
     *
     * @param shell the shell representing the entire program.
     * @param menuLabel the text to display in the menu item
     * corresponding to this export algorithm.
     * @param mnemonic the keyboard mnemonic to associate with the
     * corresponding menu item.
     * @see normal.algorithm.Algorithm
     */
    public ReginaBinaryExporter(Shell shell, String menuLabel, int mnemonic) {
        super(shell, menuLabel, mnemonic, Standalone.instance);
    }

    public boolean exportData(NPacket packet, File file) {
        if (shell.getEngine().writeToFile(file.getAbsolutePath(), packet))
            return true;
        shell.error("An error occurred whilst attempting to export to " +
            file.getAbsolutePath() + ".");
        return false;
    }
}

