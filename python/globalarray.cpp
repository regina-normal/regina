
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "maths/nperm3.h"
#include "maths/nperm4.h"
#include "globalarray.h"

void addGlobalArray() {
    regina::python::GlobalArray<int>::wrapClass("GlobalArray_int");
    regina::python::GlobalArray2D<int>::wrapClass("GlobalArray2D_int");
    regina::python::GlobalArray3D<int>::wrapClass("GlobalArray3D_int");
    regina::python::GlobalArray<unsigned>::wrapClass("GlobalArray_unsigned");
    regina::python::GlobalArray<const char*>::wrapClass("GlobalArray_char_string");
    regina::python::GlobalArray<regina::NPerm3>::wrapClass("GlobalArray_NPerm3");
    regina::python::GlobalArray<regina::NPerm4>::wrapClass("GlobalArray_NPerm4");
    regina::python::GlobalArray2D<regina::NPerm4>::wrapClass("GlobalArray2D_NPerm4");
}

