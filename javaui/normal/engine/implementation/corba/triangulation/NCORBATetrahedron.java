
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

package normal.engine.implementation.corba.triangulation;

import normal.engine.triangulation.NPerm;
import normal.engine.implementation.corba.Regina.Triangulation.*;
import normal.engine.implementation.corba.*;

public class NCORBATetrahedron extends CORBAShareableObject
		implements normal.engine.triangulation.NTetrahedron {
	public NTetrahedron data;
	public static final Class CORBAClass = NTetrahedron.class;
	public static final Class helperClass = NTetrahedronHelper.class;

    public NCORBATetrahedron(NTetrahedron data) {
		super(data);
		this.data = data;
    }

	public static NCORBATetrahedron newWrapper(NTetrahedron source) {
		return (source == null ? null : new NCORBATetrahedron(source));
	}

	public normal.engine.triangulation.NTetrahedron
			getAdjacentTetrahedron(int face) {
		return NCORBATetrahedron.newWrapper(data.getAdjacentTetrahedron(face));
	}
	public normal.engine.triangulation.NPerm
			getAdjacentTetrahedronGluing(int face) {
		return new NPerm(data.getAdjacentTetrahedronGluing(face));
	}
	public int getAdjacentFace(int face) {
		return data.getAdjacentFace(face);
	}
	public boolean hasBoundary() {
		return data.hasBoundary();
	}

	public normal.engine.triangulation.NComponent getComponent() {
		return NCORBAComponent.newWrapper(data.getComponent());
	}

	public normal.engine.triangulation.NVertex getVertex(int vertex) {
		return NCORBAVertex.newWrapper(data.getVertex(vertex));
	}
	public normal.engine.triangulation.NEdge getEdge(int edge) {
		return NCORBAEdge.newWrapper(data.getEdge(edge));
	}
	public normal.engine.triangulation.NFace getFace(int face) {
		return NCORBAFace.newWrapper(data.getFace(face));
	}
	
	public normal.engine.triangulation.NPerm getEdgeMapping(int edge) {
		return new NPerm(data.getEdgeMapping(edge));
	}
	public normal.engine.triangulation.NPerm getFaceMapping(int face) {
		return new NPerm(data.getFaceMapping(face));
	}

	public void joinTo(int myFace,
			normal.engine.triangulation.NTetrahedron you,
			normal.engine.triangulation.NPerm gluing) {
		data.joinTo(myFace, ((NCORBATetrahedron)you).data,
			gluing.getPermCode());
	}
	public normal.engine.triangulation.NTetrahedron unjoin(int myFace) {
		return NCORBATetrahedron.newWrapper(data.unjoin(myFace));
	}
	public void isolate() {
		data.isolate();
	}

	public String getDescription() {
		return data.getDescription();
	}
	public void setDescription(String description) {
		data.setDescription(description);
	}
}
