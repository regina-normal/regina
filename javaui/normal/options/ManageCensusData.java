
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import normal.Shell;
import normal.mainui.TopologyPane;
import org.gjt.btools.ext.ExtensionFilenameFilter;
import org.gjt.btools.gui.Positioner;
import org.gjt.btools.gui.component.*;

/**
 * A modal dialog allowing the census data files to be configured by the user.
 * If OK is pressed, the working option set will be changed and written to
 * file.
 */
public class ManageCensusData extends JDialog implements ActionListener {
    /**
     * Add a census data file.
     */
    private JButton add;
    /**
     * Remove a census data file.
     */
    private JButton remove;
    /**
     * OK button.
     */
    private JButton ok;

    /**
     * The table of census data files.
     */
    private JTable censusTable;
    /**
     * The model for the census data file table.
     */
    private CensusTableModel censusTableModel;
    /**
     * The list of paths to census data files.
     */
    private Vector censusPaths;
    /**
     * The list of whether or not census data files should be used.
     */
    private Vector censusUse;

    /**
     * The shell corresponding to the entire program.
     */
    private Shell shell;
    /**
     * A filename filter representing census data files.
     */
    private ExtensionFilenameFilter filter = TopologyPane.filenameFilter;

    /**
     * Creates a new census data configuration dialog.
     *
     * @param shell the shell corresponding to the entire program.
     */
    public ManageCensusData(Shell shell) {
        super(shell.getPrimaryFrame(), "Census Data Files", true);
        this.shell = shell;
        init();
        pack();
        Positioner.centerOnScreen(this);
    }

    /**
     * Initialise the interface components.
     */
    private void init() {
        censusPaths = new Vector();
        censusUse = new Vector();
        Enumeration e = shell.getOptions().getCensusData().elements();
        NormalOptionSet.CensusData census;
        while (e.hasMoreElements()) {
            census = (NormalOptionSet.CensusData)e.nextElement();
            censusPaths.addElement(census.getCensusPath());
            censusUse.addElement(new Boolean(census.shouldUseCensus()));
        }

        add = new JButton("Add");
        remove = new JButton("Remove");
        ok = new JButton("OK");
        JButton cancel = new JButton("Cancel");

        // Set up the table.
        censusTableModel = new CensusTableModel();
        censusTable = new JTable(censusTableModel);
        JScrollPane tableScroller = new JScrollPane(censusTable);
        tableScroller.setPreferredSize(new Dimension(400, 150));

        CheckBoxRenderer cr = new CheckBoxRenderer();
        censusTable.getColumnModel().getColumn(0).setCellRenderer(cr);
        censusTable.getColumnModel().getColumn(0).setCellEditor(cr);
        censusTable.getColumnModel().getColumn(1).setCellRenderer(
            new CensusNameRenderer());
        censusTable.getColumnModel().getColumn(1).setCellEditor(
            new LongStringRenderer(2));

        censusTable.getColumnModel().getColumn(0).setPreferredWidth(40);
        censusTable.getColumnModel().getColumn(1).setPreferredWidth(280);

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
            chooser.setDialogTitle("Select census data file...");
            if (chooser.showOpenDialog(shell.getPrimaryFrame()) ==
                    chooser.APPROVE_OPTION) {
                File chosen = chooser.getSelectedFile();
                censusPaths.addElement(chosen.getAbsolutePath());
                censusUse.addElement(new Boolean(true));
                censusTableModel.fireTableRowsInserted(
                    censusPaths.size() - 1, censusPaths.size() - 1);
                censusTable.scrollRectToVisible(
                    censusTable.getCellRect(censusPaths.size() - 1, 0, true));

                // Update the system properties.
                String fileDir = chosen.getParentFile().getAbsolutePath();
                if (fileDir == null)
                    fileDir = ".";
                options.setStringOption("LastDir", fileDir);
                options.writeToFile();
            }
        } else if (e.getSource() == remove) {
            if (censusTable.getSelectedRowCount() == 0)
                shell.error("No census files are selected to delete.");
            else {
                int[] rows = censusTable.getSelectedRows();
                Arrays.sort(rows);
                for (int i = rows.length - 1; i >= 0; i--) {
                    censusPaths.removeElementAt(rows[i]);
                    censusUse.removeElementAt(rows[i]);
                }
                censusTableModel.fireTableRowsDeleted(rows[0],
                    rows[rows.length - 1]);
            }
        } else if (e.getSource() == ok) {
            Vector censusData = new Vector();
            Enumeration ePaths = censusPaths.elements();
            Enumeration eUse = censusUse.elements();
            while (ePaths.hasMoreElements()) {
                censusData.addElement(new NormalOptionSet.CensusData(
                    (String)ePaths.nextElement(),
                    ((Boolean)eUse.nextElement()).booleanValue()));
            }

            NormalOptionSet options = shell.getOptions();
            options.setCensusData(censusData);
            options.writeToFile();
            dispose();
        } else
            dispose();
    }

    /**
     * Used to render census data file names.
     */
    private class CensusNameRenderer extends DefaultTableCellRenderer {
        /**
         * Create a new renderer.
         */
        public CensusNameRenderer() {
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
     * Provides a model for the table containing the census data files.
     */
    private class CensusTableModel extends AbstractTableModel {
        /**
         * Returns the number of rows.
         *
         * @return the number of rows.
         */
        public int getRowCount() {
            return censusPaths.size();
        }

        /**
         * Returns the number of columns.
         *
         * @return the number of columns.
         */
        public int getColumnCount() {
            return 2;
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
                case 1: return "Census Data File";
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
                case 0: return censusUse.elementAt(row);
                case 1: return censusPaths.elementAt(row);
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
                case 0: censusUse.setElementAt(value, row); break;
                case 1: censusPaths.setElementAt(value, row); break;
            }
            fireTableCellUpdated(row, col);
        }
    }
}

