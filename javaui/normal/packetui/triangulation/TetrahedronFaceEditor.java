
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

package normal.packetui.triangulation;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.text.*;
import normal.Shell;
import normal.engine.triangulation.*;
import btools.gui.*;
import btools.gui.dialog.*;

/**
 * Used to edit cells containing face gluings for tetrahedra.
 * These are buttons which, when pressed, will bring up a dialog in which
 * the user can specify a new gluing.
 */
public class TetrahedronFaceEditor extends JButton implements TableCellEditor {
    /**
     * Number of clicks required to start editing.
     */
    public static final int clicksToEdit = 1;

    /**
     * Specific tetrahedron being edited.
     * @serial
     */
    private TetrahedronModel value = null;

    /**
     * Face of the tetrahedron currently being edited.
     * @serial
     */
    private int face;

    /**
     * List of all tetrahedra in the current triangulation.
     * @serial
     */
    private Vector tetrahedra;

    /**
     * Was OK pressed in the face gluing dialog?
     * @serial
     */
    private boolean gluingConfirmed;

    /**
     * The tetrahedron to which this will be glued.
     * @serial
     */
    private TetrahedronModel otherTetrahedron;

    /**
     * The face of the other tetrahedron in the gluing.
     * @serial
     */
    private int otherFace;

    /**
     * The gluing permutation that was requested.
     * @serial
     */
    private NPerm requestedGluing;

    /**
     * List of event listeners for this cell editor.
     * @serial
     */
    private EventListenerList listenerList = new EventListenerList();

    /**
     * Event to be fired when appropriate.
     */
    transient private ChangeEvent changeEvent = null;

    /**
     * The shell representing the entire program.
     * @serial
     */
    private Shell shell;
            
