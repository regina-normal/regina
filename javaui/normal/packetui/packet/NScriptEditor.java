
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

package normal.packetui.packet;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.text.*;
import normal.*;
import normal.console.*;
import normal.engine.*;
import normal.engine.packet.*;
import normal.packetui.*;
import btools.ext.*;
import btools.gui.*;
import btools.gui.component.*;
import btools.gui.dialog.*;
import org.python.core.*;
import org.python.util.*;

/**
 * An interface through which a user can view or edit a script.
 * See <tt>PacketUI</tt> for more details.
 *
 * @see normal.packetui.PacketUI
 */
public class NScriptEditor extends DefaultPacketEditor {
    /**
     * The packet associated with this interface.
     * @serial
     */
    private NScript packet;
    
    /**
     * The shell representing the entire program.
     * @serial
     */
    private Shell shell;

    /**
     * The text region containing the packet's data.
     * @serial
     */
    private JTextArea text;

    /**
     * The document used by the text region.
     * @serial
     */
    private PlainDocument doc;

    /**
     * The names of the variables associated with this script.
     */
    private Vector variableNames = new Vector();

    /**
     * The values of the variables associated with this script.
     */
    private Vector variableValues = new Vector();

    /**
     * Do we wish to show a JPython console afterwards?
     */
    private JCheckBox showConsole;

    /**
     * A table displaying the variables that will be set in JPython
     * before the script is run.
     */
    private JTable variablesTable;

    /**
     * The model for the variables table.
     */
    private VariablesTableModel variablesTableModel;

    /**
     * A combo box with which variable values can be selected.
     */
    private JComboBox variableValueEditor;

    /**
     * Create a new interface to edit a given packet.
     *
     * @param packet the packet to be associated with this interface.
     * @param shell the shell representing the entire program.
     * @param editor <tt>true</tt> if this interface is to be used
     * for editing, or <tt>false</tt> if it is for viewing only.
     */
    public NScriptEditor(NPacket packet, Shell shell, boolean editor) {
        super(editor);
        this.packet = (NScript)packet;
        this.shell = shell;
        init(editor);
    }

    public NPacket getPacket() {
        return packet;
    }

