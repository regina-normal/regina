
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

package normal.engine.triangulation;

import normal.engine.*;
import java.util.*;

public interface NTetrahedron extends ShareableObject {
    public NTetrahedron getAdjacentTetrahedron(int face);
    public NPerm getAdjacentTetrahedronGluing(int face);
    public int getAdjacentFace(int face);
    public boolean hasBoundary();

    public void joinTo(int myFace, NTetrahedron you, NPerm gluing);
    public NTetrahedron unjoin(int myFace);
    public void isolate();
    
    public String getDescription();
    public void setDescription(String description);

    public NComponent getComponent();

    public NVertex getVertex(int vertex);
    public NEdge getEdge(int edge);
    public NFace getFace(int face);

    public NPerm getEdgeMapping(int edge);
    public NPerm getFaceMapping(int face);
}

