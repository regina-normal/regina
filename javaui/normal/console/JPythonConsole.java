
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

import normal.*;
import btools.gui.component.*;
import org.python.core.*;
import org.python.util.*;

/**
 * Provides a console that is a JPython interpreter.
 */
public class JPythonConsole extends ConsolePane {
	/**
	 * The system state to be used with JPython.
	 */
	private static PySystemState systemState;

    /**
     * The actual JPython interpreter that is used.
     */
    private InteractiveInterpreter jpython;

    /**
     * Create a new JPython console.
     */
    public JPythonConsole() {
        super();

        setConsoleOutputAttributes(getConsoleInputAttributes());
        setConsolePromptAttributes(getConsoleInputAttributes());
        setPrompt(getPySystemState().ps1.toString());

        // Create the new JPython interpreter.
        jpython = new InteractiveInterpreter();
        jpython.setOut(new PyFile(getOutputStream(), "<JPython Console>"));
        jpython.setErr(new PyFile(getOutputStream(), "<JPython Console>"));

        // Run a dummy command to speed up response on the first real command.
        jpython.exec("2");
    }

    /**
     * Returns the JPython interpreter used by this console.  This can
     * be used to set JPython variables and the like.
     *
     * @return the JPython interpreter used by this console.
     */
    public PythonInterpreter getPythonInterpreter() {
        return jpython;
    }

    protected String processInput(String line) {
        if (jpython.buffer.length() > 0)
            jpython.buffer.append('\n');
        jpython.buffer.append(line);
        boolean more =
            jpython.runsource(jpython.buffer.toString(), "<JPython Console>");
        if (more) {
            setPrompt(getPySystemState().ps2.toString());
        } else {
            setPrompt(getPySystemState().ps1.toString());
            jpython.resetbuffer();
        }
        return null;
    }

    /**
     * Returns the shared JPython system state.  The single system state
     * should be shared amongst all JPython consoles, and will only
     * be created when first asked for.  If a system state cannot be created,
     * <tt>null</tt> will be returned.
     *
     * @return the shared system state, or <tt>null</tt> if one could
     * not be created.
     */
    public static PySystemState getPySystemState() {
        if (systemState == null)
            try {
                systemState = new PySystemState();
            } catch (Throwable th) {
				th.printStackTrace();
			}
        return systemState;
    }
}
