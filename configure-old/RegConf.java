
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Configuration utility                                                 *
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

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;

/**
 * A configuration utility for setting runtime options for Regina.
 * This class serves as both the application class and the primary
 * dialog class.
 */
public class RegConf extends JDialog implements ActionListener {
    /**
     * The option set containing all runtime options.
     */
    private static Properties options;
    /**
     * The file in which runtime options will be stored.
     */
    private static File optionsFile;
    /**
     * The directory containing the runtime options file.
     */
    private static File optionsDir;
	/**
	 * The system property that tells us in which directory
	 * options files are placed.
	 */
	private static final String optionsDirProperty = "REGINA_OPTIONS";
	/**
	 * The directory to use for options files if system property
	 * <i>optionsDirProperty</i> does not exist or is inaccessible.
	 */
	private static final String optionsDirDefault = ".";
    /**
     * The name of the runtime options file, without directory
     * information.
     */
    private static final String optionsFileName = "runtime.opt";

    /**
     * The name of the general Java UI field.
     */
    private static final String javaUIName = "regina.jar";
    /**
     * The name of the JNI Java UI field.
     */
    private static final String javaJNIName = "regina-jni.jar";
    /**
     * The name of the CORBA Java UI field.
     */
    private static final String javaCORBAName = "regina-corba.jar";
    /**
     * The name of the documentation jar field.
     */
    private static final String docjarName = "regina-docs.jar";
    /**
     * The name of the Java command field.
     */
    private static final String javaName = "Java command";

    /**
     * The name of the JNI engine directory field.
     */
    private static final String JNIDirName = "JNI engine directory";

    /**
     * The name of the BTools field.
     */
    private static final String btoolsName = "btools.jar";
    /**
     * The name of the Jython field.
     */
    private static final String jythonName = "jython.jar";
    /**
     * The name of the JavaHelp field.
     */
    private static final String javahelpName = "jh.jar";
    /**
     * The name of the extra classes field.
     */
    private static final String extraclassesName = "Extra classes";

    /**
     * The name of the interface field.
     */
    private static final String interfaceStyleName = "Interface";
    /**
     * The name of the engine style field.
     */
    private static final String engineStyleName = "Engine";
    /**
     * The name of the JNI engine name field.
     */
    private static final String JNIEngineName = "JNI engine name";
    /**
     * The name of the CORBA engine host field.
     */
    private static final String CORBAHostName = "CORBA engine host";
    /**
     * The name of the CORBA engine port field.
     */
    private static final String CORBAPortName = "CORBA engine port";

    /**
     * The general Java UI field.
     */
    private JTextField javaUI;
    /**
     * The JNI Java UI field.
     */
    private JTextField javaJNI;
    /**
     * The CORBA Java UI field.
     */
    private JTextField javaCORBA;
    /**
     * The documentation jar field.
     */
    private JTextField docjar;
    /**
     * The Java command field.
     */
    private JTextField java;

    /**
     * The JNI engine directory field.
     */
    private JTextField JNIDir;

    /**
     * The BTools field.
     */
    private JTextField btools;
    /**
     * The Jython field.
     */
    private JTextField jython;
    /**
     * The JavaHelp field.
     */
    private JTextField javahelp;
    /**
     * The extra classes field.
     */
    private JTextField extraclasses;

    /**
     * The interface field.
     */
    private JComboBox interfaceStyle;
    /**
     * The engine style field.
     */
    private JComboBox engineStyle;
    /**
     * The JNI engine name field.
     */
    private JTextField JNIEngine;
    /**
     * The CORBA engine host field.
     */
    private JTextField CORBAHost;
    /**
     * The CORBA engine port field.
     */
    private JTextField CORBAPort;

    /**
     * Are we explicitly specifying the interface field?
     */
    private JCheckBox interfaceStyleUse;
    /**
     * Are we explicitly specifying the engine style field?
     */
    private JCheckBox engineStyleUse;
    /**
     * Are we explicitly specifying the JNI engine name field?
     */
    private JCheckBox JNIEngineUse;
    /**
     * Are we explicitly specifying the CORBA engine host field?
     */
    private JCheckBox CORBAHostUse;
    /**
     * Are we explicitly specifying the CORBA engine port field?
     */
    private JCheckBox CORBAPortUse;

    /**
     * The last directory used for browsing through files.
     */
    private File lastDir;

