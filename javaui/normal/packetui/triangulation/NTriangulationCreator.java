
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
import javax.swing.text.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.engine.split.NSignature;
import normal.engine.triangulation.*;
import normal.packetfilter.*;
import normal.packetui.*;
import org.gjt.btools.ext.*;
import org.gjt.btools.gui.dialog.*;

/**
 * An interface through which a user can create a new triangulation.
 * See <tt>PacketCreator</tt> for more details.
 *
 * @see normal.packetui.PacketCreator
 */
public class NTriangulationCreator extends JPanel implements PacketCreator {
    /**
     * Button representing an empty triangulation.
     */
    private JRadioButton empty;
    
    /**
     * Button representing a layered triangulation.
     */
    private JRadioButton layered;
    
    /**
     * Button representing a lens space.
     */
    private JRadioButton lensSpace;

    /**
     * Button representing a dehydrated triangulation.
     */
    private JRadioButton dehydrated;
    
    /**
     * Button representing the enclosing triangulation of a splitting
     * surface.
     */
    private JRadioButton splittingSurface;

    /**
     * Button representing a random triangulation.
     */
    private JRadioButton random;
    
    /**
     * Parameter for a layered triangulation.
     */
    private JTextField layeredA;
    
    /**
     * Parameter for a layered triangulation.
     */
    private JTextField layeredB;
    
    /**
     * Parameter for a layered triangulation.
     */
    private JTextField layeredC;
    
    /**
     * Parameter for a lens space.
     */
    private JTextField lensP;
    
    /**
     * Parameter for a lens space.
     */
    private JTextField lensQ;

    /**
     * Dehydrated triangulation string.
     */
    private JTextField dehydration;

    /**
     * Splitting surface signature.
     */
    private JTextField signature;

    /**
     * Number of tetrahedra for a random triangulation.
     */
    private JTextField randomTetrahedra;

    /**
     * Create the new interface.
     */
    public NTriangulationCreator() {
        init();
    }

