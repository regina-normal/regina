
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

package normal.console;

import java.awt.Frame;
import normal.*;

/**
 * Provides various utilities for use with Jython consoles.
 */
public class ConsoleUtils {
    /**
     * Returns the greeting to offer the user when the program is run as
     * a standalone Jython console.
	 * The greeting may span multiple lines, but no final newline
	 * should be included.
     *
     * @return the program console greeting.
     */
    public static String standaloneGreeting() {
        return Application.program + ' ' + Application.version + '\n' +
            Application.comments + '\n' + Application.copyright;
    }

    /**
     * Runs a standalone text Jython console with no GUI.
     * Standard input and standard output will be used.
     *
	 * @param shell the shell representing the entire program.
     */
    public static void runTextConsole(Shell shell) {
        System.out.println();
        System.out.println(standaloneGreeting());
        System.out.println();

        // Start the interpreter.
        org.python.util.InteractiveConsole console =
            new org.python.util.InteractiveConsole();
		JPythonUtils.setupInterpreter(console, shell, System.out);
        System.out.println();
        console.interact();
    }

	/**
	 * Creates a new standalone graphical Jython console.
	 * The console will be displaying appropriate welcome messages.
	 * The frame will still need to be shown and the console
	 * will still need to be started (through
	 * <tt>JPythonConsoleFrame.startConsole()</tt>).
	 *
	 * @param shell the shell representing the entire program.
	 * @param standalone <tt>true</tt> if and only if this frame will in
	 * fact be the entire program, with no other GUI present.
	 * @return the created Jython console frame.
	 *
	 * @see normal.console.JPythonConsoleFrame#startConsole()
	 * @see normal.console.JPythonConsoleFrame#startConsole(java.lang.String)
	 */
	public static JPythonConsoleFrame createGraphicalConsole(Shell shell,
			boolean standalone) {
		JPythonConsoleFrame frame = new JPythonConsoleFrame(shell, null,
			standalone);
		frame.getConsole().outputMessage(standaloneGreeting() + "\n\n" +
			org.python.util.InteractiveConsole.getDefaultBanner() + "\n\n");
		return frame;
	}
}
