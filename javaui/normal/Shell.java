
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

package normal;

import java.applet.Applet;
import java.awt.*;
import java.io.*;
import java.net.URL;
import java.util.Properties;
import javax.swing.*;
import normal.console.ConsoleUtils;
import normal.engine.Engine;
import normal.mainui.NormalFrame;
import normal.options.NormalOptionSet;
import btools.gui.Positioner;
import btools.gui.lookandfeel.LookAndFeelItem;

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
     * Specifies an engine accessed as an external server application
     * using CORBA.
     */
    public static final int engineCORBA = 22;

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
     * Have we successfully located the JPython classes?
     */
    private boolean foundJPython = false;

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
        String optionsDirName = null;
        try {
            optionsDirName = System.getProperty(Application.optionsDirProperty);
        } catch (Throwable th) {
        }
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
     * Have we successfully located the JPython classes?
     *
     * @return <tt>true</tt> if and only if we have successfully located
     * the JPython classes.
     */
    public boolean hasFoundJPython() {
        return foundJPython;
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
     * <tt>getParameter("foo", 2, true, true, "my favourite string")</tt>.
     * <p>
     * If the value of parameter <tt>foo</tt> is specified for an applet
     * by the applet parameter <tt>foo</tt>, the same function call will
     * retrieve the parameter value, since arguments <i>nMinus</i>,
     * <i>useEquals</i> and <i>caseSensitive</i> are irrelevant to
     * applet parameter specifications.
     *
     * @param paramName the parameter name.
     * @param nMinus the number of minus signs preceding the parameter
     * name; this must be non-negative.
     * This argument may be ignored by the implementation of this
     * routine if it is not relevant to the style of parameter
     * specification used by this type of shell.
     * @param useEquals <tt>true</tt> if the parameter is specified using
     * the form <tt><u>name=value</u></tt>, or <tt>false</tt> if the parameter
     * is specified using the form <tt><u>name value</u></tt>.
     * This argument may be ignored by the implementation of this
     * routine if it is not relevant to the style of parameter
     * specification used by this type of shell.
     * @param caseSensitive <tt>true</tt> if and only if the parameter
     * name is case sensitive.
     * This argument may be ignored by the implementation of this
     * routine if it is not relevant to the style of parameter
     * specification used by this type of shell.
     * @param paramDescription a description of what this parameter
     * represents.
     * @return the requested parameter, or <tt>null</tt> if this
     * parameter was not specified as an explicit program parameter.
     */
    public abstract String getParameter(String paramName, int nMinus,
        boolean useEquals, boolean caseSensitive, String paramDescription);

    /**
     * Returns the style of user interface that should be used.
     * This will be one of the user interface constants
     * defined in this class, or will be <tt>invalid</tt>.
     *
     * @param runtimeOptions the runtime options as set by the RegConf
     * configuration utility.  This parameter may be <tt>null</tt>.
     * @return the style of user interface to use.
     */
    public abstract int getUIType(Properties runtimeOptions);
    /**
     * Returns the style of engine access that should be used.
     * This will be one of the engine access constants
     * defined in this class, or will be <tt>invalid</tt>.
     *
     * @param runtimeOptions the runtime options as set by the RegConf
     * configuration utility.  This parameter may be <tt>null</tt>.
     * @return the style of engine access to use.
     */
    public abstract int getEngineType(Properties runtimeOptions);
    /**
     * Should we show a splash screen while the program is initialising?
     *
     * @return <tt>true</tt> if and only if we should show a splash
     * screen.
     */
    public abstract boolean shouldShowSplashScreen();
    /**
     * Are we even allowed to try to locate the JPython classes?
     *
     * @return <tt>true</tt> if and only if we are allowed to attempt to
     * locate JPython.
     */
    public abstract boolean mayUseJPython();
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
     * An engine is loaded and a new GUI frame or JPython console is
     * created in which the user can work.
     *
     * @see normal.mainui.NormalFrame
     */
    public void run() {
        // Check the JDK and BTools versions.
        {
            boolean btoolsOK = true;
            try {
                if (btools.Version.btoolsVersion.compareTo(
                        Application.minimumBToolsVersion) < 0)
                    btoolsOK = false;

                String jdkVersion = btools.Version.javaVersion();
                if (jdkVersion != null)
                    if (jdkVersion.compareTo(
                            Application.minimumJDKVersion) < 0) {
                        error("Your web browser is using Java version " +
                            jdkVersion + ", which is too old to run "
                            + Application.program + ".  At least version " +
                            Application.minimumJDKVersion +
                            " is required.  Please download a newer" +
                            " version of your web browser.");
                        exit(1);
                        return;
                    }
            } catch (Throwable th) {
                th.printStackTrace();
                btoolsOK = false;
            }

            if (! btoolsOK) {
                error("You are using BTools version " +
                    btools.Version.btoolsVersion +
                    ", which is too old to run " + Application.program +
                    ".  At least version " +
                    Application.minimumBToolsVersion +
                    " is required.  Please download a newer version of " +
                    "BTools.");
                exit(1);
                return;
            }
        }

        // Read in the runtime options.
        Properties runtimeOptions = null;
        try {
            FileInputStream in = new FileInputStream(
                new File(getOptionsDir(), Application.runtimeOptionsFile));
            runtimeOptions = new Properties();
            runtimeOptions.load(in);
            in.close();
        } catch (Throwable th) {
            runtimeOptions = null;
        }

        // Find out what kind of UI is desired.
        int uiType = getUIType(runtimeOptions);
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

        // Set the JPython options.
        if (mayUseJPython()) {
            try {
                org.python.core.Options.showJavaExceptions = true;
                org.python.core.Options.classBasedExceptions = false;
                foundJPython = true;
            } catch (Throwable th) {}
        }

        // Open up the help set.
        try {
            URL url = javax.help.HelpSet.findHelpSet(null,
                Application.helpSetLoc);
            foundJavaHelp = true;
            // Doesn't matter if a null pointer exception is raised in
            // the following lines, due to the try-catch blocks.
            if (url != null)
                helpBroker =
                    new javax.help.HelpSet(null, url).createHelpBroker();
        } catch (Throwable th) {
        }

        // Read in the option set.
        options = loadOptionSet();
        if (options == null)
            options = new NormalOptionSet();

        // Attempt to access an engine.
        engine = new EngineLoader(this, runtimeOptions).loadEngine();
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
                if (foundJPython) {
                    ConsoleUtils.runTextConsole(engine);
                    exit(0);
                    return;
                } else {
                    error("JPython could not be found.\n" +
                        "Please ensure [jpython.jar] is on your classpath.");
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
                    NormalFrame frame = new NormalFrame(this);
                    Positioner.centerOnScreen(frame);
                    if (splash != null)
                        splash.dispose();
                    frame.show();
                    primaryFrame = frame;
                } else {
                    // Console window UI:
                    if (foundJPython) {
                        normal.console.JPythonConsoleFrame frame =
                            new normal.console.JPythonConsoleFrame(this, true);
                        frame.startConsole(ConsoleUtils.consoleGreeting() +
                            "\n\n" +
                            ConsoleUtils.consoleSetup(
                                frame.getPythonInterpreter(), engine) +
                            "\n\n" +
                            org.python.util.InteractiveConsole.
                                getDefaultBanner() + '\n');
                        if (splash != null)
                            splash.dispose();
                        Positioner.centerOnScreen(frame);
                        frame.show();
                        primaryFrame = frame;
                    } else {
                        error("JPython could not be found.  Please ensure " +
                            "[jpython.jar] is on your classpath.");
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
     * forward slashes) to a resource belonging to the documentation
     * jar.  An example is "normal/docs/options.html".  The path will
     * contain no leading slash.
     * <p>
     * If the given help topic ID cannot be resolved, the location of a
     * suitable default page will be returned instead.
     * 
     * @param id the ID of the help topic to find, or <tt>null</tt>
     * if the location of the default page should be returned instead.
     * @return the location of the requested help page.
     */
    public String getHelpPageResource(String id) {
        String path = null;
        if (id != null) {
            try {
                javax.help.HelpSet set =
                    ((javax.help.HelpBroker)helpBroker).getHelpSet();
                path = set.getCombinedMap().getURLFromID(
                    javax.help.Map.ID.create(id, set)).getPath();
            
                // Remove any leading "jar!".
                int pos = path.lastIndexOf("jar!");
                if (pos >= 0) {
                    path = path.substring(pos + 4);
                    if (path.length() == 0)
                        path = null;
                } else
                    path = null;

                // Remove any leading '/'.
                if (path != null)
                    if (! Character.isLetterOrDigit(path.charAt(0)))
                        path = path.substring(1);
            } catch (Throwable th) {
            }
        }

        if (path == null)
            path = Application.defaultHelpPage;
        return path;
    }
}

