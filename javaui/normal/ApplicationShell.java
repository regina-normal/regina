
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

package normal;

import java.applet.Applet;
import java.awt.Frame;
import java.awt.event.*;
import java.io.*;
import java.lang.reflect.*;
import java.util.*;
import normal.engine.Engine;
import normal.options.NormalOptionSet;
import org.gjt.btools.gui.dialog.MessageBox;
import org.gjt.btools.utilities.*;

/**
 * The program shell to use when the program is run as a standalone
 * application.
 */
public class ApplicationShell extends Shell {
    /**
     * The command-line arguments to the application.
     */
    private String[] args;

    /**
     * The command-line arguments to the application, split into
     * flags, variables and extra arguments.
     */
    private CommandLineArguments splitArgs;

    /**
     * Are we running a command-line text-only interface?
     */
    private boolean textOnly = false;

    /**
     * The standard input stream reader for use with a command-line text-only
     * interface.
     */
    private BufferedReader stdin = null;

    /**
     * The object used to view help through the JavaHelp system if
     * appropriate.
     */
    private ActionListener helpViewer = null;

    /**
     * The JavaHelp method used to set a context sensitive help ID.
     */
    private Method setHelpIDMethod = null;

    /**
     * Creates a new program shell for a standalone application.
     *
     * @param args the command-line arguments to the application.
     */
    public ApplicationShell(String[] args) {
        this.args = args;

        String[] flagNames = {
            "-q", "--quiet", "-v", "--verbose",
            "--gui", "--console", "--text",
            "--jni", "--corba"
        };
        String[] varNames = {
            "--jnilibname",
            "-ORBInitialHost", "-ORBInitialPort",
            "-org.omg.CORBA.ORBInitialHost", "-org.omg.CORBA.ORBInitialPort"
        };

        this.splitArgs = new CommandLineArguments(flagNames, varNames, args);
    }

    public int getShellStyle() {
        return shellApplication;
    }
    public String[] getCommandLineParameters() {
        return args;
    }
    public Applet getAppletParameters() {
        return null;
    }
    public String getParameter(String paramName, int nMinus) {
        // Build the prefix for which we are hunting.
        StringBuffer prefix = new StringBuffer();
        for (int i = 0; i < nMinus; i++)
            prefix.append('-');
        prefix.append(paramName);

        // Hunt for the prefix.
        return (String)splitArgs.getVariables().get(prefix.toString());
    }

    public Vector getFileParameters() {
        return new Vector(splitArgs.getExtraArgs());
    }

