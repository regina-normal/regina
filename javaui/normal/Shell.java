
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

package normal;

import java.applet.Applet;
import java.awt.*;
import java.io.*;
import java.lang.reflect.*;
import java.net.URL;
import java.util.*;
import javax.swing.*;
import normal.console.*;
import normal.engine.Engine;
import normal.mainui.NormalFrame;
import normal.options.NormalOptionSet;
import org.gjt.btools.gui.Positioner;
import org.gjt.btools.gui.lookandfeel.LookAndFeelItem;

/**
 * Contains the primary program execution routine <tt>run()</tt>.
 * Abstract methods are presented whose implementations depend upon
 * whether we are running as an applet or as a standalone application.
 *
 * @see #run
 */
public abstract class Shell {
    /**
     * Represents an option whose value has not been specified.
     */
    public static final int unspecified = 0;
    /**
     * Represents an invalue value for an option.
     */
    public static final int invalid = -1;

    /**
     * Represents a program shell for a standalone application.
     */
    public static final int shellApplication = 1;
    /**
     * Represents a program shell for an applet.
     */
    public static final int shellApplet = 2;
    
    /**
     * Specifies a full graphical user interface.
     */
    public static final int UIFullGUI = 11;
    /**
     * Specifies a console window user interface.
     */
    public static final int UIConsoleWindow = 12;
    /**
     * Specifies a text console user interface.
     */
    public static final int UITextConsole = 13;

    /**
     * Specifies an engine accessed as a shared library using JNI.
     */
    public static final int engineJNI = 21;

    /**
     * The calculation engine currently in use.
     */
    private Engine engine;
    /**
     * The current user options for the program.
     */
    private NormalOptionSet options;
    /**
     * The directory in which options files are stored.
     */
    private File optionsDir = null;

    /**
     * The primary frame of the user interface.
     */
    private Frame primaryFrame = null;

    /**
     * A list of open Jython consoles.
     */
    private Vector openConsoles = new Vector();

    /**
     * Have we successfully located the Jython classes?
     */
    private boolean foundJython = false;

    /**
     * Have we successfully located the JavaHelp classes?
     */
    private boolean foundJavaHelp = false;
    /**
     * The help broker for use with the documentation system.
     * This is really of class <tt>javax.help.HelpBroker</tt> but is
     * stored as an object to avoid problems if the JavaHelp classes
     * cannot be located.
     */
    private Object helpBroker = null;

    /**
     * Returns the calculation engine currently in use.
     *
     * @return the calculation engine.
     */
    public Engine getEngine() {
        return engine;
    }
    /**
     * Returns the current user options for the program.
     *
     * @return the current user options.
     */
    public NormalOptionSet getOptions() {
        return options;
    }
    /**
     * Returns the directory in which options files are placed.
     *
     * @return the options files directory.
     */
    public File getOptionsDir() {
        if (optionsDir != null)
            return optionsDir;

        // Try to read the corresponding system property.
        String optionsDirName = NormalOptionSet.getSystemProperty(
            Application.optionsDirProperty);
        if (optionsDirName == null || optionsDirName.length() == 0)
            optionsDirName = Application.optionsDirDefault;
 
        // Create the actual file object.
        optionsDir = new File(optionsDirName);
        try {
            optionsDir = optionsDir.getCanonicalFile();
        } catch (IOException exc) {
        }

        return optionsDir;
    }

    /**
     * Adds the given console to the list of all open Jython consoles.
     * This should only be called from within the
     * <tt>JPythonConsoleFrame</tt> constructor.
     *
     * @param console the console to add; this should not currently be
     * in the list.
     *
     * @see #unregisterConsole
     */
    public void registerConsole(JPythonConsoleFrame console) {
        openConsoles.addElement(console);
    }

