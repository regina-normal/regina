
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
import javax.swing.*;
import org.python.util.PythonInterpreter;

/**
 * Provides a frame containing a JPython console and associated tools.
 */
public class JPythonConsoleFrame extends JFrame {
    /**
     * Is this window the entire program?
     */
    private boolean standalone;

    /**
     * The shell representing the entire program.
     */
    private Shell shell;

    /**
     * The actual JPython console contained in this window.
     */
    private JPythonConsole console;

    /**
     * Creates a new JPython console frame.  Note that the console must
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
        super(Application.program + " JPython Console");
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
        int width = options.getIntOption("PreferredJPythonWidth");
        int height = options.getIntOption("PreferredJPythonHeight");
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

        // Set up the buttons.
        JButton help = new JButton("Help");
        JButton close = new JButton("Close");
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout());
        buttonPanel.add(close);
        buttonPanel.add(help);

        // Put everything together.
        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(new JScrollPane(console),
            BorderLayout.CENTER);
        getContentPane().add(buttonPanel, BorderLayout.SOUTH);

        // Add appropriate event listeners.
        help.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                shell.viewHelp("jpython");
            }
        });
        close.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                closeConsole();
            }
        });
        // Remove the default key mapping for Ctrl-D so we can
        // make it close the window instead.
        console.getKeymap().removeKeyStrokeBinding(
            KeyStroke.getKeyStroke(KeyEvent.VK_D, KeyEvent.CTRL_MASK));
        addKeyListener(new KeyAdapter() {
            // Close on Ctrl-D, just as in a plain text console.
            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode() == e.VK_D &&
                        e.getModifiers() == e.CTRL_MASK)
                    closeConsole();
            }
        });
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
     * Returns the actual JPython console used by this frame.
     *
     * @return the actual JPython console.
     */
    public JPythonConsole getConsole() {
        return console;
    }

    /**
     * Returns the JPython interpreter used by this console frame.
     * This can be used to set JPython variables and the like.
     *
     * @return the JPython interpreter used by this console frame.
     */
    public PythonInterpreter getPythonInterpreter() {
        return console.getPythonInterpreter();
    }

    /**
     * Closes the console window and exits the application if
     * appropriate.
     */
    public void closeConsole() {
        Frame frame = shell.getPrimaryFrame();
        if (frame != null && frame instanceof NormalFrame)
            ((NormalFrame)frame).disownConsole(this);

        // Save the preferred console window size.
        Dimension size = getSize();
        NormalOptionSet options = shell.getOptions();
        options.setIntOption("PreferredJPythonWidth", size.width);
        options.setIntOption("PreferredJPythonHeight", size.height);
        options.writeToFile();

        dispose();
        if (standalone)
            shell.exit(0);
    }
}

