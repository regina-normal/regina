
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

package normal.exports;

import java.io.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.packetfilter.*;

/**
 * An exporter that exports a script to a plain text file.
 *
 * @see normal.exports.Exporter
 */
public class ScriptExporter extends Exporter {
    /**
     * Creates a new exporter.
     *
     * @param shell the shell representing the entire program.
     * @param menuLabel the text to display in the menu item
     * corresponding to this export algorithm.
     * @see normal.algorithm.Algorithm
     */
    public ScriptExporter(Shell shell, String menuLabel) {
        super(shell, menuLabel, new SingleClass(NScript.class));
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
    public ScriptExporter(Shell shell, String menuLabel, int mnemonic) {
        super(shell, menuLabel, mnemonic, new SingleClass(NScript.class));
    }

    public boolean exportData(NPacket packet, File file) {
        NScript script = (NScript)packet;
        
        try {
            FileWriter writer = new FileWriter(file);

            // Output the name of the script.
            writer.write("### Regina Script: ");
            writer.write(packet.getPacketLabel());
            writer.write("\n###\n");

            // Output the value of each variable.
            long n = script.getNumberOfVariables();
            long i;
            for (i = 0; i < n; i++) {
                writer.write("### Variable ");
                writer.write(script.getVariableName(i));
                writer.write(": ");
                writer.write(script.getVariableValue(i));
                writer.write('\n');
            }
            writer.write("###\n### Begin Script\n");

            n = script.getNumberOfLines();
            for (i = 0; i < n; i++) {
                writer.write(script.getLine(i));
                writer.write('\n');
            }

            writer.close();
            return true;
        } catch (IOException exc) {
            exc.printStackTrace();
            shell.error("An I/O error occurred whilst attempting to write to "
                + file.getAbsolutePath() + ".");
            return false;
        }
    }
}

