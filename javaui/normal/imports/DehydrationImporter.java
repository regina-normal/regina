
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

package normal.imports;

import java.io.*;
import normal.Shell;
import normal.engine.Engine;
import normal.engine.packet.*;
import normal.engine.triangulation.NTriangulation;

/**
 * An interface through which the user can import a dehydrated
 * triangulation list.  See <tt>Importer</tt> for further details.
 *
 * @see normal.imports.Importer
 */
public class DehydrationImporter extends Importer {
    /**
     * The column of the text file containing dehydrated triangulation
     * strings.
     */
    private int colDehydration = 0;

    /**
     * The column of the text file containing manifold names.
     * This should be negative if manifold names are not stored.
     */
    private int colName = -1;
    
    /**
     * The number of initial lines of the text file to ignore.
     */
    private int ignoreLines = 0;

    /**
     * Creates a new importer.
     *
     * @param shell the shell representing the entire program.
     */
    public DehydrationImporter(Shell shell) {
        super(shell, null);
    }
     
    public String getDialogTitle() {
        return "Import Dehydrated Triangulation List";
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

        StreamTokenizer tok = new StreamTokenizer(reader);
        tok.eolIsSignificant(true);

        // Read the file one line at a time.
        Engine engine = shell.getEngine();
        NPacket packet = engine.newNContainer();
        packet.setPacketLabel("Dehydrated Triangulations");
        long nErrors = 0;

        try {
            // Ignore the appropriate number of initial lines.
            int tokenType = tok.nextToken();
            for (int i = 0; i < ignoreLines; i++) {
                // We have read the first token on the next line.
                while (tokenType != tok.TT_EOF && tokenType != tok.TT_EOL)
                    tokenType = tok.nextToken();
                if (tokenType == tok.TT_EOL)
                    tokenType = tok.nextToken();
            }

            // Process each line thereafter.
            String dehydration;
            String name;
            String token;
            int nextTokenIndex;
            NTriangulation tri;
            NText message;

            while (tokenType != tok.TT_EOF) {
                // We have read the first token on the next line.
                dehydration = null;
                name = null;
                nextTokenIndex = 0;

                // Find the dehydration and manifold name.
                while ((nextTokenIndex <= colDehydration ||
                        nextTokenIndex <= colName) &&
                        tokenType != tok.TT_EOF &&
                        tokenType != tok.TT_EOL) {
                    if (nextTokenIndex == colDehydration ||
                            nextTokenIndex == colName) {
                        // This particular token is one we are
                        // interested in.
                        if (tokenType == tok.TT_NUMBER)
                            token = String.valueOf(tok.nval);
                        else
                            token = tok.sval;

                        if (nextTokenIndex == colDehydration)
                            dehydration = token;
                        if (nextTokenIndex == colName)
                            name = token;
                    }
                    tokenType = tok.nextToken();
                    nextTokenIndex++;
                }

                // Move to the end of the line.
                while (tokenType != tok.TT_EOF && tokenType != tok.TT_EOL)
                    tokenType = tok.nextToken();
                if (tokenType == tok.TT_EOL)
                    tokenType = tok.nextToken();

                // Process the triangulation if one was read.
                if (dehydration != null) {
                    // Name the triangulation after the dehydration
                    // string if no better name is offered.
                    if (name == null)
                        name = dehydration;

                    // Dehydrate!
                    tri = engine.newNTriangulation();
                    if (tri.insertRehydration(dehydration)) {
                        tri.setPacketLabel(name);
                        packet.insertChildLast(tri);
                    } else {
                        tri.destroy();
                        message = engine.newNText(
                            "Invalid dehydration string:\n" + dehydration);
                        message.setPacketLabel(name);
                        packet.insertChildLast(message);
                        nErrors++;
                    }
                }
            }

            reader.close();
        } catch (IOException exc) {
            shell.error(
                "An I/O error occurred whilst attempting to import from " +
                dataFile.getAbsolutePath() + ".");
            packet.destroy();
            return null;
        }

        if (nErrors == 1)
            shell.error("1 dehydration string was invalid.");
        else if (nErrors > 1)
            shell.error(String.valueOf(nErrors) +
                " dehydration strings were invalid.");

        return packet;
    }
}
 
