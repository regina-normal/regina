
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

package normal.mainui;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import org.gjt.btools.gui.*;
import org.gjt.btools.gui.component.EditMenu;
import org.gjt.btools.gui.dialog.*;
import org.gjt.btools.gui.lookandfeel.*;
import org.gjt.btools.image.*;
import normal.*;
import normal.algorithm.*;
import normal.console.*;
import normal.engine.*;
import normal.engine.packet.*;
import normal.engine.file.NFile;
import normal.imports.*;
import normal.options.*;
import normal.packetui.*;

/**
 * The main window in which a user will work.
 * Containes a tabbed pane holding a <tt>FilePane</tt> for each open file.
 * Also provides a menu bar and status bar.
 * <p>
 * Global commands are implemented here.  Commands specific to a particular
 * type of <tt>FilePane</tt> are implemented in the corresponding
 * <tt>FilePane</tt> subclass.
 *
 * @see normal.mainui.FilePane
 */
public class NormalFrame extends JFrame implements LookAndFeelSetter {

    // ------------------
    //    DATA MEMBERS
    // ------------------

    /**
     * The number of recent files to remember.
     */
    public static final int recentFileCount = 6;

    /**
     * The shell representing the entire program.
     */
    private Shell shell;
    
    /**
     * The calculation engine currently being used.
     */
    private Engine engine;
    
    /**
     * The current set of user options.
     */
    private NormalOptionSet options;
     
    /**
     * A pane containing a tab for each open file.
     */
    private JTabbedPane fileTabs = new JTabbedPane();
    
    /**
     * Menu containing the list of most recently used files.
     */
    private JMenu menuFileRecent;

    /**
     * Edit menu to be shared amongst text components.
     */
    private EditMenu menuEdit;

    /**
     * Menu items reflecting the name of the currently selected packet.
     */
    private Vector packetNameMenuItems = new Vector();

    /**
     * List of components that should be enabled if and only if a file is
     * currently open.
     */
    private Vector needFile = new Vector();

    /**
     * List of components that should be enabled if and only if a packet
     * is currently selected in the tree.
     */
    private Vector needPacket = new Vector();

    /**
     * Contains routines to call when the user asks for a recent file.
     */
    private ActionListener recentFileActionListener;

    /**
     * The object used to view help through the JavaHelp system if
     * appropriate.
     */
    private ActionListener helpViewer = null;

    // -----------------
    //    CONSTRUCTOR
    // -----------------

    /**
     * Create a new frame in which to work.
     *
     * @param shell the shell representing the entire program.
     */
    public NormalFrame(Shell shell) {
        this.shell = shell;
        this.engine = shell.getEngine();
        this.options = shell.getOptions();

        init();
        reflectCurrentFile();
    }

    // --------------------
    //    QUERY ROUTINES
    // --------------------

    /**
     * Returns the shell representing the entire program.
     *
     * @return the shell representing the entire program.
     */
    public Shell getShell() {
        return shell;
    }

    /**
     * Returns the calculation engine currently in use.
     *
     * @return the engine currently in use.
     */
    public Engine getEngine() {
        return engine;
    }

    /**
     * Returns the current user options for the program.
     *
     * @return the current user options.
     */
    public NormalOptionSet getOptions() {
        return options;
    }

    /**
     * Returns the file pane currently being worked upon.
     *
     * @return the current file pane, or <tt>null</tt> if no file pane is
     * open.
     */
    public FilePane getCurrentFilePane() {
        return (FilePane)fileTabs.getSelectedComponent();
    }

    /**
     * Return the topology pane currently being worked upon.
     * If a file pane is being worked upon that is not a topology pane,
     * this routine will return <tt>null</tt>.
     *
     * @return the current topology pane, or <tt>null</tt> if no topology
     * pane is currently being worked upon.
     */
    public TopologyPane getCurrentTopologyPane() {
        FilePane ans = (FilePane)fileTabs.getSelectedComponent();
        if (ans instanceof TopologyPane)
            return (TopologyPane)ans;
        else
            return null;
    }

    // ------------------------------
    //    COMPONENT INITIALISATION
    // ------------------------------

