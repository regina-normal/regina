
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

package normal.engine.triangulation;

import normal.engine.*;
import java.util.*;

public interface NVertex extends ShareableObject {
    int SPHERE = 1;
    int DISC = 2;
    int TORUS = 3;
    int KLEIN_BOTTLE = 4;
    int NON_STANDARD_CUSP = 5;
    int NON_STANDARD_BDRY = 6;

    NComponent getComponent();
    NBoundaryComponent getBoundaryComponent();

    int getLink();
    boolean isLinkClosed();
    boolean isIdeal();
    boolean isBoundary();
    boolean isStandard();
    boolean isLinkOrientable();
    long getLinkEulerCharacteristic();

    long getNumberOfEmbeddings();
    NVertexEmbedding getEmbedding(long index);
}

