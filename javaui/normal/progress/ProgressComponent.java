
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
import normal.engine.progress.NProgress;

/**
 * Represents a component in which the state of progress of an operation
 * can be displayed.  For instance, this could be a progress bar or a label.
 * <p>
 * The component need not be this object itself, as long as the
 * component can be provided upon request.
 * The component should not be padded with empty space on the outside;
 * this will be done by whatever container the component is placed in.
 */
public interface ProgressComponent {
    /**
     * Returns the component used to display the state of progress.
     *
     * @return the component used to display the state of progress.
     */
    Component getComponent();

    /**
     * Updates this component to reflect the given state of progress.
     *
     * @param progress the state of progress that this component should
     * report.
     */
    void update(NProgress progress);
}

