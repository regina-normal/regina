
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

package normal.mainui;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import org.gjt.btools.ext.*;
import org.gjt.btools.gui.component.*;
import org.gjt.btools.gui.dialog.*;
import normal.*;
import normal.console.*;

/**
 * Represents a pane in which the user can edit a Jython library.
 */
public class LibraryPane extends FilePane {

    // ---------------
    //    CONSTANTS
    // ---------------

    /**
     * The default file extension used for Jython libraries.
     */
    public static final String defaultFileExtension = ".py";

    /**
     * Filter for data files.
     */
    public static final ExtensionFilenameFilter filenameFilter =
        new ExtensionFilenameFilter(defaultFileExtension,
        "Jython Libraries (*" + defaultFileExtension + ')');
    
    // ------------------
    //    DATA MEMBERS
    // ------------------

    /**
     * Text component that does the actual editing.
     */
    private JTextArea text;

    /**
     * Contents of the primary text component.
     */
    private Document doc;

    // --------------------
    //    INITIALISATION
    // --------------------

    /**
     * Create a new pane with a new default set of working data.
     *
     * @param shell the shell representing the entire program.
     * @return the new pane.
     */
    public static LibraryPane newPane(Shell shell) {
        return new LibraryPane(shell, new PlainDocument());
    }

    /**
     * Create a new pane with the working data read from the given file.
     * Any I/O or related errors will be reported to the user.
     *
     * @param shell the shell representing the entire program.
     * @param file the file from which to read.
     * @return the new pane, or <tt>null</tt> if an error occurred.
     */
    public static LibraryPane newPane(Shell shell, File file) {
        FileInputStream fin;
        try {
            fin = new FileInputStream(file);
        } catch (Throwable th) {
            shell.error("The requested file does not exist or " +
                "cannot be accessed.");
            return null;
        }
        BufferedReader buf = new BufferedReader(new InputStreamReader(fin));
        PlainDocument doc = new PlainDocument();

        try {
            String line = buf.readLine();
            while (line != null) {
                doc.insertString(doc.getLength(), line + '\n', null);
                line = buf.readLine();
            }
            buf.close();
        } catch (IOException e) {
            try {
                buf.close();
            } catch (Throwable th) {}
            shell.error("An I/O error occurred whilst reading the " +
                "requested file.");
            return null;
        } catch (BadLocationException e) {
            // We should never see this!
            e.printStackTrace();
            return null;
        }

        return new LibraryPane(shell, doc);
    }

    /**
     * Create a new pane containing the given working data.
     *
     * @param shell the shell representing the entire program.
     * @param doc a document containing the text to edit; this will be
     * used as the document for the corresponding text component.
     */
    private LibraryPane(Shell shell, Document doc) {
        super(shell);
        this.doc = doc;

        init();
    }

    public boolean saveFile(File file) {
        FileOutputStream fout;
        try {
            fout = new FileOutputStream(file);
        } catch (Throwable th) {
            getShell().error(
                "The requested file could not be opened for writing.");
            return false;
        }
        BufferedWriter buf = new BufferedWriter(new OutputStreamWriter(fout));

        try {
            String line = doc.getText(0, doc.getLength());
            buf.write(line, 0, line.length());
            buf.close();
        } catch (IOException e) {
            try {
                buf.close();
            } catch (Throwable th) {}
            getShell().error(
                "An I/O error occurred whilst writing to the requested file.");
            return false;
        } catch (BadLocationException e) {
            // We should never see this!
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public javax.swing.filechooser.FileFilter getFileFilter() {
        return filenameFilter;
    }

    public String getFileExtension() {
        return defaultFileExtension;
    }

    /**
     * Initialise interface components.
     */
    private void init() {
        // Set the layout manager.
        setLayout(new BorderLayout());

        // Initialise the components.
        text = new JTextArea(doc);
        text.setLineWrap(false);
        text.setTabSize(4);
        Font font = text.getFont();
        text.setFont(new Font("Monospaced", Font.PLAIN,
            font == null ? 12 : font.getSize()));

        JButton run = new JButton("Run", Images.btnConsole.image());
        run.setEnabled(getShell().hasFoundJython());
        JButton compile = new JButton("Compile");
        compile.setEnabled(getShell().hasFoundJython());
        JButton help = new JButton("Help");
        help.setAlignmentX(help.CENTER_ALIGNMENT);

        // Put everything together.
        JPanel buttonPane = new JPanel();
        buttonPane.setLayout(new FlowLayout());
        buttonPane.add(run);
        buttonPane.add(compile);
        buttonPane.add(help);

        add(new PaddedPane(new JScrollPane(text), 0, 5, 5, 5),
            BorderLayout.CENTER);
        add(buttonPane, BorderLayout.NORTH);

        // Add event listeners.
        doc.addDocumentListener(new DocumentListener() {
            public void changedUpdate(DocumentEvent e) {
                setDirty(true);
            }
            public void insertUpdate(DocumentEvent e) {
                setDirty(true);
            }
            public void removeUpdate(DocumentEvent e) {
                setDirty(true);
            }
        });
        run.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                run();
            }
        });
        compile.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                compile();
            }
        });
        help.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                getShell().viewHelp("jython");
            }
        });

        // Tidy up.
        setWorkingTextComponent(text);
    }

    // -------------
    //    ACTIONS
    // -------------

    private void run() {
        getShell().error("Not yet implemented.");
    }

    private void compile() {
        getShell().error("Not yet implemented.");
    }
}

