
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

package normal.console;

import normal.*;
import normal.mainui.NormalFrame;
import normal.options.NormalOptionSet;
import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import java.io.*;
import javax.swing.*;
import javax.swing.text.Keymap;
import org.gjt.btools.gui.component.EditMenu;
import org.gjt.btools.image.*;
import org.python.util.PythonInterpreter;

/**
 * Provides a frame containing a Jython console and associated tools.
 */
public class JPythonConsoleFrame extends JFrame
		implements PropertyChangeListener {
    /**
     * Is this window the entire program?
     */
    private boolean standalone;

    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * The actual Jython console contained in this window.
     */
    private JPythonConsole console;

	/**
	 * The break processing menu item.
	 */
	private JMenuItem menuConsoleBreak;

	/**
	 * The close console menu item.
	 */
	private JMenuItem menuConsoleClose;

    /**
     * Creates a new Jython console frame.  Note that the console must
     * still be started before it is offered to the user for
     * interaction; this can be done through the routine
     * <tt>startConsole()</tt>.
     *
     * @param shell the shell representing the entire program.
     * @param standalone <tt>true</tt> if and only if this frame will
     * in fact be the entire program, with no other GUI present.
     * @see #startConsole
     */
    public JPythonConsoleFrame(Shell shell, boolean standalone) {
        super(Application.program + " Jython Console");
        this.shell = shell;
        this.standalone = standalone;
    
        Frame frame = shell.getPrimaryFrame();
        if (frame != null && frame instanceof NormalFrame)
            ((NormalFrame)frame).ownConsole(this);

        console = new JPythonConsole();

        init();

        // Set the size of the window according to what is found in the
        // option set.
        NormalOptionSet options = shell.getOptions();
        int width = options.getIntOption("PreferredJythonWidth");
        int height = options.getIntOption("PreferredJythonHeight");
        if (width <= 0)
            width = 550;
        if (height <= 0)
            height = 450;
        setSize(new Dimension(width, height));
        setIconImage(Images.btnConsole.image().getImage());
    }

    /**
     * Starts the console contained in this frame with the given
     * greeting.  If the given greeting is <tt>null</tt>, no greeting
     * will be offered to the user.
     *
     * @param greeting the greeting to offer the user when the console
     * is started, or <tt>null</tt> if no greeting is to be offered.
     */
    public void startConsole(String greeting) {
        console.startConsole(greeting);
    }

    /**
     * Initialise the interface components.
     */
    private void init() {
        // Set up the console.
        console.setBorder(BorderFactory.createLoweredBevelBorder());

		// Set up the menus.
		JMenu menuConsole = new JMenu("Console");
		menuConsole.setMnemonic(KeyEvent.VK_C);
		menuConsoleBreak = new JMenuItem("Break Processing",
			Standard16.cross.image());
		menuConsoleBreak.setMnemonic(KeyEvent.VK_B);
		menuConsoleBreak.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_C, ActionEvent.CTRL_MASK));
		menuConsoleBreak.setEnabled(false);
		JMenuItem menuConsoleSave = new JMenuItem("Save Contents",
			Standard16.save.image());
		menuConsoleSave.setMnemonic(KeyEvent.VK_S);
		menuConsoleSave.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_S, ActionEvent.ALT_MASK));
		menuConsoleClose = new JMenuItem("Close",
			Standard16.close.image());
		menuConsoleClose.setMnemonic(KeyEvent.VK_C);
		menuConsoleClose.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_D, ActionEvent.CTRL_MASK));
		menuConsole.add(menuConsoleBreak);
		menuConsole.addSeparator();
		menuConsole.add(menuConsoleSave);
		menuConsole.add(menuConsoleClose);

		JMenu menuHelp = new JMenu("Help");
		menuHelp.setMnemonic(KeyEvent.VK_H);
		JMenuItem menuHelpJython = new JMenuItem("Jython Help",
			Standard16.help.image());
		menuHelpJython.setMnemonic(KeyEvent.VK_J);
		menuHelp.add(menuHelpJython);

		JMenuBar bar = new JMenuBar();
		bar.add(menuConsole);
		bar.add(new EditMenu(console));
		bar.add(menuHelp);
		setJMenuBar(bar);

		// Set up some standard actions.
		Action actionBreak = new AbstractAction() {
			public void actionPerformed(ActionEvent e) {
                   console.cancelProcessing();
			}
		};
		Action actionClose = new AbstractAction() {
			public void actionPerformed(ActionEvent e) {
                   closeConsole();
			}
		};

		// Add menu event listeners.
		menuConsoleBreak.addActionListener(actionBreak);
		menuConsoleSave.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				saveContents();
			}
		});
		menuConsoleClose.addActionListener(actionClose);
		menuHelpJython.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				shell.viewHelp("jython");
			}
		});

        // Put everything together.
        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(new JScrollPane(console),
            BorderLayout.CENTER);

        // Add key mappings.
		Keymap keymap = console.addKeymap("Jython Console",
			console.getKeymap());
		keymap.addActionForKeyStroke(KeyStroke.getKeyStroke(
			KeyEvent.VK_C, KeyEvent.CTRL_MASK), actionBreak);
		keymap.addActionForKeyStroke(KeyStroke.getKeyStroke(
			KeyEvent.VK_D, KeyEvent.CTRL_MASK), actionClose);
		console.setKeymap(keymap);

		console.addPropertyChangeListener("processing", this);
        addFocusListener(new FocusAdapter() {
            public void focusGained(FocusEvent e) {
                console.requestFocus();
            }
        });
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                closeConsole();
            }
        });
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
    }

	/**
	 * For internal use.
	 * Called when the console starts or stops processing.
	 *
	 * @param e the corresponding event.
	 */
	public void propertyChange(PropertyChangeEvent e) {
		boolean proc = console.isProcessing();
		menuConsoleBreak.setEnabled(proc);
		menuConsoleClose.setEnabled(! proc);
	}

    /**
     * Returns the actual Jython console used by this frame.
     *
     * @return the actual Jython console.
     */
    public JPythonConsole getConsole() {
        return console;
    }

    /**
     * Returns the Jython interpreter used by this console frame.
     * This can be used to set Jython variables and the like.
     *
     * @return the Jython interpreter used by this console frame.
     */
    public PythonInterpreter getPythonInterpreter() {
        return console.getPythonInterpreter();
    }

    /**
     * Closes the console window and exits the application if
     * appropriate.
     */
    public void closeConsole() {
		if (console.isProcessing()) {
			console.oops();
			return;
		}

        Frame frame = shell.getPrimaryFrame();
        if (frame != null && frame instanceof NormalFrame)
            ((NormalFrame)frame).disownConsole(this);

        // Save the preferred console window size.
        Dimension size = getSize();
        NormalOptionSet options = shell.getOptions();
        options.setIntOption("PreferredJythonWidth", size.width);
        options.setIntOption("PreferredJythonHeight", size.height);
        options.writeToFile();

        dispose();
        if (standalone)
            shell.exit(0);
    }

	/**
	 * Saves the contents of this console to a file.
	 */
	public void saveContents() {
		NormalOptionSet options = shell.getOptions();
		JFileChooser chooser = new JFileChooser();
		chooser.setCurrentDirectory(new File(
			options.getStringOption("LastDir", ".")));
		chooser.setDialogTitle("Save console contents...");
		if (chooser.showSaveDialog(this) != chooser.APPROVE_OPTION)
			return;

		// Attempt to save the console contents.
		File dest = chooser.getSelectedFile();
		try {
			console.writeContentsToFile(dest);
		} catch (IOException exc) {
			shell.error("An error occurred whilst attempting to write to [" +
				dest.getAbsolutePath() + "].");
			return;
		}

		// Update the system properties.
		String fileDir = dest.getParentFile().getAbsolutePath();
		if (fileDir == null)
			fileDir = ".";
		options.setStringOption("LastDir", fileDir);
		options.writeToFile();
	}
}

