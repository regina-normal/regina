
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

package normal.mainui;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import btools.ext.*;
import btools.gui.*;
import btools.gui.component.EditMenu;
import btools.gui.dialog.*;
import btools.gui.lookandfeel.*;
import btools.image.*;
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
 * Containes a tabbed pane holding a <tt>SystemPane</tt> for each open file.
 * Also provides a menu bar and status bar.
 * <p>
 * Global (not SystemPane-specific) commands are implemented here.
 * SystemPane-specific commands are implemented in <tt>SystemPane</tt>.
 *
 * @see normal.mainui.SystemPane
 */
public class NormalFrame extends JFrame implements LookAndFeelSetter {
    /**
     * The number of recent files to remember.
     */
    public static final int recentFileCount = 4;

    /**
     * The shell representing the entire program.
     */
    private Shell shell;
    
    /**
     * The calculation engine currently being used.
     * @serial
     */
    private Engine engine;
    
    /**
     * The current set of user options.
     * @serial
     */
    private NormalOptionSet options;
     
    /**
     * Filter for data files.
     * @serial
     */
    static private ExtensionFilenameFilter filenameFilter =
        new ExtensionFilenameFilter(normal.Application.fileExtension,
        normal.Application.program + " Data Files (" +
		normal.Application.fileExtension + ")");
    
    /**
     * A pane containing a tab for each open file.
     * @serial
     */
    private JTabbedPane systemTabs = new JTabbedPane();
    
    /**
     * The status bar.
     * @serial
     */
    private JLabel statusBar = new JLabel();

    /**
     * Menu containing the list of most recently used files.
     * @serial
     */
    private JMenu menuFileRecent;

    /**
     * Changeable menu item at the head of the Packet menu.
     * @serial
     */
    private JMenuItem packetMenuHeader;

    /**
     * Changeable menu item at the head of the Modify menu.
     * @serial
     */
    private JMenuItem modifyMenuHeader;

    /**
     * List of components that should be enabled if and only if a file is
     * currently open.
     * @serial
     */
    private Vector needFile = new Vector();

    /**
     * List of components that should be enabled if and only if a packet
     * is currently selected.
     * @serial
     */
    private Vector needPacket = new Vector();

    /**
     * Contains routines to call when the user asks for a recent file.
     * @serial
     */
    private ActionListener recentFileActionListener;

    /**
     * List of open Jython consoles.
     * @serial
     */
    private Vector consoles = new Vector();

