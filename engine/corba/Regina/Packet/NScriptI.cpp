
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
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

#include "NScriptI.h"

CORBA::Long NScript_i::getNumberOfLines() {
	return GET_ENGINE_OBJECT(NScript, this)->getNumberOfLines();
}
char* NScript_i::getLine(CORBA::Long index) {
	return GET_ENGINE_OBJECT(NScript, this)->getLine(index).dupe();
}
void NScript_i::addFirst(const char* line) {
	GET_ENGINE_OBJECT(NScript, this)->addFirst(line);
}
void NScript_i::addLast(const char* line) {
	GET_ENGINE_OBJECT(NScript, this)->addLast(line);
}
void NScript_i::insertAtPosition(const char* line, CORBA::Long index) {
	GET_ENGINE_OBJECT(NScript, this)->insertAtPosition(line, index);
}
void NScript_i::replaceAtPosition(const char* line, CORBA::Long index) {
	GET_ENGINE_OBJECT(NScript, this)->replaceAtPosition(line, index);
}
void NScript_i::removeLineAt(CORBA::Long index) {
	GET_ENGINE_OBJECT(NScript, this)->removeLineAt(index);
}
void NScript_i::removeAllLines() {
	GET_ENGINE_OBJECT(NScript, this)->removeAllLines();
}
CORBA::Long NScript_i::getNumberOfVariables() {
	return GET_ENGINE_OBJECT(NScript, this)->getNumberOfVariables();
}
char* NScript_i::getVariableName(CORBA::Long index) {
	return GET_ENGINE_OBJECT(NScript, this)->getVariableName(index).dupe();
}
char* NScript_i::getVariableValue_long(CORBA::Long index) {
	return GET_ENGINE_OBJECT(NScript, this)->getVariableValue(index).dupe();
}
char* NScript_i::getVariableValue_string(const char* name) {
	return GET_ENGINE_OBJECT(NScript, this)->getVariableValue(name).dupe();
}
CORBA::Long NScript_i::getVariableIndex(const char* name) {
	return GET_ENGINE_OBJECT(NScript, this)->getVariableIndex(name);
}
void NScript_i::addVariable(const char* name, const char* value) {
	GET_ENGINE_OBJECT(NScript, this)->addVariable(name, value);
}
void NScript_i::removeVariableAt(CORBA::Long index) {
	GET_ENGINE_OBJECT(NScript, this)->removeVariableAt(index);
}
void NScript_i::removeVariable(const char* name) {
	GET_ENGINE_OBJECT(NScript, this)->removeVariable(name);
}
void NScript_i::removeAllVariables() {
	GET_ENGINE_OBJECT(NScript, this)->removeAllVariables();
}

