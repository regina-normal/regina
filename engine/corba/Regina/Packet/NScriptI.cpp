
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

#include "NScriptI.h"

CORBA::Long NScript_i::getNumberOfLines() {
    return MY_ENGINE_OBJECT->getNumberOfLines();
}
char* NScript_i::getLine(CORBA::Long index) {
    return MY_ENGINE_OBJECT->getLine(index).dupe();
}
void NScript_i::addFirst(const char* line) {
    MY_ENGINE_OBJECT->addFirst(line);
}
void NScript_i::addLast(const char* line) {
    MY_ENGINE_OBJECT->addLast(line);
}
void NScript_i::insertAtPosition(const char* line, CORBA::Long index) {
    MY_ENGINE_OBJECT->insertAtPosition(line, index);
}
void NScript_i::replaceAtPosition(const char* line, CORBA::Long index) {
    MY_ENGINE_OBJECT->replaceAtPosition(line, index);
}
void NScript_i::removeLineAt(CORBA::Long index) {
    MY_ENGINE_OBJECT->removeLineAt(index);
}
void NScript_i::removeAllLines() {
    MY_ENGINE_OBJECT->removeAllLines();
}
CORBA::Long NScript_i::getNumberOfVariables() {
    return MY_ENGINE_OBJECT->getNumberOfVariables();
}
char* NScript_i::getVariableName(CORBA::Long index) {
    return MY_ENGINE_OBJECT->getVariableName(index).dupe();
}
char* NScript_i::getVariableValue_long(CORBA::Long index) {
    return MY_ENGINE_OBJECT->getVariableValue(index).dupe();
}
char* NScript_i::getVariableValue_string(const char* name) {
    return MY_ENGINE_OBJECT->getVariableValue(name).dupe();
}
CORBA::Long NScript_i::getVariableIndex(const char* name) {
    return MY_ENGINE_OBJECT->getVariableIndex(name);
}
void NScript_i::addVariable(const char* name, const char* value) {
    MY_ENGINE_OBJECT->addVariable(name, value);
}
void NScript_i::removeVariableAt(CORBA::Long index) {
    MY_ENGINE_OBJECT->removeVariableAt(index);
}
void NScript_i::removeVariable(const char* name) {
    MY_ENGINE_OBJECT->removeVariable(name);
}
void NScript_i::removeAllVariables() {
    MY_ENGINE_OBJECT->removeAllVariables();
}

