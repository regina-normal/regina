
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

package normal.console;

import java.io.*;
import java.util.*;
import normal.Shell;
import normal.options.*;
import org.python.core.*;
import org.python.modules.*;
import org.python.util.*;

/**
 * Provides various utilities for compiling and running Jython code.
 */
public class JPythonUtils {
    /**
     * The commands to be run whenever a new Jython session is started.
     * These commands will be run (in the order in which they appear in
     * the array) before anything else is done.
     * Each command should be a single element in the array.
     */
    public static final String[] startup = {
        "import java", "from java.lang import *",
        "import normal", "import org.gjt.btools"
    };

    /**
     * Initialises the given Jython interpreter.
     * The startup commands in <tt>JPythonUtils.startup</tt> are run
     * and the variable <tt>engine</tt> is set to the given calculation
     * engine.
     *
     * @param interpreter the Jython interpreter to set up; this should
     * be of class <tt>org.python.util.PythonInterpreter</tt> but is
     * passed as an <tt>Object</tt> to ease stability in the cases where
     * the python classes are not available.
     * @param shell the shell representing the entire program.
     * @param out the output stream to which messages should be sent.
     * These messages will inform the user of the initialisation that
     * has been done; there may be multiple lines of output, and a final
     * newline is guaranteed.
     */
    public static void setupInterpreter(Object interpreter, Shell shell,
            OutputStream out) {
        PythonInterpreter realInterpreter = (PythonInterpreter)interpreter;
        PyObject code;
        StringBuffer error = new StringBuffer();

        // Writer to which messages are sent.
        PrintWriter writer = new PrintWriter(out, true);

        writer.println("Running startup commands...");
        for (int i = 0; i < startup.length; i++) {
            code = compileCode(startup[i], error);
            if (code == null) {
                writer.println("Error compiling: " + startup[i]);
                writer.println(error);
                error = new StringBuffer();
            } else {
                if (! runCode(code, realInterpreter, error)) {
                    writer.println("Error running: " + startup[i]);
                    writer.println(error);
                    error = new StringBuffer();
                }
            }
        }

        // Set the engine.
        realInterpreter.set("engine", shell.getEngine());
        writer.println("The calculation engine " +
            "(type normal.engine.Engine) is in the variable [engine].");

        // Load libraries.
        File libFile;
        NormalOptionSet.JythonLibrary lib;
        Enumeration e = shell.getOptions().getJythonLibraries().elements();
        while (e.hasMoreElements()) {
            lib = (NormalOptionSet.JythonLibrary)e.nextElement();
            if (lib.shouldUseLibrary()) {
                libFile = new File(lib.getLibraryPath());
                writer.println("Loading: " + libFile.getName());

                code = compileFile(libFile, error);
                if (code == null) {
                    writer.println(error);
                    error = new StringBuffer();
                } else {
                    if (! runCode(code, realInterpreter, error)) {
                        writer.println(error);
                        error = new StringBuffer();
                    }
                }
            }
        }
    }

    /**
     * Compiles the given block of Jython code.  The block of code
     * should be presented as a number of lines of code separated by
     * newlines.
     * <p>
     * If a compile error occurs, the corresponding error message will
     * be appended to the given string buffer.
     *
     * @param code the block of code to compile.
     * @param error a string buffer to which any compile errors will be
     * appended.  Note that compile errors may contain many lines of
     * information and will always end in a final newline.
     * @return the compiled code on success, or <tt>null</tt> on
     * failure.
     */
    public static PyObject compileCode(String code, StringBuffer error) {
        PyObject compiled = null;
        try {
            compiled = codeop.compile_command(
                code, "<script>", "exec");
        } catch (RuntimeException exc) {
            if (exc instanceof PyException) {
                if (Py.matchException((PyException)exc, Py.SyntaxError)) {
                    compiled = null;
                    error.append(exc.toString());
                } else
                    throw exc;
            } else
                throw exc;
        }
        if (compiled == Py.None) {
            compiled = null;
            error.append(
                "The script was incomplete; Jython expects more code.");
        }

        // Append a final newline to the error message if necessary.
        if (compiled == null) {
            int len = error.length();
            if (len == 0)
                error.append('\n');
            else if (error.charAt(len - 1) != '\n')
                error.append('\n');
        }

        return compiled;
    }

    /**
     * Compiles the given Jython file.
     * If a compile or file I/O error occurs, the corresponding error
     * message will be appended to the given string buffer.
     *
     * @param file the Jython file to compile.
     * @param error a string buffer to which any compile errors will be
     * appended.  Note that compile errors may contain many lines of
     * information and will always end in a final newline.
     * @return the compiled code on success, or <tt>null</tt> on
     * failure.
     */
    public static PyObject compileFile(File file, StringBuffer error) {
        FileInputStream in;
        try {
            in = new FileInputStream(file);
        } catch (Exception e) {
            error.append("Could not open file [" +
                file.getAbsolutePath() + "] for reading.\n");
            error.append(e.toString());
            if (error.charAt(error.length() - 1) != '\n')
                error.append('\n');
            return null;
        }

        PyObject compiled = null;
        try {
            compiled = Py.compile(in, file.getName(), "exec");
        } catch (RuntimeException exc) {
            if (exc instanceof PyException) {
                if (Py.matchException((PyException)exc, Py.SyntaxError)) {
                    compiled = null;
                    error.append(exc.toString());
                } else
                    throw exc;
            } else
                throw exc;
        }
        if (compiled == Py.None) {
            compiled = null;
            error.append(
                "The file was incomplete; Jython expects more code.");
        }

        // Append a final newline to the error message if necessary.
        if (compiled == null) {
            int len = error.length();
            if (len == 0)
                error.append('\n');
            else if (error.charAt(len - 1) != '\n')
                error.append('\n');
        }

        return compiled;
    }

    /**
     * Runs the given pre-compiled Jython code.
     * If a runtime error occurs, the corresponding error
     * message will be appended to the given string buffer.
     *
     * @param compiledCode the pre-compiled Jython code to run.
     * @param interpreter the Jython interpreter in which to run the code.
     * @param error a string buffer to which any runtime errors will be
     * appended.  Note that runtime errors may contain many lines of
     * information and will always end in a final newline.
     * @return <tt>true</tt> on success or <tt>false</tt> on failure.
     */
    public static boolean runCode(PyObject compiledCode,
            PythonInterpreter interpreter, StringBuffer error) {
        try {
            interpreter.exec(compiledCode);
            return true;
        } catch (RuntimeException exc) {
            if (exc instanceof PyException) {
                // Runtime error!
                error.append(exc.toString());
                int len = error.length();
                if (len == 0)
                    error.append('\n');
                else if (error.charAt(len - 1) != '\n')
                    error.append('\n');
                return false;
            } else
                throw exc;
        }
    }
}

