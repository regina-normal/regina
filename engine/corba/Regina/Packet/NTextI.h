
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

#ifndef __NTEXTI_H
#define __NTEXTI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ntext.h"
    #include "corbatools.h"
#else
    #include "engine/packet/ntext.h"
    #include "corba/corbatools.h"
#endif

#include "NTextIDL.h"
#include "NPacketI.h"

class NText_i : public virtual Regina::Packet::_sk_NText,
        public NPacket_i {
    protected:
        NText_i(::NText* newCppPtr) : NPacket_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER(NText, NText_i, Regina::Packet::NText_ptr)

        virtual char* getText();
        virtual void setText(const char* newText);
};

#endif