    /**
     * Create the interface's internal components.
     */
    private void init() {
        // Set the layout manager.
        setLayout(new GridBagLayout());

        // Initialise the components.
        ButtonGroup type = new ButtonGroup();
        empty = new JRadioButton("Empty", true);
        layered = new JRadioButton("Layered solid torus (a,b,c):");
        lensSpace = new JRadioButton("Lens space L(p,q):");
        dehydrated = new JRadioButton("Dehydration:");
        splittingSurface = new JRadioButton("Splitting surface:");
        random = new JRadioButton("Random:");
        type.add(empty);
        type.add(layered);
        type.add(lensSpace);
        type.add(dehydrated);
        type.add(splittingSurface);
        type.add(random);
        random.setEnabled(false);
        
        JPanel layeredPane = new JPanel();
        layeredPane.setLayout(new FlowLayout(FlowLayout.LEFT, 5, 0));
        layeredA = new JTextField(new NonNegativeIntegerDocument(), "", 3);
        layeredB = new JTextField(new NonNegativeIntegerDocument(), "", 3);
        layeredC = new JTextField(new NonNegativeIntegerDocument(), "", 3);
        layeredPane.add(new JLabel("a ="));
        layeredPane.add(layeredA);
        layeredPane.add(new JLabel("b ="));
        layeredPane.add(layeredB);
        layeredPane.add(new JLabel("c ="));
        layeredPane.add(layeredC);
        layeredA.setEnabled(false);
        layeredB.setEnabled(false);
        layeredC.setEnabled(false);
        
        JPanel lensPane = new JPanel();
        lensPane.setLayout(new FlowLayout(FlowLayout.LEFT, 5, 0));
        lensP = new JTextField(new NonNegativeIntegerDocument(), "", 3);
        lensQ = new JTextField(new NonNegativeIntegerDocument(), "", 3);
        lensPane.add(new JLabel("p ="));
        lensPane.add(lensP);
        lensPane.add(new JLabel("q ="));
        lensPane.add(lensQ);
        lensP.setEnabled(false);
        lensQ.setEnabled(false);

        JPanel dehydrationPane = new JPanel();
        dehydrationPane.setLayout(new FlowLayout(FlowLayout.LEFT, 5, 0));
        dehydration = new JTextField(new DehydrationDocument(), "", 12);
        dehydrationPane.add(new JLabel("String:"));
        dehydrationPane.add(dehydration);
        dehydration.setEnabled(false);

        JPanel splittingSurfacePane = new JPanel();
        splittingSurfacePane.setLayout(new FlowLayout(FlowLayout.LEFT, 5, 0));
        signature = new JTextField(12);
        splittingSurfacePane.add(new JLabel("Signature:"));
        splittingSurfacePane.add(signature);
        signature.setEnabled(false);

        JPanel randomPane = new JPanel();
        randomPane.setLayout(new FlowLayout(FlowLayout.LEFT, 5, 0));
        randomTetrahedra = new JTextField(new NonNegativeIntegerDocument(),
            "", 3);
        randomPane.add(new JLabel("Tetrahedra:"));
        randomPane.add(randomTetrahedra);
        randomTetrahedra.setEnabled(false);

        // Insert the components.
        GridBagConstraints cButton = new GridBagConstraints();
        GridBagConstraints cExtra = new GridBagConstraints();
        cButton.anchor = cButton.WEST;
        cButton.fill = cButton.NONE;
        cButton.gridx = 0;
        cExtra.anchor = cExtra.WEST;
        cExtra.fill = cExtra.NONE;
        cExtra.gridx = 1;
        add(empty, cButton);
        add(layered, cButton);
        cExtra.gridy = 1;
        add(layeredPane, cExtra);
        add(lensSpace, cButton);
        cExtra.gridy = 2;
        add(lensPane, cExtra);
        add(dehydrated, cButton);
        cExtra.gridy = 3;
        add(dehydrationPane, cExtra);
        add(splittingSurface, cButton);
        cExtra.gridy = 4;
        add(splittingSurfacePane, cExtra);
        add(random, cButton);
        cExtra.gridy = 5;
        add(randomPane, cExtra);
        
        // Add selection listeners.
        layered.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                boolean on = (e.getStateChange() == ItemEvent.SELECTED);
                layeredA.setEnabled(on);
                layeredB.setEnabled(on);
            }
        });
        lensSpace.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                boolean on = (e.getStateChange() == ItemEvent.SELECTED);
                lensP.setEnabled(on);
                lensQ.setEnabled(on);
            }
        });
        dehydrated.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                boolean on = (e.getStateChange() == ItemEvent.SELECTED);
                dehydration.setEnabled(on);
            }
        });
        splittingSurface.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                boolean on = (e.getStateChange() == ItemEvent.SELECTED);
                signature.setEnabled(on);
            }
        });
        random.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                boolean on = (e.getStateChange() == ItemEvent.SELECTED);
                randomTetrahedra.setEnabled(on);
            }
        });
        DocumentListener layeredListener = new DocumentListener() {
            public void insertUpdate(DocumentEvent e) {
                updateLayeredC();
            }
            public void removeUpdate(DocumentEvent e) {
                updateLayeredC();
            }
            public void changedUpdate(DocumentEvent e) {
                updateLayeredC();
            }
        };
        layeredA.getDocument().addDocumentListener(layeredListener);
        layeredB.getDocument().addDocumentListener(layeredListener);
    }

    /**
     * Updates the value of <tt>layeredC</tt> according to the values of
     * <tt>layeredA</tt> and <tt>layeredB</tt>.
     */
    private void updateLayeredC() {
        long a = ((NonNegativeIntegerDocument)layeredA.getDocument()).
            getLongValue();
        long b = ((NonNegativeIntegerDocument)layeredB.getDocument()).
            getLongValue();
        if (a >= 0 && b >= 0)
            layeredC.setText(String.valueOf(a+b));
        else
            layeredC.setText("");
    }

    public NPacket makeNewPacket(Shell shell, NPacket parent,
            Dialog parentDialog) {
        if (layered.isSelected()) {
            long a = ((NonNegativeIntegerDocument)layeredA.getDocument()).
                getLongValue();
            long b = ((NonNegativeIntegerDocument)layeredB.getDocument()).
                getLongValue();
            if (a < 0 || b < 0) {
                MessageBox.fgNote(parentDialog,
                    "Both a and b must be non-negative integers.");
                return null;
            }
            if (a > b) {
                long tmp = a;
                a = b;
                b = tmp;
            }
            if (b == 0) {
                MessageBox.fgNote(parentDialog,
                    "At least one of a and b must be positive.");
                return null;
            }
            if (gcd(a,b) > 1) {
                MessageBox.fgNote(parentDialog, "gcd(a,b) must be 1.");
                return null;
            }
            NTriangulation newTri = shell.getEngine().newNTriangulation();
            newTri.insertLayeredSolidTorus(a, b);
            return newTri;
        } else if (lensSpace.isSelected()) {
            long p = ((NonNegativeIntegerDocument)lensP.getDocument()).
                getLongValue();
            long q = ((NonNegativeIntegerDocument)lensQ.getDocument()).
                getLongValue();
            if (p < 0 || q < 0) {
                MessageBox.fgNote(parentDialog,
                    "Both p and q must be non-negative integers.");
                return null;
            }
            if (q >= p)
                if (! (p == 0 && q == 1)) {
                    MessageBox.fgNote(parentDialog,
                        "Unless this is L(0,1), you must have p > q.");
                    return null;
                }
            if (gcd(p,q) != 1) {
                MessageBox.fgNote(parentDialog, "gcd(p,q) must be 1.");
                return null;
            }
            NTriangulation newTri = shell.getEngine().newNTriangulation();
            newTri.insertLensSpace(p, q);
            return newTri;
        } else if (dehydrated.isSelected()) {
            if (dehydration.getDocument().getLength() == 0) {
                MessageBox.fgNote(parentDialog,
                    "The dehydration string cannot be empty.");
                return null;
            }
            NTriangulation newTri = shell.getEngine().newNTriangulation();
            if (! newTri.insertRehydration(dehydration.getText())) {
                MessageBox.fgNote(parentDialog,
                    "The given string does not represent a dehydrated " +
                    "triangulation.");
                newTri.destroy();
                return null;
            }
            return newTri;
        } else if (splittingSurface.isSelected()) {
            if (signature.getDocument().getLength() == 0) {
                MessageBox.fgNote(parentDialog,
                    "The splitting surface signature cannot be empty.");
                return null;
            }
            NSignature sig = shell.getEngine().parseSignature(
                signature.getText());
            if (sig == null) {
                MessageBox.fgNote(parentDialog,
                    "The given splitting surface signature is not valid.");
                return null;
            }
            NTriangulation newTri = sig.triangulate();
            sig.destroy();
            return newTri;
        } else if (random.isSelected()) {
            return null;
        } else
            return shell.getEngine().newNTriangulation();
    }

    public Component getCreator() {
        return this;
    }

    public String getDialogTitle() {
        return "New Triangulation";
    }

    public String getSuggestedPacketLabel() {
        return "New Triangulation";
    }

    public PacketFilter getPacketFilter() {
        return AllowAll.instance;
    }

    /**
     * Finds the greatest common divisor of two integers.
     * <p>
     * <b>Precondition:</b> Both integers are non-negative.
     *
     * @param a one of the integers to examine.
     * @param the other integer to examine.
     * @return the non-negative greatest common divisor of the two
     * given integers.
     */
    private static long gcd(long a, long b) {
        long tmp;
        while (a != b && b != 0) {
            tmp = a;
            a = b;
            b = tmp % b;
        }
        return a;
    }

    /**
     * Document class used to enter a dehydration string into a text
     * component.
     * Ensures the input contains only alphabetical characters and
     * converts them to lower case on the fly.
     */
    private class DehydrationDocument extends PlainDocument {
        /**
         * Attempts to insert a string into the document.
         *
         * @param offs the position at which to insert the string.
         * @param str the string to insert.
         * @param a the attributes to give the string.
         * @throws BadLocationException thrown if the string may not be
         * inserted at the requested position.
         */
        public void insertString(int offs, String str, AttributeSet a)
                throws BadLocationException {
            StringBuffer working = new StringBuffer(str);
            for (int i = 0; i < working.length(); i++) {
                if (Character.isUpperCase(working.charAt(i)))
                    working.setCharAt(i, Character.toLowerCase(
                        working.charAt(i)));
                else if (! Character.isLowerCase(working.charAt(i))) {
                    Toolkit.getDefaultToolkit().beep();
                    return;
                }
            }
            super.insertString(offs, working.toString(), a);
        }
    }
}
