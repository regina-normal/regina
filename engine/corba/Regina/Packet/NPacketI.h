
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

#ifndef __NPACKETI_H
#define __NPACKETI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "npacket.h"
#else
    #include "engine/packet/npacket.h"
#endif

#include "NPacketIDL.h"
#include "ShareableObjectI.h"

class NPacket_i : public virtual POA_Regina::Packet::NPacket,
        public ShareableObject_i {
	STANDARD_ENGINE_TYPEDEFS(NPacket_i, NPacket, Regina::Packet::NPacket)

    protected:
        NPacket_i(::NPacket* newCppPtr) : ShareableObject_i(newCppPtr) {
        }
    public:
        static Regina::Packet::NPacket_ptr newWrapper(::NPacket* newCppPtr);

        virtual CORBA::Long getPacketType();
        virtual char* getPacketName();
        virtual char* getPacketLabel();
        virtual void setPacketLabel(const char* newLabel);
        virtual char* getFullName();
        virtual Regina::Packet::NPacket_ptr getFirstTreeChild();
        virtual Regina::Packet::NPacket_ptr getLastTreeChild();
        virtual Regina::Packet::NPacket_ptr getPrevTreeSibling();
        virtual Regina::Packet::NPacket_ptr getNextTreeSibling();
        virtual Regina::Packet::NPacket_ptr getTreeParent();
        virtual Regina::Packet::NPacket_ptr getTreeMatriarch();
        virtual void insertChildFirst(Regina::Packet::NPacket_ptr child);
        virtual void insertChildLast(Regina::Packet::NPacket_ptr child);
        virtual void insertChildAfter(Regina::Packet::NPacket_ptr newChild,
            Regina::Packet::NPacket_ptr prevChild);
        virtual void makeOrphan();
        virtual void swapWithNextSibling();
        virtual Regina::Packet::NPacket_ptr nextTreePacket_();
        virtual Regina::Packet::NPacket_ptr firstTreePacket(const char* type);
        virtual Regina::Packet::NPacket_ptr nextTreePacket_string(
            const char* type);
        virtual Regina::Packet::NPacket_ptr findPacketLabel(const char* type);
        virtual char* makeUniqueLabel(const char* base);
        virtual CORBA::Long levelsDownTo(
            Regina::Packet::NPacket_ptr descendant);
        virtual CORBA::Long levelsUpTo(
            Regina::Packet::NPacket_ptr ancestor);
        virtual CORBA::Boolean isGrandparentOf(
            Regina::Packet::NPacket_ptr descendant);
        virtual CORBA::Long totalTreeSize();
        virtual CORBA::Boolean dependsOnParent();
        virtual CORBA::Boolean isPacketEditable();
        virtual Regina::Packet::NPacket_ptr clonePacket(
            CORBA::Boolean cloneDescendants, CORBA::Boolean end);
};

#endif