    /**
     * Removes the given console from the list of all open Jython consoles.
     * This should only be called from within
     * <tt>JPythonConsoleFrame.closeConsole()</tt>.
     *
     * @param console the console to remove; this should currently
     * belong to the list.
     *
     * @see #registerConsole
     */
    public void unregisterConsole(JPythonConsoleFrame console) {
        openConsoles.removeElement(console);
    }

    /**
     * Are there currently any open Jython consoles?
     *
     * @return <tt>true</tt> if and only if there is at least one open
     * console.
     */
    public boolean hasOpenConsoles() {
        return ! openConsoles.isEmpty();
    }

    /**
     * Returns the list of all open Jython consoles.
     *
     * @return the list of all open consoles.
     */
    public Enumeration getConsoles() {
        return openConsoles.elements();
    }

    /**
     * Returns one of the currently open Jython consoles.
     *
     * @return an open Jython console, or <tt>null</tt> if there are no
     * open consoles.
     */
    public JPythonConsoleFrame getConsole() {
        return (openConsoles.isEmpty() ? null :
            (JPythonConsoleFrame)openConsoles.elementAt(0));
    }

    /**
     * Have we successfully located the Jython classes?
     *
     * @return <tt>true</tt> if and only if we have successfully located
     * the Jython classes.
     */
    public boolean hasFoundJython() {
        return foundJython;
    }
    /**
     * Have we successfully located the JavaHelp classes?
     *
     * @return <tt>true</tt> if and only if we have successfully located
     * the JavaHelp classes.
     */
    public boolean hasFoundJavaHelp() {
        return foundJavaHelp;
    }
    /**
     * Have we successfully located both the documentation jar and the
     * JavaHelp classes?
     *
     * @return <tt>true</tt> if and only if we have successfully located
     * both the documentation jar and the JavaHelp classes.
     */
    public boolean hasFoundDocumentation() {
        return (helpBroker != null);
    }
    /**
     * Returns the help broker for use with the documentation system.
     * This is really of class <tt>javax.help.HelpBroker</tt> but is
     * returned as an object to avoid problems if the JavaHelp classes
     * cannot be located.
     *
     * @return the help broker, or <tt>null</tt> if the JavaHelp classes
     * or the documentation jar are unavailable.
     */
    public Object getHelpBroker() {
        return helpBroker;
    }

    /**
     * Returns the primary frame of the user interface.
     *
     * @return the primary frame, or <tt>null</tt> if there is no
     * primary frame.
     */
    public Frame getPrimaryFrame() {
        return primaryFrame;
    }

    /**
     * Returns the <tt>NormalFrame</tt> in which the user is working.
     *
     * @return the <tt>NormalFrame</tt>, or <tt>null</tt> if there is no
     * such frame.
     * @see normal.mainui.NormalFrame
     */
    public NormalFrame getNormalFrame() {
        if (primaryFrame instanceof NormalFrame)
            return (NormalFrame)primaryFrame;
        else
            return null;
    }

    /**
     * Returns the specific style of program shell in use.
     * This must be one of the shell constants defined in this class.
     */
    public abstract int getShellStyle();
    /**
     * Returns a list of command-line parameters to the program.
     *
     * @return the command-line parameters, or <tt>null</tt> if these
     * are unavailable.
     */
    public abstract String[] getCommandLineParameters();
    /**
     * Returns an applet object containing applet parameters for the
     * program.
     *
     * @return the object containing applet parameters, or <tt>null</tt>
     * if such an object is unavailable.
     */
    public abstract Applet getAppletParameters();
    /**
     * Returns the requested explicit program parameter.  An explicit
     * program parameter is a command-line parameter or an applet
     * parameter.
     * <p>
     * For example, if the value of parameter <tt>foo</tt> is specified
     * on the command line using the syntax <tt><u>--foo=value</u></tt>, the
     * string <tt>value</tt> can be extracted by calling
     * <tt>getParameter("foo", 2)</tt>.
     * <p>
     * If the value of parameter <tt>foo</tt> is specified for an applet
     * by the applet parameter <tt>foo</tt>, the same function call will
     * retrieve the parameter value, since argument <i>nMinus</i>,
     * is irrelevant to applet parameter specifications.
     *
     * @param paramName the parameter name.
     * @param nMinus the number of minus signs preceding the parameter
     * name; this must be non-negative.
     * This argument may be ignored by the implementation of this
     * routine if it is not relevant to the style of parameter
     * specification used by this type of shell.
     * @return the requested parameter, or <tt>null</tt> if this
     * parameter was not specified as an explicit program parameter.
     */
    public abstract String getParameter(String paramName, int nMinus);
    /**
     * Returns all filenames that were passed as program parameters.
     * These are the files that the user has requested be opened on
     * startup.  They are generally specified by command-line parameters
     * or applet parameters.
     *
     * @return a vector of strings representing all filename parameters.
     */
    public abstract Vector getFileParameters();

