
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

package normal.options;

import java.io.*;
import btools.utilities.OptionSet;

/**
 * Provides an option set with a cache of frequently used options for
 * faster access.  These are accessed through direct variable lookup
 * as opposed to the usual dictionary search with string comparisons.
 * <p>
 * These frequently used options should <i>only</i> be accessed through
 * the appropriate <tt>get</tt><i>OptionName</i><tt>()</tt>
 * and <tt>set</tt><i>OptionName</i><tt>()</tt> methods, and never through
 * the standard methods <tt>getStringOption()</tt>,
 * <tt>setBooleanOption()</tt>, etc.
 * <p>
 * Options that are not explicitly cached by <tt>NormalOptionSet</tt> should,
 * however, still be accessed through the standard <tt>OptionSet</tt> access
 * methods.
 * <p>
 * Cached options will always provide values.  If they do not appear in the
 * option set, they will be given default values upon construction of the
 * option set.
 */
public class NormalOptionSet extends OptionSet {
    /**
     * The comment to place at the beginning of the option file.
     */
    public static final String normalComment = "User options for " +
        normal.Application.program;
    
    /**
     * Full option name for a particular cached option.
     */
    public static final String optionAutoDock = "AutoDock";
    
    /**
     * Full option name for a particular cached option.
     */
    public static final String optionDisplayIcon = "DisplayIcon";

    /**
     * The default for a particular cached option.
     */
    public static final boolean defaultAutoDock = true;
    
    /**
     * The default for a particular cached option.
     */
    public static final boolean defaultDisplayIcon = true;
    
    /**
     * A particular cached option.
     */
    private boolean autoDock;
    
    /**
     * A particular cached option.
     */
    private boolean displayIcon;

	/**
	 * Creates a new option set not associated with any file.
	 */
	public NormalOptionSet() {
		super();
	}
    
    /**
     * Creates a new option set based on the given file.
     * All options stored in the file will be loaded.  Any errors
     * that occur when reading from file will be ignored.
     *
     * @param optionFile the file to/from which this option set is to
     * be written/read.
     * @see #NormalOptionSet(File, boolean)
     */
    public NormalOptionSet(File optionFile) {
        super(optionFile, normalComment);
    }

    /**
     * Creates a new option set based on the given file.
     * All options stored in the file will be loaded.
     *
     * @param optionFile the file to/from which this option set is to
     * be written/read.
     * @param forceLoad if set to <tt>false</tt>, we will ignore
     * any errors in reading from file.  If set to <tt>true</tt>, an
     * exception will be thrown if an error occurs.
     * @throws IOException thrown if an error occurs in reading from file.
     * @see #NormalOptionSet(File)
     */
    public NormalOptionSet(File optionFile, boolean forceLoad)
            throws IOException {
        super(optionFile, normalComment, forceLoad);
    }
    
    /**
     * Attempt to read this option set from file.
     *
     * @throws IOException thrown if an error occurs in reading from file.
     */
    protected void readFromFile() throws IOException {
        IOException caught = null;
        try {
            super.readFromFile();
        } catch (IOException e) {
            caught = e;
        }
        autoDock = getBooleanOption(optionAutoDock, defaultAutoDock);
        displayIcon = getBooleanOption(optionDisplayIcon, defaultDisplayIcon);
        if (caught != null)
            throw caught;
    }

    /**
     * Attempt to write this option set to file.
     *
     * @param forceWrite if set to <tt>false</tt>, we will ignore
     * any errors in writing to file.  If set to <tt>true</tt>, an
     * exception will be thrown if an error occurs.
     * @throws IOException thrown if an error occurs in writing to file.
     */
    public void writeToFile(boolean forceWrite) throws IOException {
        setBooleanOption(optionAutoDock, autoDock);
        setBooleanOption(optionDisplayIcon, displayIcon);
        super.writeToFile(forceWrite);
    }

    /**
     * Get a particular cached option.
     *
     * @return the current value of the option.
     */
    public boolean getAutoDock() {
        return autoDock;
    }
    
    /**
     * Set a particular cached option.
     *
     * @param value the new value for the option.
     */
    public void setAutoDock(boolean value) {
        autoDock = value;
    }
        
    /**
     * Get a particular cached option.
     *
     * @return the current value of the option.
     */
    public boolean getDisplayIcon() {
        return displayIcon;
    }
    
    /**
     * Set a particular cached option.
     *
     * @param value the new value for the option.
     */
    public void setDisplayIcon(boolean value) {
        displayIcon = value;
    }
}