    /**
     * Create a new editor for a face gluing of a tetrahedron.
     *
     * @param tetrahedra the list of tetrahedra forming the triangulation
     * that the user is editing.
     * @param shell the shell representing the entire program.
     */
    public TetrahedronFaceEditor(Vector tetrahedra, Shell shell) {
        super("...");
        this.tetrahedra = tetrahedra;
        this.shell = shell;
        setBorder(BorderFactory.createRaisedBevelBorder());
        setToolTipText("Press to edit face gluing");
        addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                gluingConfirmed = false;
                new GluingDialog().show();
                if (gluingConfirmed)
                    stopCellEditing();
                else
                    cancelCellEditing();
            }
        });
    }

    /**
     * Determines if the given event is cause to start editing the table
     * cell.
     *
     * @param e the event under consideration.
     * @return <tt>true</tt> if and only if the table cell should be
     * edited.
     */
    public boolean isCellEditable(EventObject e) {
        // If there weren't enough mouse clicks, don't edit.
        if (e instanceof MouseEvent)
            if (((MouseEvent)e).getClickCount() < clicksToEdit)
                return false;
        return true;
    }

    /**
     * Returns the value of the cell being edited.
     * This will in fact return the tetrahedron whose row is being
     * edited.
     *
     * @return the value of the table cell being edited.
     */
    public Object getCellEditorValue() {
        return value;
    }

    /**
     * Stops editing the table cell and stores any changes.
     *
     * @return <tt>true</tt> if and only if editing was successfully
     * stopped.
     */
    public boolean stopCellEditing() {
        if (! gluingConfirmed) {
            cancelCellEditing();
            return false;
        }
        fireEditingStopped();
        // Unglue anything already present.
        if (value.adjacent[face] != null) {
            int currentYourFace = value.gluings[face].imageOf(face);
            value.adjacent[face].adjacent[currentYourFace] = null;
            value.adjacent[face] = null;
        }
        // Make any new requested gluings.
        if (otherTetrahedron != null) {
            if (otherTetrahedron.adjacent[otherFace] != null) {
                // Unglue the other tetrahedron from its existing committment.
                int farAwayFace = otherTetrahedron.gluings[otherFace].
                    imageOf(otherFace);
                otherTetrahedron.adjacent[otherFace].adjacent[farAwayFace] =
                    null;
                otherTetrahedron.adjacent[otherFace] = null;
            }
            value.adjacent[face] = otherTetrahedron;
            otherTetrahedron.adjacent[otherFace] = value;
            value.gluings[face] = requestedGluing;
            otherTetrahedron.gluings[otherFace] = requestedGluing.inverse();
        }
        return true;
    }

    /**
     * Cancels editing the table cell and disregards any changes.
     */
    public void cancelCellEditing() {
        fireEditingCancelled();
    }

    /**
     * Prepares the component for editing as requested by the given
     * event.
     *
     * @param e the event that caused editing to begin.
     * @return <tt>true</tt> if and only if the table cell being edited
     * should be selected.
     */
    public boolean shouldSelectCell(EventObject e) {
        requestFocus();
        return false;
    }

    /**
     * Returns the component that actually does the editing.
     *
     * @param table the table in which a cell is being edited.
     * @param value the value of the table cell being edited (which will
     * in fact be the tetrahedron whose row is being edited).
     * @param isSelected whether or not the table cell is currently
     * selected.
     * @param row the row of the table cell being edited.
     * @param column the column of the table cell being edited.
     * @return the component that does the editing.
     */
    public Component getTableCellEditorComponent(JTable table, Object value,
            boolean isSelected, int row, int column) {
        this.value = (TetrahedronModel)value;
        this.face = 4 - column;
        this.gluingConfirmed = false;
        return this;
    }

    /**
     * Add an event listener to await events from this object.
     * <p>
     * <b>Precondition:</b> The given event listener is not currently
     * listening for events from this object.
     *
     * @param l the listener to add.
     */
    public void addCellEditorListener(CellEditorListener l) {
        listenerList.add(CellEditorListener.class, l);
    }

    /**
     * Remove an event listener from the collection of listeners
     * awaiting events from this object.
     * <p>
     * <b>Precondition:</b> The given event listener is currently
     * listening for events from this object.
     *
     * @param l the listener to remove.
     */
    public void removeCellEditorListener(CellEditorListener l) {
        listenerList.remove(CellEditorListener.class, l);
    }

    /**
     * Notify all event listeners that editing has stopped.
     */
    private void fireEditingStopped() {
        // Guaranteed to return a non-null array.
        Object[] listeners = listenerList.getListenerList();
        // Process the listeners last to first, notifying
        // those that are interested in this event.
        for (int i = listeners.length-2; i>=0; i-=2) {
            if (listeners[i]==CellEditorListener.class) {
                // Lazily create the event.
                if (changeEvent == null)
                    changeEvent = new ChangeEvent(this);
                ((CellEditorListener)listeners[i+1]).
                    editingStopped(changeEvent);
            }    
        }
    }

    /**
     * Notify all event listeners that editing has been cancelled.
     */
    private void fireEditingCancelled() {
        // Guaranteed to return a non-null array.
        Object[] listeners = listenerList.getListenerList();
        // Process the listeners last to first, notifying
        // those that are interested in this event.
        for (int i = listeners.length-2; i>=0; i-=2) {
            if (listeners[i]==CellEditorListener.class) {
                // Lazily create the event.
                if (changeEvent == null)
                    changeEvent = new ChangeEvent(this);
                ((CellEditorListener)listeners[i+1]).
                    editingCanceled(changeEvent);
            }    
        }
    }

    /**
     * A dialog through which the user can specify which face of which
     * tetrahedron will be glued to the current face of the current
     * tetrahedron.
     */
    private class GluingDialog extends JDialog implements ActionListener,
            ItemListener {
        /**
         * A combo box from which the user can choose the tetrahedron
         * to which we will glue the current tetrahedron.
         */
        private JComboBox tetrahedron;
        
        /**
         * A text field in which the user can enter the face to which
         * the current face will be glued.
         */
        private JTextField vertices;

        /**
         * The OK button.
         */
        private JButton ok;
        
        /**
         * Create a new dialog.
         */
        public GluingDialog() {
            super(shell.getPrimaryFrame(), "Edit Face Gluing", true);
            init();
            pack();
            Positioner.centerOnScreen(this);
        }
        
        /**
         * Initialise the dialog components.
         */
        private void init() {
            // Create panels.
            JPanel infoPanel = new JPanel();
            JPanel buttonPanel = new JPanel();
            
            // Set layout managers.
            getContentPane().setLayout(new BorderLayout());
            infoPanel.setLayout(new GridLayout(3,2));
            buttonPanel.setLayout(new FlowLayout());
            
            // Set up the input components.
            int nTetrahedra = tetrahedra.size();
            String[] tetChoices = new String[nTetrahedra+1];
            tetChoices[0] = "(boundary)";
            for (int i=0; i<nTetrahedra; i++) {
                tetChoices[i+1] = String.valueOf(i);
                if (((TetrahedronModel)tetrahedra.elementAt(i)).
                        description.length() > 0)
                    tetChoices[i+1] += " (" + ((TetrahedronModel)tetrahedra.
                        elementAt(i)).description + ")";
            }
            tetrahedron = new JComboBox(tetChoices);
            tetrahedron.setEditable(true);
            vertices = new JTextField(new FaceDocument(), "", 3);
            if (value.adjacent[face] == null) {
                tetrahedron.setSelectedIndex(0);
                vertices.setText("");
                vertices.setEnabled(false);
            } else {
                tetrahedron.setSelectedIndex(value.adjacent[face].index + 1);
                vertices.setText(NPerm.faceDescription(
                    value.gluings[face].composeWith(
                    NPerm.faceOrdering(face))));
                vertices.setEnabled(true);
            }
            
            // Create the information panel.
            infoPanel.add(new JLabel("Tetrahedron "));
            infoPanel.add(new JLabel("Face"));
            if (value.description.length() == 0)
                infoPanel.add(new JLabel(String.valueOf(value.index)));
            else
                infoPanel.add(new JLabel(String.valueOf(value.index)
                    + "( " + value.description + ')'));
            infoPanel.add(new JLabel(NPerm.faceDescription(face)));
            infoPanel.add(tetrahedron);
            infoPanel.add(vertices);
            
            // Create the button panel.
            ok = new JButton("OK");
            JButton cancel = new JButton("Cancel");
            buttonPanel.add(ok);
            buttonPanel.add(cancel);
            
            // Insert components.
            getContentPane().add(infoPanel, BorderLayout.CENTER);
            getContentPane().add(buttonPanel, BorderLayout.SOUTH);
            
            // Add event listeners.
            ok.addActionListener(this);
            cancel.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        dispose();
                    }
                });
            tetrahedron.addItemListener(this);
            getRootPane().setDefaultButton(ok);
            setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        }

        /**
         * Called when the user expresses the desire to close the dialog
         * and store any changes.
         *
         * @param e the action performed by the user.
         */
        public void actionPerformed(ActionEvent e) {
            if (verifyGluing()) {
                gluingConfirmed = true;
                dispose();
            }
        }

        /**
         * Called when the value in the combo box is changed.
         * Tries to establish precisely which tetrahedron the user
         * has specified.
         * The vertices box is also disabled if the boundary was
         * selected.
         *
         * @param e the combo box alteration event.
         */
        public void itemStateChanged(ItemEvent e) {
            if (e.getStateChange() != e.SELECTED)
                return;

            boolean valid = true;
            if (tetrahedron.getSelectedIndex() < 0) {
                // Start guessing.
                String text = (String)tetrahedron.getSelectedItem();
                if (text == null)
                    valid = false;
                else if (text.equals("INVALID")) {
                    // Make sure this call is not just a result of this
                    // routine setting the combo box contents to "INVALID".
                    ok.setEnabled(false);
                    vertices.setEnabled(true);
                    return;
                } else {
                    text = text.trim();
                    if (text.length() == 0)
                        // Assume it's the boundary.
                        tetrahedron.setSelectedIndex(0);
                    else {
                        // See if we've got an integer.
                        try {
                            int tetIndex = Integer.parseInt(text);
                            if (tetIndex < 0 || tetIndex >= tetrahedra.size())
                                valid = false;
                            else
                                tetrahedron.setSelectedIndex(tetIndex + 1);
                        } catch (NumberFormatException exc) {
                            valid = false;
                        }
                    }
                }
            }

            if (! valid) {
                tetrahedron.setSelectedItem("INVALID");
                MessageBox.fgNote(this,
                    "You must specify either the boundary or one of the " +
                    "tetrahedra in the triangulation.");
            }
            ok.setEnabled(valid);
            vertices.setEnabled(tetrahedron.getSelectedIndex() != 0);
        }

        /**
         * Verify that the requested gluing is legal.  The user will
         * be prompted with message boxes if the gluing is illegal.
         *
         * @return <tt>true</tt> if and only if the gluing
         * specified by the user is legal.
         */
        private boolean verifyGluing() {
            String yourFaceString = vertices.getText();
            int yourIndex = tetrahedron.getSelectedIndex() - 1;
            if (yourIndex < -1) {
                MessageBox.fgNote(this,
                    "You must specify either the boundary or one of the " +
                    "tetrahedra in the triangulation.");
                return false;
            }
            if (yourIndex == -1) {
                otherTetrahedron = null;
                return true;
            }
            if (yourFaceString.length() != 3) {
                MessageBox.fgNote(this, "A face should be specified by three " +
                    "vertices, such as 302, in the order in which they " +
                    "will be glued to the vertices of the other tetrahedron.");
                return false;
            }
            otherFace = 6 - (yourFaceString.charAt(0)-'0')
                             - (yourFaceString.charAt(1)-'0')
                             - (yourFaceString.charAt(2)-'0');
            if (yourIndex == value.index && otherFace == face) {
                MessageBox.fgNote(this, "You cannot glue a face to itself.");
                return false;
            }
            otherTetrahedron =
                (TetrahedronModel)tetrahedra.elementAt(yourIndex);
            NPerm myFace = NPerm.faceOrdering(face);
            requestedGluing = new NPerm(face, otherFace,
                myFace.imageOf(0), yourFaceString.charAt(0) - '0',
                myFace.imageOf(1), yourFaceString.charAt(1) - '0',
                myFace.imageOf(2), yourFaceString.charAt(2) - '0');
            if (otherTetrahedron.adjacent[otherFace] != null) {
                if (otherTetrahedron.adjacent[otherFace] != value) {
                    if (! MessageBox.confirm(this, "Tetrahedron " + yourIndex +
                            " is already glued to something else along " +
                            "the specified face.  Continue?"))
                        return false;
                } else if (! otherTetrahedron.gluings[otherFace].
                        composeWith(requestedGluing).isIdentity()) {
                    if (! MessageBox.confirm(this,
                            "These two faces are already glued together " +
                            "using the vertices in a different order.  " +
                            "Continue?"))
                        return false;
                }
            }
            return true;
        }
        
        /**
         * Document class used to enter a tetrahedron face specification
         * into a text component.
         * Ensures the input contains no more than three characters,
         * that only 0-3 are used and that no character occurs more
         * than once.
         */
        private class FaceDocument extends PlainDocument {
            /**
             * Attempts to insert a string into the document.
             *
             * @param offs the position at which to insert the string.
             * @param str the string to insert.
             * @param a the attributes to give the string.
             * @throws BadLocationException thrown if the string may not
             * be inserted at the requested position.
             */
            public void insertString(int offs, String str, AttributeSet a)
                    throws BadLocationException {
                int docLength = getLength();
                String docText = getText(0, docLength);
                if ((docLength + str.length()) > 3) {
                    Toolkit.getDefaultToolkit().beep();
                    return;
                }
                int used[] = new int[4];
                boolean error = false;
                int i;
                char c;
                for (i=0; i<4; i++)
                    used[i] = 0;
                for (i=0; i < docLength; i++)
                    used[docText.charAt(i) - '0']++;
                for (i=0; i < str.length(); i++) {
                    c = str.charAt(i);
                    if (c < '0' || c > '3') {
                        error = true;
                        break;
                    }
                    if (used[c - '0'] > 0) {
                        error = true;
                        break;
                    }
                    used[c - '0']++;
                }
                if (error)
                    Toolkit.getDefaultToolkit().beep();
                else
                    super.insertString(offs, str, a);
            }
        }
    }
}
