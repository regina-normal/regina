
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

#include "file/nfile.h"

#include "EngineI.h"
#include "NContainerI.h"
#include "NScriptI.h"
#include "NTextI.h"
#include "NCORBARandomAccessResource.h"

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

Regina::Packet::NPacket_ptr Engine_i::readFromFile(const char* ref) {
    try {
        CORBA::Object_var res = orb->string_to_object(ref);
        if (CORBA::is_nil(res)) {
            std::cerr <<
                "Could not destringify the CORBA random access resource.\n";
            return Regina::Packet::NPacket::_nil();
        }
        Regina::File::NRandomAccessResource_var data =
            Regina::File::NRandomAccessResource::_narrow(res);
        if (CORBA::is_nil(data)) {
            std::cerr << "Could not narrow the random access resource to "
                << "the correct class.\n";
            return Regina::Packet::NPacket::_nil();
        }
        NCORBARandomAccessResource* corbaRes =
            new NCORBARandomAccessResource(data);

        // Actually read from file.
        ::NFile f;
        if (f.open(corbaRes, NRandomAccessResource::READ)) {
            ::NPacket* ans = f.readPacketTree();
            f.close();
            return NPacket_i::newWrapper(ans);
        }
    } catch (CORBA::COMM_FAILURE&) {
        std::cerr << "Caught CORBA communication failure in readFromFile().\n";
    } catch (CORBA::Exception&) {
        std::cerr << "Caught CORBA exception in readFromFile().\n";
    } catch (...) {
        std::cerr << "Caught unknown exception in readFromFile().\n";
    }
    return Regina::Packet::NPacket::_nil();
}
CORBA::Boolean Engine_i::writeToFile(const char* ref,
        Regina::Packet::NPacket_ptr packet) {
    try {
        CORBA::Object_var res = orb->string_to_object(ref);
        if (CORBA::is_nil(res)) {
            std::cerr <<
                "Could not destringify the CORBA random access resource.\n";
            return Regina::Packet::NPacket::_nil();
        }
        Regina::File::NRandomAccessResource_var data =
            Regina::File::NRandomAccessResource::_narrow(res);
        if (CORBA::is_nil(data)) {
            std::cerr << "Could not narrow the random access resource to "
                << "the correct class.\n";
            return Regina::Packet::NPacket::_nil();
        }
        NCORBARandomAccessResource* corbaRes =
            new NCORBARandomAccessResource(data);

        // Actually write to file.
        ::NFile f;
        if (f.open(corbaRes, NRandomAccessResource::WRITE)) {
            f.writePacketTree(GET_ENGINE_OBJECT(NPacket, packet));
            f.close();
            return true;
        }
    } catch (CORBA::COMM_FAILURE&) {
        std::cerr << "Caught CORBA communication failure in writeToFile().\n";
    } catch (CORBA::Exception&) {
        std::cerr << "Caught CORBA exception in writeToFile().\n";
    } catch (...) {
        std::cerr << "Caught unknown exception in writeToFile().\n";
    }
    return false;
}

