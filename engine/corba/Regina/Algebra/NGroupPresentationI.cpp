
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

#include "NGroupPresentationI.h"
#include "NGroupExpressionI.h"

CORBA::Long NGroupPresentation_i::addGenerator(CORBA::Long num) {
    return MY_ENGINE_OBJECT->addGenerator(num);
}
void NGroupPresentation_i::addRelation(
        Regina::Algebra::NGroupExpression_ptr rel) {
    MY_ENGINE_OBJECT->addRelation(GET_ENGINE_OBJECT(NGroupExpression, rel));
}
CORBA::Long NGroupPresentation_i::getNumberOfGenerators() {
    return MY_ENGINE_OBJECT->getNumberOfGenerators();
}
CORBA::Long NGroupPresentation_i::getNumberOfRelations() {
    return MY_ENGINE_OBJECT->getNumberOfRelations();
}
Regina::Algebra::NGroupExpression_ptr NGroupPresentation_i::getRelation(
        CORBA::Long index) {
    return NGroupExpression_i::newWrapper((::NGroupExpression*)
        &MY_ENGINE_OBJECT->getRelation(index));
}
CORBA::Boolean NGroupPresentation_i::intelligentSimplify() {
    return MY_ENGINE_OBJECT->intelligentSimplify();
}
char* NGroupPresentation_i::recogniseGroup() {
    return MY_ENGINE_OBJECT->recogniseGroup().dupe();
}