    /**
     * Initialise the interface components.
     */        
    private void init() {
        // Miscellaneous initialisation.
        int preferredWidth = options.getIntOption("PreferredWidth");
        int preferredHeight = options.getIntOption("PreferredHeight");
        if (preferredWidth <= 0)
            preferredWidth = 600;
        if (preferredHeight <= 0)
            preferredHeight = 500;
        setSize(new Dimension(preferredWidth, preferredHeight));
        setTitle(normal.Application.program);
        setIconImage(Images.mainSmallIcon.image().getImage());
        
        // Set layout managers.
        getContentPane().setLayout(new BorderLayout());
        
        // Initialise components.
        fileTabs.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                reflectCurrentFile();
            }
        });
        
        // Insert components.
        setJMenuBar(makeMenus());
        getContentPane().add(makeToolBar(), BorderLayout.NORTH);
        getContentPane().add(fileTabs, BorderLayout.CENTER);

        // Add event listeners.                
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                fileExit();
            }
        });
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
    }
    
    /**
     * Create the menus for this frame.
     *
     * @return the completely filled menu bar.
     */
    private JMenuBar makeMenus() {
        boolean fileIO = shell.mayAccessFiles();

        JMenuBar menuBar = new JMenuBar();

        // --- File menu ---

        JMenu menuFile = new JMenu("File");
        menuFile.setMnemonic(KeyEvent.VK_F);

        JMenu menuFileNew = new JMenu("New");
        menuFileNew.setIcon(Standard16.document.image());
        menuFileNew.setMnemonic(KeyEvent.VK_N);

        JMenuItem menuFileNewTopology = new JMenuItem("Topology Data",
            Images.btnFileNewTopology.image());
        menuFileNewTopology.setMnemonic(KeyEvent.VK_T);
        menuFileNewTopology.setAccelerator(KeyStroke.getKeyStroke(
            KeyEvent.VK_N, ActionEvent.ALT_MASK));
        menuFileNewTopology.addActionListener(
            new java.awt.event.ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    fileNewTopology();
                }
            });
        menuFileNew.add(menuFileNewTopology);

        JMenuItem menuFileNewLibrary = new JMenuItem("Jython Library",
            Images.btnFileNewLibrary.image());
        menuFileNewLibrary.setMnemonic(KeyEvent.VK_J);
        menuFileNewLibrary.setEnabled(shell.hasFoundJython());
        menuFileNewLibrary.addActionListener(
            new java.awt.event.ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    fileNewLibrary();
                }
            });
        menuFileNew.add(menuFileNewLibrary);

        menuFile.add(menuFileNew);

        if (fileIO) {
            JMenuItem menuFileOpen = new JMenuItem("Open",
                Standard16.open.image());
            menuFileOpen.setMnemonic(KeyEvent.VK_O);
            menuFileOpen.setAccelerator(KeyStroke.getKeyStroke(
                KeyEvent.VK_O, ActionEvent.ALT_MASK));
            menuFileOpen.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    fileOpen();
                }
            });
            menuFile.add(menuFileOpen);

            JMenuItem menuFileSave = new JMenuItem("Save",
                Standard16.save.image());
            menuFileSave.setMnemonic(KeyEvent.VK_S);
            menuFileSave.setAccelerator(KeyStroke.getKeyStroke(
                KeyEvent.VK_S, ActionEvent.ALT_MASK));
            menuFileSave.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    fileSave();
                }
            });
            menuFile.add(menuFileSave);
            needFile.addElement(menuFileSave);

            JMenuItem menuFileSaveAs = new JMenuItem("Save As");
            menuFileSaveAs.setMnemonic(KeyEvent.VK_A);
            menuFileSaveAs.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    fileSaveAs();
                }
            });
            menuFile.add(menuFileSaveAs);
            needFile.addElement(menuFileSaveAs);
        }

        JMenuItem menuFileClose = new JMenuItem("Close",
            Standard16.close.image());
        menuFileClose.setMnemonic(KeyEvent.VK_C);
        menuFileClose.setAccelerator(KeyStroke.getKeyStroke(
            KeyEvent.VK_C, ActionEvent.ALT_MASK));
        menuFileClose.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileClose();
            }
        });
        menuFile.add(menuFileClose);
        needFile.addElement(menuFileClose);

           recentFileActionListener = new ActionListener() {
               public void actionPerformed(ActionEvent e) {
                   fileOpen(new File(e.getActionCommand()));
               }
           };

        if (fileIO) {
            menuFile.addSeparator();

            menuFileRecent = new JMenu("Recent");
            menuFileRecent.setMnemonic(KeyEvent.VK_R);
            updateRecentFilesMenu();
            menuFile.add(menuFileRecent);

            menuFile.addSeparator();

            JMenu menuFileImport = new JMenu("Import");
            menuFileImport.setMnemonic(KeyEvent.VK_I);

            JMenuItem menuFileImportSnapPea =
                new JMenuItem("SnapPea Triangulation");
            menuFileImportSnapPea.setMnemonic(KeyEvent.VK_S);
            menuFileImportSnapPea.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    TopologyPane m = getCurrentTopologyPane();
                    if (m == null) {
                        fileNewTopology();
                        m = getCurrentTopologyPane();
                        if (m == null)
                            return;
                    }
                    m.importData(new SnapPeaImporter(shell));
                }
            });
            menuFileImport.add(menuFileImportSnapPea);

            menuFile.add(menuFileImport);
        }

        menuFile.addSeparator();

        JMenuItem menuFileExit = new JMenuItem("Exit");
        menuFileExit.setMnemonic(KeyEvent.VK_X);
        menuFileExit.setAccelerator(KeyStroke.getKeyStroke(
            KeyEvent.VK_X, ActionEvent.ALT_MASK));
        menuFileExit.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileExit();
            }
        });
        menuFile.add(menuFileExit);

        menuBar.add(menuFile);

        // --- Edit menu ---

        menuEdit = new EditMenu();

        menuBar.add(menuEdit);
        
        // --- Packet menu ---
        JMenu menuPacket = new JMenu("Packet");
        menuPacket.setMnemonic(KeyEvent.VK_P);

        JMenuItem packetMenuHeader = new JMenuItem("(no packet)");
        menuPacket.add(packetMenuHeader);
        packetNameMenuItems.add(packetMenuHeader);

        menuPacket.addSeparator();

        JMenu menuPacketNew = new JMenu("New");
        menuPacketNew.setMnemonic(KeyEvent.VK_N);
        ActionListener newListener = new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                TopologyPane m = getCurrentTopologyPane();
                if (m != null)
                    m.packetNew(e.getActionCommand());
            }
        };

        JMenuItem menuPacketNewContainer = new JMenuItem("Container",
            PacketUIManager.containerIcon);
        menuPacketNewContainer.setMnemonic(KeyEvent.VK_C);
        menuPacketNewContainer.addActionListener(newListener);
        menuPacketNew.add(menuPacketNewContainer);

        JMenuItem menuPacketNewText = new JMenuItem("Text",
            PacketUIManager.textIcon);
        menuPacketNewText.setMnemonic(KeyEvent.VK_X);
        menuPacketNewText.addActionListener(newListener);
        menuPacketNew.add(menuPacketNewText);

        JMenuItem menuPacketNewTriangulation = new JMenuItem("Triangulation",
            PacketUIManager.triangulationIcon);
        menuPacketNewTriangulation.setMnemonic(KeyEvent.VK_T);
        menuPacketNewTriangulation.addActionListener(newListener);
        menuPacketNew.add(menuPacketNewTriangulation);

        JMenuItem menuPacketNewNormalSurfaceList =
            new JMenuItem("Normal Surface List",
            PacketUIManager.surfaceListIcon);
        menuPacketNewNormalSurfaceList.setMnemonic(KeyEvent.VK_N);
        menuPacketNewNormalSurfaceList.addActionListener(newListener);
        menuPacketNew.add(menuPacketNewNormalSurfaceList);

        JMenuItem menuPacketNewSurfaceFilter =
            new JMenuItem("Surface Filter",
            PacketUIManager.filterIcon);
        menuPacketNewSurfaceFilter.setMnemonic(KeyEvent.VK_F);
        menuPacketNewSurfaceFilter.addActionListener(newListener);
        menuPacketNew.add(menuPacketNewSurfaceFilter);

        JMenuItem menuPacketNewScript = new JMenuItem("Script",
            PacketUIManager.scriptIcon);
        menuPacketNewScript.setMnemonic(KeyEvent.VK_S);
        menuPacketNewScript.addActionListener(newListener);
        menuPacketNew.add(menuPacketNewScript);

        menuPacketNew.addSeparator();

        JMenuItem menuPacketNewCensus = new JMenuItem("Census");
        menuPacketNewCensus.setMnemonic(KeyEvent.VK_E);
        menuPacketNewCensus.addActionListener(newListener);
        menuPacketNew.add(menuPacketNewCensus);

        menuPacket.add(menuPacketNew);

        JMenu menuPacketClone = new JMenu("Clone");
        menuPacketClone.setMnemonic(KeyEvent.VK_L);

        JMenuItem menuPacketCloneDesc = new JMenuItem("Include Descendants");
        menuPacketCloneDesc.setMnemonic(KeyEvent.VK_I);
        menuPacketCloneDesc.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                TopologyPane m = getCurrentTopologyPane();
                if (m != null)
                    m.packetClone(true);
            }
        });
        menuPacketClone.add(menuPacketCloneDesc);

        JMenuItem menuPacketCloneNoDesc = new JMenuItem("Exclude Descendants");
        menuPacketCloneNoDesc.setMnemonic(KeyEvent.VK_X);
        menuPacketCloneNoDesc.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                TopologyPane m = getCurrentTopologyPane();
                if (m != null)
                    m.packetClone(false);
            }
        });
        menuPacketClone.add(menuPacketCloneNoDesc);

        menuPacket.add(menuPacketClone);
        needPacket.addElement(menuPacketClone);

        menuPacket.addSeparator();

        JMenuItem menuPacketView = new JMenuItem("View");
        menuPacketView.setMnemonic(KeyEvent.VK_V);
        menuPacketView.setAccelerator(KeyStroke.getKeyStroke(
            KeyEvent.VK_V, ActionEvent.ALT_MASK));
        menuPacketView.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                TopologyPane m = getCurrentTopologyPane();
                if (m != null)
                    m.packetView();
            }
        });
        menuPacket.add(menuPacketView);
        needPacket.addElement(menuPacketView);

        JMenuItem menuPacketRename = new JMenuItem("Rename");
        menuPacketRename.setMnemonic(KeyEvent.VK_R);
        menuPacketRename.setAccelerator(KeyStroke.getKeyStroke(
            KeyEvent.VK_R, ActionEvent.ALT_MASK));
        menuPacketRename.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                TopologyPane m = getCurrentTopologyPane();
                if (m != null)
                    m.packetRename();
            }
        });
        menuPacket.add(menuPacketRename);
        needPacket.addElement(menuPacketRename);

        JMenuItem menuPacketDelete = new JMenuItem("Delete");
        menuPacketDelete.setMnemonic(KeyEvent.VK_D);
        menuPacketDelete.setAccelerator(KeyStroke.getKeyStroke(
            KeyEvent.VK_DELETE, 0));
        menuPacketDelete.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                TopologyPane m = getCurrentTopologyPane();
                if (m != null)
                    m.packetDelete();
            }
        });
        menuPacket.add(menuPacketDelete);
        needPacket.addElement(menuPacketDelete);

        JMenuItem menuPacketRefresh = new JMenuItem("Refresh Subtree");
        menuPacketRefresh.setMnemonic(KeyEvent.VK_F);
        menuPacketRefresh.setAccelerator(KeyStroke.getKeyStroke(
            KeyEvent.VK_F, ActionEvent.ALT_MASK));
        menuPacketRefresh.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                TopologyPane m = getCurrentTopologyPane();
                if (m != null)
                    m.packetRefresh();
            }
        });
        menuPacket.add(menuPacketRefresh);
        needPacket.addElement(menuPacketRefresh);

        menuBar.add(menuPacket);
        needFile.addElement(menuPacket);

        // --- Modify menu ---

        JMenu menuModify = new JMenu("Modify");
        menuModify.setMnemonic(KeyEvent.VK_M);

        JMenuItem modifyMenuHeader = new JMenuItem("(no packet)");
        menuModify.add(modifyMenuHeader);
        packetNameMenuItems.addElement(modifyMenuHeader);

        menuModify.addSeparator();

        Modification.makeMenu(shell, menuModify, needFile);
        needFile.addElement(menuModify);

        menuBar.add(menuModify);
        
        // --- Tools menu ---
        JMenu menuTools = new JMenu("Tools");
        menuTools.setMnemonic(KeyEvent.VK_T);

        JMenuItem menuToolsJythonConsole = new JMenuItem("Jython Console",
            Images.btnConsole.image());
        menuToolsJythonConsole.setMnemonic(KeyEvent.VK_J);
        menuToolsJythonConsole.setAccelerator(KeyStroke.getKeyStroke(
            KeyEvent.VK_J, ActionEvent.ALT_MASK));
        menuToolsJythonConsole.setEnabled(shell.hasFoundJython());
        menuToolsJythonConsole.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                startJythonConsole();
            }
        });
        menuTools.add(menuToolsJythonConsole);

        menuBar.add(menuTools);

        // --- Options menu ---

        JMenu menuOptions = new JMenu("Options");
        menuOptions.setMnemonic(KeyEvent.VK_O);

        JMenu menuOptionsDisplay = new JMenu("Display");
        menuOptionsDisplay.setMnemonic(KeyEvent.VK_D);

        JCheckBoxMenuItem menuOptionsAutoDock =
            new JCheckBoxMenuItem("Automatic Window Docking",
            options.getAutoDock());
        menuOptionsAutoDock.setMnemonic(KeyEvent.VK_W);
        menuOptionsAutoDock.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                options.setAutoDock(e.getStateChange() == e.SELECTED);
                options.writeToFile();
            }
        });
        menuOptionsDisplay.add(menuOptionsAutoDock);

        JCheckBoxMenuItem menuOptionsDisplayIcon =
            new JCheckBoxMenuItem("Display Icon", options.getDisplayIcon());
        menuOptionsDisplayIcon.setMnemonic(KeyEvent.VK_D);
        menuOptionsDisplayIcon.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                optionsDisplayIcon(e.getStateChange() == e.SELECTED);
            }
        });
        menuOptionsDisplay.add(menuOptionsDisplayIcon);

        menuOptions.add(menuOptionsDisplay);

        if (fileIO) {
            JMenu menuOptionsFile = new JMenu("File");
            menuOptionsFile.setMnemonic(KeyEvent.VK_F);

            JCheckBoxMenuItem menuOptionsAutoExtension =
                new JCheckBoxMenuItem("Automatic File Extension",
                    options.getBooleanOption("AutoFileExtension", true));
            menuOptionsAutoExtension.setMnemonic(KeyEvent.VK_E);
            menuOptionsAutoExtension.addItemListener(new ItemListener() {
                public void itemStateChanged(ItemEvent e) {
                    options.setBooleanOption("AutoFileExtension",
                        e.getStateChange() == e.SELECTED);
                    options.writeToFile();
                }
            });
            menuOptionsFile.add(menuOptionsAutoExtension);

            menuOptions.add(menuOptionsFile);
        }

        JMenuItem menuOptionsHelpBrowser = new JMenuItem("Help Browser...");
        menuOptionsHelpBrowser.setMnemonic(KeyEvent.VK_H);
        menuOptionsHelpBrowser.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                new SelectHelpBrowser(shell).show();
            }
        });
        menuOptions.add(menuOptionsHelpBrowser);

        if (fileIO) {
            JMenuItem menuOptionsJythonLibs =
                new JMenuItem("Jython Libraries...");
            menuOptionsJythonLibs.setMnemonic(KeyEvent.VK_J);
            menuOptionsJythonLibs.setEnabled(shell.hasFoundJython());
            menuOptionsJythonLibs.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    new ManageJPythonLibraries(shell).show();
                }
            });
            menuOptions.add(menuOptionsJythonLibs);
        }

        menuOptions.add(new LookAndFeelMenu(this));

        menuBar.add(menuOptions);
                
        // --- Help menu ---

        JMenu menuHelp = new JMenu("Help");
        menuHelp.setMnemonic(KeyEvent.VK_H);

        JMenuItem menuHelpContents = new JMenuItem("Contents",
            Standard16.help.image());
        menuHelpContents.setMnemonic(KeyEvent.VK_C);
        menuHelpContents.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                shell.viewHelp(null);
            }
        });
        menuHelp.add(menuHelpContents);

        menuHelp.addSeparator();

        JMenuItem menuHelpAbout = new JMenuItem("About");
        menuHelpAbout.setMnemonic(KeyEvent.VK_A);
        menuHelpAbout.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                helpAbout();
            }
        });
        menuHelp.add(menuHelpAbout);

        menuBar.add(menuHelp);
        
        return menuBar;
    }

    /**
     * Create the tool bar for this frame.
     *
     * @return the completely filled tool bar.
     */
    private JToolBar makeToolBar() {
        boolean fileIO = shell.mayAccessFiles();

        JToolBar toolBar = new JToolBar();

        JButton newFile = new JButton(Images.btnFileNewTopology.image());
        newFile.setToolTipText("New Topology Data");
        newFile.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileNewTopology();
            }
        });
        toolBar.add(newFile);

        if (fileIO) {
            JButton openFile = new JButton(Standard16.open.image());
            openFile.setToolTipText("Open File");
            openFile.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    fileOpen();
                }
            });
            toolBar.add(openFile);

            JButton saveFile = new JButton(Standard16.save.image());
            saveFile.setToolTipText("Save File");
            saveFile.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    fileSave();
                }
            });
            toolBar.add(saveFile);
            needFile.addElement(saveFile);
        }

        JButton closeFile = new JButton(Standard16.close.image());
        closeFile.setToolTipText("Close File");
        closeFile.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileClose();
            }
        });
        toolBar.add(closeFile);
        needFile.addElement(closeFile);

        toolBar.addSeparator();

        JButton jythonConsole = new JButton(Images.btnConsole.image());
        jythonConsole.setToolTipText("Jython Console");
        jythonConsole.setEnabled(shell.hasFoundJython());
        jythonConsole.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                startJythonConsole();
            }
        });
        toolBar.add(jythonConsole);

        toolBar.addSeparator();

        JButton help = new JButton(Standard16.help.image());
        help.setToolTipText("Help");
        help.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                shell.viewHelp(null);
            }
        });
        toolBar.add(help);

        return toolBar;
    }

    // ---------------------
    //    UPDATE ROUTINES
    // ---------------------

    /**
     * Updates menus and other global interface components to reflect the
     * current open file.  All components will be updated.
     */
    public void reflectCurrentFile() {
        FilePane file = getCurrentFilePane();
        boolean hasFile = (file != null);

        // Update needFile.
        Enumeration e = needFile.elements();
        while (e.hasMoreElements())
            ((JComponent)e.nextElement()).setEnabled(hasFile);

        // Update needPacket and packetNameMenuItems.
        if (file == null || ! (file instanceof TopologyPane))
            reflectPacket(null);
        else
            reflectPacket(((TopologyPane)file).getSelectedPacket());

        // Update menuEdit.
        menuEdit.setTarget(file == null ? null : file.getWorkingTextComponent());
    }

    /**
     * Updates menus and other global interface components to reflect the
     * current working text component.  Only text component-dependent
     * components will be updated.
     */
    public void reflectCurrentTextComponent() {
        FilePane file = getCurrentFilePane();
        menuEdit.setTarget(file == null ? null : file.getWorkingTextComponent());
    }

    /**
     * Updates menus and other global interface components to reflect the
     * current packet selected in the current topology pane.  Only
     * packet-dependent components will be updated.
     */
    public void reflectCurrentPacket() {
        TopologyPane sys = getCurrentTopologyPane();
        reflectPacket(sys == null ? null : sys.getSelectedPacket());
    }

    /**
     * Updates menus and other global interface components to reflect the
     * given packet.  Only packet-dependent components will be updated.
     *
     * @param packet the packet to reflect, or <tt>null</tt> if no packet
     * should be reflected.
     */
    private void reflectPacket(NPacket packet) {
        // Update needPacket.
        Enumeration e = needPacket.elements();
        while (e.hasMoreElements())
            ((JComponent)e.nextElement()).setEnabled(packet != null);

        // Update packetNameMenuItems.
        e = packetNameMenuItems.elements();
        if (packet == null)
            while (e.hasMoreElements())
                ((JMenuItem)e.nextElement()).setText("(no packet)");
        else {
            String label = "USING: " + packet.getPacketLabel();
            while (e.hasMoreElements())
                ((JMenuItem)e.nextElement()).setText(label);
        }
    }

    // ----------------------
    //    RECENT FILE MENU
    // ----------------------

    /**
     * Updates the items in the File-Recent menu.
     */
    private void updateRecentFilesMenu() {
        menuFileRecent.removeAll();
        JMenuItem item;
        String file;
        for (int i=0; i<recentFileCount; i++) {
            file = options.getStringOption("RecentFile" + String.valueOf(i));
            if (file != null) {
                item = new JMenuItem(file);
                menuFileRecent.add(item);
                item.addActionListener(recentFileActionListener);
            }
        }
    }
    
    /**
     * Inserts the given file into the list of recent files and updates the
     * menu.  The options in which the list is stored will not be written
     * to file.
     *
     * @param filename the recent file to add to the list.
     */
    private void addRecentFile(String filename) {
        String file;
        boolean spaceMade = false;
        int i;
        
        // First check it's not already in the list.        
        for (i=0; i<recentFileCount; i++) {
            file = options.getStringOption("RecentFile" + String.valueOf(i));
            if (file != null)
                if (file.equalsIgnoreCase(filename)) {
                    // The file is already here in the list;
                    // Reshuffle everything above down to this point.
                    for (int j=i-1; j>=0; j--) {
                        file = options.getStringOption("RecentFile" +
                            String.valueOf(j));
                        if (file != null)
                            options.setStringOption("RecentFile" +
                                String.valueOf(j+1), file);
                    }
                    spaceMade = true;
                    break;
                }
        }

        if (! spaceMade) {
            // Shuffle everything else down the list.
            for (i=recentFileCount-2; i>=0; i--) {
                file = options.getStringOption("RecentFile" +
                    String.valueOf(i));
                if (file != null)
                    options.setStringOption("RecentFile" + String.valueOf(i+1),
                        file);
            }
        }
        
        // Add the file to the top of the list.
        options.setStringOption("RecentFile0", filename);
        updateRecentFilesMenu();
    }

    // --------------------------
    //    GENERAL MODIFICATION
    // --------------------------
    
    /**
     * Ensures the visual file-is-dirty marker is displayed
     * for the given file pane if and only if it should be.
     * This need only be called from <tt>FilePane.setDirty()</tt>.
     *
     * @param pane the file pane under examination.
     */
    void updateDirtyMarker(FilePane pane) {
        fileTabs.setIconAt(fileTabs.indexOfComponent(pane),
            pane.getDirty() ? Standard16.save.image() : null);
    }
    
    /**
     * Insert the given file pane into the UI.
     *
     * @param m the file pane to insert.
     * @param label the label to give this file pane in the UI.
     * This label will be placed on the corresponding file tab.
     */
    private void insertFilePane(FilePane pane, String label) {
        fileTabs.insertTab(label, null /* icon */,
            pane, null /* tooltip */, fileTabs.getTabCount());
    }
        
    /**
     * Attempt to close the given file pane.
     * If data has changed, the user will be asked if they really want to
     * do this.  If so, the pane will be closed; if not, no further action
     * will be taken.
     *
     * @param pane the pane that we will try to close.
     * @return <tt>true</tt> if and only if the file pane was closed.
     */
    private boolean closeFilePane(FilePane pane) {
        if (! pane.canClose())
            return false;
        fileTabs.remove(pane);
        pane.cleanUp();
        return true;
    }

    // -------------
    //    ACTIONS
    // -------------

    /**
     * Create a new topology file.
     */
    private void fileNewTopology() {
        FilePane m = TopologyPane.newPane(shell);
        insertFilePane(m, "New Data");
        fileTabs.setSelectedComponent(m);
    }

    /**
     * Create a new Jython library.
     */
    private void fileNewLibrary() {
        FilePane m = LibraryPane.newPane(shell);
        insertFilePane(m, "New Library");
        fileTabs.setSelectedComponent(m);
    }

    /**
     * Open the specified file.
     *
     * @param file the file to open, containing both file and path information.
     */
    private void fileOpen(File file) {
        // Make a new file pane.
        FilePane m;
        if (TopologyPane.filenameFilter.accept(file))
            m = TopologyPane.newPane(shell, file);
        else if (LibraryPane.filenameFilter.accept(file))
            m = LibraryPane.newPane(shell, file);
        else {
            shell.error("I don't know what type of file this is.  Make " +
                "sure the filename has the proper extension (such as .rga " +
                "for topology data files).");
            return;
        }

        if (m == null)
            return;

        // Update the file details for the new pane.
        File fileDir = file.getParentFile();
        if (fileDir == null)
            fileDir = new File(".");
        m.setFileDir(fileDir);
        m.setFileName(file.getName());
        
        // Add the new file to the UI.
        insertFilePane(m, file.getName());
        fileTabs.setSelectedComponent(m);
        
        // Update the last used directory and recent files list.
        options.setStringOption("LastDir", fileDir.getAbsolutePath());
        addRecentFile(file.getAbsolutePath());
        options.writeToFile();
    }
    
    /**
     * Open a new file.
     */
    private void fileOpen() {
        // Open a file dialog.
        JFileChooser chooser = new JFileChooser();
        chooser.addChoosableFileFilter(TopologyPane.filenameFilter);
        chooser.addChoosableFileFilter(LibraryPane.filenameFilter);
        chooser.setFileFilter(TopologyPane.filenameFilter);
        chooser.setCurrentDirectory(
            new File(options.getStringOption("LastDir", ".")));
        chooser.setDialogTitle("Open file...");
        if (chooser.showOpenDialog(this) == chooser.APPROVE_OPTION)
            try {
                fileOpen(chooser.getSelectedFile().getCanonicalFile());
            } catch (IOException e) {
                fileOpen(chooser.getSelectedFile());
            }
    }

    /**
     * Save the active file.
     */
    private void fileSave() {
        FilePane m = getCurrentFilePane();
        if (m == null)
            return;

        // If there is no default filename, we really have a File-Save As.
        if (m.getFileName() == null) {
            fileSaveAs();
            return;
        }

        if (! m.canSave())
            return;
        
        // Attempt to save to the default file.
        if (m.saveFile(new File(m.getFileDir(), m.getFileName())))
            m.setDirty(false);
    }

    /**
     * Save the active file.
     */
    private void fileSaveAs() {
        FilePane m = getCurrentFilePane();
        if (m == null)
            return;
        if (! m.canSave())
            return;

        // Open a file dialog.
        JFileChooser chooser = new JFileChooser();
        chooser.setFileFilter(m.getFileFilter());

        File fileDir = m.getFileDir();
        if (fileDir == null)
            chooser.setCurrentDirectory(
                new File(options.getStringOption("LastDir", ".")));
        else
            chooser.setCurrentDirectory(fileDir);

        String fileName = m.getFileName();
        if (fileName != null)
            chooser.setSelectedFile(new File(fileName));

        chooser.setDialogTitle("Save file...");
        if (chooser.showSaveDialog(this) != chooser.APPROVE_OPTION)
            return;

        // Determine the requested file.
        File dest;
        try {
               dest = chooser.getSelectedFile().getCanonicalFile();
        } catch (IOException e) {
            dest = chooser.getSelectedFile();
        }

        // Add a file extension if appropriate.
        if ((! m.getFileFilter().accept(dest)) &&
                options.getBooleanOption("AutoFileExtension", true))
            dest = new File(dest.getAbsolutePath() + m.getFileExtension());

        // Attempt to save to the requested file.
        m.saveFile(dest);
        
        // Update the file properties.
        m.setDirty(false);
        m.setFileName(dest.getName());
        fileDir = dest.getParentFile();
        if (fileDir == null)
            fileDir = new File(".");
        m.setFileDir(fileDir);
        fileTabs.setTitleAt(fileTabs.indexOfComponent(m), dest.getName());
            
        // Update the last used directory and recent files list.
        options.setStringOption("LastDir", fileDir.getAbsolutePath());
        addRecentFile(dest.getAbsolutePath());
        options.writeToFile();
    }

    /**
     * Close the active file.
     */
    private void fileClose() {
        FilePane m = getCurrentFilePane();
        if (m == null)
            return;
            
        // Close the file pane.
        closeFilePane(m);
    }
    
    /**
     * Exit the program.
     */
    private void fileExit() {
        // Check to see that each open file is prepared to die, and
        // clean up properly after it.
        while (fileTabs.getTabCount() > 0) {
            if (! closeFilePane((FilePane)fileTabs.getComponentAt(0)))
                return;
        }

        // Close each open Jython console.
        JPythonConsoleFrame console;
        while (shell.hasOpenConsoles()) {
            console = shell.getConsole();
            if (console.getConsole().isProcessing()) {
                shell.error(
                    "One of the Jython consoles is still processsing input.");
                return;
            } else
                console.closeConsole();
        }
        
        // Save preferred screen positions to file.
        // The writeToFile will also write the divider location, which is set
        // when the file pane closes.
        Dimension currentSize = getSize();
        options.setIntOption("PreferredWidth", currentSize.width);
        options.setIntOption("PreferredHeight", currentSize.height);
        options.writeToFile();

        // Exit the program.
        dispose();
        shell.exit(0);
    }

    public boolean setLookAndFeel(LookAndFeelItem lookAndFeel) {
        if (! lookAndFeel.setLookAndFeel()) {
            shell.error("An error occurred whilst attempting to " +
                "change the new look and feel.");
            return false;
        }

        // Store the new look and feel in our options.
        options.setStringOption("LookAndFeel", lookAndFeel.getClassName());
        options.writeToFile();

        // Update each top-level container.
        lookAndFeel.updateLookAndFeel(this);

        int tot = fileTabs.getTabCount();
        for (int i=0; i<tot; i++)
            ((FilePane)fileTabs.getComponentAt(i)).updateLookAndFeel();

        Enumeration e = shell.getConsoles();
        while (e.hasMoreElements())
            lookAndFeel.updateLookAndFeel((JFrame)e.nextElement());

        return true;
    }

    /**
     * Alters whether or not the icon is displayed.
     *
     * @param display <tt>true</tt> if and only if the icon should be
     * displayed.
     */
    public void optionsDisplayIcon(boolean display) {
        options.setDisplayIcon(display);
        options.writeToFile();
        
        // Update each file tab.
        int tot = fileTabs.getTabCount();
        for (int i=0; i<tot; i++)
            ((FilePane)fileTabs.getComponentAt(i)).setDisplayIcon(display);
    }

    /**
     * Open and start a new Jython console frame.
     */
    private void startJythonConsole() {
        JPythonConsoleFrame console =
            ConsoleUtils.createGraphicalConsole(shell, false);
        Positioner.centerOnScreen(console);
        console.show();
        console.startConsole();
    }

    /**
     * Display an About box.
     */
    private void helpAbout() {
        AboutBox about = new AboutBox(shell);
        Positioner.centerOnScreen(about);
        about.show();
    }
}

