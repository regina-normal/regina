
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

package normal;

import java.applet.Applet;
import java.awt.Frame;
import java.awt.event.*;
import java.io.*;
import java.lang.reflect.*;
import java.util.Properties;
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
    public String getParameter(String paramName, int nMinus,
            boolean useEquals, boolean caseSensitive,
            String paramDescription) {
        // Build the prefix for which we are hunting.
        StringBuffer prefix = new StringBuffer();
        int i;
        for (i = 0; i < nMinus; i++)
            prefix.append('-');
        prefix.append(paramName);

        // Hunt for the prefix.
        String value = null;
        boolean thisIsIt;
        if (useEquals) {
            // Hunt for "prefix=value".
            prefix.append('=');
            String prefixString = prefix.toString();
            int prefixLen = prefixString.length();

            for (i = 0; i < args.length; i++)
                if (args[i].length() >= prefixLen) {
                    if (caseSensitive)
                        thisIsIt = args[i].substring(0, prefixLen).
                            equals(prefixString);
                    else
                        thisIsIt = args[i].substring(0, prefixLen).
                            equalsIgnoreCase(prefixString);
                    if (thisIsIt) {
                        if (value == null)
                            value = args[i].substring(prefixLen);
                        else
                            error("Warning:  More than one " +
                                paramDescription +
                                " was specified on the command line.  " +
                                "The value [" + value + "] will be used.");
                    }
                }
        } else {
            // Hunt for "prefix value".
            String prefixString = prefix.toString();
            for (i = 0; i < args.length; i++) {
                if (caseSensitive)
                    thisIsIt = args[i].equals(prefixString);
                else
                    thisIsIt = args[i].equalsIgnoreCase(prefixString);
                if (thisIsIt) {
                    if (i < args.length - 1) {
                        if (value == null)
                            value = args[i + 1];
                        else
                            error("Warning:  More than one " +
                                paramDescription +
                                " was specified on the command line.  " +
                                "The value [" + value + "] will be used.");
                        i++;
                    } else
                        error("No value was specified for parameter [" +
                            prefixString + "].  No value will be assigned " +
                            "to the " + paramDescription + ".");
                }
            }
        }
        return value;
    }

    public int getUIType() {
        int uiType = unspecified;

        // Look up the command-line parameters.
        for (int i = 0; i < args.length; i++) {
            if (args[i].equalsIgnoreCase("--gui")) {
                if (uiType != unspecified && uiType != UIFullGUI) {
                    error("More than one UI type has been requested.");
                    return invalid;
                } else
                    uiType = UIFullGUI;
            } else if (args[i].equalsIgnoreCase("--console")) {
                if (uiType != unspecified && uiType != UIConsoleWindow) {
                    error("More than one UI type has been requested.");
                    return invalid;
                } else
                    uiType = UIConsoleWindow;
            } else if (args[i].equalsIgnoreCase("--text")) {
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
        for (int i = 0; i < args.length; i++) {
            if (args[i].equalsIgnoreCase("--jni")) {
                if (engineType != unspecified && engineType != engineJNI) {
                    error("More than one style of engine access has been " +
                        "requested.");
                    return invalid;
                } else
                    engineType = engineJNI;
            } else if (args[i].equalsIgnoreCase("--corba")) {
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
    public boolean mayAccessFiles() {
        return (getEngine().style() == engineJNI);
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
}

