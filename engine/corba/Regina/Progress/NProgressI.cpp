
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

#include "NProgressI.h"

CORBA::Boolean NProgress_i::hasChanged() {
    return GET_ENGINE_OBJECT(NProgress, this)->hasChanged();
}
CORBA::Boolean NProgress_i::isFinished() {
    return GET_ENGINE_OBJECT(NProgress, this)->isFinished();
}
CORBA::Boolean NProgress_i::isCancellable() {
    return GET_ENGINE_OBJECT(NProgress, this)->isCancellable();
}
void NProgress_i::cancel() {
    GET_ENGINE_OBJECT(NProgress, this)->cancel();
}
CORBA::Boolean NProgress_i::isCancelled() {
    return GET_ENGINE_OBJECT(NProgress, this)->isCancelled();
}
char* NProgress_i::getDescription() {
    return GET_ENGINE_OBJECT(NProgress, this)->getDescription().dupe();
}
CORBA::Boolean NProgress_i::isPercent() {
    return GET_ENGINE_OBJECT(NProgress, this)->isPercent();
}
CORBA::Double NProgress_i::getPercent() {
    return GET_ENGINE_OBJECT(NProgress, this)->getPercent();
}

