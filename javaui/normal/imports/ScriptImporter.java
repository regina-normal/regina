
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

import java.io.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.mainui.LibraryPane;
import org.gjt.btools.ext.ExtensionFilenameFilter;

/**
 * An interface through which the user can import a script from a text file.
 * See <tt>Importer</tt> for further details.
 *
 * @see normal.imports.Importer
 */
public class ScriptImporter extends Importer {
    /**
     * Creates a new importer.
     *
     * @param shell the shell representing the entire program.
     */
    public ScriptImporter(Shell shell) {
        super(shell, new ExtensionFilenameFilter(
            LibraryPane.defaultFileExtension,
            "Jython Scripts (*" + LibraryPane.defaultFileExtension + ')'));
    }
     
    public String getDialogTitle() {
        return "Import Jython Script";
    }

    protected NPacket importData(Shell shell, File dataFile) {
        FileReader reader;
        try {
            reader = new FileReader(dataFile);
        } catch (FileNotFoundException exc) {
            shell.error("File " + dataFile.getAbsolutePath() +
                " could not be found.");
            return null;
        }

        BufferedReader breader = new BufferedReader(reader);
        NScript script = shell.getEngine().newNScript();
        script.setPacketLabel("Imported Script");

        try {
            String line = breader.readLine();
            boolean readingMetadata = true;
            int result;
            while (line != null) {
                if (readingMetadata) {
                    result = processMetadata(line, script);
                    if (result != 1)
                        readingMetadata = false;
                    if (result == 0)
                        script.addLast(line);
                } else
                    script.addLast(line);

                line = breader.readLine();
            }

            breader.close();
        } catch (IOException exc) {
            shell.error("An error occurred whilst attempting to import from "
                + dataFile.getAbsolutePath() + ".");
            script.destroy();
            return null;
        }

        return script;
    }

    /**
     * Processes any metadata contained in the given line taken from the text
     * file.
     *
     * @param line the line from the text file to examine.
     * @param script the script packet currently being constructed.
     * @return 1 if the line contained metadata that was successfully
     * processed, -1 if the line contained an instruction to stop
     * processing metadata all together, and 0 otherwise.
     */
    private static int processMetadata(String line, NScript script) {
        line = line.trim();
        if (! line.startsWith("###"))
            return 0;
        line = line.substring(3).trim();

        int pos;

        // Check for empty metadata.
        if (line.length() == 0)
            return 1;

        // Check for a script name.
        if (line.startsWith("Regina Script")) {
            pos = line.indexOf(':');
            if (pos < 0)
                return 0;
            line = line.substring(pos + 1).trim();
            if (line.length() == 0)
                return 0;
            script.setPacketLabel(line);
            return 1;
        }

        // Check for a variable.
        if (line.startsWith("Variable")) {
            line = line.substring(8);
            pos = line.indexOf(':');
            if (pos < 0)
                return 0;
            script.addVariable(line.substring(0, pos).trim(),
                line.substring(pos + 1).trim());
            return 1;
        }

        // Check for an end-of-metadata flag.
        if (line.equals("Begin Script")) {
            return -1;
        }

        // Unrecognised metadata.
        return 0;
    }
}
 
