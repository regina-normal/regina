
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

package normal.mainui;

import java.awt.*;
import java.awt.event.*;
import java.util.Enumeration;
import javax.swing.*;
import javax.swing.border.*;
import normal.Images;
import normal.engine.packet.*;
import normal.packetui.*;
import org.gjt.btools.gui.dialog.*;
import org.gjt.btools.image.*;

/**
 * A pane in which the user can interact with a packet.
 * This will generally contain a <tt>PacketUI</tt> which will be
 * responsible for the actual interaction; this can be accessed
 * through the method <tt>getUI()</tt>.
 * This pane will also display packet identification details and give
 * the user some buttons for standard actions.
 * <p>
 * When a <tt>PacketPane</tt> acquires, loses or changes its own personal
 * frame due to an external force, the <tt>setSurroundingFrame()</tt> method
 * should be called accordingly to let the pane know about its new environment.
 *
 * @see normal.packetui.PacketUI
 */
public class PacketPane extends JPanel implements UIHasChangesListener {
    /**
     * The <tt>TopologyPane</tt> that spawned this pane.
     * @serial
     */
    private TopologyPane topologyPane;

    /**
     * The personal frame in which this pane lives.
     * @serial
     */
    private JFrame surroundingFrame = null;
        
    /**
     * The interface responsible for the actual interaction.
     * @serial
     */
    private PacketUI ui;
    
    /**
     * Label displaying packet identification details.
     * @serial
     */
    protected JLabel packetLabel = new JLabel();

    /**
     * Button with which the user can dock or undock the pane.
     * @serial
     */
    protected JButton dockButton = new JButton();

    /**
     * Icon signifying whether or not there are unapplied changes.
     */
    private JLabel dirtyFlag;

    /**
     * Button with which the user can apply changes to the engine.
     */
    protected JButton applyButton;

    /**
     * Create a new pane.
     *
     * @param topologyPane the <tt>TopologyPane</tt> that spawned this pane.
     * @param ui the interface the user will use.
     */
    public PacketPane(TopologyPane topologyPane, PacketUI ui) {
        this.ui = ui;
        this.topologyPane = topologyPane;
        init();
    }

