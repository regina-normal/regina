
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
        String libName = shell.getParameter("jnilibname", 2);
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
            + libName + ".dll] or [cyg" + libName
            + ".dll] and unix-style platforms will look for [lib" + libName
            + ".so].  The current path that Java will search for libraries is ["
            + libSearchPath + "].");
        error(errMessage.toString());
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