    /**
     * Notifies the user that a file system error occurred, and exits
     * the program.
     */
    public static void fileSystemError() {
        JOptionPane.showMessageDialog(null,
            "An error occurred whilst querying the file system.",
            "Error", JOptionPane.ERROR_MESSAGE);
        System.exit(1);
    }

    /**
     * Runs the entire configuration application.
     */
    public static void main(String[] args) {
        // Find the full pathname of the options directory.
		String optionsDirName = null;
		try {
			optionsDirName = System.getProperty(optionsDirProperty);
		} catch (Throwable th) {
		}
		if (optionsDirName == null || optionsDirName.length() == 0)
			optionsDirName = optionsDirDefault;

        try {
            optionsDir = new File(optionsDirName).getCanonicalFile();
        } catch (IOException exc) {
            fileSystemError();
        }

        // Check that this is a directory.  Try to create it if it doesn't
		// exist.
		String optionsDirError = null;
		try {
			if (! optionsDir.exists()) {
				try {
					if (! optionsDir.mkdir()) {
						optionsDirError =
							"This directory could not be created.";
					}
				} catch (SecurityException sex) {
					optionsDirError =
						"You do not have permissions to create this directory.";
				}
			} else if (! optionsDir.isDirectory()) {
				optionsDirError = "This is a file, not a directory.";
			}
		} catch (SecurityException sex) {
			optionsDirError =
				"You do not have permissions to query this directory.";
		}

        if (optionsDirError != null) {
            Object[] messages = {
                "Options are being stored in directory ["
					+ optionsDir.toString() + "].",
				optionsDirError,
				"You can set the options directory manually by storing it",
				"in the environment variable REGINA_OPTIONS."
            };
            JOptionPane.showMessageDialog(null, messages,
                "Error", JOptionPane.ERROR_MESSAGE);
			System.exit(1);
        }

        // Read the current options from file if available.
        optionsFile = new File(optionsDir, optionsFileName);
        options = new Properties();

        if (optionsFile.exists()) {
            try {
                FileInputStream in = new FileInputStream(optionsFile);
                options.load(in);
                in.close();
            } catch (IOException exc) {
                JOptionPane.showMessageDialog(null,
                    "File [" + optionsFile.toString() + "] could not be read.",
                    "Error", JOptionPane.ERROR_MESSAGE);
                System.exit(1);
            }
        } else {
            Object[] messages = {
                "Runtime options file [" + optionsFileName +
                    "] cannot be found",
                "in directory [" + optionsDir.toString() + "].",
                "A new file will be created."
            };
            JOptionPane.showMessageDialog(null, messages, "Information",
                JOptionPane.INFORMATION_MESSAGE);
        }

        // Bring up a dialog for the user to interact with.
        new RegConf().show();
    }

    /**
     * Creates a new dialog in which the user can alter runtime options
     * for Regina.
     */
    public RegConf() {
        super((JFrame)null, "Regina Application Runtime Configuration", true);
        lastDir = optionsDir;
        init();
        pack();

        // Position in the centre of the screen.
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        Dimension dlgSize = getSize();
        if (dlgSize.height > screenSize.height)
            dlgSize.height = screenSize.height;
        if (dlgSize.width > screenSize.width)
            dlgSize.width = screenSize.width;
        setLocation((screenSize.width - dlgSize.width) / 2,
            (screenSize.height - dlgSize.height) / 2);
    }

