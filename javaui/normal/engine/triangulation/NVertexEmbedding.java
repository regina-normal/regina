
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

public class NVertexEmbedding {
    private NTetrahedron tetrahedron;
    private int vertex;

    public NVertexEmbedding(NTetrahedron tetrahedron, int vertex) {
        this.tetrahedron = tetrahedron;
        this.vertex = vertex;
    }
    public NVertexEmbedding(NVertexEmbedding cloneMe) {
        this.tetrahedron = cloneMe.tetrahedron;
        this.vertex = cloneMe.vertex;
    }

    public NTetrahedron getTetrahedron() {
        return tetrahedron;
    }
    public int getVertex() {
        return vertex;
    }
}