    /**
     * Returns the style of user interface that should be used.
     * This will be one of the user interface constants
     * defined in this class, or will be <tt>invalid</tt>.
     *
     * @return the style of user interface to use.
     */
    public abstract int getUIType();
    /**
     * Returns the style of engine access that should be used.
     * This will be one of the engine access constants
     * defined in this class, or will be <tt>invalid</tt>.
     *
     * @return the style of engine access to use.
     */
    public abstract int getEngineType();
    /**
     * Should we show a splash screen while the program is initialising?
     *
     * @return <tt>true</tt> if and only if we should show a splash
     * screen.
     */
    public abstract boolean shouldShowSplashScreen();
    /**
     * May the user interface access the local filesystem on its own?
     *
     * @return <tt>true</tt> if and only if the UI has local file access.
     */
    public abstract boolean mayUIAccessFiles();
    /**
     * May the user interface ask the engine to access the local filesystem?
     *
     * @return <tt>true</tt> if and only if the engine has local file access.
     */
    public abstract boolean mayEngineAccessFiles();
    /**
     * Are we even allowed to try to locate the Jython classes?
     *
     * @return <tt>true</tt> if and only if we are allowed to attempt to
     * locate Jython.
     */
    public abstract boolean mayUseJython();
    /**
     * Loads the user options for the program.
     *
     * @return the user options for the program, or <tt>null</tt> if
     * they could not be loaded.
     */
    public abstract NormalOptionSet loadOptionSet();
    /**
     * Display the given information message to the user.
     *
     * @param message the message to display.
     */
    public abstract void inform(String message);
    /**
     * Offers the user the given yes/no question.
     *
     * @return <tt>true</tt> if the user answered yes, or <tt>false</tt>
     * if the user answered no.
     */
    public abstract boolean confirm(String message);
    /**
     * Display the given error message to the user.
     *
     * @param message the message to display.
     */
    public abstract void error(String message);
    /**
     * Displays the given help topic in the help system.
     *
     * @param id the ID of the help topic to display, or <tt>null</tt>
     * if help should be opened at the default page.
     */
    public abstract void viewHelp(String id);
    /**
     * Exits the program entirely.
     * It is assumed that all windows have already been disposed and we
     * are about to exit from any routines belonging to the program that
     * are still running.
     *
     * @param status the exit status to report to the system if
     * applicable.
     */
    public abstract void exit(int status);