    public int getUIType() {
        int uiType = unspecified;

        // Look up the command-line parameters.
        Enumeration e = splitArgs.getFlags().elements();
        String arg;
        while (e.hasMoreElements()) {
            arg = (String)e.nextElement();
            if (arg.equals("--gui")) {
                if (uiType != unspecified && uiType != UIFullGUI) {
                    error("More than one UI type has been requested.");
                    return invalid;
                } else
                    uiType = UIFullGUI;
            } else if (arg.equals("--console")) {
                if (uiType != unspecified && uiType != UIConsoleWindow) {
                    error("More than one UI type has been requested.");
                    return invalid;
                } else
                    uiType = UIConsoleWindow;
            } else if (arg.equals("--text")) {
                if (uiType != unspecified && uiType != UITextConsole) {
                    error("More than one UI type has been requested.");
                    return invalid;
                } else
                    uiType = UITextConsole;
            }
        }

        // Look up the runtime options if necessary.
        if (uiType == unspecified) {
            String iface = OptionSet.getSystemProperty("REGINA_INTERFACE");
            if (iface != null) {
                if (iface.equalsIgnoreCase("gui"))
                    uiType = UIFullGUI;
                else if (iface.equalsIgnoreCase("console"))
                    uiType = UIConsoleWindow;
                else if (iface.equalsIgnoreCase("text"))
                    uiType = UITextConsole;
                else {
                    error("An invalid style of interface [" + iface
                        + "] has been requested.");
                    return invalid;
                }
            }
        }

        // Use the default if nothing has been selected.
        if (uiType == unspecified)
            uiType = UIFullGUI;

        // Set the text-only flag if appropriate.
        if (uiType == UITextConsole)
            textOnly = true;

        return uiType;
    }
    public int getEngineType() {
        int engineType = unspecified;

        // Look up the command-line parameters.
        Enumeration e = splitArgs.getFlags().elements();
        String arg;
        while (e.hasMoreElements()) {
            arg = (String)e.nextElement();
            if (arg.equals("--jni")) {
                if (engineType != unspecified && engineType != engineJNI) {
                    error("More than one style of engine access has been " +
                        "requested.");
                    return invalid;
                } else
                    engineType = engineJNI;
            } else if (arg.equals("--corba")) {
                if (engineType != unspecified && engineType != engineCORBA) {
                    error("More than one style of engine access has been " +
                        "requested.");
                    return invalid;
                } else
                    engineType = engineCORBA;
            }
        }

        // Look up the runtime options if necessary.
        if (engineType == unspecified) {
            String engine = OptionSet.getSystemProperty("REGINA_ENGINE");
            if (engine != null) {
                if (engine.equalsIgnoreCase("jni"))
                    engineType = engineJNI;
                else if (engine.equalsIgnoreCase("corba"))
                    engineType = engineCORBA;
                else {
                    error("An invalid style of engine access [" +
                        engine + "] has been requested.");
                    return invalid;
                }
            }
        }

        // Use the default if nothing has been selected.
        if (engineType == unspecified)
            engineType = engineJNI;

        return engineType;
    }
    public boolean shouldShowSplashScreen() {
        return (! textOnly);
    }
    public boolean mayUIAccessFiles() {
        return true;
    }
    public boolean mayEngineAccessFiles() {
        return true;
    }
    public boolean mayUseJython() {
        return true;
    }
    public NormalOptionSet loadOptionSet() {
        try {
            return new NormalOptionSet(new File(getOptionsDir(),
                Application.optionsFile));
        } catch (Throwable th) {
            return null;
        }
    }
    public void inform(String message) {
        if (textOnly) {
            System.out.println();
            System.out.println(message);
        } else
            MessageBox.fgNote(getPrimaryFrame(), message);
    }
    public boolean confirm(String message) {
        if (textOnly) {
            System.out.println();
            System.out.println(message);
            BufferedReader reader =
                new BufferedReader(new InputStreamReader(System.in));
            while (true) {
                System.out.print("[Y/n] : ");
                try {
                    String line = stdin.readLine().trim();
                    if (line.length() == 0)
                        return true;
                    if (Character.toLowerCase(line.charAt(0)) == 'y')
                        return true;
                    if (Character.toLowerCase(line.charAt(0)) == 'n')
                        return false;
                } catch (IOException e) {
                    System.err.println("Could not read from standard input.");
                    System.exit(1);
                }
            }
        } else
            return MessageBox.confirm(getPrimaryFrame(), message);
    }
    public void error(String message) {
        if (textOnly) {
            System.err.println();
            System.err.println(message);
        } else {
            try {
                MessageBox.fgNote(getPrimaryFrame(), message);
            } catch (Throwable th) {
                System.err.println();
                System.err.println(message);
            }
        }
    }
    public void viewHelp(String id) {
        if (getOptions().getBooleanOption("HelpJavaHelp", true)) {
            // Use the internal JavaHelp system.
            if (! hasFoundJavaHelp()) {
                error("The JavaHelp system could not be found.  " +
                    "Ensure [jh.jar] is on your classpath, or use the " +
                    "options menu to select a different help browser.");
                return;
            }
            if (! hasFoundDocumentation()) {
                error("The documentation jar could not be found.  " +
                    "Ensure it is on your classpath, or use the options " +
                    "menu to select a different help browsing method.");
                return;
            }
            try {
                // Check we have the methods we'll need.
                if (setHelpIDMethod == null) {
                    Class[] params = {
                        java.awt.Component.class, java.lang.String.class
                    };
                    setHelpIDMethod = ClassLoader.getSystemClassLoader().
                        loadClass("javax.help.CSH").getMethod(
                        "setHelpIDString", params);
                }

                // Create a new help viewer if necessary.
                if (helpViewer == null) {
                    Class displayHelpClass = ClassLoader.getSystemClassLoader().
                        loadClass("javax.help.CSH$DisplayHelpFromSource");
                    Class[] createParams = {
                        ClassLoader.getSystemClassLoader().loadClass(
                            "javax.help.HelpBroker")
                    };
                    Constructor construct =
                        displayHelpClass.getConstructor(createParams);
                    Object[] createArgs = {
                        getHelpBroker()
                    };
                    helpViewer =
                        (ActionListener)construct.newInstance(createArgs);
                }

                // Prepare to jump to the correct help page.
                Frame frame = getPrimaryFrame();
                Object[] setIDArgs = {
                    frame, id == null ? "index" : id
                };
                setHelpIDMethod.invoke(null, setIDArgs);

                // Open the help page.
                helpViewer.actionPerformed(new ActionEvent(frame, 0, ""));
            } catch (Throwable th) {
                error("An error occurred whilst trying to browse " +
                    "help using JavaHelp.  The options menu can be used " +
                    "to select a different help browser.");
            }
        } else {
            // Use an external help browser.

            // Get the help page to view.
            String page = getHelpPageResource(id);

            String docsLoc =
                getOptions().getStringOption("HelpDocs", "").trim();
            if (docsLoc.length() == 0) {
                error("An external help browser has been requested but " +
                    "the documentation location has not been specified.  " +
                    "The options menu can be used to rectify this problem.");
                return;
            }

            page = ResourceUtils.combineLocations(docsLoc, page);

            // Spawn the external viewer.
            String command = getOptions().getStringOption("HelpBrowser", "");
            if (command.length() == 0) {
                error("An external help browser has been requested but " +
                    "the browser details have not been specified.  " +
                    "The options menu can be used to rectify this problem.");
                return;
            }

            command = (command + " " + page).trim();
            try {
                Runtime.getRuntime().exec(command);
            } catch (SecurityException se) {
                error("The Java security manager will not allow the " +
                    "requested external help browser to be executed.  " +
                    "The options menu can be used to select the internal " +
                    "JavaHelp browser.\nThe command executed was [" +
                    command + "].");
            } catch (Throwable th) {
                error("An error occurred whilst trying to spawn the " +
                    "requested external help browser.  The options menu " +
                    "can be used to select a different browser.\n" +
                    "The command executed was [" + command + "].");
            }
        }
    }
    public void exit(int status) {
        System.exit(status);
    }

