
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
import java.awt.*;
import java.util.Properties;
import normal.engine.*;

import normal.options.NormalOptionSet;

/**
 * Used to initialise access to the calculation engine.
 *
 * @see normal.engine.Engine
 */
public class EngineLoader {
    /**
     * Default shared library name for the JNI calculation engine.
     * Note that the real name of the library may differ
     * between platforms.  For instance, a library called <i>name</i> will
     * be found as lib<i>name</i>.so on unix-style platforms, or
     * <i>name</i>.dll on Win32 platforms.
     */
    public static final String defaultJNILibrary = "regina-engine-jni";
    /**
     * Default naming service host for the CORBA calculation engine.
     */
    public static final String defaultCORBAHost = "localhost";
    /**
     * Default naming service port for the CORBA calculation engine.
     */
    public static final String defaultCORBAPort = "8088";

    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * Creates a new engine loader for the given program shell.
     *
     * @param shell the shell representing the entire program.
     */
    public EngineLoader(Shell shell) {
        this.shell = shell;
    }

    /**
     * Initialises access to the calculation engine.
     *
     * @return the new calculation engine, or <tt>null</tt> if an error
     * occurred.
     */
    public Engine loadEngine() {
        // Find out what style of engine access should be used.
        int engineType = shell.getEngineType();
        if (engineType == shell.invalid)
            return null;
        
        // Initialise access to the engine.
        switch (engineType) {
            case Shell.engineJNI: return loadJNIEngine();
            case Shell.engineCORBA: return loadCORBAEngine();
            default: return null;
        }
    }

    /**
     * Initialises access to a JNI engine.
     *
     * @return the new working engine, or <tt>null</tt> if an error
     * occurred.
     */    
    private Engine loadJNIEngine() {
        // Determine the library name to use.
        String libName = shell.getParameter("jnilibname", 2, true, false,
            "JNI library name");
        if (libName == null) {
            libName = NormalOptionSet.getSystemProperty("REGINA_JNIENGINE",
                defaultJNILibrary);
        }

        // Attempt to load the engine.
        try {
            return new normal.engine.implementation.jni.JNIEngine(libName);
        } catch (NoClassDefFoundError err) {
            error("The classes for the JNI engine interface could not be " +
                "found.  Ensure that [regina-jni.jar] is on your classpath.");
        } catch (Exception e) {
            if (e instanceof
                    normal.engine.implementation.jni.JNILibraryException)
                failedLibraryLoad(libName,
                    ((normal.engine.implementation.jni.JNILibraryException)e).
                    getLibraryPath());
            else
                e.printStackTrace();
        }
        return null;
    }

    /**
     * Called when a JNI library failed to load.
     * Displays an error message to the user.
     *
     * @param libName the name of the library that failed to load.
     * @param libSearchPath the current library search path.
     */
    private void failedLibraryLoad(String libName, String libSearchPath) {
        StringBuffer errMessage =
            new StringBuffer("The external JNI calculation engine library ["
            + libName + "] failed to load.\n");
        errMessage.append("Note that on some platforms the real name of the " +
            "library will be different.  ");
        errMessage.append("For instance, Win32 platforms will look for ["
            + libName
            + ".dll] and unix-style platforms will look for [lib" + libName
            + ".so].  The current path that Java will search for libraries is ["
            + libSearchPath + "].");
        error(errMessage.toString());
    }

    /**
     * Initialises access to a CORBA engine.
     *
     * @return the new working engine, or <tt>null</tt> if an error
     * occurred.
     */    
    private Engine loadCORBAEngine() {
        // Determine the host and port to use.
        boolean gaveHostParameter = false;
        boolean gavePortParameter = false;

        String host = shell.getParameter(
            "org.omg.CORBA.ORBInitialHost", 1, false, true,
            "CORBA naming service host");
        if (host == null) {
            host = shell.getParameter("ORBInitialHost", 1, false,
                true, "CORBA naming service host");
            if (host == null) {
                host = NormalOptionSet.getSystemProperty(
                    "REGINA_CORBAHOST", defaultCORBAHost);
            } else
                gaveHostParameter = true;
        }

        String port = shell.getParameter(
            "org.omg.CORBA.ORBInitialPort", 1, false, true,
            "CORBA naming service port");
        if (port == null) {
            port = shell.getParameter("ORBInitialPort", 1, false,
                true, "CORBA naming service port");
            if (port == null) {
                port = NormalOptionSet.getSystemProperty(
                    "REGINA_CORBAPORT", defaultCORBAPort);
            } else
                gavePortParameter = true;
        }

        // Attempt to connect to the engine.
        String[] args = shell.getCommandLineParameters();
        Applet applet = shell.getAppletParameters();

        // Make sure we have either args or applet available to use.
        if (args == null && applet == null) {
            String[] newArgs = { };
            args = newArgs;
        }

        // Insert the host and port into the argument list if necessary.
        if (args != null &&
                ((! gaveHostParameter) || (! gavePortParameter))) {
            int newLen = args.length + (gaveHostParameter ? 0 : 2) +
                (gavePortParameter ? 0 : 2);
            String[] newArgs = new String[newLen];

            int i;
            for (i = 0; i < args.length; i++)
                newArgs[i] = args[i];

            i = args.length;
            if (! gaveHostParameter) {
                newArgs[i] = "-ORBInitialHost";
                newArgs[i + 1] = host;
                i += 2;
            }
            if (! gavePortParameter) {
                newArgs[i] = "-ORBInitialPort";
                newArgs[i + 1] = port;
                i += 2;
            }

            args = newArgs;
        }

        try {
            if (args != null)
                return new normal.engine.implementation.corba.CORBAEngine(
                    args, host, port);
            if (applet != null)
                return new normal.engine.implementation.corba.CORBAEngine(
                    applet, host, port);
        } catch (NoClassDefFoundError err) {
            error("The classes for the CORBA engine interface could not be " +
                "found.  Ensure that [regina-corba.jar] is on your classpath.");
        } catch (Exception e) {
            if (e instanceof
                    normal.engine.implementation.corba.CORBAException)
                error("A CORBA connection to the engine could not be " +
                    "initialised.  " + e.getMessage() +
                    "\nThe requested naming service was at host [" + host +
                    "], port [" + port + "].");
            else
                e.printStackTrace();
        }
        return null;
    }

    /**
     * Displays the given error message.
     *
     * @param message the message to display to the user.
     * @return <tt>null</tt>.
     */
    private Engine error(String message) {
        shell.error(message);
        return null;
    }
}

