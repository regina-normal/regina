
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

import normal.Application;
import normal.engine.Engine;

/**
 * Provides various utilities for use with JPython consoles.
 */
public class ConsoleUtils {
	/**
	 * The commands to be run whenever a new JPython session is started.
	 * These commands will be run before anything else is done.
	 * Each command should be terminated by a newline.  All commands
	 * should be concatenated into a single string.
	 */
	public static final String startup =
		"import java;\nfrom java.lang import *;\n" +
			"import normal;\nimport btools;";
	
	/**
	 * Runs a standalone text JPython console with no GUI.
	 * Standard input and standard output will be used.
	 *
	 * @param engine the calculation engine currently in use.
	 */
	public static void runTextConsole(Engine engine) {
		System.out.println();
		System.out.println(consoleGreeting());
		System.out.println();

		// Start the interpreter.
		org.python.util.InteractiveConsole console =
			new org.python.util.InteractiveConsole();
		System.out.println(consoleSetup(console, engine));
		System.out.println();
		console.interact();
	}
	
	/**
	 * Returns the greeting to offer the user when the program is run as
	 * a standalone JPython console without the full GUI.
	 * No final newline is included.
	 *
	 * @return the program console greeting.
	 */
	public static String consoleGreeting() {
		return Application.program + ' ' + Application.version + '\n' +
			Application.comments + '\n' + Application.copyright;
	}

    /**
     * Runs the standard startup commands and
     * sets up the local variables in a JPython interpreter that is to
     * be used standalone, without the full program GUI.
	 * This routine will return a
     * string to be displayed to the user to notify them of this setup;
     * no final newline will be included.
     *
     * @param interpreter the JPython interpreter to set up; this should
     * be of class <tt>org.python.util.PythonInterpreter</tt> but is
     * passed as an <tt>Object</tt> to avoid crashes in case the python
     * classes are not available.
     * @param engine the calculation engine currently in use.
     * @return a string to inform the user of the newly arranged setup.
     */
    public static String consoleSetup(Object interpreter, Engine engine) {
        try {
            org.python.core.PyObject code =
				org.python.modules.codeop.compile_command(
                startup, "<startup>", "exec");
            if (code != org.python.core.Py.None)
                ((org.python.util.PythonInterpreter)interpreter).exec(code);
        } catch (RuntimeException exc) {}

        ((org.python.util.PythonInterpreter)interpreter).set("engine", engine);
        return "The calculation engine (type normal.engine.Engine) is " +
            "in the variable [engine].";
    }
}
