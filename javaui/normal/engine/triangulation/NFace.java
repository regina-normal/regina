
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

package normal.engine.triangulation;

import normal.engine.*;
import java.util.*;

public interface NFace extends ShareableObject {
    public static final int TRIANGLE = 1;
    public static final int SCARF = 2;
    public static final int PARACHUTE = 3;
    public static final int CONE = 4;
    public static final int MOBIUS = 5;
    public static final int HORN = 6;
    public static final int DUNCEHAT = 7;
    public static final int L31 = 8;

    public NComponent getComponent();
    public NBoundaryComponent getBoundaryComponent();
    public NVertex getVertex(int vertex);
    public NEdge getEdge(int edge);
    public NPerm getEdgeMapping(int edge);

    public boolean isBoundary();
    public int getType();
    public int getSubtype();
    public boolean isMobiusBand();
    public boolean isCone();
    
    public int getNumberOfEmbeddings();
    public NFaceEmbedding getEmbedding(int index);
}

