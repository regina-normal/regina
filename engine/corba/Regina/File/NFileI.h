
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

#ifndef __NFILEI_H
#define __NFILEI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nfile.h"
#else
    #include "engine/file/nfile.h"
#endif

#include "NFileIDL.h"
#include "ShareableObjectI.h"

class NFile_i : public virtual POA_Regina::File::NFile,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NFile_i, NFile, Regina::File::NFile)

    protected:
        NFile_i(::NFile* newCppPtr) : ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual CORBA::Boolean open(const char* fileName,
            CORBA::Long openMode);
        virtual void close();
        virtual CORBA::Long getOpenMode();
        virtual CORBA::Long getMajorVersion();
        virtual CORBA::Long getMinorVersion();
        virtual CORBA::Boolean versionEarlierThan(CORBA::Long major,
            CORBA::Long minor);
        virtual void writePacketTree(Regina::Packet::NPacket_ptr packet);
        virtual Regina::Packet::NPacket_ptr readPacketTree_();
        virtual Regina::Packet::NPacket_ptr readPacketTree_NPacket(
            Regina::Packet::NPacket_ptr parent);
};

#endif

