
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

package normal.mainui;

import java.io.File;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import normal.*;
import normal.engine.Engine;
import normal.options.NormalOptionSet;

/**
 * A pane in which the user can work with a single file.  Subclasses of
 * <tt>FilePane</tt> should be used for different types of file with
 * which the user can work.
 * <p>
 * When a <tt>FilePane</tt> has been removed from the user interface and
 * is about to be destroyed, its <tt>cleanUp()</tt> method should be
 * called.  Otherwise the underlying data structures may not be cleaned
 * up properly.
 *
 * @see #cleanUp
 */
public abstract class FilePane extends JPanel {
    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * Are there unsaved changes to this file?
     */
    private boolean isFileDirty = false;

    /**
     * Directory in which the working data was last saved to or read from.
     */
    private File fileDir = null;

    /**
     * File in which the working data was last saved to or read from.
     * This does not include directory information.
     */
    private String fileName = null;

    /**
     * A human-readable string describing the type of file open in this
     * pane.
     */
    private String fileType = "Unknown";

    /**
     * The primary text component with which this pane is working.
     */
    private JTextComponent primaryTextComponent = null;

    /**
     * List of open Jython consoles that are attached to this file.
     */
    private Vector consoles = new Vector();

    /**
     * Create a new pane.
     *
     * @param shell the shell representing the entire program.
     */
    public FilePane(Shell shell) {
        super();
        this.shell = shell;
    }

    /**
     * Determines if there is any reason this file should not be saved.
     * The default implementation simply returns <tt>true</tt> (the file
     * may be saved).
     *
     * @return <tt>true</tt> if and only if this file may be saved.
     */
    public boolean canSave() {
        return true;
    }

    /**
     * Saves the working data to the given file.
     * Any I/O or related errors should be reported to the user.
     * There is no need to update the dirty flag; this will be done
     * elsewhere.
     *
     * @param file the file to which to save.
     * @return <tt>true</tt> if and only if the file was successfully saved.
     */
    public abstract boolean saveFile(File file);

    /**
     * Returns a file filter describing this particular type of file.
     *
     * @return a file filter for this type of file.
     */
    public abstract javax.swing.filechooser.FileFilter getFileFilter();

    /**
     * Returns the default file extension used for this particular type
     * of file.  The extension returned will begin with a period (.).
     *
     * @return the default file extension.
     */
    public abstract String getFileExtension();

    /**
     * Returns the shell representing the entire program.
     *
     * @return the shell representing the entire program.
     */
    public Shell getShell() {
        return shell;
    }
    /**
     * Returns the <tt>NormalFrame</tt> containing this file pane.
     *
     * @return the enclosing <tt>NormalFrame</tt>, or <tt>null</tt> if
     * there is no such frame.
     */
    public NormalFrame getNormalFrame() {
        return shell.getNormalFrame();
    }
    /**
     * Returns the calculation engine currently in use.
     *
     * @return the engine currently in use.
     */
    public Engine getEngine() {
        return shell.getEngine();
    }
    /**
     * Returns the current user options for the program.
     *
     * @return the current user options.
     */
    public NormalOptionSet getOptions() {
        return shell.getOptions();
    }

    /**
     * Have any changes been made to the working data since the last save?
     *
     * @return <tt>true</tt> if and only if there are unsaved changes.
     */
    public boolean getDirty() {
        return isFileDirty;
    }
    /**
     * Set whether or not unsaved changes have been made to the working data.
     * The corresonding visual marker in the parent frame will be updated.
     *
     * @param isDirty <tt>true</tt> if and only if there are unsaved changes.
     */
    public void setDirty(boolean isDirty) {
        isFileDirty = isDirty;
        shell.getNormalFrame().updateDirtyMarker(this);
    }

