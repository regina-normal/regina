
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

package normal.mainui;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;
import org.gjt.btools.ext.*;
import org.gjt.btools.gui.component.*;
import org.gjt.btools.gui.dialog.*;
import org.gjt.btools.image.*;
import normal.*;
import normal.console.JPythonConsoleFrame;
import normal.engine.*;
import normal.engine.file.*;
import normal.engine.packet.*;
import normal.imports.*;
import normal.options.NormalOptionSet;
import normal.packetui.*;
import normal.packetfilter.*;

/**
 * Represents a pane in which the user can navigate through and interact
 * with a system of packets.  The pane will work with one and only one
 * packet tree.
 * <tt>PacketPane</tt> interfaces can be opened and closed through the
 * <tt>insertPacketPane()</tt> and <tt>removePacketPane()</tt> functions.
 * The member <tt>AllPacketPanes</tt> contains a list of all packet
 * interfaces currently open.
 * <p>
 * Note the various <tt>fire...()</tt> routines that <b>must</b> be called
 * whenever certain modifications occur with a packet or the packet
 * tree.
 *
 * @see PacketPane
 */
public class TopologyPane extends FilePane {

    // ---------------
    //    CONSTANTS
    // ---------------

    /**
     * Used with <tt>dockPacketPane()</tt>.
     * @see #dockPacketPane
     */
    public static final int PANE_FLOAT = 1;

    /**
     * Used with <tt>dockPacketPane()</tt>.
     * @see #dockPacketPane
     */
    public static final int PANE_REMOVE = 2;

    /**
     * Used with <tt>dockPacketPane()</tt>.
     * @see #dockPacketPane
     */
    public static final int PANE_ASK_FLOAT = 3;

    /**
     * Used with <tt>dockPacketPane()</tt>.
     * @see #dockPacketPane
     */
    public static final int PANE_ASK_DO_NOTHING = 4;
    
    /**
     * The number of packets beyond which groups of new packets will
     * never be displayed in individual windows.
     */
    public static final int SHOW_PACKETS_THRESHOLD = 3;

    /**
     * The number of places a packet moves up or down the tree when the
     * double up or double down arrow is pressed.
     */
    public static final int treePageSize = 10;

    /**
     * The cell renderer used to draw the visual packet tree.
     */
    public static final PacketTreeCellRenderer cellRenderer =
        new PacketTreeCellRenderer();

    /**
     * The default file extension used for topology data files.
     */
    public static final String defaultFileExtension = ".rga";

    /**
     * Filter for data files.
     */
    public static final ExtensionFilenameFilter filenameFilter =
        new ExtensionFilenameFilter(defaultFileExtension,
        "Topology Data Files (*" + defaultFileExtension + ')');

    /**
     * The base of the file type description for topology data files.
     */
    public static final String topologyFileTypeBase = "Topology data";
    
    // ------------------
    //    DATA MEMBERS
    // ------------------

    /**
     * The packet at the root of the packet tree.
     */
    private NPacket rootPacket;
    
    /**
     * The tree node corresponding to the root of the packet tree.
     */
    private PacketTreeNode rootNode;
    
    /**
     * The structure of the entire visual tree that mirrors the packet tree.
     */
    private DefaultTreeModel treeModel;
    
    /**
     * The visual tree that mirrors the packet tree.
     */
    private JTree packetTree;
    
    /**
     * The split pane separating the visual tree
     * from the working area.
     */
    private JSplitPane splitPane;

    /**
     * The current working area.
     */
    private JPanel workingArea = new JPanel();
    
    /**
     * Packet pane in the current working area.
     */
    private PacketPane workingPane = null;

    /**
     * List of all packet panes belonging to this system.
     */
    private Vector allPacketPanes = new Vector();

    /**
     * The panel containing the icon and packet tree.
     */
    private JPanel navigationPanel = new JPanel();

    /**
     * The program icon displayed in the navigation panel.
     */
    private JComponent icon;

    /**
     * View button for the main topology pane interface.
     */
    private JButton viewBtn;

    /**
     * Rename button for the main topology pane interface.
     */
    private JButton renameBtn;

    /**
     * Delete button for the main topology pane interface.
     */
    private JButton deleteBtn;

    /**
     * Refresh button for the main topology pane interface.
     */
    private JButton refreshBtn;

    /**
     * Tree reorganisation up button.
     */
    private JButton treeUp;
    /**
     * Tree reorganisation page up button.
     */
    private JButton treePageUp;
    /**
     * Tree reorganisation up to top button.
     */
    private JButton treeUpTop;
    /**
     * Tree reorganisation down button.
     */
    private JButton treeDown;
    /**
     * Tree reorganisation page down button.
     */
    private JButton treePageDown;
    /**
     * Tree reorganisation down to bottom button.
     */
    private JButton treeDownBottom;
    /**
     * Tree reorganisation left button.
     */
    private JButton treeLeft;
    /**
     * Tree reorganisation right button.
     */
    private JButton treeRight;

    // --------------------
    //    INITIALISATION
    // --------------------

    /**
     * Create a new pane with a new default set of working data.
     *
     * @param shell the shell representing the entire program.
     * @return the new pane.
     */
    public static TopologyPane newPane(Shell shell) {
        NPacket root = shell.getEngine().newNContainer();
        root.setPacketLabel("Container");
        
        return new TopologyPane(shell, root, topologyFileTypeBase +
            "\nNew file");
    }

    /**
     * Create a new pane with the working data read from the given file.
     * Any I/O or related errors will be reported to the user.
     *
     * @param shell the shell representing the entire program.
     * @param file the file from which to read.
     * @return the new pane, or <tt>null</tt> if an error occurred.
     */
    public static TopologyPane newPane(Shell shell, File file) {
        // Don't use readFileMagic() since we want to get the file's
        // engine version while we're at it.
        String pathname = file.getAbsolutePath();

        // Determine the file type.
        NFileInfo info = shell.getEngine().identifyFileInfo(pathname);

        if (info == null) {
            shell.error("The format of file [" + file.getName() +
                "] could not be determined.  " +
                "The file might contain invalid data.");
            return null;
        }

        // Read the packet tree.
        NPacket root;
        if (info.getType() == NFileInfo.TYPE_XML) {
            root = shell.getEngine().readXMLFile(pathname);
        } else if (info.getType() == NFileInfo.TYPE_BINARY) {
            root = shell.getEngine().readFromFile(pathname);
        } else
            root = null;

        if (root == null) {
            shell.error("The data stored in file [" + file.getName() +
                "] could not be interpreted.  " +
                "The file might contain invalid data.");
            info.destroy();
            return null;
        }

        // Collect the engine version and file format and clean up.
        String desc = info.getTypeDescription() + ", engine " +
            info.getEngine();
        info.destroy();
        return new TopologyPane(shell, root, topologyFileTypeBase + "\n" + desc);
    }