    /**
     * Initialises the interface components.
     */
    private void init() {
        // Set up the grid bag constraints.
        GridBagConstraints cLabel = new GridBagConstraints();
        GridBagConstraints cField = new GridBagConstraints();
        GridBagConstraints cButton = new GridBagConstraints();
        GridBagConstraints cHelp = new GridBagConstraints();
        cLabel.gridx = 0; cField.gridx = 1; cButton.gridx = 2; cHelp.gridx = 3;
        cLabel.fill = cLabel.NONE; cField.fill = cField.HORIZONTAL;
            cButton.fill = cButton.NONE; cHelp.fill = cHelp.NONE;
        cLabel.weightx = 0; cField.weightx = 0;
            cButton.weightx = 0; cHelp.weightx = 0;
        cLabel.weighty = 0; cField.weighty = 0;
            cButton.weighty = 0; cHelp.weightx = 0;
        Insets i = new Insets(5,5,5,5);
        cLabel.insets = i; cField.insets = i;
            cButton.insets = i; cHelp.insets = i;
        cLabel.anchor = cLabel.WEST;

        // Set up the user interface pane.
        JPanel uiPane = new JPanel();
        uiPane.setLayout(new GridBagLayout());
        try {
            javaUI = new JTextField(options.getProperty("JavaUI",
                new File("bin", "regina.jar").getCanonicalPath()), 25);
            javaJNI = new JTextField(options.getProperty("JavaJNI",
                new File("bin", "regina-jni.jar").getCanonicalPath()), 25);
            javaCORBA = new JTextField(options.getProperty("JavaCORBA",
                new File("bin", "regina-corba.jar").getCanonicalPath()), 25);
            docjar = new JTextField(options.getProperty("DocJar",
                new File("bin", "regina-docs.jar").getCanonicalPath()), 25);
            java = new JTextField(options.getProperty("Java", "java"), 25);
        } catch (IOException exc) {
            fileSystemError();
        }
        uiPane.add(new JLabel(javaUIName + ":"), cLabel);
        uiPane.add(javaUI, cField);
        uiPane.add(new BrowseButton(javaUI, "regina.jar"), cButton);
        uiPane.add(new HelpButton(
            "This is the jar containing the binary distribution",
            "of the general Regina Java user interface."), cHelp);
        uiPane.add(new JLabel(javaJNIName + ":"), cLabel);
        uiPane.add(javaJNI, cField);
        uiPane.add(new BrowseButton(javaJNI, "regina-jni.jar"), cButton);
        uiPane.add(new HelpButton(
            "This is the jar containing the binary distribution",
            "of the Java UI component that interacts with the",
            "calculation engine as an external library using JNI."), cHelp);
        uiPane.add(new JLabel(javaCORBAName + ":"), cLabel);
        uiPane.add(javaCORBA, cField);
        uiPane.add(new BrowseButton(javaCORBA, "regina-corba.jar"), cButton);
        uiPane.add(new HelpButton(
            "This is the jar containing the binary distribution",
            "of the Java UI component that interacts with the",
            "calculation engine as an external (and possibly",
            "remote) server application using CORBA."), cHelp);
        uiPane.add(new JLabel(docjarName + ":"), cLabel);
        uiPane.add(docjar, cField);
        uiPane.add(new BrowseButton(docjar, "regina-docs.jar"), cButton);
        uiPane.add(new HelpButton(
            "This is the jar containing the complete",
            "documentation for Regina."), cHelp);
        uiPane.add(new JLabel(javaName + ":"), cLabel);
        uiPane.add(java, cField);
        uiPane.add(new BrowseButton(), cButton);
        uiPane.add(new HelpButton(
            "This is the command typed at the console",
            "to run a Java application."), cHelp);

        // Set up the engine pane.
        JPanel enginePane = new JPanel();
        enginePane.setLayout(new GridBagLayout());
        try {
            JNIDir = new JTextField(options.getProperty("JNIDir",
                new File("bin").getCanonicalPath()), 25);
        } catch (IOException exc) {
            fileSystemError();
        }
        enginePane.add(new JLabel(JNIDirName + ":"), cLabel);
        enginePane.add(JNIDir, cField);
        enginePane.add(new BrowseButton(JNIDir, true), cButton);
        enginePane.add(new HelpButton(
            "(Only for accessing the calculation engine as a",
            "shared library from the Java UI using JNI:)",
            "This is the directory housing the library that",
            "contains the Regina calculation engine."), cHelp);

        // Set up the library pane.
        JPanel libPane = new JPanel();
        libPane.setLayout(new GridBagLayout());
        btools = new JTextField(options.getProperty("BTools", ""), 25);
        jython = new JTextField(options.getProperty("JPython", ""), 25);
        javahelp = new JTextField(options.getProperty("JavaHelp", ""), 25);
        extraclasses = new JTextField(options.getProperty("ExtraClasses", ""),
            25);
        libPane.add(new JLabel(btoolsName + ":"), cLabel);
        libPane.add(btools, cField);
        libPane.add(new BrowseButton(btools, "btools.jar"), cButton);
        libPane.add(new HelpButton(
            "This is the jar containing the binary distribution",
            "of BTools, a collection of miscellaneous Java",
            "utility classes."), cHelp);
        libPane.add(new JLabel(jythonName + ":"), cLabel);
        libPane.add(jython, cField);
        libPane.add(new BrowseButton(jython, "jython.jar"), cButton);
        libPane.add(new HelpButton(
            "This is the jar containing the binary distribution",
            "of Jython, a Java implementation of python."), cHelp);
        libPane.add(new JLabel(javahelpName + ":"), cLabel);
        libPane.add(javahelp, cField);
        libPane.add(new BrowseButton(javahelp, "jh.jar"), cButton);
        libPane.add(new HelpButton(
            "This is the jar containing the binary distribution",
            "of JavaHelp, a Java help browsing system."), cHelp);
        libPane.add(new JLabel(extraclassesName + ":"), cLabel);
        libPane.add(extraclasses, cField);
        libPane.add(new BrowseButton(), cButton);
        libPane.add(new HelpButton(
            "This is a list of jars containing any extra runtime Java",
            "classes necessary for running Regina.",
            "Jars should be separated by the classpath separator (a colon",
            "on linux/unix systems and a semicolon on windows systems).",
            "Typically nothing will be required here."), cHelp);

        // Set up the options pane.
        JPanel optPane = new JPanel();
        optPane.setLayout(new GridBagLayout());

        String interfaceStyleOpt = options.getProperty("Interface");
        String engineStyleOpt = options.getProperty("Engine");
        String JNIEngineOpt = options.getProperty("JNIEngine");
        String CORBAHostOpt = options.getProperty("CORBAHost");
        String CORBAPortOpt = options.getProperty("CORBAPort");

        String[] interfaceOptions = { "Full GUI", "Graphical console only",
            "Text-based console only" };
        interfaceStyle = new JComboBox(interfaceOptions);
        if (interfaceStyleOpt == null)
            interfaceStyle.setSelectedIndex(0);
        else if (interfaceStyleOpt.equals("console"))
            interfaceStyle.setSelectedIndex(1);
        else if (interfaceStyleOpt.equals("text"))
            interfaceStyle.setSelectedIndex(2);
        else
            interfaceStyle.setSelectedIndex(0);
        String[] engineOptions = { "JNI (shared library)",
            "CORBA (server application)" };
        engineStyle = new JComboBox(engineOptions);
        if (engineStyleOpt == null)
            engineStyle.setSelectedIndex(0);
        else if (engineStyleOpt.equals("corba"))
            engineStyle.setSelectedIndex(1);
        else
            engineStyle.setSelectedIndex(0);
        JNIEngine = new JTextField(JNIEngineOpt == null ?
            "regina-engine-jni" : JNIEngineOpt, 25);
        CORBAHost = new JTextField(CORBAHostOpt == null ?
            "localhost" : CORBAHostOpt, 25);
        CORBAPort = new JTextField(CORBAPortOpt == null ?
            "8088" : CORBAPortOpt, 25);

        optPane.add(interfaceStyleUse = new UseBox(interfaceStyleName + ":",
            interfaceStyle, interfaceStyleOpt != null), cLabel);
        optPane.add(interfaceStyle, cField);
        optPane.add(new HelpButton(
            "This specifies which style of user interface to",
            "use when running Regina.  The full GUI is the",
            "standard interface.  The console interfaces offer",
            "direct access to Jython scripting without running",
            "the full GUI."), cButton);
        optPane.add(engineStyleUse = new UseBox(engineStyleName + ":",
            engineStyle, engineStyleOpt != null), cLabel);
        optPane.add(engineStyle, cField);
        optPane.add(new HelpButton(
            "This specifies how to access the calculation engine.",
            "JNI accesses the engine as a shared library.",
            "CORBA accesses the engine as an external server",
            "application, possibly running on a different machine."), cButton);
        optPane.add(JNIEngineUse = new UseBox(JNIEngineName + ":",
            JNIEngine, JNIEngineOpt != null), cLabel);
        optPane.add(JNIEngine, cField);
        optPane.add(new HelpButton(
            "(Only for JNI engine access:)",
            "This is the platform-independent name of the JNI",
            "calculation engine library.  See `Program Options'",
            "in the Regina documentation for further details."), cButton);
        optPane.add(CORBAHostUse = new UseBox(CORBAHostName + ":",
            CORBAHost, CORBAHostOpt != null), cLabel);
        optPane.add(CORBAHost, cField);
        optPane.add(new HelpButton(
            "(Only for CORBA engine access:)",
            "This is the host on which the CORBA name service",
            "is running."), cButton);
        optPane.add(CORBAPortUse = new UseBox(CORBAPortName + ":",
            CORBAPort, CORBAPortOpt != null), cLabel);
        optPane.add(CORBAPort, cField);
        optPane.add(new HelpButton(
            "(Only for CORBA engine access:)",
            "This is the port on which the CORBA name service",
            "is running."), cButton);

        // Set up the button pane.
        JPanel buttonPane = new JPanel();
        buttonPane.setLayout(new FlowLayout());
        JButton ok = new JButton("Save");
        JButton cancel = new JButton("Cancel");
        buttonPane.add(ok);
        buttonPane.add(cancel);

        // Set up the entire thing.
        getContentPane().setLayout(new BorderLayout());
        JTabbedPane tabs = new JTabbedPane(JTabbedPane.TOP);
        tabs.add("Interface", uiPane);
        tabs.add("Engine", enginePane);
        tabs.add("Libraries", libPane);
        tabs.add("Options", optPane);
        getContentPane().add(tabs, BorderLayout.CENTER);
        getContentPane().add(buttonPane, BorderLayout.SOUTH);

        ok.addActionListener(this);
        cancel.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                close();
            }
        });
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                close();
            }
        });
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
    }

    /**
     * Called when the user attempts to close the dialog.
     */
    private void close() {
        JOptionPane.showMessageDialog(this,
            "Configuration has been cancelled.",
            "Cancelled", JOptionPane.INFORMATION_MESSAGE);
        System.exit(0);
    }

    /**
     * Called when the user attempts to save the runtime options.
     */
    public void actionPerformed(ActionEvent e) {
        if (isEmpty(javaUIName, javaUI) || // Interface
                badFile(javaUIName, javaUI) ||
                isEmpty(javaName, java) ||
                isEmpty(btoolsName, btools) || // Libraries
                badFile(btoolsName, btools) ||
                badToken(CORBAHostName, CORBAHost) || // Options
                badPositiveInteger(CORBAPortName, CORBAPort))
            return;

        // Interface
        if (! checkUnavailableFile(javaJNIName,
                "access to the engine using JNI", javaJNI))
            return;
        if (! checkUnavailableFile(javaCORBAName,
                "access to the engine using CORBA", javaCORBA))
            return;
        if (! checkUnavailableFile(docjarName,
                "the internal documentation viewer", docjar))
            return;

        // Engine
        if (! checkUnavailableDir(JNIDirName,
                "access to the engine using JNI", JNIDir))
            return;

        // Libraries
        if (! checkUnavailableFile(jythonName,
                "Jython scripting", jython))
            return;
        if (! checkUnavailableFile(javahelpName,
                "the internal help browser", javahelp))
            return;

        // Options
        if (JNIEngineUse.isSelected())
            if (isEmpty(JNIEngineName, JNIEngine))
                return;
        if (CORBAHostUse.isSelected())
            if (isEmpty(CORBAHostName, CORBAHost))
                return;
        if (CORBAPortUse.isSelected())
            if (isEmpty(CORBAPortName, CORBAPort))
                return;

        options.setProperty("JavaUI", javaUI.getText().trim());
        options.setProperty("JavaJNI", javaJNI.getText().trim());
        options.setProperty("JavaCORBA", javaCORBA.getText().trim());
        options.setProperty("DocJar", docjar.getText().trim());
        options.setProperty("Java", java.getText().trim());

        options.setProperty("JNIDir", JNIDir.getText().trim());

        options.setProperty("BTools", btools.getText().trim());
        options.setProperty("JPython", jython.getText().trim());
        options.setProperty("JavaHelp", javahelp.getText().trim());
        options.setProperty("ExtraClasses", extraclasses.getText().trim());

        if (interfaceStyleUse.isSelected())
            switch (interfaceStyle.getSelectedIndex()) {
                case 1: options.setProperty("Interface", "console"); break;
                case 2: options.setProperty("Interface", "text"); break;
                default: options.setProperty("Interface", "gui"); break;
            }
        else
            options.remove("Interface");
        if (engineStyleUse.isSelected())
            switch (engineStyle.getSelectedIndex()) {
                case 1: options.setProperty("Engine", "corba"); break;
                default: options.setProperty("Engine", "jni"); break;
            }
        else
            options.remove("Engine");
        if (JNIEngineUse.isSelected())
            options.setProperty("JNIEngine", JNIEngine.getText().trim());
        else
            options.remove("JNIEngine");
        if (CORBAHostUse.isSelected())
            options.setProperty("CORBAHost", CORBAHost.getText().trim());
        else
            options.remove("CORBAHost");
        if (CORBAPortUse.isSelected())
            options.setProperty("CORBAPort", CORBAPort.getText().trim());
        else
            options.remove("CORBAPort");

        try {
            FileOutputStream out = new FileOutputStream(optionsFile);
            options.store(out, "Runtime options for Regina");
            out.close();
        } catch (IOException exc) {
            JOptionPane.showMessageDialog(this,
                "File [" + optionsFile.toString() + "] could not be written.",
                "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        JOptionPane.showMessageDialog(this,
            "The runtime options have been saved.",
            "Success", JOptionPane.INFORMATION_MESSAGE);

        System.exit(0);
    }

    /**
     * Performs an interactive emptiness check on the given text field.
     * If the field is empty, this routine determines whether or not the
     * user is happy with this situation.
     * Otherwise this routine simply returns successfully.
     *
     * @param name the name of the field.
     * @param service the name of the service that will be denied if the
     * given field remains empty.
     * @param field the field under examination.
     * @return <tt>true</tt> if everything is okay or if the user wishes to
     * proceed regardless, or <tt>false</tt> if everything is
     * irretrievably invalid or if the user wishes to change the situation.
     */
    public boolean checkEmpty(String name, String service,
            JTextField field) {
        if (field.getText().trim().length() > 0)
            return true;
        Object[] messages = {
            "Field [" + name + "] is empty.",
            "This means that " + service,
            "will be unavailable.",
            "Do you wish to change this?"
        };
        return (JOptionPane.showConfirmDialog(this, messages, "Warning",
            JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE)
            == JOptionPane.NO_OPTION);
    }

    /**
     * Performs a complex check on the given file field.
     * If the field is non-empty, this routine verifies that it refers
     * to an existing file.
     * If the field is empty, this routine determines whether or not
     * the user is happy with this situation.
     *
     * @param name the name of the field.
     * @param service the name of the service that will be denied if the
     * given field remains empty.
     * @param field the field under examination.
     * @return <tt>true</tt> if everything is okay or if the user wishes to
     * proceed regardless, or <tt>false</tt> if everything is
     * irretrievably invalid or if the user wishes to change the situation.
     */
    public boolean checkUnavailableFile(String name, String service,
            JTextField field) {
        if (checkEmpty(name, service, field))
            if (! badFile(name, field))
                return true;
        return false;
    }

    /**
     * Performs a complex check on the given directory field.
     * If the field is non-empty, this routine verifies that it refers
     * to an existing directory.
     * If the field is empty, this routine determines whether or not
     * the user is happy with this situation.
     *
     * @param name the name of the field.
     * @param service the name of the service that will be denied if the
     * given field remains empty.
     * @param field the field under examination.
     * @return <tt>true</tt> if everything is okay or if the user wishes to
     * proceed regardless, or <tt>false</tt> if everything is
     * irretrievably invalid or if the user wishes to change the situation.
     */
    public boolean checkUnavailableDir(String name, String service,
            JTextField field) {
        if (checkEmpty(name, service, field))
            if (! badDir(name, field))
                return true;
        return false;
    }

    /**
     * Determines if the given text field containins invalid data.
     * This merely checks whether or not the given field is empty.
     * An empty field is considered invalid; anything else is considered
     * valid.
     *
     * @param name the name of the field.
     * @param field the field under examination.
     * @return <tt>true</tt> if and only if the field contains invalid data.
     */
    private boolean isEmpty(String name, JTextField field) {
        if (field.getText().trim().length() == 0) {
            JOptionPane.showMessageDialog(this,
                "Field [" + name + "] cannot be empty.",
                "Error", JOptionPane.ERROR_MESSAGE);
            return true;
        }
        return false;
    }

    /**
     * Determines if the given single text token field contains invalid
     * data.
     * A field containing spaces or tabs that are not at the front or
     * end is considered invalid.
     * An empty field however is not considered invalid.
     *
     * @param name the name of the field.
     * @param field the field under examination.
     * @return <tt>true</tt> if and only if the field contains invalid data.
     */
    private boolean badToken(String name, JTextField field) {
        String text = field.getText().trim();
        if (text.length() == 0)
            return false;
        if (text.indexOf(' ') >= 0 || text.indexOf('\t') >= 0) {
            JOptionPane.showMessageDialog(this,
                "Field [" + name + "] cannot contain spaces.",
                "Error", JOptionPane.ERROR_MESSAGE);
            return true;
        }
        return false;
    }

    /**
     * Determines if the given positive integer field contains invalid
     * data.  An empty field is not considered invalid.
     *
     * @param name the name of the field.
     * @param field the field under examination.
     * @return <tt>true</tt> if and only if the field contains invalid data.
     */
    private boolean badPositiveInteger(String name, JTextField field) {
        String text = field.getText().trim();
        if (text.length() == 0)
            return false;

        try {
            if (Long.parseLong(text) <= 0) {
                JOptionPane.showMessageDialog(this,
                    "Field [" + name +
                    "] does not contain a positive integer.",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return true;
            }
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this,
                "Field [" + name + "] does not contain a positive integer.",
                "Error", JOptionPane.ERROR_MESSAGE);
            return true;
        }
        return false;
    }

    /**
     * Determines if the given file field containins invalid data.
     * This checks whether or not the given field refers to an existing
     * file.
     * An empty field is not considered invalid.
     *
     * @param name the name of the field.
     * @param field the field under examination.
     * @return <tt>true</tt> if and only if the field contains invalid data.
     */
    private boolean badFile(String name, JTextField field) {
        String text = field.getText().trim();
        if (text.length() == 0)
            return false;

        File f = new File(text);
        if (! f.exists()) {
            JOptionPane.showMessageDialog(this,
                "Field [" + name + "] names a file that does not exist.",
                "Error", JOptionPane.ERROR_MESSAGE);
            return true;
        }
        if (f.isDirectory()) {
            JOptionPane.showMessageDialog(this,
                "Field [" + name + "] names a directory instead of a file.",
                "Error", JOptionPane.ERROR_MESSAGE);
            return true;
        }
        return false;
    }

    /**
     * Determines if the given directory field containins invalid data.
     * This checks whether or not the given field refers to an existing
     * directory.
     * An empty field is not considered invalid.
     *
     * @param name the name of the field.
     * @param field the field under examination.
     * @return <tt>true</tt> if and only if the field contains invalid data.
     */
    private boolean badDir(String name, JTextField field) {
        String text = field.getText().trim();
        if (text.length() == 0)
            return false;

        if (new File(field.getText().trim()).isDirectory())
            return false;
        JOptionPane.showMessageDialog(this,
            "Field [" + name + "] is not a directory.",
            "Error", JOptionPane.ERROR_MESSAGE);
        return true;
    }

    /**
     * A button displaying a question mark that, when pressed, brings up
     * a message box containing help information.
     */
    private static class HelpButton extends JButton
            implements ActionListener {
        /**
         * The lines of the help information to display.
         */
        private String[] message;

        /**
         * Creates a new help button with no help information.
         * This button will be disabled.
         */
        public HelpButton() {
            super("?");
            setEnabled(false);
        }

        /**
         * Creates a new help button with the given help information.
         * It is assumed that the help information fits on one line.
         *
         * @param line the one line of help information.
         */
        public HelpButton(String line) {
            super("?");
            String[] message = { line };
            this.message = message;
            addActionListener(this);
        }

        /**
         * Creates a new help button with the given help information.
         * It is assumed that the help information fits on two lines.
         *
         * @param line1 the first line of help information.
         * @param line2 the second line of help information.
         */
        public HelpButton(String line1, String line2) {
            super("?");
            String[] message = { line1, line2 };
            this.message = message;
            addActionListener(this);
        }

        /**
         * Creates a new help button with the given help information.
         * It is assumed that the help information fits on three lines.
         *
         * @param line1 the first line of help information.
         * @param line2 the second line of help information.
         * @param line3 the third line of help information.
         */
        public HelpButton(String line1, String line2, String line3) {
            super("?");
            String[] message = { line1, line2, line3 };
            this.message = message;
            addActionListener(this);
        }

        /**
         * Creates a new help button with the given help information.
         * It is assumed that the help information fits on four lines.
         *
         * @param line1 the first line of help information.
         * @param line2 the second line of help information.
         * @param line3 the third line of help information.
         * @param line4 the fourth line of help information.
         */
        public HelpButton(String line1, String line2, String line3,
                String line4) {
            super("?");
            String[] message = { line1, line2, line3, line4 };
            this.message = message;
            addActionListener(this);
        }

        /**
         * Creates a new help button with the given help information.
         * It is assumed that the help information fits on five lines.
         *
         * @param line1 the first line of help information.
         * @param line2 the second line of help information.
         * @param line3 the third line of help information.
         * @param line4 the fourth line of help information.
         * @param line5 the fifth line of help information.
         */
        public HelpButton(String line1, String line2, String line3,
                String line4, String line5) {
            super("?");
            String[] message = { line1, line2, line3, line4, line5 };
            this.message = message;
            addActionListener(this);
        }

        /**
         * Called when the user presses this help button.
         */
        public void actionPerformed(ActionEvent e) {
            JOptionPane.showMessageDialog(this, message, "Help",
                JOptionPane.INFORMATION_MESSAGE);
        }
    }

    /**
     * A check box that determines whether or not a program option is to be
     * explicitly specified.
     * The enabled state of the given program option field will be
     * automatically kept synchronised with the checked state of this
     * check box.
     */
    private class UseBox extends JCheckBox implements ItemListener {
        /**
         * The program option field whose enabled state will be linked
         * to this check box.
         */
        private JComponent optionField;

        /**
         * Creates a new check box linked to the given program option
         * field.
         *
         * @param label the text to place beside this check box.
         * @param optionField the program option field whose enabled state
         * will be linked to this check box.
         * @param checked <tt>true</tt> if and only if the check box
         * should be initially checked.
         */
        public UseBox(String label, JComponent optionField, boolean checked) {
            super(label, checked);
            this.optionField = optionField;

            optionField.setEnabled(checked);
            addItemListener(this);
        }

        /**
         * Called when the checked state of the check box has changed.
         *
         * @param e the state change event details.
         */
        public void itemStateChanged(ItemEvent e) {
            optionField.setEnabled(e.getStateChange() == e.SELECTED);
        }
    }

    /**
     * A Button that, when pressed, allows the user to select a file or
     * directory.  The results of the selection will be placed in a
     * specified text field.
     */
    private class BrowseButton extends JButton implements ActionListener {
        /**
         * The text field in which to place the results of the
         * selection.
         */
        private JTextField field;
        /**
         * The name of the file to search for if a specific file is
         * required.
         */
        private String filename;
        /**
         * Are we searching for a directory or a file?
         */
        private boolean dirsOnly;

        /**
         * Creates a new button linked to no field.
         * This button will be disabled.
         */
        public BrowseButton() {
            super("Browse...");
            setEnabled(false);
        }

        /**
         * Creates a new button linked to the given field.
         *
         * @param field the text field in which to place the results of
         * the selection.
         * @param dirsOnly <tt>true</tt> if this button should be used
         * to search for a directory, or <tt>false</tt> if this button
         * should be used to search for an actual file.
         */
        public BrowseButton(JTextField field, boolean dirsOnly) {
            super("Browse...");
            this.field = field;
            this.filename = null;
            this.dirsOnly = dirsOnly;
            addActionListener(this);
        }

        /**
         * Creates a new button linked to the given field.
         * This button will allow the user to search for a file
         * (not a directory) with the given specific name.
         *
         * @param field the text field in which to place the results of
         * the selection.
         * @param filename the specific name of the file to search for,
         * with no directory information.
         */
        public BrowseButton(JTextField field, String filename) {
            super("Browse...");
            this.field = field;
            this.filename = filename;
            this.dirsOnly = false;
            addActionListener(this);
        }

        /**
         * Called when the user presses this browse button.
         */
        public void actionPerformed(ActionEvent e) {
            JFileChooser chooser = new JFileChooser();
            if (lastDir != null)
                chooser.setCurrentDirectory(lastDir);
            chooser.setDialogTitle(dirsOnly ? "Locate directory..." :
                "Locate file...");
            String current = field.getText().trim();
            if (current.length() > 0)
                chooser.setSelectedFile(new File(current));
            if (dirsOnly)
                chooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
            if (filename != null)
                chooser.setFileFilter(new BrowseButtonFilter());
            if (chooser.showOpenDialog(RegConf.this) ==
                    chooser.APPROVE_OPTION) {
                File sel = chooser.getSelectedFile();
                field.setText(sel.getAbsolutePath());
                lastDir = sel.getParentFile();
            }
        }

        /**
         * A file filter that only accepts files of the type being
         * searched for by this browse button.
         * All directories are also accepted.
         */
        private class BrowseButtonFilter
                extends javax.swing.filechooser.FileFilter {
            /**
             * Determines if the given file is accepted by this filter.
             *
             * @param f the file whose acceptability is being determined.
             * @return <tt>true</tt> if and only if the given file is
             * accepted.
             */
            public boolean accept(File f) {
                try {
                    if (f.isDirectory())
                        return true;
                } catch (Throwable th) {
                    return true;
                }
                if (dirsOnly)
                    return false;
                if (filename == null)
                    return true;
                return (f.getName().equalsIgnoreCase(filename));
            }

            /**
             * Returns a description of this file filter.
             *
             * @return a description of this file filter.
             */
            public String getDescription() {
                if (filename == null)
                    return (dirsOnly ? "Directories" : "All files");
                return filename;
            }
        }
    }
}

