
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

import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import normal.Shell;
import normal.console.JPythonUtils;
import org.gjt.btools.ext.ExtensionFilenameFilter;
import org.gjt.btools.gui.Positioner;
import org.gjt.btools.gui.component.*;
import org.gjt.btools.gui.dialog.*;

/**
 * A modal dialog allowing the Jython libraries to be configured by the user.
 * If OK is pressed, the working option set will be changed and written to
 * file.
 */
public class ManageJPythonLibraries extends JDialog implements ActionListener {
	/**
	 * Add a library.
	 */
	private JButton add;
	/**
	 * Remove a library.
	 */
	private JButton remove;
    /**
     * OK button.
     */
    private JButton ok;
	/**
	 * Compile button renderer.
	 */
	private ButtonRenderer compile;

	/**
	 * The table of Jython libraries.
	 */
	private JTable libTable;
	/**
	 * The model for the Jython libraries table.
	 */
	private LibTableModel libTableModel;
	/**
	 * The list of paths to Jython library files.
	 */
	private Vector libPaths;
	/**
	 * The list of whether or not Jython libraries should be used.
	 */
	private Vector libUse;

    /**
     * The shell corresponding to the entire program.
     */
    private Shell shell;
	/**
	 * A filename filter representing Jython libraries.
	 */
	private ExtensionFilenameFilter filter =
		new ExtensionFilenameFilter(".py", "Jython libraries (*.py)");

    /**
     * Creates a new Jython libraries configuration dialog.
     *
     * @param shell the shell corresponding to the entire program.
     */
    public ManageJPythonLibraries(Shell shell) {
        super(shell.getPrimaryFrame(), "Jython Libraries", true);
        this.shell = shell;
        init();
        pack();
        Positioner.centerOnScreen(this);
    }

    /**
     * Initialise the interface components.
     */
    private void init() {
		libPaths = new Vector();
		libUse = new Vector();
		Enumeration e = shell.getOptions().getJythonLibraries().elements();
		NormalOptionSet.JythonLibrary lib;
		while (e.hasMoreElements()) {
			lib = (NormalOptionSet.JythonLibrary)e.nextElement();
			libPaths.addElement(lib.getLibraryPath());
			libUse.addElement(new Boolean(lib.shouldUseLibrary()));
		}

		add = new JButton("Add");
		remove = new JButton("Remove");
		ok = new JButton("OK");
		JButton cancel = new JButton("Cancel");

		// Set up the table.
		libTableModel = new LibTableModel();
		libTable = new JTable(libTableModel);
		JScrollPane tableScroller = new JScrollPane(libTable);
		tableScroller.setPreferredSize(new Dimension(400, 150));

		CheckBoxRenderer cr = new CheckBoxRenderer();
		compile = new ButtonRenderer("(press)");
		compile.setBorder(BorderFactory.createEtchedBorder());
		compile.addActionListener(this);
		libTable.getColumnModel().getColumn(0).setCellRenderer(cr);
		libTable.getColumnModel().getColumn(0).setCellEditor(cr);
		libTable.getColumnModel().getColumn(1).setCellRenderer(
			new LibNameRenderer());
		libTable.getColumnModel().getColumn(1).setCellEditor(
			new LongStringRenderer(2));
		libTable.getColumnModel().getColumn(2).setCellRenderer(compile);
		libTable.getColumnModel().getColumn(2).setCellEditor(compile);

		libTable.getColumnModel().getColumn(0).setPreferredWidth(40);
		libTable.getColumnModel().getColumn(1).setPreferredWidth(280);
		libTable.getColumnModel().getColumn(2).setPreferredWidth(80);

		// Create the action panel.
		JPanel actionPanel = new JPanel();
		actionPanel.setLayout(new FlowLayout());
		actionPanel.add(add);
		actionPanel.add(remove);

        // Create the button panel.
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout());
        buttonPanel.add(ok);
        buttonPanel.add(cancel);

        // Insert components.
        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(actionPanel, BorderLayout.NORTH);
        getContentPane().add(new PaddedPane(tableScroller, 5),
            BorderLayout.CENTER);
        getContentPane().add(buttonPanel, BorderLayout.SOUTH);
    
