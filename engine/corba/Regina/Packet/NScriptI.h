
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#ifndef __NSCRIPTI_H
#define __NSCRIPTI_H

#include "packet/nscript.h"

#include "NScriptIDL.h"
#include "NPacketI.h"

class NScript_i : public virtual POA_Regina::Packet::NScript,
        public NPacket_i {
    STANDARD_ENGINE_TYPEDEFS(NScript_i, NScript, Regina::Packet::NScript)

    protected:
        NScript_i(::NScript* newCppPtr) : NPacket_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual CORBA::Long getNumberOfLines();
        virtual char* getLine(CORBA::Long index);
        virtual void addFirst(const char* line);
        virtual void addLast(const char* line);
        virtual void insertAtPosition(const char* line, CORBA::Long index);
        virtual void replaceAtPosition(const char* line, CORBA::Long index);
        virtual void removeLineAt(CORBA::Long index);
        virtual void removeAllLines();
        virtual CORBA::Long getNumberOfVariables();
        virtual char* getVariableName(CORBA::Long index);
        virtual char* getVariableValue_long(CORBA::Long index);
        virtual char* getVariableValue_string(const char* name);
        virtual CORBA::Long getVariableIndex(const char* name);
        virtual void addVariable(const char* name, const char* value);
        virtual void removeVariableAt(CORBA::Long index);
        virtual void removeVariable(const char* name);
        virtual void removeAllVariables();
};

#endif

