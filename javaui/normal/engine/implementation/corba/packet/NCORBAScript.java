
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

package normal.engine.implementation.corba.packet;

import normal.engine.implementation.corba.Regina.Packet.*;
import normal.engine.implementation.corba.*;

public class NCORBAScript extends NCORBAPacket
        implements normal.engine.packet.NScript {
    public NScript data;
    public static final Class CORBAClass = NScript.class;
    public static final Class helperClass = NScriptHelper.class;

    protected NCORBAScript(NScript data) {
        super(data);
        this.data = data;
    }

    public static NCORBAScript newWrapper(NScript source) {
        return (source == null ? null : new NCORBAScript(source));
    }

    public long getNumberOfLines() {
        return data.getNumberOfLines();
    }
    public String getLine(long index) {
        return data.getLine((int)index);
    }
 
    public void addFirst(String line) {
        data.addFirst(line);
    }
    public void addLast(String line) {
        data.addLast(line);
    }
    public void insertAtPosition(String line, long index) {
        data.insertAtPosition(line, (int)index);
    }
    public void replaceAtPosition(String line, long index) {
        data.replaceAtPosition(line, (int)index);
    }
    public void removeLineAt(long index) {
        data.removeLineAt((int)index);
    }
    public void removeAllLines() {
        data.removeAllLines();
    }
 
    public long getNumberOfVariables() {
        return data.getNumberOfVariables();
    }
    public String getVariableName(long index) {
        return data.getVariableName((int)index);
    }
    public String getVariableValue(long index) {
        return data.getVariableValue_long((int)index);
    }
    public String getVariableValue(String name) {
        return data.getVariableValue_string(name);
    }
    public long getVariableIndex(String name) {
        return data.getVariableIndex(name);
    }
 
    public void addVariable(String name, String value) {
        data.addVariable(name, value);
    }
    public void removeVariableAt(long index) {
        data.removeVariableAt((int)index);
    }
    public void removeVariable(String name) {
        data.removeVariable(name);
    }
    public void removeAllVariables() {
        data.removeAllVariables();
    }
}

