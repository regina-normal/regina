
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
import java.io.File;
import java.net.*;
import java.util.*;
import normal.engine.Engine;
import normal.options.NormalOptionSet;
import org.gjt.btools.gui.dialog.MessageBox;
import org.gjt.btools.utilities.*;

/**
 * The program shell to use when the program is run as an applet.
 */
public class AppletShell extends Shell {
    /**
     * The applet we are running.
     */
    private Applet applet;

    /**
     * Creates a new program shell for the given applet.
     *
     * @param applet the applet we are running.
     */
    public AppletShell(Applet applet) {
        this.applet = applet;
    }

    public int getShellStyle() {
        return shellApplet;
    }
    public String[] getCommandLineParameters() {
        return null;
    }
    public Applet getAppletParameters() {
        return applet;
    }
    public String getParameter(String paramName, int nMinus) {
        return applet.getParameter(paramName);
    }
    public Vector getFileParameters() {
        // An applet shouldn't allow the user to open files.
        return new Vector();
    }

    public int getUIType() {
        int uiType = unspecified;

        // Look up the applet parameters.
        String iface = applet.getParameter("Interface");

        // Look up the runtime options if necessary.
        if (iface == null)
            iface = OptionSet.getSystemProperty("REGINA_INTERFACE");

        // Return the correct UI type.
        if (iface == null)
            return UIFullGUI;
        else if (iface.equalsIgnoreCase("gui"))
            return UIFullGUI;

        error("An invalid style of interface [" + iface +
            "] has been requested.  " +
            "Only style [gui] is allowed for the web page applet.");
        return invalid;
    }
    public int getEngineType() {
        int engineType = unspecified;

        // Look up the applet parameters.
        String engine = applet.getParameter("Engine");

        // Look up the runtime options if necessary.
        if (engine == null)
            engine = OptionSet.getSystemProperty("REGINA_ENGINE");

        // Return the correct engine type.
        if (engine == null)
            return engineCORBA;
        else if (engine.equalsIgnoreCase("corba"))
            return engineCORBA;

        error("An invalid style of engine access [" + engine +
            "] has been requested.  " +
            "Only style [corba] is allowed for the web page applet.");
        return invalid;
    }
    public boolean shouldShowSplashScreen() {
        return false;
    }
    public boolean mayUIAccessFiles() {
        return false;
    }
    public boolean mayEngineAccessFiles() {
        return false;
    }
    public boolean mayUseJython() {
        return false;
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
        MessageBox.fgNote(getPrimaryFrame(), message);
    }
    public boolean confirm(String message) {
        return MessageBox.confirm(getPrimaryFrame(), message);
    }
    public void error(String message) {
        try {
            MessageBox.fgNote(getPrimaryFrame(), message);
        } catch (Throwable th) {
            System.err.println();
            System.err.println(message);
        }
    }
    public void viewHelp(String id) {
        // Find the correct help page.
        String docsDir = applet.getParameter("Docs");
        if (docsDir == null) {
            try {
                docsDir = new URL(applet.getDocumentBase(), "docs").
                    toExternalForm();
            } catch (MalformedURLException e) {
                docsDir = "docs";
            }
        }
        String resource = getHelpPageResource(id);
        String helpPage = ResourceUtils.combineLocations(docsDir, resource);

        // Point the browser at the correct help page.
        try {
            applet.getAppletContext().showDocument(new URL(helpPage),
                "ReginaHelp");
        } catch (MalformedURLException e) {
            error("The help page [" + helpPage +
                "] could not be displayed in your web browser.");
        }
    }
    public void exit(int status) {
    }
}