    /**
     * Returns the directory of the working file.
     *
     * @return the directory of the working file, or <tt>null</tt> if
     * this has not been set.
     */
    public File getFileDir() {
        return fileDir;
    }
    /**
     * Returns the name of the working file.
     * This will not include directory information.
     *
     * @return the name of the working file, or <tt>null</tt> if this has
     * not been set.
     */
    public String getFileName() {
        return fileName;
    }
    /**
     * Returns the file type description for this pane.
     * This will be a human-readable string.
     *
     * @return the file type description.
     */
    public String getFileType() {
        return fileType;
    }
    /**
     * Sets the directory of the working file.
     *
     * @param fileDir the directory of the working file.
     */
    public void setFileDir(File fileDir) {
        this.fileDir = fileDir;
    }
    /**
     * Sets the name of the working file.
     * This should not include directory information.
     *
     * @param fileName the name of the working file.
     */
    public void setFileName(String fileName) {
        this.fileName = fileName;
    }
    /**
     * Sets the file type description for this pane.
     * This should be a human-readable string.
     *
     * @param fileType the new file type description; this must not be
     * <tt>null</tt>.
     */
    public void setFileType(String fileType) {
        this.fileType = fileType;
    }

    /**
     * Returns the primary text component with which this pane is
     * working.
     *
     * @return the primary text component, or <tt>null</tt> if there is
     * no primary text component.
     */
    public JTextComponent getWorkingTextComponent() {
        return primaryTextComponent;
    }
    /**
     * Sets the given text component as the primary text component with
     * which this pane is working.  The parent frame will be notified of
     * the change.
     *
     * @param text the new primary text component, or <tt>null</tt> if
     * there is no primary text component.
     */
    public void setWorkingTextComponent(JTextComponent text) {
        primaryTextComponent = text;
        getNormalFrame().reflectCurrentTextComponent();
    }

    /**
     * Causes this file pane to take ownership of the given Jython
     * console.  The console will have its look and feel updated with
     * this pane and will be closed with this pane.
     *
     * @param console the console to take ownership of; this should be of
     * class <tt>normal.console.JPythonConsoleFrame</tt>, but it is
     * passed as a <tt>JFrame</tt> to avoid problems if Jython is not
     * available.
     * @see #disownConsole
     */
    public void ownConsole(JFrame console) {
        consoles.addElement(console);
    }
    /**
     * Causes this file pane to relinquish ownership of the given Jython
     * console.
     * <p>
     * <b>Precondition:</b> The file pane currently has ownership of the
     * given console as given by <tt>ownConsole</tt>.
     *
     * @param console the console to relinquish ownership of.
     * @see #ownConsole
     */
    public void disownConsole(JFrame console) {
        consoles.removeElement(console);
    }

    /**
     * Set whether or not the program icon is to be displayed in this
     * file pane.  The default implementation does nothing.
     *
     * @param display <tt>true</tt> if and only if the program icon is to
     * be displayed.
     */
    public void setDisplayIcon(boolean display) {
    }
    /**
     * Update the look and feel of each top-level window spawned by this
     * file pane in order to reflect the current swing look and feel.
     * Note that the file pane itself should not be updated; this will
     * have already been done by its parent container.
     * The default implementation updates the look and feel for each
     * owned console.
     */
    public void updateLookAndFeel() {
        Enumeration e = consoles.elements();
        while (e.hasMoreElements())
            SwingUtilities.updateComponentTreeUI((JFrame)e.nextElement());
    }

    /**
     * Find out whether or not we are allowed to close this file pane.
     * The default implementation checks with the user if the file is
     * dirty.
     *
     * @return <tt>true</tt> if and only if we may close this file pane.
     */
    public boolean canClose() {
        // Check with the user if changes will be lost.
        if (isFileDirty)
            if (! shell.confirm("Are you sure you wish to abandon " +
                    "any changes to this file?"))
                return false;
        return true;
    }
    /**
     * Tidies up before being destroyed.
     * This should only be called when the entire file pane has been
     * removed from the user interface and is about to be destroyed.
     * The default implementation does nothing.
     */
    void cleanUp() {
    }
}