    /**
     * Perform all initialisation and insertion of components of this
     * packet pane.
     *
     * @see normal.mainui.PacketPane
     */
    private void init() {
        // Set up the components.
        dirtyFlag = new JLabel() {
            public Dimension getMinimumSize() {
                return dockButton.getMinimumSize();
            }
            public Dimension getPreferredSize() {
                return dockButton.getPreferredSize();
            }
        };

        packetLabel.setHorizontalAlignment(JLabel.CENTER);
        packetLabel.setIcon(PacketUIManager.packetIcon(ui.getPacket()));
        packetLabel.setBorder(BorderFactory.createEtchedBorder());
        dockButton.setBorder(BorderFactory.createEtchedBorder());
        if (ui.isEditor()) {
            dirtyFlag.setBorder(BorderFactory.createEtchedBorder());
            dirtyFlag.setHorizontalAlignment(SwingConstants.CENTER);
            dirtyFlag.setToolTipText("No unapplied changes");
        }

        refreshDockButton();
        refreshLabel();

        JButton closeButton = new JButton("Close");
        JButton refreshButton = new JButton("Refresh");
        JButton tagsButton = new JButton("Tags...");
        applyButton = new JButton("Apply");

        applyButton.setEnabled(false);

        // Set up the action panel.
        Box actionPanel = new Box(BoxLayout.X_AXIS);
        actionPanel.add(Box.createRigidArea(tagsButton.getPreferredSize()));
        actionPanel.add(Box.createHorizontalGlue());
        actionPanel.add(refreshButton);
        if (ui.isEditor())
            actionPanel.add(applyButton);
        actionPanel.add(closeButton);
        actionPanel.add(Box.createHorizontalGlue());
        actionPanel.add(tagsButton);
         
        // Prepare the title pane.
        JPanel titlePane = new JPanel();
        titlePane.setLayout(new BorderLayout());
        titlePane.add(dockButton, BorderLayout.EAST);
        titlePane.add(packetLabel, BorderLayout.CENTER);
        if (ui.isEditor())
            titlePane.add(dirtyFlag, BorderLayout.WEST);
        
        // Insert all the components.
        setLayout(new BorderLayout());
        add(titlePane, BorderLayout.NORTH);
        add(ui.getInterface(), BorderLayout.CENTER);
        add(actionPanel, BorderLayout.SOUTH);
    
        // Set up event listeners.
        ui.addUIHasChangesListener(this);

        dockButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                dockUndock();
            }
        });
        closeButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                close();
            }
        });
        refreshButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                ui.packetWasChanged(ui.getPacket(), null,
                    surroundingFrame);
            }
        });
        tagsButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                new TagEditor(topologyPane, ui.getPacket()).show();
            }
        });
        applyButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (! ui.getPacket().isPacketEditable()) {
                    MessageBox.fgNote(getParentFrame(),
                        "One of its children will not allow this " +
                            "packet to change.  Try cloning the packet " +
                            "(without descendants) and modifying the clone " +
                            "instead.");
                    return;
                }
                ui.applyChanges(topologyPane.getEngine());
                topologyPane.setDirty(true);
                topologyPane.firePacketWasChanged(ui.getPacket(), ui);
            }
        });
    }
    
    /**
     * Set the picture on the dock/undock button as appropriate.
     */
    protected void refreshDockButton() {
        if (topologyPane.isFloating(this)) {
            dockButton.setIcon(Images.btnDockUndocked.image());
            dockButton.setPressedIcon(Images.btnDockDocked.image());
            dockButton.setToolTipText("Dock");
        }
        else {
            dockButton.setIcon(Images.btnDockDocked.image());
            dockButton.setPressedIcon(Images.btnDockUndocked.image());
            dockButton.setToolTipText("Undock");
        }
    }
    
    /**
     * Set the text in the packet identification label.
     */
    public void refreshLabel() {
        // Does the packet have any tags?
        String tagString;
        NPacket packet = ui.getPacket();
        if (packet.hasTags()) {
            Enumeration e = packet.getTags();
            tagString = " : " + e.nextElement();
            if (e.hasMoreElements())
                tagString = tagString + ", ...";
        } else
            tagString = new String();

        packetLabel.setText(packet.getFullName() + tagString);
    }

    /**
     * Return the <tt>PacketUI</tt> interface contained in this
     * packet pane.
     *
     * @return the associated <tt>PacketUI</tt> interface.
     */
    public PacketUI getUI() {
        return ui;
    }

    /**
     * Return the system pane that spawned this packet pane.
     *
     * @return the system pane that spawned this packet pane.
     */
    public TopologyPane getTopologyPane() {
        return topologyPane;
    }

    /**
     * Remove this packet pane from the user interface due to such a request
     * from the user.
     */
    protected void close() {
        topologyPane.removePacketPane(this, false);
    }
    
    /**
     * Dock or undock this packet pane as appropriate.
     */
    private void dockUndock() {
        if (topologyPane.isDocked(this))
            topologyPane.floatPacketPane(this);
        else
            topologyPane.dockPacketPane(this, TopologyPane.PANE_ASK_FLOAT);
        refreshDockButton();
    }

    /**
     * Return the packet with which this pane is working.
     */
    public NPacket getPacket() {
        return ui.getPacket();
    }

    /**
     * Returns the frame in which this pane lives, regardless of whether
     * it has a personal devoted frame.
     *
     * @return the frame in which this pane lives; this will never be
     * <tt>null</tt>.
     */
    public Frame getParentFrame() {
        if (surroundingFrame == null)
            return topologyPane.getNormalFrame();
        else
            return surroundingFrame;
    }

    /**
     * Returns the personal devoted frame in which this pane lives.
     * Such a frame should be devoted to the sole purpose of
     * holding this pane.
     *
     * @return the surrounding frame, or <tt>null</tt> if there is
     * no such devoted frame.
     */
    public JFrame getSurroundingFrame() {
        return surroundingFrame;
    }
    
    /**
     * Notifies that pane that its personal devoted surrounding frame has
     * changed.  Such a frame should be devoted to the sole purpose
     * of holding this frame.
     *
     * @param frame the new surrounding frame, or <tt>null</tt> if
     * this pane no longer has a personal devoted frame.
     */
    public void setSurroundingFrame(JFrame frame) {
        surroundingFrame = frame;
    }
        
    /**
     * Would this pane mind if it were closed down?
     *
     * @return <tt>true</tt> if and only if it is happy to be closed.
     */
    public boolean canClose() {
        if (ui.hasChanges())
            return MessageBox.confirm(getParentFrame(),
                "Are you sure you wish to abandon any changes made to " +
                    "this packet?");
        return true;
    }
    
    /**
     * Called when the pane has been closed and is about to be destroyed.
     */
    public void cleanUp() {
        ui.cleanUp();
    }

    public void UIStatusChanged(UIHasChangesEvent e) {
        boolean hasChanges = e.hasChanges();
        applyButton.setEnabled(hasChanges);
        dirtyFlag.setIcon(hasChanges ? Standard16.save.image() : null);
        dirtyFlag.setToolTipText(hasChanges ? "There are unapplied changes" :
            "No unapplied changes");
    }
}

