
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

package normal.progress;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import normal.engine.progress.*;
import btools.gui.*;

/**
 * A dialog that displays the current state of progress of a long
 * operation.
 * <p>
 * This dialog is modal.  It queries a given progress manager for the
 * current state of progress and displays this information to the user,
 * updating it on a regular basis.  Once the
 * operation has finished, the dialog automatically closes itself and
 * returns control of the program to the user.
 * <p>
 * The facility also exists for the user to cancel the
 * operation, but this feature has not yet been implemented in the
 * calculation engine.
 * <p>
 * To use a progress dialog, a new dialog should be created and
 * then <tt>runLifeCycle()</tt> should be called.  This routine is all you
 * need to call; it will not return until the operation has either finished
 * or been cancelled and everything has been cleaned up.
 *
 * @see normal.engine.progress.NProgressManager
 * @see #runLifeCycle
 */
public class ProgressDialog extends JDialog implements Runnable {
    /**
     * The number of milliseconds between successive queries as to
     * whether progress reporting has begun.
     */
    public static final long initialPollPeriod = 100;
    /**
     * The number of milliseconds between successive queries upon the
     * current state of progress.
     */
    public static final long pollPeriod = 1000;

    /**
     * The progress reporter through which progress is queried.
     */
    private NProgress progress;
    /**
     * The progress manager through which progress is queried.
     */
    private NProgressManager manager;
    /**
     * The component used to report progress details.
     */
    private ProgressComponent progressComponent;

    /**
     * Allows the underlying operation to be cancelled.
     */
    private JButton cancel = null;

    /**
     * Create a new dialog and fully prepare all of its interface
     * elements.
     *
     * @param parentFrame the frame to use as a parent for this dialog.
     * @param operation a description of the operation whose progress we
     * are reporting.
     * @param manager the progress manager through which we are
     * obtaining progress reports.
     */
    public ProgressDialog(Frame parentFrame, String operation,
            NProgressManager manager) {
        super(parentFrame, "Progress...", true);
        this.manager = manager;
        
        // Wait for progress reporting to start.
        while (! manager.isStarted()) {
            try {
                Thread.sleep(initialPollPeriod);
            } catch (InterruptedException e) {}
        }
        progress = manager.getProgress();

        init(operation);
        pack();
        Positioner.centerOnScreen(this);
    }
    /**
     * Create a new dialog and fully prepare all of its interface
     * elements.
     *
     * @param parentDialog the dialog to use as a parent for this dialog.
     * @param operation a description of the operation whose progress we
     * are reporting.
     * @param manager the progress manager through which we are
     * obtaining progress reports.
     */
    public ProgressDialog(Dialog parentDialog, String operation,
            NProgressManager manager) {
        super(parentDialog, "Progress...", true);
        this.manager = manager;
        
        // Wait for progress reporting to start.
        while (! manager.isStarted()) {
            try {
                Thread.sleep(initialPollPeriod);
            } catch (InterruptedException e) {}
        }
        progress = manager.getProgress();

        init(operation);
        pack();
        Positioner.centerOnScreen(this);
    }

    /**
     * Create the dialog's interface elements.
     *
     * @param operation a description of the operation whose progress we are
     * reporting.
     */
    private void init(String operation) {
        getContentPane().setLayout(new BorderLayout());

        if (progress.isPercent())
            progressComponent = new ProgressPercent();
        else
            progressComponent = new ProgressMessage();
        progressComponent.update(progress);

        JPanel titlePane = new JPanel();
        titlePane.setLayout(new GridLayout(1, 1));
        titlePane.setBorder(BorderFactory.createRaisedBevelBorder());
        titlePane.add(new JLabel(operation, SwingConstants.CENTER));

        JPanel mainPane = new JPanel();
        mainPane.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
        mainPane.add(progressComponent.getComponent());

        getContentPane().add(titlePane, BorderLayout.NORTH);
        getContentPane().add(mainPane, BorderLayout.CENTER);

        if (progress.isCancellable()) {
            cancel = new JButton("Cancel operation");

            JPanel buttonPane = new JPanel();
            buttonPane.setLayout(new FlowLayout());
            buttonPane.add(cancel);
            getContentPane().add(buttonPane, BorderLayout.SOUTH);

            cancel.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    cancel.setEnabled(false);
                    progress.cancel();
                }
            });
        }
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
    }

    /**
     * Continually updates the progress display until the underlying
     * operation has finished, at which point the dialog is closed and
     * the progress manager is destroyed.
     * If the dialog is not initially visible, this routine will wait
     * until it is visible before it begins querying progress.
     * This routine does not exit until all of the above tasks have been
     * completed.
     * <p>
     * This routine should not be called from outside this class;
     * outsiders should use <tt>runLifeCycle()</tt> instead.
     *
     * @see #runLifeCycle
     */
    public void run() {
        while (! isVisible()) {
            try {
                Thread.sleep(pollPeriod);
            } catch (InterruptedException e) {}
        }

        while (! manager.isFinished()) {
            if (progress.hasChanged()) {
                progressComponent.update(progress);
                pack();
                //Positioner.centerOnScreen(this);
            }
            try {
                Thread.sleep(pollPeriod);
            } catch (InterruptedException e) {}
        }

        dispose();
        manager.destroy();
    }

    /**
     * Runs a complete life cycle for this dialog.
     * The dialog is displayed, progress is updated until the underlying
     * operation has finished, the dialog is closed and the progress
     * manager is destroyed.
     * This routine does not exit until all of these tasks have been
     * completed.
     */
    public void runLifeCycle() {
        new Thread(this).start();
        show();
    }
}

