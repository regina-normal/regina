
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

package normal;

import org.gjt.btools.image.StandardImage;

/**
 * A repository of various images.
 */
public class Images {
    /**
     * A button used to create a new topology file.
     */
    public static final StandardImage btnFileNewTopology =
        new StandardImage("normal/images/buttons/fileNewTopology.gif",
        Images.class);
    /**
     * A button used to create a new Jython library.
     */
    public static final StandardImage btnFileNewLibrary =
        new StandardImage("normal/images/buttons/fileNewLibrary.gif",
        Images.class);
    /**
     * A button representing a Jython console.
     */
    public static final StandardImage btnConsole =
        new StandardImage("normal/images/buttons/console.gif", Images.class);
    /**
     * A button representing a docked pane.
     */
    public static final StandardImage btnDockDocked =
        new StandardImage("normal/images/buttons/dockDocked.gif",
            Images.class);
    /**
     * A button representing an undocked pane.
     */
    public static final StandardImage btnDockUndocked =
        new StandardImage("normal/images/buttons/dockUndocked.gif",
            Images.class);
    /**
     * A button representing a Jython script.
     */
    public static final StandardImage btnScript =
        new StandardImage("normal/images/buttons/script.gif",
            Images.class);

    /**
     * The program title image.
     */
    public static final StandardImage mainTitle =
        new StandardImage("normal/images/main/title.jpg", Images.class);
    /**
     * The large program icon.
     */
    public static final StandardImage mainLargeIcon =
        new StandardImage("normal/images/main/largeicon.jpg", Images.class);
    /**
     * The medium program icon.
     */
    public static final StandardImage mainMediumIcon =
        new StandardImage("normal/images/main/mediumicon.gif", Images.class);
    /**
     * The small program icon.
     */
    public static final StandardImage mainSmallIcon =
        new StandardImage("normal/images/main/tinyicon.gif", Images.class);

    /**
     * The SnapPea icon.
     */
    public static final StandardImage snapPeaIcon =
        new StandardImage("normal/images/main/snappea.gif", Images.class);
    /**
     * An icon representing dehydration and rehydration.
     */
    public static final StandardImage hydrationIcon =
        new StandardImage("normal/images/main/hydration.gif", Images.class);
}

