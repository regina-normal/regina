
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

#include "NPacketI.h"

CORBA::Long NPacket_i::getPacketType() {
    return MY_ENGINE_OBJECT->getPacketType();
}
char* NPacket_i::getPacketName() {
    return MY_ENGINE_OBJECT->getPacketName().dupe();
}
char* NPacket_i::getPacketLabel() {
    return MY_ENGINE_OBJECT->getPacketLabel().dupe();
}
void NPacket_i::setPacketLabel(const char* newLabel) {
    MY_ENGINE_OBJECT->setPacketLabel(newLabel);
}
char* NPacket_i::getFullName() {
    return MY_ENGINE_OBJECT->getFullName().dupe();
}
Regina::Packet::NPacket_ptr NPacket_i::getFirstTreeChild() {
    return NPacket_i::newWrapper(MY_ENGINE_OBJECT->getFirstTreeChild());
}
Regina::Packet::NPacket_ptr NPacket_i::getLastTreeChild() {
    return NPacket_i::newWrapper(MY_ENGINE_OBJECT->getLastTreeChild());
}
Regina::Packet::NPacket_ptr NPacket_i::getPrevTreeSibling() {
    return NPacket_i::newWrapper(MY_ENGINE_OBJECT->getPrevTreeSibling());
}
Regina::Packet::NPacket_ptr NPacket_i::getNextTreeSibling() {
    return NPacket_i::newWrapper(MY_ENGINE_OBJECT->getNextTreeSibling());
}
Regina::Packet::NPacket_ptr NPacket_i::getTreeParent() {
    return NPacket_i::newWrapper(MY_ENGINE_OBJECT->getTreeParent());
}
Regina::Packet::NPacket_ptr NPacket_i::getTreeMatriarch() {
    return NPacket_i::newWrapper(MY_ENGINE_OBJECT->getTreeMatriarch());
}
void NPacket_i::insertChildFirst(Regina::Packet::NPacket_ptr child) {
    MY_ENGINE_OBJECT->insertChildFirst(GET_ENGINE_OBJECT(NPacket, child));
}
void NPacket_i::insertChildLast(Regina::Packet::NPacket_ptr child) {
    MY_ENGINE_OBJECT->insertChildLast(GET_ENGINE_OBJECT(NPacket, child));
}
void NPacket_i::insertChildAfter(Regina::Packet::NPacket_ptr newChild,
        Regina::Packet::NPacket_ptr prevChild) {
    MY_ENGINE_OBJECT->insertChildAfter(GET_ENGINE_OBJECT(NPacket, newChild),
        GET_ENGINE_OBJECT(NPacket, prevChild));
}
void NPacket_i::makeOrphan() {
    MY_ENGINE_OBJECT->makeOrphan();
}
void NPacket_i::swapWithNextSibling() {
    MY_ENGINE_OBJECT->swapWithNextSibling();
}
Regina::Packet::NPacket_ptr NPacket_i::nextTreePacket_() {
    return NPacket_i::newWrapper(MY_ENGINE_OBJECT->nextTreePacket());
}
Regina::Packet::NPacket_ptr NPacket_i::firstTreePacket(const char* type) {
    return NPacket_i::newWrapper(MY_ENGINE_OBJECT->firstTreePacket(type));
}
Regina::Packet::NPacket_ptr NPacket_i::nextTreePacket_string(
        const char* type) {
    return NPacket_i::newWrapper(MY_ENGINE_OBJECT->nextTreePacket(type));
}
Regina::Packet::NPacket_ptr NPacket_i::findPacketLabel(const char* type) {
    return NPacket_i::newWrapper(MY_ENGINE_OBJECT->findPacketLabel(type));
}
char* NPacket_i::makeUniqueLabel(const char* base) {
    return MY_ENGINE_OBJECT->makeUniqueLabel(base).dupe();
}
CORBA::Long NPacket_i::levelsDownTo(
        Regina::Packet::NPacket_ptr descendant) {
    return MY_ENGINE_OBJECT->
        levelsDownTo(GET_ENGINE_OBJECT(NPacket, descendant));
}
CORBA::Long NPacket_i::levelsUpTo(
        Regina::Packet::NPacket_ptr ancestor) {
    return MY_ENGINE_OBJECT->levelsUpTo(GET_ENGINE_OBJECT(NPacket, ancestor));
}
CORBA::Boolean NPacket_i::isGrandparentOf(
        Regina::Packet::NPacket_ptr descendant) {
    return MY_ENGINE_OBJECT->
        isGrandparentOf(GET_ENGINE_OBJECT(NPacket, descendant));
}
CORBA::Long NPacket_i::getNumberOfChildren() {
    return MY_ENGINE_OBJECT->getNumberOfChildren();
}
CORBA::Long NPacket_i::getNumberOfDescendants() {
    return MY_ENGINE_OBJECT->getNumberOfDescendants();
}
CORBA::Long NPacket_i::getTotalTreeSize() {
    return MY_ENGINE_OBJECT->getTotalTreeSize();
}
CORBA::Boolean NPacket_i::dependsOnParent() {
    return MY_ENGINE_OBJECT->dependsOnParent();
}
CORBA::Boolean NPacket_i::isPacketEditable() {
    return MY_ENGINE_OBJECT->isPacketEditable();
}
Regina::Packet::NPacket_ptr NPacket_i::clonePacket(
        CORBA::Boolean cloneDescendants, CORBA::Boolean end) {
    return NPacket_i::newWrapper(MY_ENGINE_OBJECT->
        clone(cloneDescendants, end));
}

