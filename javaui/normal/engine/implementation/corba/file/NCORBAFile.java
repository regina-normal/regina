
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

package normal.engine.implementation.corba.file;

import normal.engine.implementation.corba.Regina.File.*;
import normal.engine.implementation.corba.packet.*;
import normal.engine.implementation.corba.*;

public class NCORBAFile extends CORBAShareableObject
		implements normal.engine.file.NFile {
	public NFile data;
	public static final Class CORBAClass = NFile.class;
	public static final Class helperClass = NFileHelper.class;

    protected NCORBAFile(NFile data) {
		super(data);
		this.data = data;
    }

	public static NCORBAFile newWrapper(NFile source) {
		return (source == null ? null : new NCORBAFile(source));
	}

	public boolean open(String fileName, int openMode) {
		return data.open(fileName, openMode);
	}
	public void close() {
		data.close();
	}

	public int getOpenMode() {
		return data.getOpenMode();
	}
	public int getMajorVersion() {
		return data.getMajorVersion();
	}
	public int getMinorVersion() {
		return data.getMinorVersion();
	}
	public boolean versionEarlierThan(int major, int minor) {
		return data.versionEarlierThan(major, minor);
	}

	public void writePacketTree(normal.engine.packet.NPacket packet) {
		data.writePacketTree(((NCORBAPacket)packet).data);
	}
	public normal.engine.packet.NPacket readPacketTree() {
		return NCORBAPacket.newWrapper(data.readPacketTree_());
	}
	public normal.engine.packet.NPacket readPacketTree(
			normal.engine.packet.NPacket parent) {
		return NCORBAPacket.newWrapper(data.readPacketTree_NPacket(
			((NCORBAPacket)parent).data));
	}
}
