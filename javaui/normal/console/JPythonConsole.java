
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

import normal.*;
import org.gjt.btools.gui.component.*;
import org.python.core.*;
import org.python.util.*;

/**
 * Provides a console that is a Jython interpreter.
 */
public class JPythonConsole extends ConsolePane {
    /**
     * The system state to be used with Jython.
     */
    private static PySystemState systemState;

    /**
     * The actual Jython interpreter that is used.
     */
    private InteractiveInterpreter jython;

    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * Create a new Jython console.
     *
     * @param shell the shell representing the entire program.
     */
    public JPythonConsole(Shell shell) {
        super();
        this.shell = shell;

        setConsoleOutputAttributes(getConsoleInputAttributes());
        setConsolePromptAttributes(getConsoleInputAttributes());
        setPrompt(getPySystemState().ps1.toString());

        // Create the new Jython interpreter.
        jython = new InteractiveInterpreter();
        jython.setOut(new PyFile(getOutputStream(), "<Jython Console>"));
        jython.setErr(new PyFile(getOutputStream(), "<Jython Console>"));

        // Run a dummy command to speed up response on the first real command.
        jython.exec("2");
    }

    /**
     * Returns the Jython interpreter used by this console.  This can
     * be used to set Jython variables and the like.
     *
     * @return the Jython interpreter used by this console.
     */
    public PythonInterpreter getPythonInterpreter() {
        return jython;
    }

    /**
     * Returns the shell representing the entire program.
     *
     * @return the shell representing the entire program.
     */
    public Shell getShell() {
        return shell;
    }

    /**
     * Console preprocessing routine.  This will set up the interpreter
     * as described in <tt>JPythonUtils.setupInterpreter()</tt>.
     *
     * @see normal.console.JPythonUtils#setupInterpreter
     */
    protected void preProcess() {
        // Set up the interpreter.
        JPythonUtils.setupInterpreter(jython, shell, getOutputStream());
        outputLine();
    }

    protected String processInput(String line) {
        if (jython.buffer.length() > 0)
            jython.buffer.append('\n');
        jython.buffer.append(line);
        boolean more =
            jython.runsource(jython.buffer.toString(), "<Jython Console>");
        if (more) {
            setPrompt(getPySystemState().ps2.toString());
        } else {
            setPrompt(getPySystemState().ps1.toString());
            jython.resetbuffer();
        }
        return null;
    }

    /**
     * Returns the shared Jython system state.  The single system state
     * should be shared amongst all Jython consoles, and will only
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
