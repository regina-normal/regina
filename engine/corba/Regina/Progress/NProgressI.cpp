
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

#include "NProgressI.h"

CORBA::Boolean NProgress_i::hasChanged() {
    return MY_ENGINE_OBJECT->hasChanged();
}
CORBA::Boolean NProgress_i::isFinished() {
    return MY_ENGINE_OBJECT->isFinished();
}
CORBA::Boolean NProgress_i::isCancellable() {
    return MY_ENGINE_OBJECT->isCancellable();
}
void NProgress_i::cancel() {
    MY_ENGINE_OBJECT->cancel();
}
CORBA::Boolean NProgress_i::isCancelled() {
    return MY_ENGINE_OBJECT->isCancelled();
}
char* NProgress_i::getDescription() {
    return MY_ENGINE_OBJECT->getDescription().dupe();
}
CORBA::Boolean NProgress_i::isPercent() {
    return MY_ENGINE_OBJECT->isPercent();
}
CORBA::Double NProgress_i::getPercent() {
    return MY_ENGINE_OBJECT->getPercent();
}

