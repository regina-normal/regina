
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
import java.awt.*;
import java.awt.event.*;

/**
 * Wrapper class for the entire application.
 * Provides little more than entry points for the standalone application
 * and the applet, after which control is passed to
 * <tt>normal.Shell.run()</tt>.
 * <p>
 * Some static program information strings are also provided for use by
 * about boxes and the like.
 *
 * @see normal.Shell#run
 */
public class Application extends Applet implements ActionListener {
    /**
     * Program name.
     */
    public static final String program = "Regina";

    /**
     * Program version.
     */
    public static final String version = "2.2";

    /**
     * Program date.
     */
    public static final String date = "1 October 2001";

    /**
     * Copyright information.
     */
    public static final String copyright =
        "Copyright (c) 1999-2001, Ben Burton";

    /**
     * Miscellaneous comments.
     */
    public static final String comments = "A Normal Surface Theory Calculator";

    /**
     * Author information.
     */
    public static final String author = "Written by Ben Burton";

    /**
     * String offering major acknowledgements.
     * This string may contain internal newlines, but no final newline is
     * given.
     */
    public static final String majorThanks =
        "Normal 1.0 was written by David Letscher, who also provided " +
            "invaluable technical advice for this project.";

    /**
     * String thanking other people invovled.
     * This string may contain internal newlines, but no final newline is
     * given.
     */
    public static final String thanks =
        "Additional code contributed by:\n" +
        "    David Letscher\n" +
        "Thanks to:\n" +
        "    William Jaco\n" +
        "    Hyam Rubinstein\n" +
        "    Jeff Weeks\n" +
        "    The American Institute of Mathematics\n" +
        "    Oklahoma State University\n" +
        "    The University of Melbourne, Australia";

    /**
     * String providing a contact for further comments or questions.
     * This string may contain internal newlines, but no final newline is
     * given.
     */
    public static final String contact =
        "Contact: Ben Burton (benb@acm.org)";
    
    /**
     * String providing notes to place prominently above the full
     * license details.
     * This string may contain internal newlines, but no final newline is
     * given.
     */
    public static final String licenseNotes =
        "Regina has been released under the GNU General Public License, " +
        "as described below.\n" +
        "It is hereby explicitly allowed that this software may be run" +
        "with any implementation of the core Java classes (the classes that" +
        "implement the official Java API) without the GPL applying to that" +
        "implementation of the core Java classes.";
    /**
     * Location of the file containing the full license details.
     */
    public static final String licenseFile = "normal/gpl.txt";

    /**
     * System property dictating in which directory options files are
     * stored.
     */
    public static final String optionsDirProperty = "REGINA_OPTIONS_LOCAL";

    /**
     * Directory to use for options files if system property
     * <i>optionsDirProperty</i> does not exist or is inaccessible.
     */
    public static final String optionsDirDefault = ".";

    /**
     * User options file.
     */
    public static final String optionsFile = "regina.opt";

    /**
     * Location of the default help set in the documentation jar.
     */
    public static final String helpSetLoc = "normal/docs/regina.hs";

    /**
     * Location of the default help page relative to the docs directory.
     */
    public static final String defaultHelpPage = "index.html";

    /**
     * The version string representing the oldest allowable JDK with
     * which this program can be run.
     */
    public static final String minimumJDKVersion = "1.3";

    /**
     * The version string representing the oldest allowable BTools with
     * which this program can be run.
     */
    public static final String minimumBToolsVersion = "1.1";

    /**
     * Entry point for the applet.
     */
    public void init() {
        setLayout(new FlowLayout());

        try {
            addComponents();
        } catch (Throwable th) {
            Button button = new Button("Run " + program + "...");
            button.addActionListener(this);
            add(button);
        }
    }

    /**
     * Adds components to the applet panel.
     */
    private void addComponents() {
        javax.swing.JButton button = new javax.swing.JButton(
            "Run " + program + "...",
            Images.mainMediumIcon.finalImage());
        button.addActionListener(this);
        add(button);
    }

    /**
     * Returns a description of the applet.
     *
     * @return a description of the applet.
     */
    public String getAppletInfo() {
        return program + " - " + comments;
    }

    /**
     * Returns details of the available applet parameters.
     *
     * @return applet parameter details.
     */
    public String[][] getParameterInfo() {
        String[][] info = {
            { "Docs", "String",
                "the directory containing the unpacked documentation" },
            { "Interface", "String",
                "the style of interface to use (currently only gui)" },
            { "ORBInitialHost", "String", "the CORBA name service host" },
            { "ORBInitialPort", "String", "the CORBA name service port" }
        };
        return info;
    }

    /**
     * Called when the user presses the run button in the applet panel.
     *
     * @param e details of the button pressing event.
     */
    public void actionPerformed(ActionEvent e) {
        new AppletShell(this).run();
    }

    /**
     * Entry point for the standalone application.
     *
     * @param args the command-line arguments to the application.
     */
    public static void main(String[] args) {
        new ApplicationShell(args).run();
    }
}

