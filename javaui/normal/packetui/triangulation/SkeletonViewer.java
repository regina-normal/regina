
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
import javax.swing.border.*;
import javax.swing.event.*;
import javax.swing.table.*;
import normal.engine.packet.NPacket;
import normal.engine.triangulation.*;
import normal.packetui.*;
import org.gjt.btools.gui.*;
import org.gjt.btools.gui.component.*;

/**
 * An interface through which a user can view the skeleton
 * of a triangulation.
 */
public class SkeletonViewer extends DefaultPacketViewer
        implements PacketInfoUI {
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
     * @serial
     */
    private NTriangulation triangulation;
        
    /**
     * Label displaying the number of vertices.
     * @serial
     */
    private JLabel nVertices;

    /**
     * Label displaying the number of edges.
     * @serial
     */
    private JLabel nEdges;

    /**
     * Label displaying the number of faces.
     * @serial
     */
    private JLabel nFaces;

    /**
     * Label displaying the number of tetrahedra.
     * @serial
     */
    private JLabel nTetrahedra;

    /**
     * Label displaying the number of components.
     * @serial
     */
    private JLabel nComponents;

    /**
     * Label displaying the number of boundary components.
     * @serial
     */
    private JLabel nBoundaryComponents;

    /**
     * Button for viewing vertex details.
     * @serial
     */
    private JButton viewVertices;
     
    /**
     * Button for viewing edge details.
     * @serial
     */
    private JButton viewEdges;
     
    /**
     * Button for viewing face details.
     * @serial
     */
    private JButton viewFaces;

    /**
     * Button for viewing tetrahedron details.
     * @serial
     */
    private JButton viewTetrahedra;

    /**
     * Button for viewing component details.
     * @serial
     */
    private JButton viewComponents;

    /**
     * Button for viewing boundary component details.
     * @serial
     */
    private JButton viewBoundaryComponents;

    /**
     * Label for vertices.
     * @serial
     */
    private JLabel labelVertices;

    /**
     * Label for edges.
     * @serial
     */
    private JLabel labelEdges;

    /**
     * Label for faces.
     * @serial
     */
    private JLabel labelFaces;

    /**
     * Label for tetrahedra.
     * @serial
     */
    private JLabel labelTetrahedra;

    /**
     * Label for components.
     * @serial
     */
    private JLabel labelComponents;

    /**
     * Label for boundary components.
     * @serial
     */
    private JLabel labelBoundaryComponents;

    /**
     * A list of all frames for viewing skeleton details
     * that are currently open.
     * @serial
     */
    private Vector skeletonFrames = new Vector();

    /**
     * Create a new interface to display the given triangulation skeleton.
     *
     * @param triangulation the triangulation whose skeleton we will display.
     */
    public SkeletonViewer(NTriangulation triangulation) {
        this.triangulation = triangulation;
        init();
    }

    public NPacket getPacket() {
        return triangulation;
    }

    /**
     * Create the interface elements.
     */
    private void init() {
        // Set the layout manager.
        setLayout(new GridBagLayout());
        
        // Initialise the components.
        nVertices = new JLabel();
        nEdges = new JLabel();
        nFaces = new JLabel();
        nTetrahedra = new JLabel();
        nComponents = new JLabel();
        nBoundaryComponents = new JLabel();
        viewVertices = new JButton("View");
        viewEdges = new JButton("View");
        viewFaces = new JButton("View");
        viewTetrahedra = new JButton("View");
        viewTetrahedra.setEnabled(false);
        viewComponents = new JButton("View");
        viewBoundaryComponents = new JButton("View");
        viewVertices.setBorder(BorderFactory.createEtchedBorder());
        viewEdges.setBorder(BorderFactory.createEtchedBorder());
        viewFaces.setBorder(BorderFactory.createEtchedBorder());
        viewTetrahedra.setBorder(BorderFactory.createEtchedBorder());
        viewComponents.setBorder(BorderFactory.createEtchedBorder());
        viewBoundaryComponents.setBorder(BorderFactory.createEtchedBorder());
        labelVertices = new JLabel("Vertices:");
        labelEdges = new JLabel("Edges:");
        labelFaces = new JLabel("Faces:");
        labelTetrahedra = new JLabel("Tetrahedra:");
        labelComponents = new JLabel("Components:");
        labelBoundaryComponents = new JLabel("Boundary Components:");
        GridBagConstraints cLabel = new GridBagConstraints();
        GridBagConstraints cTotal = new GridBagConstraints();
        GridBagConstraints cView = new GridBagConstraints();
        cLabel.anchor = cLabel.WEST;
        cLabel.fill = cLabel.NONE;
        cLabel.gridx = 0;
        cTotal.anchor = cTotal.CENTER;
        cTotal.fill = cTotal.NONE;
        cTotal.gridx = 1;
        cTotal.insets = new Insets(0, 5, 0, 5);
        cView.anchor = cView.CENTER;
        cView.fill = cView.NONE;
        cView.gridx = 2;
        cView.ipadx = 5;
        cView.ipady = 2;
        cView.insets = new Insets(0, 5, 0, 0);
        add(labelVertices, cLabel);
        add(nVertices, cTotal);
        add(viewVertices, cView);
        add(labelEdges, cLabel);
        add(nEdges, cTotal);
        add(viewEdges, cView);
        add(labelFaces, cLabel);
        add(nFaces, cTotal);
        add(viewFaces, cView);
        add(labelTetrahedra, cLabel);
        add(nTetrahedra, cTotal);
        add(viewTetrahedra, cView);
        add(labelComponents, cLabel);
        add(nComponents, cTotal);
        add(viewComponents, cView);
        add(labelBoundaryComponents, cLabel);
        add(nBoundaryComponents, cTotal);
        add(viewBoundaryComponents, cView);

        // Other miscellaneous tasks.
        setBorder(new EmptyBorder(5,5,5,5));
        
        // Set event listeners.
        viewVertices.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    new SkeletonTableFrame(VERTICES).show();
                }
            });
        viewEdges.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    new SkeletonTableFrame(EDGES).show();
                }
            });
        viewFaces.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    new SkeletonTableFrame(FACES).show();
                }
            });
        viewComponents.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    new SkeletonTableFrame(COMPONENTS).show();
                }
            });
        viewBoundaryComponents.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    new SkeletonTableFrame(BOUNDARY_COMPONENTS).show();
                }
            });
    }

    public void reflectPacket() {
        closeAllFrames();

        nVertices.setText(String.valueOf(triangulation.getNumberOfVertices()));
        nEdges.setText(String.valueOf(triangulation.getNumberOfEdges()));
        nFaces.setText(String.valueOf(triangulation.getNumberOfFaces()));
        nTetrahedra.setText(
            String.valueOf(triangulation.getNumberOfTetrahedra()));
        nComponents.setText(
            String.valueOf(triangulation.getNumberOfComponents()));
        nBoundaryComponents.setText(
            String.valueOf(triangulation.getNumberOfBoundaryComponents()));

        viewVertices.setEnabled(true);
        viewEdges.setEnabled(true);
        viewFaces.setEnabled(true);
        viewComponents.setEnabled(true);
        viewBoundaryComponents.setEnabled(true);
        labelVertices.setEnabled(true);
        labelEdges.setEnabled(true);
        labelFaces.setEnabled(true);
        labelTetrahedra.setEnabled(true);
        labelComponents.setEnabled(true);
        labelBoundaryComponents.setEnabled(true);
    }

    public void editingElsewhere() {
        closeAllFrames();

        nVertices.setText("");
        nEdges.setText("");
        nFaces.setText("");
        nTetrahedra.setText("");
        nComponents.setText("");
        nBoundaryComponents.setText("");

        viewVertices.setEnabled(false);
        viewEdges.setEnabled(false);
        viewFaces.setEnabled(false);
        viewComponents.setEnabled(false);
        viewBoundaryComponents.setEnabled(false);
        labelVertices.setEnabled(false);
        labelEdges.setEnabled(false);
        labelFaces.setEnabled(false);
        labelTetrahedra.setEnabled(false);
        labelComponents.setEnabled(false);
        labelBoundaryComponents.setEnabled(false);
    }

    /**
     * Close all frames opened for viewing specific skeleton details.
     */
    public void closeAllFrames() {
        while(skeletonFrames.size() > 0) {
            Object e = skeletonFrames.elementAt(0);
            skeletonFrames.removeElement(e);
            ((SkeletonTableFrame)e).dispose();
        }
    }

    /**
     * Frame class used to view details of the triangulation skeleton.
     */
    public class SkeletonTableFrame extends JFrame {
        /**
         * Denotes whether to view details for vertices, edges,
         * faces or so on.  This will be one of the
         * constants <tt>VERTICES</tt>, <tt>EDGES</tt>,
         * <tt>FACES</tt>, <tt>COMPONENTS</tt> or
         * <tt>BOUNDARY_COMPONENTS</tt>.
         * @serial
         */
        private int style;
        
        /**
         * Create a new frame.
         *
         * @param style specifies whether to view details for
         * vertices, edges, faces or so on.  This must be one of the
         * constants <tt>VERTICES</tt>, <tt>EDGES</tt>,
         * <tt>FACES</tt>, <tt>COMPONENTS</tt> or
         * <tt>BOUNDARY_COMPONENTS</tt>.
         */
        public SkeletonTableFrame(int style) {
            super();
            this.style = style;
            switch(style) {
                case VERTICES: setTitle("Vertices"); break;
                case EDGES: setTitle("Edges"); break;
                case FACES: setTitle("Faces"); break;
                case COMPONENTS: setTitle("Components"); break;
                case BOUNDARY_COMPONENTS: setTitle("Boundary Components");
                    break;
            }
            init();
            this.setSize(550, 300);
            Positioner.centerOnScreen(this);
            skeletonFrames.addElement(this);
        }
        
        /**
         * Initialise the interface components.
         */
        private void init() {
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
            addWindowListener(new WindowAdapter() {
                public void windowClosed(WindowEvent e) {
                    skeletonFrames.removeElement(SkeletonTableFrame.this);
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
}