    /**
     * The primary program execution routine.
     * An engine is loaded and a new GUI frame or Jython console is
     * created in which the user can work.
     *
     * @see normal.mainui.NormalFrame
     */
    public void run() {
        // Check the JDK and BTools versions.
        {
            boolean btoolsOK = true;
            try {
                if (org.gjt.btools.Version.btoolsVersion.compareTo(
                        Application.minimumBToolsVersion) < 0)
                    btoolsOK = false;

                String jdkVersion = org.gjt.btools.Version.javaVersion();
                if (jdkVersion != null)
                    if (jdkVersion.compareTo(
                            Application.minimumJDKVersion) < 0) {
                        error("You are using Java version " +
                            jdkVersion + ", which is too old to run "
                            + Application.program + ".  At least version " +
                            Application.minimumJDKVersion +
                            " is required.  Please download a newer" +
                            " version of Java (or of your web browser).");
                        exit(1);
                        return;
                    }
            } catch (Throwable th) {
                th.printStackTrace();
                btoolsOK = false;
            }

            if (! btoolsOK) {
                error("You are using BTools version " +
                    org.gjt.btools.Version.btoolsVersion +
                    ", which is too old to run " + Application.program +
                    ".  At least version " +
                    Application.minimumBToolsVersion +
                    " is required.  Please download a newer version of " +
                    "BTools.");
                exit(1);
                return;
            }
        }

        // Find out what kind of UI is desired.
        int uiType = getUIType();
        if (uiType == invalid) {
            exit(1);
            return;
        }

        // Bring up a splash screen if appropriate.
        JFrame splash;
        if (shouldShowSplashScreen()) {
            splash = new JFrame(Application.program);
            splash.getContentPane().setLayout(new BorderLayout());
            splash.getContentPane().add(new JLabel(
                Images.mainTitle.finalImage()), BorderLayout.CENTER);
            splash.pack();
            splash.setIconImage(Images.mainSmallIcon.image().getImage());
            Positioner.centerOnScreen(splash);
            splash.show();
        } else
            splash = null;

        // Set the Jython options.
        if (mayUseJython()) {
            try {
                org.python.core.Options.showJavaExceptions = false;
                //org.python.core.Options.classBasedExceptions = false;
                org.python.core.PySystemState.initialize();
                foundJython = true;
            } catch (Throwable th) {}
        }

        // Open up the help set.
        try {
            // Doesn't matter if a null pointer exception is raised in
            // the following code, due to the try-catch blocks.

            // Find the HelpSet class.
            Class cHelpSet = ClassLoader.getSystemClassLoader().loadClass(
                "javax.help.HelpSet");

            // Get the URL of the documentation help set.
            Class[] findParams = {
                java.lang.ClassLoader.class, java.lang.String.class
            };
            Method method = cHelpSet.getMethod("findHelpSet", findParams);
            Object[] findArgs = {
                null, Application.helpSetLoc
            };
            URL url = (URL)method.invoke(null, findArgs);
                
            foundJavaHelp = true;

            if (url != null) {
                // Create a new help set.
                Class[] createParams = {
                    java.lang.ClassLoader.class, java.net.URL.class
                };
                Constructor construct = cHelpSet.getConstructor(createParams);
                Object[] createArgs = {
                    null, url
                };
                Object helpSet = construct.newInstance(createArgs);

                // Create the help broker.
                method = helpSet.getClass().getMethod("createHelpBroker", null);
                helpBroker = method.invoke(helpSet, null);
            }
        } catch (Throwable th) {
        }

        // Read in the option set.
        options = loadOptionSet();
        if (options == null)
            options = new NormalOptionSet();

        // Attempt to access an engine.
        engine = new EngineLoader(this).loadEngine();
        if (engine == null) {
            if (splash != null)
                splash.dispose();
            exit(1);
            return;
        }

        // Test the engine.
        {
            boolean badEngine = false;
            try {
                if (engine.testEngine(3) != 3) {
                    badEngine = true;
                    System.err.println(
                        "Routine testEngine() returned an incorrect result.");
                }
            } catch (Throwable th) {
                badEngine = true;
                th.printStackTrace();
            }
            if (badEngine) {
                error("The calculation engine has been loaded but does " +
                    "not appear to be working properly.  Details of the " +
                    "problem have been written to the Java console.\n" +
                    "The engine is " + engine.styleDescription() + '.');
                if (splash != null)
                    splash.dispose();
                exit(1);
                return;
            }
        }

        // Test the engine version.
        {
            boolean wrongEngine = false;
            String engineVersion = "unknown";
            try {
                engineVersion = engine.getVersionString();
                if (! engineVersion.equals(Application.version))
                    wrongEngine = true;
            } catch (Throwable th) {
                wrongEngine = true;
                th.printStackTrace();
            }

            if (wrongEngine) {
                error("The calculation engine is version " + engineVersion +
                    " but this interface is version " + Application.version +
                    ".  Please download a matching engine and/or interface.");
                if (splash != null)
                    splash.dispose();
                exit(1);
                return;
            }
        }

        // Run the program in whatever manner is appropriate for the
        // requested UI type.
        switch (uiType) {
            case UITextConsole:
                if (splash != null)
                    splash.dispose();
                if (foundJython) {
                    normal.console.ConsoleUtils.runTextConsole(this);
                    exit(0);
                    return;
                } else {
                    error("Jython could not be found.\n" +
                        "Please ensure [jython.jar] is on your classpath.");
                    exit(1);
                    return;
                }
            case UIConsoleWindow:
            case UIFullGUI:
                // Set the look and feel.
                // If there is no look and feel, set it to motif since
                // it's nicer than metal imho.
                String lookAndFeel = options.getStringOption("LookAndFeel");
                if (lookAndFeel != null) {
                    try {
                        UIManager.setLookAndFeel(lookAndFeel);
                    } catch (Throwable th) {
                    }
                } else {
                    // Try for motif.
                    LookAndFeelItem[] allLFs =
                        LookAndFeelItem.getInstalledLookAndFeels();
                    for (int i = 0; i < allLFs.length; i++)
                        if (allLFs[i].toString().indexOf("otif") >= 0)
                            if (allLFs[i].setLookAndFeel())
                                break;
                }

                // Create the frame in which to work.
                if (uiType == UIFullGUI) {
                    // Create the primary frame.
                    NormalFrame frame = new NormalFrame(this);
                    Positioner.centerOnScreen(frame);

                    // Present the primary frame to the user.
                    if (splash != null)
                        splash.dispose();
                    frame.show();
                    primaryFrame = frame;

                    // Open all files passed as program parameters.
                    if (mayEngineAccessFiles()) {
                        Enumeration e = getFileParameters().elements();
                        while (e.hasMoreElements())
                            frame.fileOpen(new File((String)e.nextElement()));
                    } else if (getFileParameters().size() > 0)
                        error("This particular interface/engine " +
                            "combination does not allow files to be " +
                            "opened.  Try starting Regina with different " +
                            "options.");
                } else {
                    // Console window UI:
                    if (foundJython) {
                        JPythonConsoleFrame frame =
                            ConsoleUtils.createGraphicalConsole(this, true);
                        if (splash != null)
                            splash.dispose();
                        Positioner.centerOnScreen(frame);
                        frame.show();
                        primaryFrame = frame;
                        frame.startConsole();
                    } else {
                        error("Jython could not be found.  Please ensure " +
                            "[jython.jar] is on your classpath.");
                        if (splash != null)
                            splash.dispose();
                        exit(1);
                        return;
                    }
                }

                // All done.
                return;
            default:
                if (splash != null)
                    splash.dispose();
                exit(1);
                return;
        }
    }

    /**
     * Returns the location of the help page correpsonding to the given help
     * topic.  The location will be the path (with components separated by
     * forward slashes) to a resource relative to the base documentation
     * directory.  An example is "options.html" or "engine/index.html".
     * The path will contain no leading slash.
     * <p>
     * The current implementation simply appends <tt>.html</tt> to the
     * topic ID.
     * <p>
     * If the topic ID is <tt>null</tt>, the location of a
     * suitable default page will be returned instead.
     * 
     * @param id the ID of the help topic to find, or <tt>null</tt>
     * if the location of the default page should be returned instead.
     * @return the location of the requested help page.
     */
    public String getHelpPageResource(String id) {
        return (id == null ? Application.defaultHelpPage : id + ".html");
    }
}