    /**
     * Create a new pane containing the given working data.
     *
     * @param shell the shell representing the entire program.
     * @param rootPacket the matriarch of the packet tree that will form
     * the working data for this pane.
     * @param fileType a human-readable string describing the type of
     * file presented in this pane.
     */
    private TopologyPane(Shell shell, NPacket rootPacket, String fileType) {
        super(shell);
        this.rootPacket = rootPacket;
        setFileType(fileType);

        init();
    }

    public boolean canSave() {
        if (unconfirmedEditPanes())
            if (! getShell().confirm(
                    "Some edit panes currently in use contain " +
                    "changes that have not yet been applied.  Do you wish " +
                    "to continue?"))
                return false;
        return true;
    }

    public boolean saveFile(File file) {
        Engine engine = getEngine();
        if (! engine.writeXMLFile(file.getAbsolutePath(), rootPacket)) {
            getShell().error(
                "The requested file could not be opened for writing.");
            return false;
        }
        setFileType(topologyFileTypeBase +
            "\nXML Regina data file, engine " + engine.getVersionString());
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
        // Create the visual tree.
        createTree();

        // Set the layout manager.
        this.setLayout(new BorderLayout());

        // Build the button panels.
        viewBtn = new JButton("View");
        renameBtn = new JButton("Rename");
        deleteBtn = new JButton("Delete");
        refreshBtn = new JButton("Refresh");
        treeUp = new JButton(Standard16.up.image());
        treeUp.setToolTipText("Move up the child list");
        treeDown = new JButton(Standard16.down.image());
        treeDown.setToolTipText("Move down the child list");
        treePageUp = new JButton(Standard16.upUp.image());
        treePageUp.setToolTipText("Move a page up the child list");
        treePageDown = new JButton(Standard16.downDown.image());
        treePageDown.setToolTipText("Move a page down the child list");
        treeUpTop = new JButton(Standard16.upTop.image());
        treeUpTop.setToolTipText("Move to the top of the child list");
        treeDownBottom = new JButton(Standard16.downBottom.image());
        treeDownBottom.setToolTipText("Move to the bottom of the child list");
        treeLeft = new JButton(Standard16.left.image());
        treeLeft.setToolTipText("Move up one level");
        treeRight = new JButton(Standard16.right.image());
        treeRight.setToolTipText("Move inside the following packet");
        JPanel packetActionPanel = new JPanel();
        packetActionPanel.setLayout(new GridLayout(2,2));
        packetActionPanel.add(viewBtn);
        packetActionPanel.add(renameBtn);
        packetActionPanel.add(deleteBtn);
        packetActionPanel.add(refreshBtn);
        JPanel treeActionPanel = new JPanel();
        treeActionPanel.setLayout(new GridLayout(2, 5));
        treeActionPanel.add(new JPanel());
        treeActionPanel.add(treeUp);
        treeActionPanel.add(treePageUp);
        treeActionPanel.add(treeUpTop);
        treeActionPanel.add(new JPanel());
        treeActionPanel.add(treeLeft);
        treeActionPanel.add(treeDown);
        treeActionPanel.add(treePageDown);
        treeActionPanel.add(treeDownBottom);
        treeActionPanel.add(treeRight);
        JTabbedPane buttonTabs = new JTabbedPane(SwingConstants.RIGHT);
        buttonTabs.addTab(null, Standard16.action.image(), packetActionPanel,
            "Packet actions");
        buttonTabs.addTab(null, Standard16.tree.image(), treeActionPanel,
            "Tree reorganisation");

        // Build the packet panel.
        JPanel packetPanel = new JPanel();
        packetTree.setShowsRootHandles(true);
        packetPanel.setLayout(new BorderLayout());
        packetPanel.add(new JScrollPane(packetTree), BorderLayout.CENTER);
        packetPanel.add(buttonTabs, BorderLayout.SOUTH);

        // Build the navigation panel.
        JLabel iconLabel = new JLabel(Images.mainLargeIcon.image());
        iconLabel.setHorizontalAlignment(JLabel.CENTER);
        iconLabel.setBorder(BorderFactory.createLoweredBevelBorder());
        icon = new PaddedPane(iconLabel, 3, false);
        if (! getOptions().getDisplayIcon())
            icon.setVisible(false);
        navigationPanel.setLayout(new BorderLayout());
        navigationPanel.add(packetPanel, BorderLayout.CENTER);
        navigationPanel.add(icon, BorderLayout.SOUTH);
        
        // Build and insert the split panel.
        workingArea.setLayout(new BorderLayout());
        splitPane = new JSplitPane();
        splitPane.setOrientation(JSplitPane.HORIZONTAL_SPLIT);
        splitPane.setLeftComponent(navigationPanel);
        splitPane.setRightComponent(workingArea);
        add(splitPane, BorderLayout.CENTER);                

        // Add event listeners.        
        packetTree.addTreeSelectionListener(new TreeSelectionListener() {
            public void valueChanged(TreeSelectionEvent e) {
                getNormalFrame().reflectCurrentPacket();
                refreshButtons();
            }
        });
        viewBtn.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                packetView();
            }
        });
        renameBtn.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                packetRename();
            }
        });
        deleteBtn.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                packetDelete();
            }
        });
        refreshBtn.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                packetRefresh();
            }
        });
        treeLeft.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                moveTreeLeft();
            }
        });
        treeRight.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                moveTreeRight();
            }
        });
        treeDown.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                moveTreeDown(1);
            }
        });
        treePageDown.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                moveTreeDown(treePageSize);
            }
        });
        treeDownBottom.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                moveTreeDown(-1);
            }
        });
        treeUp.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                moveTreeUp(1);
            }
        });
        treePageUp.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                moveTreeUp(treePageSize);
            }
        });
        treeUpTop.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                moveTreeUp(-1);
            }
        });

        // Tidy up.
        refreshButtons();

        // Set the divider location as specified in the options file.
        int dividerLocation =
            getOptions().getIntOption("DividerLocation", -1);
        if (dividerLocation > 0)
            splitPane.setDividerLocation(dividerLocation);
    }

    /**
     * Build the visual packet tree.
     */
    private void createTree() {
        createTreeModel();
        packetTree = new JTree(treeModel) {
            protected void processMouseEvent(MouseEvent e) {
                if (e.getID() == e.MOUSE_CLICKED && e.getClickCount() >= 2)
                    packetView();
                else
                    super.processMouseEvent(e);
            }
        };
        packetTree.getSelectionModel().setSelectionMode(
            TreeSelectionModel.SINGLE_TREE_SELECTION);
        packetTree.setCellRenderer(cellRenderer);
        packetTree.setRowHeight(-1);
    }

    /**
     * Build the internal structure of the visual tree.
     */
    private void createTreeModel() {
        rootNode = new PacketTreeNode(rootPacket);
        treeModel = new DefaultTreeModel(rootNode);

        int index = 0;
        NPacket child = rootPacket.getFirstTreeChild();
        while (child != null) {
            createSubTree(child, rootNode, index);
            child = child.getNextTreeSibling();
            index++;
        }
    }

    /**
     * Build the internal structure of a subtree of the visual tree and
     * insert this into the structure of the overall tree.
     *
     * @param newPacket the packet to form the head of this subtree.
     * @param parentNode the packet beneath which the new subtree will
     * be inserted.
     * @param index the new subtree will be inserted as the <i>index</i>th
     * child of <tt>parentNode</tt>.
     */
    private void createSubTree(NPacket newPacket, PacketTreeNode parentNode,
            int index) {
        PacketTreeNode newNode = new PacketTreeNode(newPacket);
        treeModel.insertNodeInto(newNode, parentNode, index);

        index = 0;
        NPacket child = newPacket.getFirstTreeChild();
        while (child != null) {
            createSubTree(child, newNode, index);
            child = child.getNextTreeSibling();
            index++;
        }
    }

    // --------------------
    //    QUERY ROUTINES
    // --------------------

    /**
     * Returns the visual packet tree.
     *
     * @return the visual packet tree.
     */
    public JTree getPacketTree() {
        return packetTree;
    }
    
    /**
     * Establish which packet node the user currently has selected in the
     * visual tree.
     *
     * @return the currently selected node, or <tt>null</tt> if no node is
     * selected.
     */
    public PacketTreeNode getSelectedNode() {
        TreePath selection =
            packetTree.getSelectionModel().getSelectionPath();
        if (selection == null)
            return null;
        return (PacketTreeNode)selection.getLastPathComponent();
    }

    /**
     * Establish which packet the user currently has selected in the visual
     * tree.
     *
     * @return the currently selected packet, or <tt>null</tt> if no
     * packet is selected.
     */
    public NPacket getSelectedPacket() {
        PacketTreeNode node = getSelectedNode();
        if (node == null)
            return null;
        else
            return node.getPacket();
    }

    /**
     * Return the packet at the root of the working packet tree.
     *
     * @return the root packet.
     */
    public NPacket getRootPacket() {
        return rootPacket;
    }

    /**
     * Return a list of all packet panes spawned from this topology pane.
     *
     * @return a list of all packet panes.  Each element of the list
     * will be of type <tt>PacketPane</tt>.
     * @see normal.mainui.PacketPane
     */
    public Enumeration getAllPacketPanes() {
        return allPacketPanes.elements();
    }

    // ---------------------
    //    UPDATE ROUTINES
    // ---------------------

    /**
     * Refreshes the enabled state of the main buttons in the mainfold pane.
     */
    public void refreshButtons() {
        boolean state = (getSelectedNode() != null);
        viewBtn.setEnabled(state);
        renameBtn.setEnabled(state);
        deleteBtn.setEnabled(state);
        refreshBtn.setEnabled(state);

        if (! state) {
            treeUp.setEnabled(false);
            treePageUp.setEnabled(false);
            treeUpTop.setEnabled(false);
            treeDown.setEnabled(false);
            treePageDown.setEnabled(false);
            treeDownBottom.setEnabled(false);
            treeLeft.setEnabled(false);
            treeRight.setEnabled(false);
        } else {
            NPacket packet = ((PacketTreeNode)getSelectedNode()).getPacket();
            boolean downState = (packet.getNextTreeSibling() != null);
            treeDown.setEnabled(downState);
            treePageDown.setEnabled(downState);
            treeDownBottom.setEnabled(downState);
            boolean upState = (packet.getPrevTreeSibling() != null);
            treeUp.setEnabled(upState);
            treePageUp.setEnabled(upState);
            treeUpTop.setEnabled(upState);

            if (packet.dependsOnParent()) {
                treeLeft.setEnabled(false);
                treeRight.setEnabled(false);
            } else {
                treeLeft.setEnabled(packet.getTreeParent() != null &&
                    packet.getTreeParent().getTreeParent() != null);
                treeRight.setEnabled(downState);
            }
        }
    }

    // ------------------------
    //    FILEPANE OVERRIDES
    // ------------------------

    public void updateLookAndFeel() {
        super.updateLookAndFeel();

        Frame frame;
        Enumeration e = allPacketPanes.elements();
        while (e.hasMoreElements()) {
            frame = ((PacketPane)e.nextElement()).getSurroundingFrame();
            if (frame != null)
                SwingUtilities.updateComponentTreeUI(frame);
        }
    }

    public void setDisplayIcon(boolean displayIcon) {
        icon.setVisible(displayIcon);
    }

    public boolean canClose() {
        if (! super.canClose())
            return false;

        // Check for unconfirmed edit panes.
        if (unconfirmedEditPanes())
            if (! getShell().confirm("Some edit panes currently in use " +
                    "contain changes that have not yet been applied.  Are " +
                    "you sure you wish to abandon these changes?"))
                return false;

        // Close all Jython consoles spawned from this system.
        Vector myConsoles = new Vector();
        JPythonConsoleFrame console;
        Enumeration e = getShell().getConsoles();
        while (e.hasMoreElements()) {
            console = (JPythonConsoleFrame)e.nextElement();
            if (rootPacket.sameObject(console.getRootPacket()))
                myConsoles.addElement(console);
        }

        e = myConsoles.elements();
        while (e.hasMoreElements()) {
            console = (JPythonConsoleFrame)e.nextElement();
            if (console.getConsole().isProcessing()) {
                getShell().error("One of the Jython consoles linked " +
                    "to this file is still processing input.");
                return false;
            }
            console.closeConsole();
        }

        // We can close!
        return true;
    }

    void cleanUp() {
        // Save visual settings.
        getOptions().setIntOption("DividerLocation",
            splitPane.getDividerLocation());
            
        // Close each open packet pane.
        while (! allPacketPanes.isEmpty())
            removePacketPane((PacketPane)allPacketPanes.lastElement(), true);
        
        // Delete the underlying native packet structure.
        rootPacket.destroy();
    }

    // --------------------------
    //    PACKET PANE HANDLING
    // --------------------------

    /**
     * Insert the given packet pane into the interface.
     * The new pane can either go into the working area or be
     * opened in a new window.
     *
     * @param pane the pane to be inserted.
     * @param preferDocked whether or not we would like the pane
     * to be docked into the working area.  If this is <tt>false</tt>,
     * a new window will be opened.  If this is <tt>true</tt>, the
     * procedure will try to place it in the working area, but if the
     * existing pane refuses to close then a new window will be opened
     * instead.
     * @see #removePacketPane
     */
    public void insertPacketPane(PacketPane pane, boolean preferDocked) {
        // Insert the new pane.
        // If we want to dock it and can't, then float it instead.
        if (preferDocked)
            if (! internalDockPacketPane(pane, PANE_ASK_DO_NOTHING))
                preferDocked = false;
        if (! preferDocked)
            internalFloatPacketPane(pane);
        
        allPacketPanes.addElement(pane);

        // Make sure the dock button is showing the correct picture.
        pane.refreshDockButton();
    }

    /**
     * Remove the given packet pane from the user interface.
     *
     * @param pane the packet pane to remove.
     * @param force when set to <tt>false</tt>, the packet pane will be asked
     * whether it wishes to close; if it does not, the pane will not be
     * removed.  When set to <tt>true</tt>, the pane will be removed
     * regardless.
     * @return <tt>true</tt> if and only if the pane was removed.  Note that
     * this is only relevant if <tt>force</tt> was set to <tt>false</tt>.
     * @see #insertPacketPane
     */    
    public boolean removePacketPane(PacketPane pane, boolean force) {
        if (! force)
            if (! pane.canClose())
                return false;
                
        if (isDocked(pane))
            internalUndockPacketPane(pane);
        else if (isFloating(pane))
            internalUnfloatPacketPane(pane);
        
        allPacketPanes.removeElement(pane);
        pane.cleanUp();
        return true;
    }

    /**
     * Dock the given packet pane into this topology pane.
     * <p>
     * <b>Preconditions:</b> The given pane has already been inserted
     * into the interface through <tt>insertPacketPane</tt>.
     *
     * @param pane the packet pane to dock.
     * @param force specifies what to do if another packet pane is in
     * the way of this pane being docked.
     * <tt>PANE_FLOAT</tt> specifies that the other pane should be
     * forced to float.
     * <tt>PANE_REMOVE</tt> specifies that the other pane should be
     * removed from the interface altogether via <tt>removePacketPane</tt>.
     * <tt>PANE_ASK_FLOAT</tt> specifies that the other pane should be
     * removed unless it does not wish this, in which case it should
     * be forced to float instead.
     * <tt>PANE_ASK_DO_NOTHING</tt> specifies that the other pane should
     * be removed unless it does not wish this, in which case the current
     * pane should not be docked.
     * @return <tt>true</tt> if and only if the pane was docked.
     * Only relevant if <tt>force</tt> is <tt>PANE_ASK_DO_NOTHING</tt>.
     */
    public boolean dockPacketPane(PacketPane pane, int force) {
        if (isDocked(pane))
            return true;
        if (isFloating(pane))
            internalUnfloatPacketPane(pane);
        if (internalDockPacketPane(pane, force))
            return true;
        else {
            internalFloatPacketPane(pane);
            return false;
        }
    }
    
    /**
     * Float the given packet pane in its own window.
     * <p>
     * <b>Preconditions:</b> The given pane has already been inserted
     * into the interface through <tt>insertPacketPane</tt>.
     *
     * @param pane the packet pane to float.
     * @see #insertPacketPane
     */
    public void floatPacketPane(PacketPane pane) {
        if (isFloating(pane))
            return;
        if (isDocked(pane))
            internalUndockPacketPane(pane);
        internalFloatPacketPane(pane);
    }
    
    /**
     * Dock the given packet pane into this topology pane.
     * <p>
     * <b>Preconditions:</b> The given pane has already been inserted
     * into the interface through <tt>insertPacketPane</tt>.
     * Also, it is currently in limbo; neither floating nor docked.
     *
     * @param pane the packet pane with which to work.
     * @param force specifies what to do if another packet pane is in
     * the way of this pane being docked.
     * <tt>PANE_FLOAT</tt> specifies that the other pane should be
     * forced to float.
     * <tt>PANE_REMOVE</tt> specifies that the other pane should be
     * removed from the interface altogether via <tt>removePacketPane</tt>.
     * <tt>PANE_ASK_FLOAT</tt> specifies that the other pane should be
     * removed unless it does not wish this, in which case it should
     * be forced to float instead.
     * <tt>PANE_ASK_DO_NOTHING</tt> specifies that the other pane should
     * be removed unless it does not wish this, in which case the current
     * pane should not be docked.
     * @return <tt>true</tt> if and only if the pane was docked.
     * Only relevant if <tt>force</tt> is <tt>PANE_ASK_DO_NOTHING</tt>.
     */
    private boolean internalDockPacketPane(PacketPane pane, int force) {
        // Remove anything that might already be there.
        if (workingPane != null) {
            if (force == PANE_REMOVE)
                removePacketPane(workingPane, true);
            else if (force == PANE_ASK_DO_NOTHING) {
                if (! removePacketPane(workingPane, false))
                    return false;
            }
            else if (force == PANE_ASK_FLOAT) {
                if (! removePacketPane(workingPane, false))
                    force = PANE_FLOAT;
            }
            if (force == PANE_FLOAT) {
                PacketPane other = workingPane;
                internalUndockPacketPane(other);
                internalFloatPacketPane(other);
            }
        }

        // Dock this packet pane.        
        workingArea.add(pane, BorderLayout.CENTER);
        workingArea.validate();
        workingPane = pane;
        setWorkingTextComponent(pane.getPacketUI().getPrimaryTextComponent());
        return true;
    }
    
    /**
     * Undock the given packet pane from this topology pane.
     * <p>
     * <b>Preconditions:</b> The given pane has already been inserted
     * into the interface through <tt>insertPacketPane</tt>.
     * Also, it is currently docked in this topology pane.
     *
     * @param pane the packet pane with which to work.
     */
    private void internalUndockPacketPane(PacketPane pane) {
        setWorkingTextComponent(null);
        workingArea.remove(pane);
        workingArea.repaint();
        workingPane = null;
    }

    /**
     * Place the given packet pane in its own window.
     * <p>
     * <b>Preconditions:</b> The given pane has already been inserted
     * into the interface through <tt>insertPacketPane</tt>.
     * Also, it is currently in limbo; neither floating nor docked.
     *
     * @param pane the packet pane with which to work.
     */
    private void internalFloatPacketPane(PacketPane pane) {
        // Create a new frame in which to hold the pane.
        PacketFrame frame = new PacketFrame(pane);
        frame.show();
        pane.setSurroundingFrame(frame);
    }
    
    /**
     * Remove the given packet pane from its own window.
     * <p>
     * <b>Preconditions:</b> The given pane has already been inserted
     * into the interface through <tt>insertPacketPane</tt>.
     * Also, it is currently floating in its own window.
     *
     * @param pane the packet pane with which to work.
     */
    private void internalUnfloatPacketPane(PacketPane pane) {
        pane.getSurroundingFrame().getContentPane().remove(pane);
        pane.getSurroundingFrame().dispose();
        pane.setSurroundingFrame(null);
    }

    /**
     * Is the given packet pane docked into this topology pane?
     *
     * @param pane the packet pane to examine.
     * @return <tt>true</tt> if and only if the given pane is currently
     * docked.
     */
    public boolean isDocked(PacketPane pane) {
        return (pane == workingPane);
    }

    /**
     * Is the given packet pane floating about in its own window?
     *
     * @param pane the packet pane to examine.
     * @return <tt>true</tt> if and only if the given pane is in its
     * own frame.
     */        
    public boolean isFloating(PacketPane pane) {
        return (pane.getSurroundingFrame() != null);
    }

    /**
     * Ensure the given packet is in a viewing pane somewhere.
     * If the given packet is
     * not being viewed, a new viewing pane will be opened; otherwise
     * nothing will be done.
     *
     * @param packet the packet to ensure is viewed.
     */
    public void ensureViewingPacket(NPacket packet) {
        Enumeration e = allPacketPanes.elements();
        while (e.hasMoreElements())
            if (((PacketPane)e.nextElement()).getPacket().sameObject(packet))
                return;

        insertPacketPane(new PacketPane(this,
            PacketUIManager.newPacketUI(packet, getShell(), this,
            packet.isPacketEditable())), true);
    }

    /**
     * Does this system have any edit panes currently open
     * containing changes that have not been applied to the engine?
     *
     * @return <tt>true</tt> if and only if there are unconfirmed edit
     * panes.
     */
    public boolean unconfirmedEditPanes() {
        PacketPane pane;
        Enumeration e = allPacketPanes.elements();
        while (e.hasMoreElements()) {
            pane = (PacketPane)e.nextElement();
            if (pane.getPacketUI().hasChanges())
                return true;
        }
        return false;
    }

    // ------------------------
    //    PACKET TREE EVENTS
    // ------------------------

    /**
     * This should be called whenever a packet's contents change,
     * so that all interfaces can be updated accordingly.
     *
     * @param packet the packet whose contents have changed.
     * @param ui the interface that caused the change; this may be
     * <tt>null</tt>.
     */
    public void firePacketWasChanged(NPacket packet, PacketUI ui) {
        // Update the interfaces.
        PacketPane pane;
        Enumeration e = allPacketPanes.elements();
        while (e.hasMoreElements()) {
            pane = (PacketPane)e.nextElement();
            pane.getPacketUI().packetWasChanged(packet, ui,
                pane.getParentFrame());
        }
    }

    /**
     * This should be called whenever a packet has been renamed,
     * so that all interfaces and the visual packet tree
     * can be updated accordingly.
     * <p>
     * Any pane containing the renamed packet will have its title(s)
     * updated.
     * <p>
     * Note that <tt>fireSubtreeHasChanged()</tt> can be called instead
     * of this routine if specific details are not available.
     *
     * @param packet the packet that has been renamed.
     * @param ui the interface that caused the renaming; this may be
     * <tt>null</tt>.
     */
    public void firePacketWasRenamed(NPacket packet, PacketUI ui) {
        // Update the interfaces.
        PacketPane pane;
        Enumeration e = allPacketPanes.elements();
        while (e.hasMoreElements()) {
            pane = (PacketPane)e.nextElement();
            pane.getPacketUI().packetWasRenamed(packet, ui,
                pane.getParentFrame());
            if (packet.sameObject(pane.getPacket())) {
                pane.refreshLabel();
                Frame f = pane.getSurroundingFrame();
                if (f != null)
                    f.setTitle(packet.getFullName());
            }
        }

        // Update the visual tree.
        PacketTreeNode node = findTreeNode(packet);
        node.setUserObject(PacketTreeNode.getNodeLabel(packet));
        treeModel.nodeChanged(node);
    }

    /**
     * This should be called whenever the tags associated with a packet
     * have been changed, so that all interfaces and the visual packet
     * tree can be updated accordingly.
     *
     * Note that this change, unlike others covered by routines
     * similar to this, is <i>not</i> propagated to individual packet
     * interfaces.
     *
     * @param packet the packet whose tags have changed.
     */
    public void firePacketTagsChanged(NPacket packet) {
        // Update the interfaces.
        PacketPane pane;
        Enumeration e = allPacketPanes.elements();
        while (e.hasMoreElements()) {
            pane = (PacketPane)e.nextElement();
            if (packet.sameObject(pane.getPacket()))
                pane.refreshLabel();
        }

        // Update the visual tree.
        PacketTreeNode node = findTreeNode(packet);
        node.setUserObject(PacketTreeNode.getNodeLabel(packet));
        treeModel.nodeChanged(node);
    }

    /**
     * This should be called whenever a packet subtree is about to be deleted,
     * so that all interfaces and the visual packet tree can be updated
     * accordingly.  This routine
     * should only be called with the root packet of that subtree and
     * should not be called again with its descendants.
     * Note that the matriarch of the packet tree should never be
     * deleted.
     * <p>
     * Any pane containing a packet that belongs to the marked
     * subtree will be forcibly closed by this routine.
     * <p>
     * This routine should be called <i>before</i> the subtree is
     * deleted; note that <tt>fireSubtreeWasDeleted()</tt> should also
     * be called <i>after</i> the subtree has been deleted.
     *
     * @param subtree the packet subtree about to be deleted.
     * @param ui the interface that caused the deletion; this may be
     * <tt>null</tt>.
     *
     * @see #fireSubtreeWasDeleted
     */
    public void fireSubtreeToBeDeleted(NPacket subtree, PacketUI ui) {
        // Update the interfaces.
        PacketPane pane;
        Vector dieplease = new Vector();
        Enumeration e = allPacketPanes.elements();
        while (e.hasMoreElements()) {
            pane = (PacketPane)e.nextElement();
            if (subtree.isGrandparentOf(pane.getPacket()))
                dieplease.add(pane);
            else
                pane.getPacketUI().subtreeToBeDeleted(subtree, ui,
                    pane.getParentFrame());
        }
        e = dieplease.elements();
        while (e.hasMoreElements())
            removePacketPane((PacketPane)e.nextElement(), true);

        // Update the visual tree.
        PacketTreeNode selected = getSelectedNode();
        if (selected != null)
            if (subtree.isGrandparentOf(selected.getPacket()))
                packetTree.getSelectionModel().clearSelection();

        PacketTreeNode packetNode = findTreeNode(subtree);
        TreeNode disownedParent = packetNode.getParent();
        packetNode.removeFromParent();
        treeModel.reload(disownedParent);
    }

    /**
     * This should be called whenever a packet subtree has been deleted,
     * so that all interfaces and the visual packet tree can be updated
     * accordingly.  This routine
     * should only be called for the root packet of that subtree and
     * should not be called again for its descendants.
     * Note that the matriarch of the packet tree should never be
     * deleted.
     * <p>
     * This routine should be called <i>after</i> the subtree has been
     * deleted; note that <tt>fireSubtreeToBeDeleted()</tt> should also
     * be called <i>before</i> the subtree is deleted.
     *
     * @param parent the parent packet immediately above the subtree
     * that was deleted.
     * @param ui the interface that caused the deletion; this may be
     * <tt>null</tt>.
     *
     * @see #fireSubtreeToBeDeleted
     */
    public void fireSubtreeWasDeleted(NPacket parent, PacketUI ui) {
        // Update the interfaces.
        PacketPane pane;
        Enumeration e = allPacketPanes.elements();
        while (e.hasMoreElements()) {
            pane = (PacketPane)e.nextElement();
            pane.getPacketUI().subtreeWasDeleted(parent, ui,
                pane.getParentFrame());
        }
    }

    /**
     * This should be called whenever a packet subtree has been inserted
     * into the overall tree, so that all interfaces and the visual
     * packet tree can be updated
     * accordingly.  This routine should only be called with
     * the root packet of that subtree and should not be called again
     * with its descendants.
     * <p>
     * Note that <tt>fireSubtreeHasChanged()</tt> can be called instead
     * of this routine if specific details are not available.
     *
     * @param subtree the root of the packet subtree that has been
     * inserted.
     * @param ui the interface that caused the insertion; this may be
     * <tt>null</tt>.
     * @param showNewPackets <tt>true</tt> if newly inserted packets
     * are to be displayed in new windows, and <tt>false</tt> if not.
     * If there are more than <tt>SHOW_PACKETS_THRESHOLD</tt> new
     * packets however, they will not be displayed regardless of this
     * argument.
     */
    public void fireSubtreeWasInserted(NPacket subtree, PacketUI ui,
            boolean showNewPackets) {
        // Update the interfaces.
        PacketPane pane;
        Enumeration e = allPacketPanes.elements();
        while (e.hasMoreElements()) {
            pane = (PacketPane)e.nextElement();
            pane.getPacketUI().subtreeWasInserted(subtree, ui,
                pane.getParentFrame());
        }

        // Update the visual tree.
        Vector newNodes = new Vector();
        PacketTreeNode parentNode = findTreeNode(subtree.getTreeParent());
        PacketTreeNode subtreeNode = new PacketTreeNode(subtree);
        NPacket sibling = subtree.getPrevTreeSibling();
        int index;
        if (sibling == null)
            index = 0;
        else if ((index = parentNode.findChildNodeIndex(sibling) + 1) <= 0)
            index = parentNode.getChildCount();
        treeModel.insertNodeInto(subtreeNode, parentNode, index);
        newNodes.addElement(subtreeNode);

        subtreeNode.verifyDescendants(treeModel, newNodes);

        scrollToNodes(newNodes);
        if (showNewPackets)
            showNodePackets(newNodes);
    }

    /**
     * This should be called whenever packets have been inserted and/or
     * renamed and/or moved in the given subtree but more specific
     * details about the alterations are not
     * available, so that all interfaces and the visual packet tree
     * can be updated accordingly.
     * Multiple packets may have been operated upon.
     * Note that the matriarch of the packet tree should never be
     * deleted.
     * <p>
     * Any pane containing a renamed packet will have its title(s)
     * updated.
     * <p>
     * Note that this routine does not cater for packet deletions.
     *
     * @param subtree the root of the packet subtree in which the
     * operations have taken place.
     * @param ui the interface that caused the operations; this may be
     * <tt>null</tt>.
     * @param showNewPackets <tt>true</tt> if newly inserted packets
     * are to be displayed in new windows, and <tt>false</tt> if not.
     * If there are more than <tt>SHOW_PACKETS_THRESHOLD</tt> new
     * packets however, they will not be displayed regardless of this
     * argument.
     */
    public void fireSubtreeHasChanged(NPacket subtree, PacketUI ui,
            boolean showNewPackets) {
        // Update the interfaces.
        PacketPane pane;
        Enumeration e = allPacketPanes.elements();
        while (e.hasMoreElements()) {
            pane = (PacketPane)e.nextElement();
            if (subtree.isGrandparentOf(pane.getPacket())) {
                // Make sure the pane has the correct name.
                   pane.refreshLabel();
                   Frame f = pane.getSurroundingFrame();
                   if (f != null)
                       f.setTitle(pane.getPacket().getFullName());
            }
            pane.getPacketUI().subtreeHasChanged(subtree, ui,
                pane.getParentFrame());
        }

        // Update the visual tree.
        PacketTreeNode subtreeNode = findTreeNode(subtree);
        PacketTreeNode tmpNode;

        e = subtreeNode.depthFirstEnumeration();
        while (e.hasMoreElements()) {
            tmpNode = (PacketTreeNode)e.nextElement();
            tmpNode.setUserObject(PacketTreeNode.getNodeLabel(
                tmpNode.getPacket()));
        }
        
        Vector newNodes = new Vector();
        subtreeNode.verifyDescendants(treeModel, newNodes);
        scrollToNodes(newNodes);
        packetTree.repaint();

        if (showNewPackets)
            showNodePackets(newNodes);
    }

    // -------------------
    //    TREE HANDLING
    // -------------------

    /**
     * Returns the node in the visual packet tree corresponding to the given
     * packet.
     *
     * @param packet the packet to find in the visual tree.
     * @return the corresonding tree node, or <tt>null</tt> if no such
     * node exists in the visual tree.
     */
    private PacketTreeNode findTreeNode(NPacket packet) {
        // First find the path from the packet back to the root packet.
        Stack path = new Stack();
        while (packet != null) {
            path.push(packet);
            packet = packet.getTreeParent();
        }

        // Now move through the stack keeping tree nodes in sync.
        path.pop();
        PacketTreeNode currNode = rootNode;
        while (! path.empty()) {
            // Invariant: currNode corresponds to the last packet popped
            // off the stack.
            currNode = currNode.findChildNode((NPacket)path.pop());
            if (currNode == null)
                return null;
        }
        return currNode;
    }

    /**
     * Makes each packet corresponding to the given set of tree nodes
     * visible in the visual packet tree.
     *
     * @param nodes a vector of <tt>PacketTreeNode</tt> nodes that
     * should be made visible in the tree.
     */
    private void scrollToNodes(Vector nodes) {
        Enumeration e = nodes.elements();
        while (e.hasMoreElements())
            packetTree.scrollPathToVisible(new TreePath(
                ((PacketTreeNode)e.nextElement()).getPath()));
    }

    /**
     * Displays each packet corresponding to the given set of tree nodes
     * in a new window.
     * If there are more than <tt>SHOW_PACKETS_THRESHOLD</tt> nodes in
     * the given set however, nothing will be displayed.
     *
     * @param nodes a vector of <tt>PacketTreeNode</tt> nodes whose
     * packets should be displayed.
     */
    private void showNodePackets(Vector nodes) {
        if (nodes.size() > SHOW_PACKETS_THRESHOLD)
            return;

        Enumeration e = nodes.elements();
        boolean justOneNode = (nodes.size() == 1);
        PacketTreeNode node;
        while (e.hasMoreElements()) {
            node = (PacketTreeNode)e.nextElement();
            insertPacketPane(new PacketPane(this,
                PacketUIManager.newPacketUI(node.getPacket(), getShell(),
                this, node.getPacket().isPacketEditable())), justOneNode);
        }
    }

    // -------------
    //    ACTIONS
    // -------------

    /**
     * Allow the user to import a packet from a file in a foreign format.
     * User input will be collected, and the new packet will be placed
     * in both the visual and underlying native trees.
     *
     * @param importer specifies the mechanics of importing from a
     * particular file format.
     * @see normal.packetui.PacketCreator
     */
    void importData(Importer importer) {
        // Try importing the data before we do anything.
        if (! importer.importData())
            return;

        // Collect further user input and incorporate the new data.
        NPacket packet = NewPacketDialog.newPacket(getShell(),
            rootNode, getSelectedNode(), importer);
        if (packet != null) {
            fireSubtreeWasInserted(packet, null, true);
            setDirty(true);
        }
    }
            
    /**
     * Allow the user to create a new packet.
     * A dialog will be created to collect user input, and the new packet
     * will be placed in both the visual and underlying native trees.
     *
     * @param command dictates what type of interface to give the user
     * and thus what type of packet will be created.  This should be
     * as required by <tt>PacketUIManager.newPacketCreator()</tt>.
     * @see normal.packetui.PacketUIManager#newPacketCreator
     */
    void packetNew(String command) {
        // Attempt to obtain an interface for collecting user input.
        PacketCreator ui = PacketUIManager.newPacketCreator(command);
        if (ui != null) {
            // Collect user input and create the new packet.
            NPacket packet = NewPacketDialog.newPacket(getShell(),
                rootNode, getSelectedNode(), ui);
            if (packet != null) {
                fireSubtreeWasInserted(packet, null, true);
                setDirty(true);
            }
        }
    }
    
    /**
     * View the currently selected packet.
     */
    void packetView() {
        PacketTreeNode chosen = getSelectedNode();
        if (chosen == null)
            return;
        packetView(chosen.getPacket());
    }

    /**
     * View the given packet.
     *
     * @param packet the specific packet to view.
     */
    void packetView(NPacket packet) {
        insertPacketPane(new PacketPane(this,
            PacketUIManager.newPacketUI(packet, getShell(), this,
            packet.isPacketEditable())), getOptions().getAutoDock());
    }

    /**
     * Clone the currently selected packet.
     *
     * @param includeDescendants <tt>true</tt> if and only if
     * descendants of the currently selected packet are also to
     * be cloned.
     */
    void packetClone(boolean includeDescendants) {
        // Establish which packet is to be cloned.
        PacketTreeNode chosen = getSelectedNode();
        if (chosen == null)
            return;

        NPacket clone = chosen.getPacket().clonePacket(includeDescendants,
            false);
        if (clone == null) {
            getShell().error("This packet may not be cloned.");
            return;
        }

        fireSubtreeWasInserted(clone, null, false);
        setDirty(true);
    }
    
    /**
     * Rename the currently selected packet.
     */
    void packetRename() {
        // Establish which packet is to be renamed.
        PacketTreeNode chosen = getSelectedNode();
        if (chosen == null)
            return;
        NPacket packet = chosen.getPacket();

        // Open a dialog to obtain a new name for the packet.
        String newName = packet.getPacketLabel();
        while (true) {
            newName = InputBox.getInput(getNormalFrame(),
                "Rename packet", newName);
            if (newName == null)
                return;
            newName = newName.trim();
            if (newName.length() == 0)
                return;

            // Can we use this name?
            if (rootPacket.findPacketLabel(newName) != null) {
                getShell().error("A packet with this label already exists.");
                newName = rootPacket.makeUniqueLabel(newName);
                continue;
            }
            break;
        }

        // We have a name we can use.
        packet.setPacketLabel(newName);
        setDirty(true);

        firePacketWasRenamed(packet, null);
    }

    /**
     * Delete the currently selected packet and all of its children
     * in the tree.
     */
    void packetDelete() {
        // Establish which packet is to be deleted.
        PacketTreeNode chosen = getSelectedNode();
        if (chosen == null)
            return;
        if (chosen == rootNode)
            return;
        NPacket packet = chosen.getPacket();

        // Check that the user really wants this!        
        if (! getShell().confirm("Are you sure you wish to delete packet "
                + packet.getPacketLabel() + " and all of its children?"))
            return;
        
        NPacket parent = packet.getTreeParent();

        // Notify interfaces and the visual tree that the packet is about to
        // be deleted.
        fireSubtreeToBeDeleted(packet, null);

        // Remove the packet from the real packet tree.
        packet.makeOrphan();

        // Destroy the underlying native packet structure.
        packet.destroy();

        // Notify interfaces that the packet has now been deleted.
        fireSubtreeWasDeleted(parent, null);

        // Tidy up.
        setDirty(true);
    }

    /**
     * Refresh the currently selected subtree in both the visual tree
     * and all interfaces.  Any changes to both tree structure and
     * packet contents will be reflected.
     */
    void packetRefresh() {
        PacketTreeNode chosen = getSelectedNode();
        if (chosen == null)
            return;
        NPacket subtree = chosen.getPacket();

        fireSubtreeHasChanged(subtree, null, false);

        // Note that packet contents may have changed also.
        // Fire such notifications for all packets in the subtree.
        NPacket p = subtree;
        while (p != null) {
            if (! subtree.isGrandparentOf(p))
                break;
            firePacketWasChanged(p, null);
            p = p.nextTreePacket();
        }

        // We have no idea if changes have been made or not!
        // Be on the safe side.
        setDirty(true);
    }

    /**
     * Moves the current packet up the tree hierarchy.
     */
    void moveTreeLeft() {
        PacketTreeNode chosen = getSelectedNode();
        if (chosen == null)
            return;
        NPacket packet = chosen.getPacket();
        if (packet.dependsOnParent() || packet.getTreeParent() == null ||
                packet.getTreeParent().getTreeParent() == null)
            return;

        NPacket parent = packet.getTreeParent();
        NPacket grandparent = parent.getTreeParent();
        packet.makeOrphan();
        grandparent.insertChildAfter(packet, parent);

        PacketTreeNode parentNode = (PacketTreeNode)chosen.getParent();
        PacketTreeNode grandparentNode =
            (PacketTreeNode)parentNode.getParent();
        treeModel.removeNodeFromParent(chosen);
        treeModel.insertNodeInto(chosen, grandparentNode,
            grandparentNode.getIndex(parentNode) + 1);

        fireSubtreeHasChanged(grandparent, null, false);
        TreePath path = new TreePath(chosen.getPath());
        packetTree.scrollPathToVisible(path);
        packetTree.setSelectionPath(path);
        setDirty(true);
    }

    /**
     * Moves the current packet inside the following packet.
     */
    void moveTreeRight() {
        PacketTreeNode chosen = getSelectedNode();
        if (chosen == null)
            return;
        NPacket packet = chosen.getPacket();
        if (packet.dependsOnParent() || packet.getNextTreeSibling() == null)
            return;

        NPacket sibling = packet.getNextTreeSibling();
        packet.makeOrphan();
        sibling.insertChildFirst(packet);

        PacketTreeNode siblingNode =
            (PacketTreeNode)chosen.getNextSibling();
        treeModel.removeNodeFromParent(chosen);
        treeModel.insertNodeInto(chosen, siblingNode, 0);

        fireSubtreeHasChanged(sibling.getTreeParent(), null, false);
        TreePath path = new TreePath(chosen.getPath());
        packetTree.scrollPathToVisible(path);
        packetTree.setSelectionPath(path);
        setDirty(true);
    }

    /**
     * Moves the current packet up in its parent's list of children.
     *
     * @param places the number of places to move the packet up, or
     * -1 if it should be moved as many places as possible.
     */
    void moveTreeUp(int places) {
        PacketTreeNode chosen = getSelectedNode();
        if (chosen == null)
            return;
        NPacket packet = chosen.getPacket();
        
        PacketTreeNode parentNode = (PacketTreeNode)chosen.getParent();
        int index = parentNode.getIndex(chosen);
        while (places != 0 && packet.getPrevTreeSibling() != null) {
            packet.getPrevTreeSibling().swapWithNextSibling();
            treeModel.removeNodeFromParent(chosen);
            index--;
            treeModel.insertNodeInto(chosen, parentNode, index);
            if (places > 0)
                places--;
        }

        fireSubtreeHasChanged(packet.getTreeParent(), null, false);
        TreePath path = new TreePath(chosen.getPath());
        packetTree.scrollPathToVisible(path);
        packetTree.setSelectionPath(path);
        setDirty(true);
    }

    /**
     * Moves the current packet down in its parent's list of children.
     *
     * @param places the number of places to move the packet down, or -1
     * if it should be moved as many places as possible.
     */
    void moveTreeDown(int places) {
        PacketTreeNode chosen = getSelectedNode();
        if (chosen == null)
            return;
        NPacket packet = chosen.getPacket();
        
        PacketTreeNode parentNode = (PacketTreeNode)chosen.getParent();
        int index = parentNode.getIndex(chosen);
        while (places != 0 && packet.getNextTreeSibling() != null) {
            packet.swapWithNextSibling();
            treeModel.removeNodeFromParent(chosen);
            index++;
            treeModel.insertNodeInto(chosen, parentNode, index);
            if (places > 0)
                places--;
        }

        fireSubtreeHasChanged(packet.getTreeParent(), null, false);
        TreePath path = new TreePath(chosen.getPath());
        packetTree.scrollPathToVisible(path);
        packetTree.setSelectionPath(path);
        setDirty(true);
    }

    // -------------
    //    CLASSES
    // -------------

    /**
     * Frame class used to contain packet panes.
     */
    private class PacketFrame extends JFrame {
        /**
         * The packet pane contained in this frame.
         */
        private PacketPane pane;

        /**
         * Create a new frame.
         * This frame will not be automatically centered since there
         * might in general be enough of these frames floating
         * about that the window manager will do a better job of arranging
         * them.
         *
         * @param pane the packet pane to be contained in this frame.
         */
        public PacketFrame(PacketPane pane) {
            super();
            this.pane = pane;
            getContentPane().setLayout(new BorderLayout());
            getContentPane().add(pane, BorderLayout.CENTER);
            this.setSize(400,400);
            setTitle(pane.getPacket().getFullName());
            setIconImage(PacketUIManager.packetIcon(pane.getPacket()).
                getImage());
            addWindowListener(new WindowAdapter() {
                public void windowClosing() {
                    PacketFrame.this.pane.close();
                }
            });
            setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        }
    }
}

