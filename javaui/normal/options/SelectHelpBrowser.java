
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

package normal.options;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import normal.Shell;
import org.gjt.btools.gui.Positioner;
import org.gjt.btools.gui.component.PaddedPane;

/**
 * A modal dialog allowing the help browser to be selected by the user.
 * If OK is pressed, the working option set will be changed and written to
 * file.
 */
public class SelectHelpBrowser extends JDialog implements ActionListener,
        ItemListener {
    /**
     * Should JavaHelp be used?
     */
    private JCheckBox useJavaHelp;
    /**
     * Label for field <tt>commandField</tt>.
     */
    private JLabel commandLabel;
    /**
     * Command used for browsing help.
     */
    private JTextField commandField;
    /**
     * Label for field <tt>locationField</tt>.
     */
    private JLabel locationLabel;
    /**
     * Location of unpacked documentation.
     */
    private JTextField locationField;
    /**
     * OK button.
     */
    private JButton ok;
    /**
     * Offers an explanation of the various fields.
     */
    private JButton explanation;

    /**
     * The shell corresponding to the entire program.
     */
    private Shell shell;

    /**
     * Creates a new help browser selection dialog.
     *
     * @param shell the shell corresponding to the entire program.
     */
    public SelectHelpBrowser(Shell shell) {
        super(shell.getPrimaryFrame(), "Help Browser", true);
        this.shell = shell;
        init();
        pack();
        Positioner.centerOnScreen(this);
    }

    /**
     * Initialise the interface components.
     */
    private void init() {
        NormalOptionSet options = shell.getOptions();

        useJavaHelp = new JCheckBox("Use standard JavaHelp");
        commandLabel = new JLabel("Command for browsing help:");
        commandField = new JTextField(options.getStringOption("HelpBrowser",
            ""), 20);
        locationLabel = new JLabel("Location of unpacked documentation:");
        locationField = new JTextField(options.getStringOption("HelpDocs",
            ""), 20);
        ok = new JButton("OK");
        JButton cancel = new JButton("Cancel");
        explanation = new JButton("Explain fields...");

        // Create the custom browser panel.
        JPanel customPanel = new JPanel();
        customPanel.setLayout(new GridLayout(0, 1));
        customPanel.add(commandLabel);
        customPanel.add(commandField);
        customPanel.add(locationLabel);
        customPanel.add(locationField);
        
        // Create the button panel.
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout());
        buttonPanel.add(ok);
        buttonPanel.add(cancel);
        buttonPanel.add(explanation);
        
        // Insert components.
        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(new PaddedPane(useJavaHelp, 5),
            BorderLayout.NORTH);
        getContentPane().add(new PaddedPane(customPanel, 5),
            BorderLayout.CENTER);
        getContentPane().add(buttonPanel, BorderLayout.SOUTH);
    
        // Add event listeners.    
        ok.addActionListener(this);
        cancel.addActionListener(this);
        explanation.addActionListener(this);
        useJavaHelp.addItemListener(this);
        getRootPane().setDefaultButton(ok);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);

        // Set the initial enabled states of components.
        useJavaHelp.setSelected(options.getBooleanOption("HelpJavaHelp",
            true));
    }

    /**
     * Called when a button has been pressed.
     *
     * @param e the button pressing event.
     */
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == explanation) {
            shell.inform(
                "If JavaHelp is selected, the internal JavaHelp system " +
                "will be used to browse help from the documentation jar.\n\n" +
                "Otherwise help will be browsed using <command> <location>, " +
                "as specified in the fields in the dialog.  Note that this " +
                "means the documentation may be online.  Otherwise it will " +
                "probably need to be unpacked somewhere on your system.\n\n" +
                "Documentation can be viewed by any web browser.  " +
                "The location entered should be the directory containing " +
                "the HTML files such as index.html and welcome.html.  " +
                "For instance, if the documentation index is " +
                "/usr/share/regina/docs/index.html, the " +
                "location entered should be /usr/share/regina/docs.");
        } else if (e.getSource() == ok) {
            if (useJavaHelp.isSelected()) {
                if (! shell.hasFoundJavaHelp())
                    if (! shell.confirm(
                            "JavaHelp cannot be found on this system.  " +
                            "Are you sure you wish to continue with these " +
                            "settings?"))
                        return;
                if (! shell.hasFoundDocumentation())
                    if (! shell.confirm(
                            "The documentation jar cannot be found on this " +
                            "system.  Are you sure you wish to continue " +
                            "with these settings?"))
                        return;
            } else {
                if (commandField.getText().trim().length() == 0) {
                    shell.error("An external help browser has " +
                        "been requested but the command has not been " +
                        "specified.");
                    return;
                }
                if (locationField.getText().trim().length() == 0) {
                    shell.error("An external help browser has " +
                        "been requested but the documentation location " +
                        "has not been specified.");
                    return;
                }
            }

            NormalOptionSet options = shell.getOptions();
            options.setBooleanOption("HelpJavaHelp",
                useJavaHelp.isSelected());
            options.setStringOption("HelpBrowser",
                commandField.getText().trim());
            options.setStringOption("HelpDocs",
                locationField.getText().trim());
            options.writeToFile();

            dispose();
        } else
            dispose();
    }

    /**
     * Called when the checkbox state has been changed.
     *
     * @param e the state changing event.
     */
    public void itemStateChanged(ItemEvent e) {
        boolean custom = (e.getStateChange() == e.DESELECTED);
        commandLabel.setEnabled(custom);
        commandField.setEnabled(custom);
        locationLabel.setEnabled(custom);
        locationField.setEnabled(custom);
        explanation.setEnabled(custom);
    }
}

