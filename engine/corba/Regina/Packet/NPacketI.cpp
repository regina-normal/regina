
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

#include "NPacketI.h"

CORBA::Long NPacket_i::getPacketType() {
	return GET_ENGINE_OBJECT(NPacket, this)->getPacketType();
}
char* NPacket_i::getPacketName() {
	return GET_ENGINE_OBJECT(NPacket, this)->getPacketName().dupe();
}
char* NPacket_i::getPacketLabel() {
	return GET_ENGINE_OBJECT(NPacket, this)->getPacketLabel().dupe();
}
void NPacket_i::setPacketLabel(const char* newLabel) {
	GET_ENGINE_OBJECT(NPacket, this)->setPacketLabel(newLabel);
}
char* NPacket_i::getFullName() {
	return GET_ENGINE_OBJECT(NPacket, this)->getFullName().dupe();
}
Regina::Packet::NPacket_ptr NPacket_i::getFirstTreeChild() {
	return NPacket_i::newWrapper(GET_ENGINE_OBJECT(NPacket, this)->
		getFirstTreeChild());
}
Regina::Packet::NPacket_ptr NPacket_i::getLastTreeChild() {
	return NPacket_i::newWrapper(GET_ENGINE_OBJECT(NPacket, this)->
		getLastTreeChild());
}
Regina::Packet::NPacket_ptr NPacket_i::getPrevTreeSibling() {
	return NPacket_i::newWrapper(GET_ENGINE_OBJECT(NPacket, this)->
		getPrevTreeSibling());
}
Regina::Packet::NPacket_ptr NPacket_i::getNextTreeSibling() {
	return NPacket_i::newWrapper(GET_ENGINE_OBJECT(NPacket, this)->
		getNextTreeSibling());
}
Regina::Packet::NPacket_ptr NPacket_i::getTreeParent() {
	return NPacket_i::newWrapper(GET_ENGINE_OBJECT(NPacket, this)->
		getTreeParent());
}
Regina::Packet::NPacket_ptr NPacket_i::getTreeMatriarch() {
	return NPacket_i::newWrapper(GET_ENGINE_OBJECT(NPacket, this)->
		getTreeMatriarch());
}
void NPacket_i::insertChildFirst(Regina::Packet::NPacket_ptr child) {
	GET_ENGINE_OBJECT(NPacket, this)->insertChildFirst(
		GET_ENGINE_OBJECT(NPacket, child));
}
void NPacket_i::insertChildLast(Regina::Packet::NPacket_ptr child) {
	GET_ENGINE_OBJECT(NPacket, this)->insertChildLast(
		GET_ENGINE_OBJECT(NPacket, child));
}
void NPacket_i::insertChildAfter(Regina::Packet::NPacket_ptr newChild,
		Regina::Packet::NPacket_ptr prevChild) {
	GET_ENGINE_OBJECT(NPacket, this)->insertChildAfter(
		GET_ENGINE_OBJECT(NPacket, newChild),
		GET_ENGINE_OBJECT(NPacket, prevChild));
}
void NPacket_i::makeOrphan() {
	GET_ENGINE_OBJECT(NPacket, this)->makeOrphan();
}
void NPacket_i::swapWithNextSibling() {
	GET_ENGINE_OBJECT(NPacket, this)->swapWithNextSibling();
}
Regina::Packet::NPacket_ptr NPacket_i::nextTreePacket_() {
	return NPacket_i::newWrapper(GET_ENGINE_OBJECT(NPacket, this)->
		nextTreePacket());
}
Regina::Packet::NPacket_ptr NPacket_i::firstTreePacket(const char* type) {
	return NPacket_i::newWrapper(GET_ENGINE_OBJECT(NPacket, this)->
		firstTreePacket(type));
}
Regina::Packet::NPacket_ptr NPacket_i::nextTreePacket_string(
		const char* type) {
	return NPacket_i::newWrapper(GET_ENGINE_OBJECT(NPacket, this)->
		nextTreePacket(type));
}
Regina::Packet::NPacket_ptr NPacket_i::findPacketLabel(const char* type) {
	return NPacket_i::newWrapper(GET_ENGINE_OBJECT(NPacket, this)->
		findPacketLabel(type));
}
char* NPacket_i::makeUniqueLabel(const char* base) {
	return GET_ENGINE_OBJECT(NPacket, this)->makeUniqueLabel(base).dupe();
}
CORBA::Long NPacket_i::levelsDownTo(
		Regina::Packet::NPacket_ptr descendant) {
	return GET_ENGINE_OBJECT(NPacket, this)->
		levelsDownTo(GET_ENGINE_OBJECT(NPacket, descendant));
}
CORBA::Long NPacket_i::levelsUpTo(
		Regina::Packet::NPacket_ptr ancestor) {
	return GET_ENGINE_OBJECT(NPacket, this)->
		levelsUpTo(GET_ENGINE_OBJECT(NPacket, ancestor));
}
CORBA::Boolean NPacket_i::isGrandparentOf(
		Regina::Packet::NPacket_ptr descendant) {
	return GET_ENGINE_OBJECT(NPacket, this)->
		isGrandparentOf(GET_ENGINE_OBJECT(NPacket, descendant));
}
CORBA::Long NPacket_i::totalTreeSize() {
	return GET_ENGINE_OBJECT(NPacket, this)->totalTreeSize();
}
CORBA::Boolean NPacket_i::dependsOnParent() {
	return GET_ENGINE_OBJECT(NPacket, this)->dependsOnParent();
}
CORBA::Boolean NPacket_i::isPacketEditable() {
	return GET_ENGINE_OBJECT(NPacket, this)->isPacketEditable();
}
Regina::Packet::NPacket_ptr NPacket_i::clonePacket(
		CORBA::Boolean cloneDescendants, CORBA::Boolean end) {
	return NPacket_i::newWrapper(GET_ENGINE_OBJECT(NPacket, this)->
		clone(cloneDescendants, end));
}

