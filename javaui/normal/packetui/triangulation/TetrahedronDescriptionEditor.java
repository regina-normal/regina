
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
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

/**
 * Used to edit cells bearing descriptions of given tetrahedron.
 * This is simply a text field into which the user can type a new description.
 */
public class TetrahedronDescriptionEditor extends JTextField
        implements TableCellEditor {
    /**
     * Number of clicks required to start editing.
     */
    public static final int clicksToEdit = 2;
    
    /**
     * Specific tetrahedron being edited.
     * @serial
     */
    private TetrahedronModel value = null;

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
     * Create a new editor for the description of a tetrahedron.
     */
    public TetrahedronDescriptionEditor() {
        setToolTipText("Description");
        addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                stopCellEditing();
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
     * Returns the value of the table cell being edited.
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
        fireEditingStopped();
        value.description = getText();
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
        selectAll();
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
        
        // Set the contents of the component.
        if (((TetrahedronModel)value).description.length() > 0)
            setText(this.value.description);
        else
            setText("");
        
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
}
