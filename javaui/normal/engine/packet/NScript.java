
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

package normal.engine.packet;

public interface NScript extends NPacket {
    public static final int packetType = 7;

    public long getNumberOfLines();
    public String getLine(long index);

    public void addFirst(String line);
    public void addLast(String line);
    public void insertAtPosition(String line, long index);
    public void replaceAtPosition(String line, long index);
    public void removeLineAt(long index);
    public void removeAllLines();

    public long getNumberOfVariables();
    public String getVariableName(long index);
    public String getVariableValue(long index);
    public String getVariableValue(String name);

    public boolean addVariable(String name, String value);
    public void removeVariable(String name);
    public void removeAllVariables();
}
