
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nfile.h"
#else
    #include "engine/file/nfile.h"
#endif

#include "EngineI.h"
#include "NContainerI.h"
#include "NScriptI.h"
#include "NTextI.h"

Regina::Packet::NContainer_ptr Engine_i::newNContainer() {
    return NContainer_i::newWrapper(new ::NContainer());
}
Regina::Packet::NScript_ptr Engine_i::newNScript() {
    return NScript_i::newWrapper(new ::NScript());
}
Regina::Packet::NText_ptr Engine_i::newNText_() {
    return NText_i::newWrapper(new ::NText());
}
Regina::Packet::NText_ptr Engine_i::newNText_string(const char* text) {
    return NText_i::newWrapper(new ::NText(text));
}

Regina::Packet::NPacket_ptr Engine_i::readFromFile(const char* fileName) {
    return NPacket_i::newWrapper(::readFromFile(fileName));
}
CORBA::Boolean Engine_i::writeToFile(const char* fileName,
        Regina::Packet::NPacket_ptr packet) {
    return ::writeToFile(fileName, GET_ENGINE_OBJECT(NPacket, packet));
}

