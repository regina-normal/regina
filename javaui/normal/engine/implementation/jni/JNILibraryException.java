
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

package normal.engine.implementation.jni;

import org.gjt.btools.utilities.OptionSet;

/**
 * Exception thrown when a shared library to be used with JNI could not
 * be loaded.
 */
public class JNILibraryException extends Exception {
    /**
     * Create a new exception because a shared library could not be loaded.
     *
     * @param libName the library that could not be loaded.
     */
    public JNILibraryException(String libName) {
        super("Could not load shared library [" + libName + "].");
    }

    /**
     * Returns the current library search path.
     *
     * @return the current library search path, or <tt>null</tt> if this
     * could not be determined.
     */    
    public String getLibraryPath() {
        return OptionSet.getSystemProperty("java.library.path");
    }    
}
