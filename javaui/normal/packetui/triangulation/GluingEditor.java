
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

package normal.packetui.triangulation;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import normal.Shell;
import normal.engine.*;
import normal.engine.packet.*;
import normal.engine.triangulation.*;
import normal.packetui.*;
import org.gjt.btools.gui.dialog.*;

/**
 * An interface through which a user can view or edit gluings of
 * tetrahedra in a triangulation.
 * The central component of this interface is a table that uses
 * a <tt>TriangulationTableModel</tt> as its model.
 *
 * @see normal.packetui.triangulation.TriangulationTableModel
 */
public class GluingEditor extends DefaultPacketEditor
        implements PacketInfoUI, TableModelListener {
    /**
     * The packet associated with this interface.
     */
    private NTriangulation packet;
    
    /**
     * The table displaying triangulation data.
     */
    private JTable table;
    
    /**
     * The model representing data to display in the table.
     */
    private TriangulationTableModel tableModel;

    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * Create a new interface to work with a given packet.
     *
     * @param packet the packet to be associated with this interface.
     * @param shell the shell representing the entire program.
     * @param editor <tt>true</tt> if this interface is to be an
     * editor, or <tt>false</tt> if it is to be a viewer.
     */
    public GluingEditor(NPacket packet, Shell shell, boolean editor) {
        super(editor);
        this.packet = (NTriangulation)packet;
        this.shell = shell;
        init();
    }

    /**
     * Create the dialog's interface elements.
     */
    private void init() {
        boolean editor = isEditor();

        // Set the layout manager.
        setLayout(new BorderLayout());

        // Initialise the button pane.
        JPanel buttonPane = new JPanel();
        buttonPane.setLayout(new FlowLayout());
        JButton addTet = new JButton("Add Tet");
        JButton delTet = new JButton("Delete Tet");
        buttonPane.add(addTet);
        buttonPane.add(delTet);

        // Initialise the table.
        tableModel = new TriangulationTableModel(packet, editor);
        table = new JTable(tableModel);
        table.getTableHeader().setReorderingAllowed(false);
        table.getColumnModel().getColumn(0).setCellRenderer(new
            TetrahedronDescriptionRenderer());
        if (editor)
            table.getColumnModel().getColumn(0).setCellEditor(new
                TetrahedronDescriptionEditor());
        TetrahedronFaceRenderer faceRenderer = new TetrahedronFaceRenderer();
        TetrahedronFaceEditor faceEditor = null;
        if (editor)
            faceEditor = new TetrahedronFaceEditor(tableModel.getTetrahedra(),
                shell);
        for (int i=0; i<4; i++) {
            table.getColumnModel().getColumn(i+1).setCellRenderer(
                faceRenderer);
            if (editor)
                table.getColumnModel().getColumn(i+1).setCellEditor(
                    faceEditor);
        }
        if (editor)
            tableModel.addTableModelListener(this);

        // Insert the components.
        if (editor)
            add(buttonPane, BorderLayout.NORTH);
        add(new JScrollPane(table), BorderLayout.CENTER);
        
        // Add action listeners.
        addTet.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                tableModel.addTetrahedron();
                table.scrollRectToVisible(table.getCellRect(
                    tableModel.getRowCount() - 1, 0, true));
            }
        });
        delTet.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (table.getSelectedRowCount() == 0)
                    shell.error("No rows are selected to delete.");
                else if (shell.confirm("Are you sure you wish to delete " +
                        "the selected tetrahedra?"))
                    tableModel.deleteTetrahedra(table.getSelectedRows());
            }
        });
    }

    public NPacket getPacket() {
        return packet;
    }
    
    public void reflectPacket() {
        table.setEnabled(true);

        ignoreSetChanged(true);
        tableModel.readFromEngine(packet);
        ignoreSetChanged(false);
        setChanged(false);
    }

    public void editingElsewhere() {
        table.setEnabled(false);
    }

    public void applyChanges(Engine engine) {
        tableModel.writeToEngine(packet, engine);
        setChanged(false);
    }
    
    /**
     * Called whenever the user changes the table data.
     * Required by TableModelListener.
     *
     * @param e an event representing the change in table data.
     */
    public void tableChanged(TableModelEvent e) {
        setChanged(true);
    }
}
