
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
import javax.swing.*;
import javax.swing.table.*;
import normal.engine.triangulation.*;
import org.gjt.btools.gui.*;
import org.gjt.btools.gui.component.*;

/**
 * A non-modal dialog which can be used to view specific details of a
 * triangulation skeleton.
 */
public class SkeletonTableFrame extends JDialog {
    /**
     * Denotes triangulation vertices.
     */
    public static final int VERTICES = 1;

    /**
     * Denotes triangulation edges.
     */
    public static final int EDGES = 2;

    /**
     * Denotes triangulation faces.
     */
    public static final int FACES = 3;

    /**
     * Denotes triangulation components.
     */
    public static final int COMPONENTS = 4;

    /**
     * Denotes boundary components.
     */
    public static final int BOUNDARY_COMPONENTS = 5;

    /**
     * The triangulation whose skeleton we are displaying.
     */
    private NTriangulation triangulation;

    /**
     * Denotes whether to view details for vertices, edges,
     * faces or so on.  This will be one of the
     * style constants defined in this class.
     */
    private int style;
    
    /**
     * Create a new dialog.
     *
     * @param owner the frame from which the new dialog will be displayed.
     * @param triangulation the triangulation whose skeleton we are
     * viewing.
     * @param style specifies whether to view details for
     * vertices, edges, faces or so on.  This must be one of the
     * style constants defined in this class.
     */
    public SkeletonTableFrame(Frame owner, NTriangulation triangulation,
            int style) {
        super(owner);
        this.triangulation = triangulation;
        this.style = style;
        init();
        this.setSize(550, 300);
        Positioner.centerOnScreen(this);
    }
    
    /**
     * Create a new dialog.
     *
     * @param owner the dialog from which the new dialog will be displayed.
     * @param triangulation the triangulation whose skeleton we are
     * viewing.
     * @param style specifies whether to view details for
     * vertices, edges, faces or so on.  This must be one of the
     * style constants defined in this class.
     */
    public SkeletonTableFrame(Dialog owner, NTriangulation triangulation,
            int style) {
        super(owner);
        this.triangulation = triangulation;
        this.style = style;
        init();
        this.setSize(550, 300);
        Positioner.centerOnScreen(this);
    }
    
