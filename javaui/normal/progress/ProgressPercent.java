
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

package normal.progress;

import java.awt.Component;
import javax.swing.*;
import normal.engine.progress.NProgress;

/**
 * A progress component that shows the state of progress as a
 * percentage.  The underlying component is a progress bar.
 */
public class ProgressPercent extends JProgressBar
        implements ProgressComponent {
    /**
     * Creates a new percentage based progress component.
     */
    public ProgressPercent() {
        super(0, 1000);
    }

    public Component getComponent() {
        return this;
    }

    public void update(NProgress progress) {
        setValue(new Double(progress.getPercent() * 10).intValue());
    }
}