    /**
     * Create the interface's interface elements.
     *
     * @param editor <tt>true</tt> if this interface is to be used
     * for editing, or <tt>false</tt> if it is for viewing only.
     */
    private void init(boolean editor) {
        // Set the layout manager.
        setLayout(new BorderLayout());
        
        // Initialise the components.
        doc = new PlainDocument();
        text = new JTextArea(doc);
        text.setLineWrap(false);
        text.setTabSize(4);
        text.setEditable(editor);
        Font font = text.getFont();
        text.setFont(new Font("Monospaced", Font.PLAIN,
            font == null ? 12 : font.getSize()));

        JButton run = new JButton("Run", Images.btnConsole.image());
        run.setAlignmentX(run.CENTER_ALIGNMENT);
        run.setEnabled(shell.hasFoundJPython());
        JButton compile = new JButton("Compile");
        compile.setAlignmentX(compile.CENTER_ALIGNMENT);
        compile.setEnabled(shell.hasFoundJPython());
        JButton warning = new JButton("Warning!");
        warning.setAlignmentX(warning.CENTER_ALIGNMENT);
        warning.setForeground(Color.red);
        JButton help = new JButton("Help");
        help.setAlignmentX(help.CENTER_ALIGNMENT);
        showConsole = new JCheckBox("Console", true);
        showConsole.setEnabled(false);
        showConsole.setAlignmentX(showConsole.CENTER_ALIGNMENT);
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.Y_AXIS));
        buttonPanel.add(run);
        buttonPanel.add(Box.createRigidArea(new Dimension(0, 5)));
        buttonPanel.add(compile);
        buttonPanel.add(Box.createRigidArea(new Dimension(0, 5)));
        buttonPanel.add(warning);
        buttonPanel.add(Box.createRigidArea(new Dimension(0, 5)));
        buttonPanel.add(help);
        buttonPanel.add(Box.createRigidArea(new Dimension(0, 5)));
        buttonPanel.add(showConsole);

        variablesTableModel = new VariablesTableModel();
        variablesTable = new JTable(variablesTableModel);
        JScrollPane variablesScroller = new JScrollPane(variablesTable);
        variablesScroller.setPreferredSize(new Dimension(60, 60));

        // Set the table cell editors.
        JTextField variableNameEditor = new JTextField();
        variableNameEditor.setDocument(new IdentifierDocument());
        variableValueEditor = new JComboBox(allAvailableVariables());
        variableValueEditor.setRenderer(new VariableRenderer());
        variablesTable.getColumnModel().getColumn(0).setCellEditor(
            new DefaultCellEditor(variableNameEditor));
        variablesTable.getColumnModel().getColumn(1).setCellEditor(
            new DefaultCellEditor(variableValueEditor));

        JPanel variablesButtonPane = new JPanel();
        JButton addVar = new JButton("Add");
        JButton delVar = new JButton("Remove");
        variablesButtonPane.setLayout(new GridLayout(2,1));
        variablesButtonPane.add(addVar);
        variablesButtonPane.add(delVar);

        JPanel variablesActionPane = new JPanel();
        variablesActionPane.setLayout(new BorderLayout());
        variablesActionPane.add(variablesButtonPane, BorderLayout.NORTH);

        JPanel variablesPane = new JPanel();
        variablesPane.setLayout(new BorderLayout());
        variablesPane.add(variablesScroller, BorderLayout.CENTER);
        if (editor)
            variablesPane.add(variablesActionPane, BorderLayout.WEST);

        JSplitPane dataPanel = new JSplitPane(JSplitPane.VERTICAL_SPLIT,
            variablesPane, new JScrollPane(text));
        dataPanel.setOneTouchExpandable(true);

        // Insert the components.
        add(dataPanel, BorderLayout.CENTER);
        add(new PaddedPane(buttonPanel, 3), BorderLayout.EAST);
        
        // Fill in all the correct values.
        reflectPacket();
        
        // Add event listeners.
        if (editor) {
            text.getDocument().addDocumentListener(new DocumentListener() {
                public void changedUpdate(DocumentEvent e) {
                    setChanged(true);
                }
                public void insertUpdate(DocumentEvent e) {
                    setChanged(true);
                }
                public void removeUpdate(DocumentEvent e) {
                    setChanged(true);
                }
            });
            variablesTableModel.addTableModelListener(
                new TableModelListener() {
                    public void tableChanged(TableModelEvent e) {
                        setChanged(true);
                    }
            });
        }
        addVar.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                variableNames.addElement("new");
                variableValues.addElement(Variable.none);
                variablesTableModel.fireTableRowsInserted(
                    variableNames.size(), variableNames.size());
                variablesTable.scrollRectToVisible(
                    variablesTable.getCellRect(variableNames.size(), 0, true));
            }
        });
        delVar.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (variablesTable.getSelectedRowCount() == 0)
                    shell.error("No variables are selected to delete.");
                else if (variablesTable.getSelectedRowCount() == 1 &&
                        variablesTable.getSelectedRow() == 0)
                    shell.error("You cannot delete the engine variable.");
                else {
                    variablesTableModel.deleteVariables(
                        variablesTable.getSelectedRows());
                }
            }
        });
        run.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                finishVariableEditing();
                if (checkVariables())
                    runScript(showConsole.isSelected());
            }
        });
        compile.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                finishVariableEditing();
                if (checkVariables())
                    if (compileScript())
                        shell.inform("The script compiles fine.");
            }
        });
        warning.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                shell.inform("You should refresh any packet " +
                    "subtree that is changed through a script or through " +
                    "the JPython console.  A button for this is located " + 
                    "beneath the packet tree.\n\n" +
                    "You should NEVER delete a packet " +
                    "through a script or through the JPython console; " +
                    "this will most likely crash the program.");
            }
        });
        help.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                shell.viewHelp("jpython");
            }
        });
    }

    /**
     * Forces any current edit of the script variables table to complete
     * itself.
     */
    private void finishVariableEditing() {
        if (variablesTable.isEditing())
            ((DefaultCellEditor)variablesTable.getCellEditor()).
                stopCellEditing();
    }

    public void reflectPacket() {
        finishVariableEditing();

        ignoreSetChanged(true);

        // Find the root packet in the packet tree.
        NPacket rootPacket = packet;
        NPacket tmpPacket = packet.getTreeParent();
        while (tmpPacket != null) {
            rootPacket = tmpPacket;
            tmpPacket = tmpPacket.getTreeParent();
        }

        long n = packet.getNumberOfVariables();
        long i;
        variableNames.removeAllElements();
        variableValues.removeAllElements();
        NPacket value;
        String valueString;
        for (i=0; i<n; i++) {
            variableNames.addElement(packet.getVariableName(i));

            valueString = packet.getVariableValue(i);
            if (valueString.length() == 0)
                variableValues.addElement(Variable.none);
            else {
                value = rootPacket.findPacketLabel(valueString);
                if (value == null)
                    variableValues.addElement(Variable.none);
                else
                    variableValues.addElement(new PacketVariable(value));
            }
        }

        try {
            if (doc.getLength() > 0)
                doc.remove(0, doc.getLength());

            n = packet.getNumberOfLines();
            for (i=0; i<n-1; i++)
                doc.insertString(doc.getLength(),
                    packet.getLine(i) + '\n', null);
            if (n > 0)
                doc.insertString(doc.getLength(), packet.getLine(n-1), null);
        } catch (BadLocationException e) {}

        variablesTableModel.fireTableDataChanged();

        ignoreSetChanged(false);
        setChanged(false);
    }
    
    public void applyChanges(Engine engine) {
        finishVariableEditing();

        packet.removeAllVariables();
        int tot = variableNames.size();
        Variable var;
        int i;
        for (i=0; i<tot; i++) {
            var = (Variable)variableValues.elementAt(i);
            if (var.getValue() instanceof NPacket)
                packet.addVariable((String)variableNames.elementAt(i),
                    ((NPacket)var.getValue()).getPacketLabel());
            else
                packet.addVariable((String)variableNames.elementAt(i), "");
        }

        packet.removeAllLines();
        Element root = doc.getDefaultRootElement();
        tot = root.getElementCount();
        Element line;
        try {
            for (i=0; i<tot; i++) {
                line = root.getElement(i);
                packet.addLast(doc.getText(line.getStartOffset(),
                    line.getEndOffset() - line.getStartOffset() - 1));
            }
        } catch (BadLocationException e) {}

        setChanged(false);
    }

    /**
     * Returns this entire script as a string to pass to JPython.
     * An extra newline will be appended to ensure no loops or procedures
     * are unfinished.  The script used will be as shown in this
     * interface, whether or not the underlying engine contains
     * something different (this is because it is the script in the
     * interface that the user is looking at).
     *
     * @return the entire script as a string.
     */
    private String entireScript() {
        /*
        StringBuffer ans = new StringBuffer();

        long n = packet.getNumberOfLines();
        for (long i=0; i<n; i++) {
            ans.append(packet.getLine(i));
            ans.append('\n');
        }
        ans.append('\n');

        return ans.toString();
        */
        try {
            return doc.getText(0, doc.getLength()) + '\n';
        } catch (BadLocationException e) {
            return null;
        }
    }

    /**
     * Attempts to compile this script.  Compile errors will be sent to
     * the given console.  The compiled code will be returned.
     *
     * @param console the console to which compile errors will be sent.
     * @return the compiled code, or <tt>null</tt> if the code did not
     * compile or if JPython expected more code.
     */
    private PyObject compileScript(ConsolePane console) {
		StringBuffer error = new StringBuffer();
		PyObject code = JPythonUtils.compileCode(entireScript(), error);
		if (code == null)
			console.outputMessage(error.toString());
		return code;
	}

    /**
     * Attempts to compile this script.  If it fails to compile, a
     * JPython console will be brought up showing the error.
     *
     * @return <tt>true</tt> if and only if the compilation was
     * successful.
     */
    private boolean compileScript() {
        JPythonConsoleFrame console =
            new JPythonConsoleFrame(shell, false);
        console.getConsole().outputLine(
			setupInterpreter(console.getPythonInterpreter()));

        if (compileScript(console.getConsole()) != null)
            return true;

        console.startConsole("\nThe script did not compile.\n");
        Positioner.centerOnScreen(console);
        console.show();
        return false;
    }

    /**
     * Runs this script, optionally bringing up a JPython console that
     * will show the results and allow further interaction.
     * If a compile error occurs, a JPython console will be brought up
     * regardless.
     *
     * @param shouldShowConsole <tt>true</tt> if and only if a JPython
     * console should be offered to the user.
     */
    private void runScript(boolean shouldShowConsole) {
        JPythonConsoleFrame console =
            new JPythonConsoleFrame(shell, false);
        console.getConsole().outputLine(
			setupInterpreter(console.getPythonInterpreter()));

		String greeting = null;
        PyObject code = compileScript(console.getConsole());
        if (code == null) {
            shouldShowConsole = true;
            greeting = "\nThe script did not compile.\n";
        } else {
            // Try actually running the code.
            console.getConsole().outputMessage("Running script [" +
                packet.getPacketLabel() + "]:\n\n");

			StringBuffer error = new StringBuffer();
			if (! JPythonUtils.runCode(code, console.getPythonInterpreter(),
					error)) {
                shouldShowConsole = true;
                console.getConsole().outputMessage(error.toString());
                greeting = "\nA runtime error occurred in the script.\n";
			}
        }

        if (shouldShowConsole) {
            console.startConsole(greeting);
            Positioner.centerOnScreen(console);
            console.show();
        }
    }

    /**
     * Runs the standard startup commands and
     * sets the variables to be used with this script in the given JPython
     * interpreter.
     *
     * @param interpreter the JPython interpreter in which to set the
     * variables.
	 * @return a text string to inform the user of the initialisation
	 * that has been done; this may contain multiple lines and will end
	 * in a final newline.
     */
    private String setupInterpreter(PythonInterpreter interpreter) {
		String message =
			JPythonUtils.setupInterpreter(interpreter, shell);

        int tot = variableNames.size();
        for (int i=0; i<tot; i++)
            interpreter.set((String)variableNames.elementAt(i),
                ((Variable)variableValues.elementAt(i)).getValue());

		return message + "Assigned values to script variables.\n";
    }

    /**
     * Checks that the names of the script variables are sane.
     * If not, the user will be notified.
     *
     * @return <tt>true</tt> if the operation that called this routine
     * should continue, or <tt>false</tt> if it should abort.
     */
    private boolean checkVariables() {
        int tot = variableNames.size();
        String name;
        for (int i=0; i<tot; i++) {
            name = (String)variableNames.elementAt(i);
            if (name.length() == 0) {
                shell.inform("One of the variables has no name.");
                return false;
            }
            if (name.equals("engine") || variableNames.indexOf(name, i+1) >= 0)
                if (! shell.confirm("The variable [" + name +
                        "] appears twice.  Do you wish to continue?"))
                    return false;
        }
        return true;
    }

    /**
     * Returns a vector containing all available values that can be
     * assigned to variables associated with this script.
     *
     * @return a vector containing all available variable values.
     */
    private Vector allAvailableVariables() {
        Vector ans = new Vector();
        ans.addElement(Variable.none);

        // Find the root packet.
        NPacket currPacket = packet;
        NPacket tmp = packet.getTreeParent();
        while (tmp != null) {
            currPacket = tmp;
            tmp = currPacket.getTreeParent();
        }

        // Add in all the available packets.
        while (currPacket != null) {
            ans.addElement(new PacketVariable(currPacket));
            currPacket = currPacket.nextTreePacket();
        }

        return ans;
    }

    public void packetWasRenamed(NPacket packet, PacketUI ui, Frame owner) {
        finishVariableEditing();

        int tot = variableNames.size();
        for (int i=0; i<tot; i++)
            if (packet.equals(((Variable)variableValues.elementAt(i)).
                    getValue()))
                variablesTableModel.fireTableCellUpdated(i+1, 1);
    }

    public void subtreeToBeDeleted(NPacket subtree, PacketUI ui, Frame owner) {
        finishVariableEditing();

        // Remove all the offensive packets from the combo box.
        Stack mustRemove = new Stack();
        DefaultComboBoxModel model = (DefaultComboBoxModel)
            variableValueEditor.getModel();
        int tot = model.getSize();
        int i;
        Variable value;
        for (i=0; i<tot; i++) {
            value = (Variable)model.getElementAt(i);
            if (value instanceof PacketVariable)
                if (subtree.isGrandparentOf((NPacket)value.getValue()))
                    mustRemove.push(new Integer(i));
        }
        while (! mustRemove.empty())
            model.removeElementAt(((Integer)mustRemove.pop()).intValue());

        // Set all offensive variables to None.
        tot = variableNames.size();
        for (i=0; i<tot; i++) {
            value = (Variable)variableValues.elementAt(i);
            if (value instanceof PacketVariable)
                if (subtree.isGrandparentOf((NPacket)value.getValue())) {
                    variableValues.setElementAt(Variable.none, i);
                    variablesTableModel.fireTableCellUpdated(i+1, 1);
                }
        }
    }

    public void subtreeWasInserted(NPacket subtree, PacketUI ui, Frame owner) {
        finishVariableEditing();

        variableValueEditor.setModel(new DefaultComboBoxModel(
            allAvailableVariables()));
    }

    public void subtreeHasChanged(NPacket subtree, PacketUI ui, Frame owner) {
        finishVariableEditing();

        variableValueEditor.setModel(new DefaultComboBoxModel(
            allAvailableVariables()));

        int tot = variableNames.size();
        Variable value;
        for (int i=0; i<tot; i++) {
            value = (Variable)variableValues.elementAt(i);
            if (value instanceof PacketVariable)
                if (subtree.isGrandparentOf((NPacket)value.getValue()))
                    variablesTableModel.fireTableCellUpdated(i+1, 1);
        }
    }

    /**
     * Represents a possible value of a variable.
     * Note that variable values should override <tt>toString()</tt> so
     * they can be placed in combo boxes and the like.
     */
    private static interface Variable {
        /**
         * A variable representing the Python value <tt>None</tt>.
         */
        public static Variable none = new NoneVariable();

        /**
         * Returns the value of this variable as an object that can be
         * used with JPython.
         *
         * @return the value of this variable.
         */
        public Object getValue();
    }

    /**
     * A variable representing the Python value <tt>None</tt>.
     */
    private static class NoneVariable implements Variable {
        /**
         * Provides a string representation of this value.
         *
         * @return a string representation of this value.
         */
        public String toString() {
            return "<None>";
        }

        public Object getValue() {
            try {
                return Py.None;
            } catch (Throwable th) {
                return null;
            }
        }
    }

    /**
     * A variable whose value is a packet.
     */
    private static class PacketVariable implements Variable {
        /**
         * The packet represented by this variable.
         */
        private NPacket packet;

        /**
         * Creates a new variable value representing the given packet.
         *
         * @param packet the packet this value will represent.
         */
        public PacketVariable(NPacket packet) {
            this.packet = packet;
        }

        /**
         * Provides a string representation of this value.
         *
         * @return a string representation of this value.
         */
        public String toString() {
            return packet.getFullName();
        }

        public Object getValue() {
            return packet;
        }

        /**
         * Determines if this and the given object are both variable
         * values representing the same packet in the underlying
         * calculation engine.
         *
         * @param compare the object to compare with this.
         * @return <tt>true</tt> if and only if this and the given
         * object are variable values representing the same packet in
         * the underlying calculation engine.
         */
        public boolean equals(Object compare) {
            if (compare instanceof Variable)
                return getValue().equals(((Variable)compare).getValue());
            return false;
        }
    }

    /**
     * Provides a renderer for variable values that puts cute
     * icons of the appropriate type next to packets.
     */
    private static class VariableRenderer extends DefaultListCellRenderer {
        /**
         * Returns the component to be used to draw the given variable
         * value.
         */
        public Component getListCellRendererComponent(JList list,
                Object value, int index, boolean isSelected,
                boolean hasFocus) {
            super.getListCellRendererComponent(list, value, index,
                isSelected, hasFocus);
            if (value instanceof PacketVariable)
                setIcon(PacketUIManager.packetIcon(
                    (NPacket)((Variable)value).getValue()));
            else
                setIcon(null);
            return this;
        }
    }

    /**
     * Provides a model for the table containing the variables
     * associated with this script.
     */
    private class VariablesTableModel extends AbstractTableModel {
        /**
         * Returns the number of rows in the table.
         *
         * @return the number of rows.
         */
        public int getRowCount() {
            return variableNames.size() + 1;
        }

        /**
         * Returns the number of columns in the table.
         *
         * @return the number of columns.
         */
        public int getColumnCount() {
            return 2;
        }

        /**
         * Determines if the requested cell is editable.
         *
         * @param row the row of the requested cell.
         * @param column the column of the requested cell.
         * @return <tt>true</tt> if and only if the requested cell is
         * editable.
         */
        public boolean isCellEditable(int row, int column) {
            return (isEditor() && row > 0);
        }

        /**
         * Returns the object stored in the requested cell.
         *
         * @param row the row of the requested cell.
         * @param column the column of the requested cell.
         */
        public Object getValueAt(int row, int column) {
            if (row == 0)
                return (column == 0 ? "engine" : "Calculation Engine");
            else if (column == 0)
                return variableNames.elementAt(row - 1);
            else
                return variableValues.elementAt(row - 1);
        }

        /**
         * Sets the value of the object in the requested cell and fires
         * a corresponding event.
         *
         * @param value the new value of the object.
         * @param row the row of the requested cell.
         * @param column the column of the requested cell.
         */
        public void setValueAt(Object value, int row, int column) {
            if (row > 0) {
                if (column == 0)
                    variableNames.setElementAt(value, row - 1);
                else
                    variableValues.setElementAt(value, row - 1);
            }
            fireTableCellUpdated(row, column);
        }

        /**
         * Returns the name of the requested column.
         *
         * @param column the requested column.
         * @return the name of the requested column.
         */
        public String getColumnName(int column) {
            if (column == 0)
                return "Variable";
            else
                return "Value";
        }

        /**
         * Deletes the given set of rows from the table and removes the
         * corresponding script variables from the internal list.
         * The list of rows to delete need not be in any particular
         * order.
         *
         * @param rowsToDelete an array containing the row numbers of
         * the rows to delete.
         */
        public void deleteVariables(int[] rowsToDelete) {
            Arrays.sort(rowsToDelete);
            for (int i = rowsToDelete.length - 1; i >= 0; i--)
                if (rowsToDelete[i] != 0) {
                    variableNames.removeElementAt(rowsToDelete[i] - 1);
                    variableValues.removeElementAt(rowsToDelete[i] - 1);
                }
            fireTableRowsDeleted(rowsToDelete[0],
                rowsToDelete[rowsToDelete.length - 1]);
        }
    }
}

