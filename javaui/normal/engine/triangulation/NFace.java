
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

package normal.engine.triangulation;

import normal.engine.*;
import java.util.*;

public interface NFace extends ShareableObject {
    int TRIANGLE = 1;
    int SCARF = 2;
    int PARACHUTE = 3;
    int CONE = 4;
    int MOBIUS = 5;
    int HORN = 6;
    int DUNCEHAT = 7;
    int L31 = 8;

    NComponent getComponent();
    NBoundaryComponent getBoundaryComponent();
    NVertex getVertex(int vertex);
    NEdge getEdge(int edge);
    NPerm getEdgeMapping(int edge);

    boolean isBoundary();
    int getType();
    int getSubtype();
    boolean isMobiusBand();
    boolean isCone();
    
    int getNumberOfEmbeddings();
    NFaceEmbedding getEmbedding(int index);
}