    /**
     * Initialise the interface components.
     */
    private void init() {
        // Set the title.
        switch(style) {
            case VERTICES: setTitle("Vertices"); break;
            case EDGES: setTitle("Edges"); break;
            case FACES: setTitle("Faces"); break;
            case COMPONENTS: setTitle("Components"); break;
            case BOUNDARY_COMPONENTS: setTitle("Boundary Components");
                break;
        }

        // Set up the button panel.
        JButton close = new JButton("Close");
        JPanel buttonPane = new JPanel();
        buttonPane.setLayout(new FlowLayout());
        buttonPane.add(close);
        
        // Set up the viewing table.
        JTable table = new JTable(new SkeletonTableModel());
        table.getTableHeader().setReorderingAllowed(false);
        table.getColumnModel().getColumn(0).setPreferredWidth(20);
        table.getColumnModel().getColumn(1).setPreferredWidth(60);
        table.getColumnModel().getColumn(2).setPreferredWidth(20);
        table.getColumnModel().getColumn(3).setPreferredWidth(300);
        SkeletonLongStringRenderer r = new SkeletonLongStringRenderer();
        table.getColumnModel().getColumn(3).setCellRenderer(r);
        table.getColumnModel().getColumn(3).setCellEditor(r);
                    
        // Insert components.
        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(new JScrollPane(table),
            BorderLayout.CENTER);
        getContentPane().add(buttonPane, BorderLayout.SOUTH);
        
        // Set action listeners.
        close.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                dispose();
            }
        });
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
    }
    
    /**
     * The table model used for the table in a <tt>SkeletonTableFrame</tt>.
     */
    public class SkeletonTableModel extends AbstractTableModel {
        /**
         * Returns the name of the given column.
         *
         * @param col the given column.
         * @return the name of the given column.
         */
        public String getColumnName(int col) {
            switch(col) {
                case 0: switch(style) {
                    case VERTICES: return "Vertex";
                    case EDGES: return "Edge";
                    case FACES: return "Face";
                    case COMPONENTS:
                    case BOUNDARY_COMPONENTS:
                        return "Cmpt";
                }
                case 1: return "Type";
                case 2: switch(style) {
                    case COMPONENTS:
                    case BOUNDARY_COMPONENTS:
                        return "Size";
                    default:
                        return "Degree";
                }
                case 3: switch(style) {
                    case COMPONENTS:
                        return "Tetrahedra";
                    case BOUNDARY_COMPONENTS:
                        return "Faces / Vertex";
                    default:
                        return "Tetrahedra (Tet vertices)";
                }
            }
            return null;
        }
        
        /**
         * Returns the number of rows in the table.
         *
         * @return the number of rows in the table.
         */
        public int getRowCount() {
            if (style == VERTICES)
                return (int)triangulation.getNumberOfVertices();
            else if (style == EDGES)
                return (int)triangulation.getNumberOfEdges();
            else if (style == FACES)
                return (int)triangulation.getNumberOfFaces();
            else if (style == COMPONENTS)
                return (int)triangulation.getNumberOfComponents();
            else if (style == BOUNDARY_COMPONENTS)
                return (int)triangulation.getNumberOfBoundaryComponents();
            else
                return 0;
        }
        
        /**
         * Returns the number of columns in the table.
         *
         * @param the number of columns in the table.
         */
        public int getColumnCount() {
            return 4;
        }
        
        /**
         * Returns the value in the given table cell.
         *
         * @param row the row of the given table cell.
         * @param col the column of the given table cell.
         * @return the value in the given table cell.
         */
        public Object getValueAt(int row, int col) {
            if (col == 0)
                return String.valueOf(row);
            if (style == VERTICES) {
                NVertex data = triangulation.getVertex(row);
                long tot = data.getNumberOfEmbeddings();
                if (col == 2)
                    return String.valueOf(tot);
                else if (col == 1) {
                    int link = data.getLink();
                    switch(link) {
                        case NVertex.SPHERE: return "";
                        case NVertex.DISC: return "Bdry";
                        case NVertex.TORUS: return "Torus cusp";
                        case NVertex.KLEIN_BOTTLE: return "Kln btl cusp";
                        case NVertex.NON_STANDARD_CUSP:
                            return "Non-std cusp";
                        case NVertex.NON_STANDARD_BDRY:
                            return "Non-std bdry";
                    }
                    return null;
                }
                // Build a long string.
                StringBuffer response = new StringBuffer();
                NVertexEmbedding emb;
                for (long i=0; i<tot; i++) {
                    emb = data.getEmbedding(i);
                    if (i > 0)
                        response.append(", ");
                    response.append(triangulation.getTetrahedronIndex(
                        emb.getTetrahedron()));
                    response.append('(');
                    response.append(emb.getVertex());
                    response.append(')');
                }
                return response;
            } else if (style == EDGES) {
                NEdge data = triangulation.getEdge(row);
                long tot = data.getNumberOfEmbeddings();
                if (col == 2)
                    return String.valueOf(tot);
                else if (col == 1) {
                    if (! data.isValid())
                        return "INVALID";
                    else if (data.isBoundary())
                        return "Bdry";
                    else
                        return "";
                }
                // Build a long string.
                StringBuffer response = new StringBuffer();
                NEdgeEmbedding emb;
                for (long i=0; i<tot; i++) {
                    emb = data.getEmbedding(i);
                    if (i > 0)
                        response.append(", ");
                    response.append(triangulation.getTetrahedronIndex(
                        emb.getTetrahedron()));
                    response.append('(');
                    response.append(NPerm.
                        edgeDescription(emb.getVertices()));
                    response.append(')');
                }
                return response;
            } else if (style == FACES) {
                NFace data = triangulation.getFace(row);
                int tot = data.getNumberOfEmbeddings();
                if (col == 2)
                    return String.valueOf(tot);
                else if (col == 1) {
                    if (data.isBoundary())
                        return "Bdry";
                    else
                        return "";
                }
                // Build a long string.
                StringBuffer response = new StringBuffer();
                NFaceEmbedding emb;
                for (int i=0; i<tot; i++) {
                    emb = data.getEmbedding(i);
                    if (i > 0)
                        response.append(", ");
                    response.append(triangulation.getTetrahedronIndex(
                        emb.getTetrahedron()));
                    response.append('(');
                    response.append(NPerm.
                        faceDescription(emb.getVertices()));
                    response.append(')');
                }
                return response;
            } else if (style == COMPONENTS) {
                NComponent data = triangulation.getComponent(row);
                long tot = data.getNumberOfTetrahedra();
                if (col == 2)
                    return String.valueOf(tot);
                else if (col == 1) {
                    StringBuffer response = new StringBuffer();
                    response.append(data.isIdeal() ? "Ideal, " :
                        "Real, ");
                    response.append(data.isOrientable() ? "Ortbl" :
                        "Non-ortbl");
                    return response;
                }
                // Build a long string.
                StringBuffer response = new StringBuffer();
                NTetrahedron tet;
                for (long i=0; i<tot; i++) {
                    tet = data.getTetrahedron(i);
                    if (i > 0)
                        response.append(", ");
                    response.append(triangulation.getTetrahedronIndex(tet));
                }
                return response;
            } else if (style == BOUNDARY_COMPONENTS) {
                NBoundaryComponent data =
                    triangulation.getBoundaryComponent(row);
                long tot = data.getNumberOfFaces();
                if (col == 2)
                    return String.valueOf(tot);
                else if (col == 1) {
                    if (data.isIdeal())
                        return "Ideal";
                    else
                        return "Real";
                }
                // Build a long string.
                StringBuffer response = new StringBuffer();
                if (data.isIdeal()) {
                    response.append("Vertex ");
                    response.append(triangulation.getVertexIndex(
                        data.getVertex(0)));
                } else {
                    response.append("Faces ");
                    for (long i=0; i<tot; i++) {
                        if (i > 0)
                            response.append(", ");
                        response.append(triangulation.getFaceIndex(
                            data.getFace(i)));
                    }
                }
                return response;
            }
            return null;
        }
        
        /**
         * Determines if the given table cell is editable.
         *
         * @param row the row of the given table cell.
         * @param col the column of the given table cell.
         * @return whether the given table cell is editable.
         */
        public boolean isCellEditable(int row, int col) {
            return (col == 3);
        }
    }

    /**
     * The class used to render long strings in this table.
     * This is simply a <tt>LongStringRenderer</tt> with
     * <tt>getDialogTitle()</tt> overridden to provide more
     * meaningful titles for content viewers.
     */
    private class SkeletonLongStringRenderer
            extends LongStringRenderer {
        /**
         * Creates a new renderer.
         */
        public SkeletonLongStringRenderer() {
            super();
        }

        /**
         * Provides a title for the content viewer
         * dialog.  This will be the ID number of the
         * corresponding skeletal component.
         */
        public String getDialogTitle(JTable table, Object value,
                int row, int column) {
            StringBuffer ans = null;
            switch(style) {
                case VERTICES:
                    ans = new StringBuffer("Vertex "); break;
                case EDGES: ans = new StringBuffer("Edge "); break;
                case FACES: ans = new StringBuffer("Face "); break;
                case COMPONENTS:
                    ans = new StringBuffer("Component "); break;
                case BOUNDARY_COMPONENTS:
                    ans = new StringBuffer("Boundary Component ");
                    break;
            }
            ans.append(String.valueOf(row));
            return ans.toString();
        }
    }
}

