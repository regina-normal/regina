
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
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
import java.awt.event.*;
import java.io.File;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import normal.Shell;
import normal.engine.packet.NPacket;
import normal.options.NormalOptionSet;
import normal.packetfilter.*;
import normal.packetui.PacketCreator;
import org.gjt.btools.gui.*;

/**
 * An interface through which the user can import data from a foreign file.
 * <p>
 * The data must be imported <i>before</i> this pane is presented to
 * the user; this can be done using routine <tt>importData()</tt>.
 * Once the data has been imported it may then be passed back to the
 * main program using the standard <tt>NewPacketDialog</tt> mechanism
 * (with this object as the corresponding <tt>PacketCreator</tt>).
 * <p>
 * Note that routine <tt>TopologyPane.importData()</tt> does most of
 * this work for you.
 * <p>
 * This class must be subclassed for each file type that may be
 * imported; routine <tt>importData(Shell, File)</tt> does the actual
 * work of reading the foreign file.  By default this packet creator
 * allows any parent packet; routine <tt>getPacketFilter()</tt>
 * may be overridden to change this behaviour.
 *
 * @see #importData()
 * @see #importData(normal.Shell, java.io.File)
 * @see normal.mainui.TopologyPane#importData
 * @see normal.packetui.NewPacketDialog
 */
public abstract class Importer extends JPanel implements PacketCreator {
    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * A filter specifying the files we are interested in.
     */
    private FileFilter filter;

    /**
     * The data that has been imported.
     */
    private NPacket data = null;

    /**
     * Have we passed the imported data back to the main program via
     * <tt>makeNewPacket()</tt>?
     */
    private boolean returnedData = false;
    
    /**
     * Creates a new importer.
     *
     * @param shell the shell representing the entire program.
     * @param filter a filter specifying the files in which we are
     * interested, or <tt>null</tt> if no filter is required.
     */
    public Importer(Shell shell, FileFilter filter) {
        this.shell = shell;
        this.filter = filter;
    }

    public Component getCreator() {
        return this;
    }

    public PacketFilter getPacketFilter() {
        return AllowAll.instance;
    }

    public String getSuggestedPacketLabel() {
        if (data == null)
            return "No data";
        else
            return data.getPacketLabel();
    }

    public NPacket makeNewPacket(Shell shell, NPacket parent,
            Dialog parentDialog) {
        if (data == null) {
            shell.error("No data has been imported.");
            return null;
        }

        // Run through all imported packets and ensure that their labels
        // will be unique.
        data.makeUniqueLabels(parent.getTreeMatriarch());

        returnedData = true;
        return data;
    }

    /**
     * Allows the user to select a file and then imports the data.
     * This must be called <i>before</i> this pane is presented to the
     * user.
     * <p>
     * Note that the data will be imported but not yet passed back to the
     * main program.  A standard <tt>NewPacketDialog</tt> should be used
     * for this task with this <tt>Importer</tt> as its
     * <tt>PacketCreator</tt> (assuming the import was successful).
     * <p>
     * Any errors that occur during the import will be reported to the
     * user during this routine.
     *
     * @return <tt>true</tt> if and only if the data was successfully
     * imported.
     *
     * @see normal.packetui.NewPacketDialog
     */
    public boolean importData() {
        // Preparatory cleanup.
        if (data != null) {
            data.destroy();
            data = null;
        }
        returnedData = false;

        // Select a file from which to import.
        JFileChooser chooser = new JFileChooser();
        NormalOptionSet options = shell.getOptions();
        chooser.setCurrentDirectory(new File(
            options.getStringOption("LastDir", ".")));
        if (filter != null)
            chooser.setFileFilter(filter);
        chooser.setDialogTitle("Import from...");
        if (chooser.showOpenDialog(shell.getPrimaryFrame()) !=
                chooser.APPROVE_OPTION)
            return false;
        File dataFile = chooser.getSelectedFile();

        // Update the system properties.
        String fileDir = dataFile.getParentFile().getAbsolutePath();
        if (fileDir == null)
            fileDir = ".";
        options.setStringOption("LastDir", fileDir);
        options.writeToFile();

        // Check that the file exists.
        try {
            if (! dataFile.exists()) {
                shell.error("The file " + dataFile.getAbsolutePath()
                    + " does not exist.");
                return false;
            }
        } catch (SecurityException e) {}

        // Do the import!
        data = importData(shell, dataFile);
        return (data != null);
    }

    /**
     * Imports the data from the given file.  This routine must be
     * overridden by different subclasses of <tt>Importer</tt> to handle
     * the importing of different types of file.
     * <p>
     * This routine should not only import the data, but also set the packet
     * label(s) for the new data to something appropriate.  The other
     * routines in this class will take care of ensuring that packet
     * labels remain unique once the data is inserted into the parent
     * tree, so this routine need not concern itself with label uniqueness.
     * <p>
     * You may assume that the given file exists.
     * Any errors that occur during the import should be reported to the
     * user during this routine.
     *
     * @param shell the shell representing the entire program.
     * @param dataFile the file from which to import data.
     * @return the newly imported data, or <tt>null</tt> if an error
     * occurred.
     */
    protected abstract NPacket importData(Shell shell, File dataFile);

    /**
     * Destroys the imported data if nobody ever bothered to use it.
     */
    protected void finalize() throws Throwable {
        if ((! returnedData) && (data != null)) {
            data.destroy();
            data = null;
        }
        super.finalize();
    }
}