    /**
     * A class that splits a set of command-line arguments into flags,
     * variables and extra arguments.
     */
    public class CommandLineArguments {
        /**
         * The flags found on the command-line.
         */
        private Vector flags = new Vector();
        /**
         * The variables found on the command-line with their
         * corresponding values.
         */
        private HashMap variables = new HashMap();
        /**
         * The extra arguments found on the command-line.
         */
        private Vector extraArgs = new Vector();

        /**
         * The flags found on the command-line in the form in which
         * they were given.
         */
        private Vector foundFlags = new Vector();
        /**
         * The variables found on the command-line in the form in which
         * they were given.
         */
        private Vector foundVariables = new Vector();

        /**
         * Parses the given set of command-line arguments and creates a
         * new structure.
         * <p>
         * Sets of flag names and variable names are passed.
         * If <tt>foo</tt> is specified as a flag name, any argument
         * matching <tt>foo</tt> will be stored as a flag.
         * If <tt>foo</tt> is specified as a variable name, any argument
         * matching <tt>foo=value</tt> or any pair of arguments matching
         * <tt>foo value</tt> will be stored as a variable.  Any
         * remaining arguments will be stored verbatim as extra arguments.
         * <p>
         * Comparisons are <b>not</b> case sensitive.  Note that any
         * dashes expected at the beginning of a flag or variable
         * assignment must be included in the corresponding flag and
         * variable name lists.
         *
         * @param flagNames the complete set of flag names to search for.
         * Any dashes that form part of a flag should be included here.
         * @param varNames the complete set of variable names to search for.
         * Any dashes that form part of a variable assignment should be
         * included here.
         * @param args the complete set of command-line arguments to parse.
         */
        public CommandLineArguments(String[] flagNames, String[] varNames,
                String[] args) {
            int n = args.length;
            int i, j, len;
            boolean done;
            String argVarName, value;
            for (i = 0; i < n; i++) {
                done = false;

                // Is it a flag?
                if (flagNames != null) {
                    for (j = 0; j < flagNames.length; j++)
                        if (args[i].equalsIgnoreCase(flagNames[j])) {
                            if (! flags.contains(flagNames[j])) {
                                flags.add(flagNames[j]);
                                foundFlags.add(args[i]);
                            }
                            done = true;
                            break;
                        }
                    if (done)
                        continue;
                }

                // Is it a variable?
                if (varNames != null) {
                    for (j = 0; j < varNames.length; j++) {
                        if (args[i].equalsIgnoreCase(varNames[j])) {
                            if (i == n - 1)
                                error("No value was specified for parameter ["
                                    + varNames[j] + "].");
                            else {
                                foundVariables.add(args[i]);
                                i++;
                                if (variables.containsKey(varNames[j]))
                                    error("Warning:  More than one " +
                                        "value for parameter [" +
                                        varNames[j] +
                                        "] was specified on " +
                                        "the command line.  The value [" +
                                        args[i] + "] will be used.");
                                variables.put(varNames[j], args[i]);
                            }
                            done = true;
                            break;
                        }

                        len = varNames[j].length();
                        if (args[i].length() > len) {
                            argVarName = args[i].substring(0, len + 1);
                            if (argVarName.equalsIgnoreCase(
                                    varNames[j] + '=')) {
                                if (args[i].length() == len + 1)
                                    error("No value was specified for " +
                                        "parameter [" + varNames[j] + "].");
                                else {
                                    value = args[i].substring(len + 1);
                                    if (variables.containsKey(varNames[j]))
                                        error("Warning:  More than one " +
                                            "value for parameter [" +
                                            varNames[j] +
                                            "] was specified on " +
                                            "the command line.  The value [" +
                                            value + "] will be used.");
                                    variables.put(varNames[j], value);
                                    foundVariables.add(argVarName);
                                }
                                done = true;
                                break;
                            }
                        }
                    }
                    if (done)
                        continue;
                }

                // Store it as an extra argument.
                extraArgs.add(args[i]);
            }
        }

