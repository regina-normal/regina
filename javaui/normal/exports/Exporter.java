
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

package normal.exports;

import java.io.File;
import javax.swing.*;
import normal.Shell;
import normal.algorithm.Algorithm;
import normal.engine.packet.NPacket;
import normal.options.NormalOptionSet;
import normal.packetfilter.*;

/**
 * An algorithm that exports data to a foreign format.
 * <p>
 * This class must be subclassed for each file type that may be
 * exported to; routine <tt>exportData(NPacket, File)</tt> does the actual
 * work of exporting to the foreign file.
 *
 * @see #exportData
 * @see normal.algorithm.Algorithm
 */
public abstract class Exporter extends Algorithm {
    /**
     * Creates a new exporter algorithm.
     *
     * @param shell the shell representing the entire program.
     * @param menuLabel the text to display in the menu item
     * corresponding to this export algorithm.
     * @param filter specifies which packets we are allowed to perform
     * this type of export upon.
     * @see normal.algorithm.Algorithm
     */
    public Exporter(Shell shell, String menuLabel, PacketFilter filter) {
        super(shell, menuLabel, "", filter, false);
    }

    /**
     * Creates a new exporter algorithm.
     *
     * @param shell the shell representing the entire program.
     * @param menuLabel the text to display in the menu item
     * corresponding to this export algorithm.
     * @param mnemonic the keyboard mnemonic to associate with the
     * corresponding menu item.
     * @param filter specifies which packets we are allowed to perform
     * this type of export upon.
     * @see normal.algorithm.Algorithm
     */
    public Exporter(Shell shell, String menuLabel, int mnemonic,
            PacketFilter filter) {
        super(shell, menuLabel, mnemonic, "", filter, false);
    }

    public boolean perform(NPacket packet) {
        // Select a file to which to export.
        JFileChooser chooser = new JFileChooser();
        NormalOptionSet options = shell.getOptions();
        chooser.setCurrentDirectory(new File(
            options.getStringOption("LastDir", ".")));

        // Don't set a filename filter so we don't mislead
        // the user into thinking we're going to add an automatic
        // extension.
        //if (fileFilter != null)
        //    chooser.setFileFilter(fileFilter);

        chooser.setDialogTitle("Export to...");
        if (chooser.showSaveDialog(shell.getPrimaryFrame()) !=
                chooser.APPROVE_OPTION)
            return false;
        File dataFile = chooser.getSelectedFile();

        // Update the system properties.
        String fileDir = dataFile.getParentFile().getAbsolutePath();
        if (fileDir == null)
            fileDir = ".";
        options.setStringOption("LastDir", fileDir);
        options.writeToFile();

        exportData(packet, dataFile);
        return false;
    }

    /**
     * Actually does the work of exporting the given packet to the given
     * file.  It can be guaranteed that the given packet satisfies the packet
     * filter passed to the exporter constructor.
     * <p>
     * This routine must be overridden by different subclasses of
     * <tt>Exporter</tt> to handle the exporting to different types of
     * file.
     * <p>
     * Any errors that occur during the export should be reported to the
     * user during this routine.
     *
     * @param packet the packet subtree to export.
     * @param file the file to which to export data.
     * @return <tt>true</tt> if and only if the data was successfully
     * exported.
     */
    public abstract boolean exportData(NPacket packet, File file);
}

