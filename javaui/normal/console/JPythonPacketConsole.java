
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

package normal.console;

import java.io.*;
import normal.*;
import normal.engine.packet.NPacket;
import org.python.util.*;

/**
 * Provides a Jython console that is linked to a specific packet tree.
 * When the Jython interpreter is initialised, variables will be set to
 * provide access to this packet tree.
 */
public class JPythonPacketConsole extends JPythonConsole {
    /**
     * The matriarch of the packet tree to which this console is linked.
     */
    private NPacket tree;

    /**
     * Create a new Jython console linked to the given packet tree.
     *
     * @param shell the shell representing the entire program.
     * @param tree the packet tree to which the new console will be linked.
     */
    public JPythonPacketConsole(Shell shell, NPacket tree) {
        super(shell);
        this.tree = tree;
    }

    protected void preProcess() {
        // Perform basic interpreter setup.
        super.preProcess();

        PythonInterpreter interpreter = getPythonInterpreter();
        PrintWriter writer = new PrintWriter(getOutputStream(), true);

        // Add variables for the packet tree to which we are linked.
        interpreter.set("root", tree);
        writer.println(
            "The root of the packet tree is in the variable [root].");

        interpreter.set("packetDict", tree.makeTreeDict());
        writer.println(
            "A map from packet labels to packets " +
            "(type java.util.Map) is in the variable [packetDict].");

        // Done!
        outputLine();
    }
}
