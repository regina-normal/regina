
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
import javax.swing.*;
import javax.swing.border.*;
import normal.*;
import btools.gui.component.*;
import btools.license.License;

/**
 * Provides an About box.
 */
public class AboutBox extends JDialog implements ActionListener {
    /**
     * The maximum length of a line in the about box.
     */
    public static final int maxLineLength = 50;

    /**
     * OK button.
     */    
    private JButton ok = new JButton();
    
    /**
     * JPython version.
     */
    private String jpythonVersion;
    
    /**
     * Label displaying the program icon.
     */
    private JLabel iconLabel = new JLabel();

	/**
	 * The shell representing the entire program.
	 */
	private Shell shell;

    /**
     * Create a new About box.
     *
     * @param shell the shell representing the entire program.
     */
    public AboutBox(Shell shell) {
        super(shell.getPrimaryFrame(), "About", true);
		this.shell = shell;
        diagnostics();
        init();
        pack();
    }

    /**
     * Find useful information about the system to display to the user.
     */
    private void diagnostics() {
        // Determine the JPython version.
        if (! shell.hasFoundJPython())
            jpythonVersion = "unavailable";
        else {
            jpythonVersion = "unknown";
            try {
                jpythonVersion = org.python.core.PySystemState.version;
            } catch (Throwable th) {}
        }
    }

    /**
     * Initialise the interface components.
     */
    private void init() {
        // Create panels.
        JPanel mainPanel = new JPanel();
        JPanel buttonPanel = new JPanel();
        
        // Set layout managers.
        getContentPane().setLayout(new BorderLayout());
        mainPanel.setLayout(new BorderLayout());
        buttonPanel.setLayout(new FlowLayout());
        
        // Create the information panel.
        JTabbedPane infoPane = new JTabbedPane();
        infoPane.setBorder(new EmptyBorder(5, 0, 5, 20));
        Box titlePane = new Box(BoxLayout.Y_AXIS);
        titlePane.add(titleRegion());
        titlePane.add(Box.createVerticalStrut(10));
        titlePane.add(authorRegion());
        titlePane.add(Box.createVerticalStrut(10));
        titlePane.add(thanksRegion());
        titlePane.add(Box.createVerticalStrut(10));
        titlePane.add(contactRegion());
        // Put in some extra space to ensure everything fits even if the
        // tabs are squished above each other.
        titlePane.add(Box.createVerticalStrut(30));
        titlePane.add(Box.createVerticalGlue());
        infoPane.add("General", new PaddedPane(titlePane, 5));
        infoPane.add("Acknowledgements", new PaddedPane(peopleRegion(), 5));
        infoPane.add("Components", new PaddedPane(infoRegion(), 5));
        JPanel licensePane = new JPanel();
        licensePane.setLayout(new BorderLayout());
        JComponent licenseNotes = licenseNotesRegion();
        licensePane.add(new PaddedPane(licenseNotes, 0, 0, 5, 0),
            BorderLayout.NORTH);
        licensePane.add(licenseDetailsRegion(), BorderLayout.CENTER);
        infoPane.add("License", new PaddedPane(licensePane, 5));

        // Create the main panel.
        iconLabel.setIcon(Images.mainLargeIcon.image());
        iconLabel.setBorder(BorderFactory.createLoweredBevelBorder());
        mainPanel.add(new PaddedPane(iconLabel, 10, false), BorderLayout.WEST);
        mainPanel.add(infoPane, BorderLayout.CENTER);
        
        // Create the button panel.
        ok.setText("OK");
        buttonPanel.add(ok, null);
        
        // Insert components.
        getContentPane().add(mainPanel, BorderLayout.CENTER);
        getContentPane().add(buttonPanel, BorderLayout.SOUTH);
    
        // Add event listeners.    
        ok.addActionListener(this);
        getRootPane().setDefaultButton(ok);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
    }

    /**
     * For internal use only.
     * Called when a button has been pressed.
     */
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == ok) {
            dispose();
        }
    }

    /**
     * Returns a newly created and filled in text region for one of the
     * specific displayed topics.
     */
    private TextDisplayRegion titleRegion() {
        TextDisplayRegion ans = new TextDisplayRegion();
        ans.insertLine(Application.program + ' ' + Application.version
			+ " (" + Application.date + ')');
        ans.insertMessage(Application.comments, maxLineLength);
        ans.insertMessage(Application.copyright, maxLineLength);
        ans.setMaximumSize(ans.getPreferredSize());
        ans.setAlignmentX(LEFT_ALIGNMENT);
        return ans;
    }

    /**
     * Returns a newly created and filled in text region for one of the
     * specific displayed topics.
     */
    private TextDisplayRegion authorRegion() {
        TextDisplayRegion ans = new TextDisplayRegion();
        ans.insertMessage(Application.author, maxLineLength);
        ans.setMaximumSize(ans.getPreferredSize());
        ans.setAlignmentX(LEFT_ALIGNMENT);
        return ans;
    }

    /**
     * Returns a newly created and filled in text region for one of the
     * specific displayed topics.
     */
    private TextDisplayRegion thanksRegion() {
        TextDisplayRegion ans = new TextDisplayRegion();
        ans.insertMessage(Application.majorThanks, maxLineLength);
        ans.setMaximumSize(ans.getPreferredSize());
        ans.setAlignmentX(LEFT_ALIGNMENT);
        return ans;
    }

    /**
     * Returns a newly created and filled in text region for one of the
     * specific displayed topics.
     */
    private TextDisplayRegion contactRegion() {
        TextDisplayRegion ans = new TextDisplayRegion();
        ans.insertMessage(Application.contact, maxLineLength);
        ans.setMaximumSize(ans.getPreferredSize());
        ans.setAlignmentX(LEFT_ALIGNMENT);
        return ans;
    }

    /**
     * Returns a newly created and filled in text region for one of the
     * specific displayed topics.
     */
    private TextDisplayRegion peopleRegion() {
        TextDisplayRegion ans = new TextDisplayRegion();
        ans.insertMessage(Application.thanks, maxLineLength);
        return ans;
    }

    /**
     * Returns a newly created and filled in text region for one of the
     * specific displayed topics.
     */
    private TextDisplayRegion infoRegion() {
        TextDisplayRegion ans = new TextDisplayRegion();

		ans.insertLine("Engine " + shell.getEngine().getVersionString());
		ans.insertLine("- " + shell.getEngine().styleDescription());
		String jdkVersion = btools.Version.javaVersion();
		if (jdkVersion == null)
			ans.insertLine("JDK unknown");
		else
        	ans.insertLine("JDK " + jdkVersion);
        ans.insertLine("BTools " + btools.Version.btoolsVersion);
        ans.insertLine("JPython " + jpythonVersion);
        if (shell.hasFoundJavaHelp()) {
            ans.insertLine("JavaHelp present");
            ans.insertLine("Documentation jar " +
                (shell.hasFoundDocumentation() ? "present" : "unavailable"));
        } else
            ans.insertLine("JavaHelp unavailable");

        return ans;
    }

    /**
     * Returns a newly created and filled in text region for one of the
     * specific displayed topics.
     */
    private TextDisplayRegion licenseNotesRegion() {
        TextDisplayRegion ans = new TextDisplayRegion();
        ans.insertMessage(Application.licenseNotes, maxLineLength);
        return ans;
    }

    /**
     * Returns a newly created and filled in component containing the
     * full license details.
     */
    private JComponent licenseDetailsRegion() {
        JScrollPane ans = new JScrollPane(License.getViewer(License.GPL));
        ans.setPreferredSize(new Dimension(200, 100));
        return ans;
    }
}
