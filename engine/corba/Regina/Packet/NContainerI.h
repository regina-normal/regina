
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

#ifndef __NCONTAINERI_H
#define __NCONTAINERI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ncontainer.h"
#else
    #include "engine/packet/ncontainer.h"
#endif

#include "NContainerIDL.h"
#include "NPacketI.h"

class NContainer_i : public virtual POA_Regina::Packet::NContainer,
        public NPacket_i {
	STANDARD_ENGINE_TYPEDEFS(NContainer_i, NContainer,
			Regina::Packet::NContainer)

    protected:
        NContainer_i(::NContainer* newCppPtr) : NPacket_i(newCppPtr) {
        }
	
	public:
		STANDARD_NEW_WRAPPER
};

#endif