        // Add event listeners.    
		add.addActionListener(this);
		remove.addActionListener(this);
        ok.addActionListener(this);
        cancel.addActionListener(this);
        getRootPane().setDefaultButton(ok);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
    }

    /**
     * Called when a button has been pressed.
     *
     * @param e the button pressing event.
     */
    public void actionPerformed(ActionEvent e) {
		if (e.getSource() == add) {
			JFileChooser chooser = new JFileChooser();
			NormalOptionSet options = shell.getOptions();
			chooser.setCurrentDirectory(new File(
				options.getStringOption("LastDir", ".")));
			chooser.setFileFilter(filter);
			chooser.setDialogTitle("Select Jython library...");
			if (chooser.showOpenDialog(shell.getPrimaryFrame()) ==
					chooser.APPROVE_OPTION) {
				File chosen = chooser.getSelectedFile();
				libPaths.addElement(chosen.getAbsolutePath());
				libUse.addElement(new Boolean(true));
				libTableModel.fireTableRowsInserted(
					libPaths.size() - 1, libPaths.size() - 1);
				libTable.scrollRectToVisible(
					libTable.getCellRect(libPaths.size() - 1, 0, true));

				// Update the system properties.
				String fileDir = chosen.getParentFile().getAbsolutePath();
				if (fileDir == null)
					fileDir = ".";
				options.setStringOption("LastDir", fileDir);
				options.writeToFile();
			}
		} else if (e.getSource() == remove) {
			if (libTable.getSelectedRowCount() == 0)
				shell.error("No libraries are selected to delete.");
			else {
				int[] rows = libTable.getSelectedRows();
				Arrays.sort(rows);
				for (int i = rows.length - 1; i >= 0; i--) {
					libPaths.removeElementAt(rows[i]);
					libUse.removeElementAt(rows[i]);
				}
				libTableModel.fireTableRowsDeleted(rows[0],
					rows[rows.length - 1]);
			}
		} else if (e.getSource() == ok) {
			Vector libraries = new Vector();
			Enumeration ePaths = libPaths.elements();
			Enumeration eUse = libUse.elements();
			while (ePaths.hasMoreElements()) {
				libraries.addElement(new NormalOptionSet.JythonLibrary(
					(String)ePaths.nextElement(),
					((Boolean)eUse.nextElement()).booleanValue()));
			}

			NormalOptionSet options = shell.getOptions();
            options.setJythonLibraries(libraries);
            options.writeToFile();
            dispose();
		} else if (e.getSource() == compile) {
			StringBuffer error = new StringBuffer();
			if (JPythonUtils.compileFile(new File(
					(String)libPaths.elementAt(compile.getCellEditorRow())),
					error) != null)
				shell.inform("The file compiles fine.");
			else {
				new LongMessageBox(this, "Compile Error", error.toString(),
					LongMessageBox.OK).runBox();
			}
        } else
            dispose();
    }

	/**
	 * Used to render Jython library names.
	 */
	private class LibNameRenderer extends DefaultTableCellRenderer {
		/**
		 * Create a new renderer.
		 */
		public LibNameRenderer() {
		}

		/**
		 * Returns the component that actually does the rendering.
		 */
		public Component getTableCellRendererComponent(JTable table,
				Object value, boolean isSelected, boolean hasFocus,
				int row, int column) {
			super.getTableCellRendererComponent(table, value,
				isSelected, hasFocus, row, column);
			setText(new File(value.toString()).getName());
			return this;
		}
	}

	/**
	 * Provides a model for the table containing the Jython libraries.
	 */
	private class LibTableModel extends AbstractTableModel {
		/**
		 * Returns the number of rows.
		 *
		 * @return the number of rows.
		 */
		public int getRowCount() {
			return libPaths.size();
		}

		/**
		 * Returns the number of columns.
		 *
		 * @return the number of columns.
		 */
		public int getColumnCount() {
			return 3;
		}

		/**
		 * Returns the name of the requested column.
		 *
		 * @param col the requested column.
		 * @return the name of the requested column.
		 */
		public String getColumnName(int col) {
			switch (col) {
				case 0: return "Use";
				case 1: return "Library Path";
				case 2: return "Compile";
				default: return null;
			}
		}

		/**
		 * Determines if the requested cell is editable.
		 *
		 * @param row the row of the requested cell.
		 * @param col the column of the requested cell.
		 * @return <tt>true</tt> if and only if the requested cell is
		 * editable.
		 */
		public boolean isCellEditable(int row, int col) {
			return true;
		}

		/**
		 * Returns the object stored in the requested cell.
		 *
		 * @param row the row of the requested cell.
		 * @param col the column of the requested cell.
		 */
		public Object getValueAt(int row, int col) {
			switch (col) {
				case 0: return libUse.elementAt(row);
				case 1: return libPaths.elementAt(row);
				case 2: return null;
				default: return null;
			}
		}

		/**
		 * Sets the value of the object in the requested cell and fires
		 * a corresponding event.
		 *
		 * @param value the new value of the object.
		 * @param row the row of the requested cell.
		 * @param col the column of the requested cell.
		 */
		public void setValueAt(Object value, int row, int col) {
			switch (col) {
				case 0: libUse.setElementAt(value, row); break;
				case 1: libPaths.setElementAt(value, row); break;
			}
			fireTableCellUpdated(row, col);
		}
	}
}