    /**
     * The object used to view help through the JavaHelp system if
     * appropriate.
     */
    private ActionListener helpViewer = null;

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
        reflectCurrentPacket();
    }

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
        statusBar.setText(" ");
        systemTabs.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                reflectCurrentPacket();
            }
        });
        
        // Insert components.
        setJMenuBar(makeMenus());
        getContentPane().add(makeToolBar(), BorderLayout.NORTH);
        getContentPane().add(systemTabs, BorderLayout.CENTER);
        getContentPane().add(statusBar, BorderLayout.SOUTH);
    }
    
    /**
     * Create the menus for this frame.
     *
     * @return the completely filled menu bar.
     */
    private JMenuBar makeMenus() {
        // File menu
        recentFileActionListener = new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileOpen(e.getActionCommand());
            }
        };
        JMenu menuFile = new JMenu("File");
		menuFile.setMnemonic(KeyEvent.VK_F);
        JMenuItem menuFileNew = new JMenuItem("New",
            Images.btnFileNew.image());
		menuFileNew.setMnemonic(KeyEvent.VK_N);
		menuFileNew.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_N, ActionEvent.ALT_MASK));
        JMenuItem menuFileOpen = new JMenuItem("Open",
            Standard16.open.image());
		menuFileOpen.setMnemonic(KeyEvent.VK_O);
		menuFileOpen.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_O, ActionEvent.ALT_MASK));
        JMenuItem menuFileSave = new JMenuItem("Save",
            Standard16.save.image());
		menuFileSave.setMnemonic(KeyEvent.VK_S);
		menuFileSave.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_S, ActionEvent.ALT_MASK));
        JMenuItem menuFileSaveAs = new JMenuItem("Save As");
		menuFileSaveAs.setMnemonic(KeyEvent.VK_A);
        JMenuItem menuFileClose = new JMenuItem("Close",
            Standard16.close.image());
		menuFileClose.setMnemonic(KeyEvent.VK_C);
		menuFileClose.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_C, ActionEvent.ALT_MASK));
        menuFileRecent = new JMenu("Recent");
		menuFileRecent.setMnemonic(KeyEvent.VK_R);
        updateRecentFilesMenu();
        JMenu menuFileImport = new JMenu("Import");
		menuFileImport.setMnemonic(KeyEvent.VK_I);
        JMenuItem menuFileImportSnapPea =
            new JMenuItem("SnapPea Triangulation");
		menuFileImportSnapPea.setMnemonic(KeyEvent.VK_S);
        JMenuItem menuFileExit = new JMenuItem("Exit");
		menuFileExit.setMnemonic(KeyEvent.VK_X);
		menuFileExit.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_X, ActionEvent.ALT_MASK));
        menuFile.add(menuFileNew);
        menuFile.add(menuFileOpen);
        menuFile.add(menuFileSave);
        menuFile.add(menuFileSaveAs);
        menuFile.add(menuFileClose);
        menuFile.addSeparator();
        menuFile.add(menuFileRecent);
        menuFile.addSeparator();
        menuFileImport.add(menuFileImportSnapPea);
        menuFile.add(menuFileImport);
        menuFile.addSeparator();
        menuFile.add(menuFileExit);
        needFile.addElement(menuFileSave);
        needFile.addElement(menuFileSaveAs);
        needFile.addElement(menuFileClose);

		// Edit menu
		JMenu menuEdit = new EditMenu();
        
        // Packet menu
        JMenu menuPacket = new JMenu("Packet");
		menuPacket.setMnemonic(KeyEvent.VK_P);
        packetMenuHeader = new JMenuItem("(no packet)");
        menuPacket.add(packetMenuHeader);
        menuPacket.addSeparator();
        JMenu menuPacketNew = new JMenu("New");
		menuPacketNew.setMnemonic(KeyEvent.VK_N);
        JMenuItem menuPacketNewContainer = new JMenuItem("Container",
            PacketUIManager.containerIcon);
		menuPacketNewContainer.setMnemonic(KeyEvent.VK_C);
        JMenuItem menuPacketNewText = new JMenuItem("Text",
            PacketUIManager.textIcon);
		menuPacketNewText.setMnemonic(KeyEvent.VK_X);
        JMenuItem menuPacketNewTriangulation = new JMenuItem("Triangulation",
            PacketUIManager.triangulationIcon);
		menuPacketNewTriangulation.setMnemonic(KeyEvent.VK_T);
        JMenuItem menuPacketNewNormalSurfaceList =
            new JMenuItem("Normal Surface List",
            PacketUIManager.surfaceListIcon);
		menuPacketNewNormalSurfaceList.setMnemonic(KeyEvent.VK_N);
        JMenuItem menuPacketNewSurfaceFilter =
            new JMenuItem("Surface Filter",
            PacketUIManager.filterIcon);
		menuPacketNewSurfaceFilter.setMnemonic(KeyEvent.VK_F);
        JMenuItem menuPacketNewScript = new JMenuItem("Script",
            PacketUIManager.scriptIcon);
		menuPacketNewScript.setMnemonic(KeyEvent.VK_S);
        JMenuItem menuPacketNewCensus = new JMenuItem("Census");
		menuPacketNewCensus.setMnemonic(KeyEvent.VK_E);
        JMenuItem menuPacketView = new JMenuItem("View");
		menuPacketView.setMnemonic(KeyEvent.VK_V);
		menuPacketView.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_V, ActionEvent.ALT_MASK));
        JMenu menuPacketClone = new JMenu("Clone");
		menuPacketClone.setMnemonic(KeyEvent.VK_L);
        JMenuItem menuPacketCloneDesc = new JMenuItem("Include Descendants");
		menuPacketCloneDesc.setMnemonic(KeyEvent.VK_I);
        JMenuItem menuPacketCloneNoDesc = new JMenuItem("Exclude Descendants");
		menuPacketCloneNoDesc.setMnemonic(KeyEvent.VK_X);
        JMenuItem menuPacketRename = new JMenuItem("Rename");
		menuPacketRename.setMnemonic(KeyEvent.VK_R);
		menuPacketRename.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_R, ActionEvent.ALT_MASK));
        JMenuItem menuPacketDelete = new JMenuItem("Delete");
		menuPacketDelete.setMnemonic(KeyEvent.VK_D);
		menuPacketDelete.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_D, ActionEvent.ALT_MASK));
        JMenuItem menuPacketRefresh = new JMenuItem("Refresh Subtree");
		menuPacketRefresh.setMnemonic(KeyEvent.VK_F);
		menuPacketRefresh.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_F, ActionEvent.ALT_MASK));
        menuPacketNew.add(menuPacketNewContainer);
        menuPacketNew.add(menuPacketNewText);
        menuPacketNew.add(menuPacketNewTriangulation);
        menuPacketNew.add(menuPacketNewNormalSurfaceList);
        menuPacketNew.add(menuPacketNewSurfaceFilter);
        menuPacketNew.add(menuPacketNewScript);
        menuPacketNew.addSeparator();
        menuPacketNew.add(menuPacketNewCensus);
        menuPacket.add(menuPacketNew);
        menuPacketClone.add(menuPacketCloneDesc);
        menuPacketClone.add(menuPacketCloneNoDesc);
        menuPacket.add(menuPacketClone);
        menuPacket.addSeparator();
        menuPacket.add(menuPacketView);
        menuPacket.add(menuPacketRename);
        menuPacket.add(menuPacketDelete);
        menuPacket.add(menuPacketRefresh);
        needFile.addElement(menuPacket);
        needPacket.addElement(menuPacketView);
        needPacket.addElement(menuPacketClone);
        needPacket.addElement(menuPacketRename);
        needPacket.addElement(menuPacketDelete);
        needPacket.addElement(menuPacketRefresh);

        // Modify menu
        JMenu menuModify = new JMenu("Modify");
		menuModify.setMnemonic(KeyEvent.VK_M);
        modifyMenuHeader = new JMenuItem("(no packet)");
        menuModify.add(modifyMenuHeader);
        menuModify.addSeparator();
        Modification.makeMenu(shell, menuModify, needFile);
        needFile.addElement(menuModify);
        
        // Tools menu
        JMenu menuTools = new JMenu("Tools");
		menuTools.setMnemonic(KeyEvent.VK_T);
        JMenuItem menuToolsJythonConsole = new JMenuItem("Jython Console",
            Images.btnConsole.image());
		menuToolsJythonConsole.setMnemonic(KeyEvent.VK_J);
		menuToolsJythonConsole.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_J, ActionEvent.ALT_MASK));
        menuToolsJythonConsole.setEnabled(shell.hasFoundJython());
        menuTools.add(menuToolsJythonConsole);

        // Options menu
        JMenu menuOptions = new JMenu("Options");
		menuOptions.setMnemonic(KeyEvent.VK_O);
        JCheckBoxMenuItem menuOptionsAutoDock =
            new JCheckBoxMenuItem("Automatic Window Docking",
            options.getAutoDock());
		menuOptionsAutoDock.setMnemonic(KeyEvent.VK_W);
        JCheckBoxMenuItem menuOptionsDisplayIcon =
            new JCheckBoxMenuItem("Display Icon", options.getDisplayIcon());
		menuOptionsDisplayIcon.setMnemonic(KeyEvent.VK_D);
        JMenu menuOptionsDisplay = new JMenu("Display");
		menuOptionsDisplay.setMnemonic(KeyEvent.VK_D);
        JCheckBoxMenuItem menuOptionsAutoExtension =
            new JCheckBoxMenuItem("Automatic Extension (" +
                Application.fileExtension + ")",
                options.getBooleanOption("AutoFileExtension", true));
		menuOptionsAutoExtension.setMnemonic(KeyEvent.VK_E);
        JMenu menuOptionsFile = new JMenu("File");
		menuOptionsFile.setMnemonic(KeyEvent.VK_F);
        JMenuItem menuOptionsHelpBrowser = new JMenuItem("Help Browser...");
		menuOptionsHelpBrowser.setMnemonic(KeyEvent.VK_H);
        JMenuItem menuOptionsJythonLibs =
			new JMenuItem("Jython Libraries...");
		menuOptionsJythonLibs.setMnemonic(KeyEvent.VK_J);
        menuOptionsJythonLibs.setEnabled(shell.hasFoundJython());
        menuOptionsDisplay.add(menuOptionsAutoDock);
        menuOptionsDisplay.add(menuOptionsDisplayIcon);
        menuOptions.add(menuOptionsDisplay);
        menuOptionsFile.add(menuOptionsAutoExtension);
        menuOptions.add(menuOptionsFile);
        menuOptions.add(menuOptionsHelpBrowser);
		menuOptions.add(menuOptionsJythonLibs);
        menuOptions.add(new LookAndFeelMenu(this));
                
        // Help menu
        JMenu menuHelp = new JMenu("Help");
		menuHelp.setMnemonic(KeyEvent.VK_H);
        JMenuItem menuHelpContents = new JMenuItem("Contents",
            Standard16.help.image());
		menuHelpContents.setMnemonic(KeyEvent.VK_C);
        JMenuItem menuHelpAbout = new JMenuItem("About");
		menuHelpAbout.setMnemonic(KeyEvent.VK_A);
        menuHelp.add(menuHelpContents);
        menuHelp.addSeparator();
        menuHelp.add(menuHelpAbout);
    
        // Entire menu bar
        JMenuBar menuBar = new JMenuBar();
        menuBar.add(menuFile);
		menuBar.add(menuEdit);
        menuBar.add(menuPacket);
        menuBar.add(menuModify);
        menuBar.add(menuTools);
        menuBar.add(menuOptions);
        menuBar.add(menuHelp);
        
        // Add event listeners.
        menuFileNew.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileNew();
            }
        });
        menuFileOpen.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileOpen();
            }
        });
        menuFileSave.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileSave();
            }
        });
        menuFileSaveAs.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileSaveAs();
            }
        });
        menuFileClose.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileClose();
            }
        });
        menuFileImportSnapPea.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                SystemPane m = currentSystem();
                if (m == null) {
                    fileNew();
                    m = currentSystem();
                    if (m == null)
                        return;
                }
                m.importData(new SnapPeaImporter(shell));
            }
        });
        menuFileExit.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileExit();
            }
        });

        // Packet->New menu.
        ActionListener newListener = new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                SystemPane m = currentSystem();
                if (m != null)
                    m.packetNew(e.getActionCommand());
            }
        };
        menuPacketNewContainer.addActionListener(newListener);
        menuPacketNewText.addActionListener(newListener);
        menuPacketNewTriangulation.addActionListener(newListener);
        menuPacketNewNormalSurfaceList.addActionListener(newListener);
        menuPacketNewSurfaceFilter.addActionListener(newListener);
        menuPacketNewScript.addActionListener(newListener);
        menuPacketNewCensus.addActionListener(newListener);

        menuPacketView.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                SystemPane m = currentSystem();
                if (m != null)
                    m.packetView();
            }
        });
        menuPacketCloneDesc.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                SystemPane m = currentSystem();
                if (m != null)
                    m.packetClone(true);
            }
        });
        menuPacketCloneNoDesc.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                SystemPane m = currentSystem();
                if (m != null)
                    m.packetClone(false);
            }
        });
        menuPacketRename.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                SystemPane m = currentSystem();
                if (m != null)
                    m.packetRename();
            }
        });
        menuPacketDelete.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                SystemPane m = currentSystem();
                if (m != null)
                    m.packetDelete();
            }
        });
        menuPacketRefresh.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                SystemPane m = currentSystem();
                if (m != null)
                    m.packetRefresh();
            }
        });
        menuToolsJythonConsole.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                startJythonConsole();
            }
        });
        menuOptionsAutoDock.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                options.setAutoDock(e.getStateChange() == e.SELECTED);
                options.writeToFile();
            }
        });
        menuOptionsDisplayIcon.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                optionsDisplayIcon(e.getStateChange() == e.SELECTED);
            }
        });
        menuOptionsAutoExtension.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                options.setBooleanOption("AutoFileExtension",
                    e.getStateChange() == e.SELECTED);
                options.writeToFile();
            }
        });
        menuOptionsHelpBrowser.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                new SelectHelpBrowser(shell).show();
            }
        });
        menuOptionsJythonLibs.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                new ManageJPythonLibraries(shell).show();
            }
        });
        menuHelpContents.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                shell.viewHelp(null);
            }
        });
        menuHelpAbout.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                helpAbout();
            }
        });
        
        // Return.
        return menuBar;
    }

    /**
     * Create the tool bar for this frame.
     *
     * @return the completely filled tool bar.
     */
    private JToolBar makeToolBar() {
        // Create the toolbar components.
        JButton newFile = new JButton(Images.btnFileNew.image());
        newFile.setToolTipText("New File");
        JButton openFile = new JButton(Standard16.open.image());
        openFile.setToolTipText("Open File");
        JButton saveFile = new JButton(Standard16.save.image());
        saveFile.setToolTipText("Save File");
        JButton closeFile = new JButton(Standard16.close.image());
        closeFile.setToolTipText("Close File");
        JButton jythonConsole = new JButton(Images.btnConsole.image());
        jythonConsole.setToolTipText("Jython Console");
        jythonConsole.setEnabled(shell.hasFoundJython());
        JButton help = new JButton(Standard16.help.image());
        help.setToolTipText("Help");
        
        // Add the buttons.
        JToolBar toolBar = new JToolBar();
        toolBar.add(newFile);
        toolBar.add(openFile);
        toolBar.add(saveFile);
        toolBar.add(closeFile);
        toolBar.addSeparator();
        toolBar.add(jythonConsole);
        toolBar.addSeparator();
        toolBar.add(help);
        needFile.addElement(saveFile);
        needFile.addElement(closeFile);

        // Add event listeners.                
        newFile.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileNew();
            }
        });
        openFile.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileOpen();
            }
        });
        saveFile.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileSave();
            }
        });
        closeFile.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fileClose();
            }
        });
        jythonConsole.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                startJythonConsole();
            }
        });
        help.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                shell.viewHelp(null);
            }
        });
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                fileExit();
            }
        });
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        
        // Return.
        return toolBar;
    }

    /**
     * Updates menus and other global interface components to reflect
     * the current system pane and the current packet within that
     * pane.
     */
    public void reflectCurrentPacket() {
        boolean haveFile, havePacket;
        NPacket packet;
        SystemPane m = currentSystem();
        if (m == null) {
            haveFile = false;
            packet = null;
            havePacket = false;
        } else {
            haveFile = true;
            packet = m.getSelectedPacket();
            havePacket = (packet != null);
        }
        
        if (havePacket) {
            String label = packet.getPacketLabel();
            packetMenuHeader.setText("USING: " + label);
            modifyMenuHeader.setText("USING: " + label);
        } else {
            packetMenuHeader.setText("(no packet)");
            modifyMenuHeader.setText("(no packet)");
        }

        Enumeration e = needFile.elements();
        while (e.hasMoreElements())
            ((JComponent)e.nextElement()).setEnabled(haveFile);
        e = needPacket.elements();
        while (e.hasMoreElements())
            ((JComponent)e.nextElement()).setEnabled(havePacket);
    }

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
    
    /**
     * Ensures the visual file-is-dirty marker is displayed
     * for the given system pane if and only if it should be.
     * This need only be called from <tt>SystemPane.setDirty()</tt>.
     *
     * @param pane the system pane under examination.
     */
    void updateDirtyMarker(SystemPane pane) {
        systemTabs.setIconAt(systemTabs.indexOfComponent(pane),
            pane.getDirty() ? Standard16.save.image() : null);
    }

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
     * Return the system pane currently being worked upon.
     *
     * @return the current system.
     */
    public SystemPane currentSystem() {
        return (SystemPane)(systemTabs.getSelectedComponent());
    }

    /**
     * Insert the given system pane into the UI.
     *
     * @param m the system pane to insert.
     * @param label the label to give this system in the UI.
     * This label will be placed on the corresponding system tab.
     */
    private void insertSystem(SystemPane system, String label) {
        systemTabs.insertTab(label, null /* icon */,
            system, null /* tooltip */, systemTabs.getTabCount());
    }
        
    /**
     * Attempt to close the given system pane.
     * If data has changed, the user will be asked if they really want to
     * do this.  If so, the pane will be closed; if not, no further action
     * will be taken.
     *
     * @param system the pane that we will try to close.
     * @return <tt>true</tt> if and only if the system pane was closed.
     */
    private boolean closeSystem(SystemPane system) {
        if (! system.canClose())
            return false;
        systemTabs.remove(system);
        system.cleanUp();
        return true;
    }

    /**
     * Create a new file.
     */
    private void fileNew() {
        // Make a new packet tree.
        NPacket newTree = engine.newNContainer();
        newTree.setPacketLabel("Container");
        
        // Make a new system pane.
        SystemPane m = new SystemPane(shell, newTree);
        
        // Add the new system to the UI.
        insertSystem(m, "New File");
        systemTabs.setSelectedComponent(m);
    }

    /**
     * Open the specified file.
     *
     * @param pathname the complete pathname of the file to open, containing
     * both file and path information.
     */
    private void fileOpen(String pathname) {
        NFile file = engine.newNFile();
        if (! file.open(pathname, file.READ)) {
            shell.error("The requested file does not exist or is " +
                "in an unknown format.");
            file.destroy();
            return;
        }
        NPacket newTree = file.readPacketTree();
        file.close();
        file.destroy();
        if (newTree == null) {
            shell.error("The requested file contains invalid data " +
                "and thus could not be opened.");
            return;
        }
        
        // Make a new system pane.
        SystemPane m = new SystemPane(shell, newTree);
        File filebits = new File(pathname);
        String fileDir = filebits.getParent();
        if (fileDir == null)
            fileDir = ".";
        m.setFileDir(fileDir);
        m.setFileName(filebits.getName());
        
        // Add the new system to the UI.
        insertSystem(m, filebits.getName());
        systemTabs.setSelectedComponent(m);
        
        // Update the last used directory and recent files list.
        options.setStringOption("LastDir", fileDir);
        addRecentFile(pathname);
        options.writeToFile();
    }
    
    /**
     * Open a new file.
     */
    private void fileOpen() {
        // Open a file dialog.
        JFileChooser chooser = new JFileChooser();
        chooser.setFileFilter(filenameFilter);
        chooser.setCurrentDirectory(
            new File(options.getStringOption("LastDir", ".")));
        chooser.setDialogTitle("Open file...");
        if (chooser.showOpenDialog(this) == chooser.APPROVE_OPTION)
            fileOpen(chooser.getSelectedFile().getAbsolutePath());
    }

    /**
     * Save the active file.
     */
    private void fileSave() {
        SystemPane m = currentSystem();
        if (m == null)
            return;
        if (m.unconfirmedEditPanes())
            if (! shell.confirm("Some edit panes currently in use contain " +
                    "changes that have not yet been applied.  Do you wish " +
                    "to continue?"))
                return;
            
        // If there is no default filename, we really have a File-Save As.
        if (m.getFileName() == null) {
            fileSaveAs();
            return;
        }
        
        // Attempt to save to the default file.
        String fileName = new File(m.getFileDir(),
            m.getFileName()).getAbsolutePath();
        NFile file = engine.newNFile();
        if (! file.open(fileName, file.WRITE)) {
            shell.error("The requested file could not be opened for writing.");
            file.destroy();
            return;
        }
        file.writePacketTree(m.getRootPacket());
        file.close();
        file.destroy();
        
        // Update the system properties.
        m.setDirty(false);
    }

    /**
     * Save the active file.
     */
    private void fileSaveAs() {
        SystemPane m = currentSystem();
        if (m == null)
            return;
        if (m.unconfirmedEditPanes())
            if (! shell.confirm("Some edit panes currently in use contain " +
                    "changes that have not yet been applied.  Do you wish " +
                    "to continue?"))
                return;

        // Open a file dialog.
        JFileChooser chooser = new JFileChooser();
        chooser.setFileFilter(filenameFilter);

        String fileDir = m.getFileDir();
        if (fileDir == null)
            chooser.setCurrentDirectory(
                new File(options.getStringOption("LastDir", ".")));
        else
            chooser.setCurrentDirectory(new File(fileDir));

        String fileName = m.getFileName();
        if (fileName != null)
            chooser.setSelectedFile(new File(fileName));

        chooser.setDialogTitle("Save file...");
        if (chooser.showSaveDialog(this) != chooser.APPROVE_OPTION)
            return;

        // Attempt to save to the requested file.
        File dest = chooser.getSelectedFile();
        if (! filenameFilter.accept(dest))
            dest = new File(dest.getAbsolutePath() +
                Application.fileExtension);
        fileName = dest.getAbsolutePath();
        NFile file = engine.newNFile();
        if (! file.open(fileName, file.WRITE)) {
            shell.error("The requested file could not be opened for writing.");
            file.destroy();
            return;
        }
        file.writePacketTree(m.getRootPacket());
        file.close();
        file.destroy();
        
        // Update the system properties.
        m.setDirty(false);
        m.setFileName(dest.getName());
        fileDir = dest.getParent();
        if (fileDir == null)
            fileDir = ".";
        m.setFileDir(fileDir);
        systemTabs.setTitleAt(systemTabs.indexOfComponent(m), dest.getName());
            
        // Update the last used directory and recent files list.
        options.setStringOption("LastDir", fileDir);
        addRecentFile(fileName);
        options.writeToFile();
    }

    /**
     * Close the active file.
     */
    private void fileClose() {
        SystemPane m = currentSystem();
        if (m == null)
            return;
            
        // Close the system pane.
        closeSystem(m);
    }
    
    /**
     * Exit the program.
     */
    private void fileExit() {
        // Check to see that each open file is prepared to die, and
        // clean up properly after it.
        while (systemTabs.getTabCount() > 0) {
            if (! closeSystem((SystemPane)systemTabs.getComponentAt(0)))
                return;
        }

        // Close each open Jython console.
        while (! consoles.isEmpty())
            ((JPythonConsoleFrame)consoles.elementAt(0)).closeConsole();
        
        // Save preferred screen positions to file.
        // The writeToFile will also write the divider location, which is set
        // when the system pane closes.
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

        int tot = systemTabs.getTabCount();
        for (int i=0; i<tot; i++)
            ((SystemPane)systemTabs.getComponentAt(i)).updateLookAndFeel();

        Enumeration e = consoles.elements();
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
        
        // Update each system tab.
        int tot = systemTabs.getTabCount();
        for (int i=0; i<tot; i++)
            ((SystemPane)systemTabs.getComponentAt(i)).setDisplayIcon(display);
    }

    /**
     * Open and start a new Jython console frame.
     */
    private void startJythonConsole() {
		Frame console = ConsoleUtils.createGraphicalConsole(shell, false);
        Positioner.centerOnScreen(console);
        console.show();
    }

    /**
     * Causes this frame to take ownership of the given Jython console.
     * The console will have its look and feel updated with this frame
     * and will be closed with this frame.
     *
     * @param console the console to take ownership of; this should be
     * of class <tt>normal.console.JPythonConsoleFrame</tt>, but it is
     * passed as a <tt>JFrame</tt> to avoid problems if Jython is not
     * available.
     */
    public void ownConsole(JFrame console) {
        consoles.addElement(console);
    }

    /**
     * Causes this frame to relinquish ownership of the given Jython
     * console.
     * <p>
     * <b>Prerequisite:</b> The frame currently has ownership of the
     * given console as given by <tt>ownConsole()</tt>.
     *
     * @param console the console to relinquish ownership of.
     * @see #ownConsole
     */
    public void disownConsole(JFrame console) {
        consoles.removeElement(console);
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

