
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

package normal.options;

import java.io.*;
import java.util.Vector;
import org.gjt.btools.utilities.OptionSet;

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
     * Full option name for a particular cached option.
     */
    public static final String optionJythonLibCount = "JythonLibCount";

    /**
     * Option name stub for a particular cached option.
     */
    public static final String optionJythonLib = "JythonLib";

    /**
     * Option name stub for a particular cached option.
     */
    public static final String optionJythonLibUse = "JythonLibUse";

    /**
     * Full option name for a particular cached option.
     */
    public static final String optionCensusDataCount = "CensusDataCount";

    /**
     * Option name stub for a particular cached option.
     */
    public static final String optionCensusData = "CensusData";

    /**
     * Option name stub for a particular cached option.
     */
    public static final String optionCensusDataUse = "CensusDataUse";

    /**
     * The default for a particular cached option.
     */
    public static final boolean defaultAutoDock = true;
    
    /**
     * The default for a particular cached option.
     */
    public static final boolean defaultDisplayIcon = false;
    
    /**
     * A particular cached option.
     */
    private boolean autoDock;
    
    /**
     * A particular cached option.
     */
    private boolean displayIcon;

    /**
     * A particular cached option.
     */
    private Vector jythonLibraries;

    /**
     * A particular cached option.
     */
    private Vector censusData;

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
     * Extra initialisation to perform before the options are read from file.
     */
    protected void init() {
        jythonLibraries = new Vector();
        censusData = new Vector();
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

        int i;
        int nLibs = getIntOption(optionJythonLibCount, 0);
        for (i = 0; i < nLibs; i++)
            jythonLibraries.addElement(new JythonLibrary(
                getStringOption(optionJythonLib + String.valueOf(i), ""),
                getBooleanOption(optionJythonLibUse + String.valueOf(i),
                false)));

        int nCensus = getIntOption(optionCensusDataCount, 0);
        for (i = 0; i < nCensus; i++)
            censusData.addElement(new CensusData(
                getStringOption(optionCensusData + String.valueOf(i), ""),
                getBooleanOption(optionCensusDataUse + String.valueOf(i),
                false)));

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

        int i;
        int nLibs = jythonLibraries.size();
        // Clear out all unnecessary library options.
        int oldNLibs = getIntOption(optionJythonLibCount, 0);
        for (i = nLibs; i < oldNLibs; i++) {
            removeOption(optionJythonLib + String.valueOf(i));
            removeOption(optionJythonLibUse + String.valueOf(i));
        }

        setIntOption(optionJythonLibCount, nLibs);
        JythonLibrary lib;
        for (i = 0; i < nLibs; i++) {
            lib = (JythonLibrary)jythonLibraries.elementAt(i);
            setStringOption(optionJythonLib + String.valueOf(i),
                lib.getLibraryPath());
            setBooleanOption(optionJythonLibUse + String.valueOf(i),
                lib.shouldUseLibrary());
        }

        int nCensus = censusData.size();
        // Clear out all unnecessary census options.
        int oldNCensus = getIntOption(optionCensusDataCount, 0);
        for (i = nCensus; i < oldNCensus; i++) {
            removeOption(optionCensusData + String.valueOf(i));
            removeOption(optionCensusDataUse + String.valueOf(i));
        }

        setIntOption(optionCensusDataCount, nCensus);
        CensusData census;
        for (i = 0; i < nCensus; i++) {
            census = (CensusData)censusData.elementAt(i);
            setStringOption(optionCensusData + String.valueOf(i),
                census.getCensusPath());
            setBooleanOption(optionCensusDataUse + String.valueOf(i),
                census.shouldUseCensus());
        }

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

    /**
     * Returns a vector of <tt>JythonLibrary</tt> objects representing
     * the set of available Jython libraries.
     *
     * @return the set of available Jython libraries.
     * @see normal.options.NormalOptionSet.JythonLibrary
     */
    public Vector getJythonLibraries() {
        return jythonLibraries;
    }

    /**
     * Sets the list of available Jython libraries.
     *
     * @param value the new list of Jython libraries; this must be a
     * vector of <tt>JythonLibrary</tt> objects.
     * @see normal.options.NormalOptionSet.JythonLibrary
     */
    public void setJythonLibraries(Vector value) {
        jythonLibraries = value;
    }

    /**
     * Returns a vector of <tt>CensusData</tt> objects representing
     * the set of available census data files.
     *
     * @return the set of available census data files.
     * @see normal.options.NormalOptionSet.CensusData
     */
    public Vector getCensusData() {
        return censusData;
    }

    /**
     * Sets the list of available census data files.
     *
     * @param value the new list of census data files; this must be a
     * vector of <tt>CensusData</tt> objects.
     * @see normal.options.NormalOptionSet.CensusData
     */
    public void setCensusData(Vector value) {
        censusData = value;
    }

    /**
     * Stores the details of a Jython library.
     */
    public static class JythonLibrary {
        /**
         * The path to the Jython library file.
         */
        private String libraryPath;
        /**
         * Specifies whether or not the library should be used.
         */
        private boolean useLibrary;

        /**
         * Creates a new Jython library specifier.
         *
         * @param libraryPath the path to the Jython library file.
         * @param useLibrary <tt>true</tt> if and only if the library
         * should be used.
         */
        public JythonLibrary(String libraryPath, boolean useLibrary) {
            this.libraryPath = libraryPath;
            this.useLibrary = useLibrary;
        }

        /**
         * Returns the path to this particular Jython library file.
         *
         * @return the path to this particular Jython library file.
         */
        public String getLibraryPath() {
            return libraryPath;
        }

        /**
         * Returns whether or not this Jython library should be used.
         *
         * @return <tt>true</tt> if and only if this Jython library
         * should be used.
         */
        public boolean shouldUseLibrary() {
            return useLibrary;
        }
    }

    /**
     * Stores the details of a census data file.
     */
    public static class CensusData {
        /**
         * The path to the census data file.
         */
        private String censusPath;
        /**
         * Specifies whether or not the census should be used.
         */
        private boolean useCensus;

        /**
         * Creates a new census data file specifier.
         *
         * @param censusPath the path to the census data file.
         * @param useCensus <tt>true</tt> if and only if the census
         * should be used.
         */
        public CensusData(String censusPath, boolean useCensus) {
            this.censusPath = censusPath;
            this.useCensus = useCensus;
        }

        /**
         * Returns the path to this particular census data file.
         *
         * @return the path to this particular census data file.
         */
        public String getCensusPath() {
            return censusPath;
        }

        /**
         * Returns whether or not this census should be used.
         *
         * @return <tt>true</tt> if and only if this census should be
         * used.
         */
        public boolean shouldUseCensus() {
            return useCensus;
        }
    }
}

