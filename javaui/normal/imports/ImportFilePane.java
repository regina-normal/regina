
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

package normal.imports;

import java.awt.*;
import java.awt.event.*;
import java.io.File;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import normal.Shell;
import btools.gui.*;

/**
 * A pane in which the user can select a file from which to import data.
 * A text field will be provided to contain the file name, as well as a
 * browse button which will bring up a dialog for GUI file selection.
 */
public class ImportFilePane extends JPanel implements ActionListener {
    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * The text field in which the user can type a filename.
     */
    private JTextField fileName;

    /**
     * A filter specifying the files we are interested in.
     */
    private FileFilter filter;
    
    /**
     * Creates a new pane.
     *
     * @param shell the shell representing the entire program.
     * @param filter a filter specifying the files in which we are
     * interested, or <tt>null</tt> if no filter is required.
     */
    public ImportFilePane(Shell shell, FileFilter filter) {
        this.shell = shell;
        this.filter = filter;

        setLayout(new BorderLayout());
        add(new JLabel("Import from:"), BorderLayout.NORTH);
        fileName = new JTextField("", 20);
        add(fileName, BorderLayout.CENTER);
        JButton browse = new JButton("Browse...");
        add(browse, BorderLayout.EAST);
        browse.addActionListener(this);
    }

    /**
     * Returns the file name currently entered or selected by the user.
     *
     * @return the currently chosen filename.
     */
    public String getFileName() {
        return fileName.getText();
    }

    /**
     * Called when the browse button is pressed.
     * Brings up a dialog in which the user can select a new filename
     * to place in the corresponding text field.
     */
    public void actionPerformed(ActionEvent e) {
        JFileChooser chooser = new JFileChooser();
        chooser.setCurrentDirectory(new File(
            shell.getOptions().getStringOption("LastDir", ".")));
        if (filter != null)
            chooser.setFileFilter(filter);
        chooser.setDialogTitle("Import from...");
        chooser.setSelectedFile(new File(fileName.getText()));
        if (chooser.showOpenDialog(shell.getPrimaryFrame()) ==
                chooser.APPROVE_OPTION)
            fileName.setText(chooser.getSelectedFile().getAbsolutePath());
    }
}