        /**
         * Returns a list of all flags specified by the user on the
         * command-line.
         * <p>
         * Only flags whose names were passed in the <i>flagNames</i>
         * argument to the constructor and that were specified on the
         * command-line will be returned.
         * <p>
         * The flags returned will have the same capitalisation as those
         * passed in <i>flagNames</i> to the constructor, and will
         * include any dashes that form part of a flag.
         *
         * @return a list of all flags specified by the user.
         *
         * @see #getFlagsGiven()
         */
        public Vector getFlags() {
            return flags;
        }
        /**
         * Returns a map describing all variables specified by the user
         * on the command-line.
         * <p>
         * Only variables whose names were passed in the <i>varNames</i>
         * argument to the constructor and that were specified on the
         * command-line will be returned.
         * <p>
         * The variable names returned will have the same capitalisation as
         * those passed in <i>varNames</i> to the constructor, and will
         * include any dashes that form part of a variable assignment.
         *
         * @return a map describing all variables specified by the user,
         * with the variable names as keys and the variable values as the
         * corresponding values in the map.
         *
         * @see #getVariablesGiven()
         */
        public HashMap getVariables() {
            return variables;
        }
        /**
         * Returns a list of all extra arguments specified by the user
         * on the command-line.
         * <p>
         * Any argument that does not match a flag passed in the
         * <i>flagNames</i> parameter to the constructor or is not
         * involved in assigning a value to a variable passed in the
         * <i>varNames</i> parameter to the constructor is considered an
         * extra argument.
         *
         * @return a list of all extra arguments specified by the user.
         */
        public Vector getExtraArgs() {
            return extraArgs;
        }

        /**
         * Returns a list of all flags specified by the user on the
         * command-line, in the form in which the user gave them.
         * <p>
         * This routine behaves like <tt>getFlags()</tt>, except that
         * the flags returned will have the capitalisation specified by
         * the user.
         *
         * @return a list of all flags specified by the user, in the
         * form in which the user gave them.
         * 
         * @see #getFlags()
         */
        public Vector getFlagsGiven() {
            return foundFlags;
        }
        /**
         * Returns the names of all variables specified by the user on the
         * command-line, in the form in which the user gave them.
         * <p>
         * The list returned will contain the name of each variable
         * specified by the user on the command-line.  The names
         * returned will have the capitalisation specified by the user, and
         * will include any dashes that form part of a variable assignment.
         * <p>
         * If a variable was specified using the syntax
         * <tt>var=value</tt>, the corresponding variable name will have
         * an equal sign (<tt>=</tt>) appended to its name.  If the
         * variable was specified using the syntax <tt>var value</tt>,
         * the corresonding variable name will be left untouched.
         *
         * @return the names of all variables specified by the user, in the
         * form in which the user gave them.
         * 
         * @see #getVariables()
         */
        public Vector getVariablesGiven() {
            return foundVariables;
        }
    }
}

