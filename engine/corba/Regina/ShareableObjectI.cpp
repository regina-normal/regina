
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

#include "utilities/stringutils.h"
#include "ShareableObjectI.h"

ShareableObject_i::ShareableObject_i(::ShareableObject* newCppPtr) :
        cppPtr(newCppPtr) {
}

void ShareableObject_i::activateObject() {
    PortableServer::POA_var poa = _default_POA();
    PortableServer::ObjectId_var id = poa->activate_object(this);
}

CORBA::Long ShareableObject_i::getCppPtr() {
    return CORBAPtrToLong(cppPtr);
}
CORBA::Boolean ShareableObject_i::sameObject(
        Regina::ShareableObject_ptr other) {
    return ((! CORBA::is_nil(other)) && other->getCppPtr() == getCppPtr());
}
void ShareableObject_i::destroy() {
    delete cppPtr;
}
char* ShareableObject_i::toString() {
    return duplicate(cppPtr->toString());
}
char* ShareableObject_i::toStringLong() {
    return duplicate(cppPtr->toStringLong());
}

