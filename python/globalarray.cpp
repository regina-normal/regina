
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "dim4/nperm5.h"
#include "triangulation/nperm.h"
#include "globalarray.h"

void addGlobalArray() {
    regina::python::GlobalArray<int>::wrapClass("GlobalArray_int");
    regina::python::GlobalArray2D<int>::wrapClass("GlobalArray2D_int");
    regina::python::GlobalArray3D<int>::wrapClass("GlobalArray3D_int");
    regina::python::GlobalArray<unsigned>::wrapClass("GlobalArray_unsigned");
    regina::python::GlobalArray<const char*>::wrapClass("GlobalArray_char_string");
    regina::python::GlobalArray<regina::NPerm>::wrapClass("GlobalArray_NPerm");
    regina::python::GlobalArray2D<regina::NPerm>::wrapClass("GlobalArray2D_NPerm");
    regina::python::GlobalArray<regina::NPerm5>::wrapClass("GlobalArray_NPerm5");
    regina::python::GlobalArray2D<regina::NPerm5>::wrapClass("GlobalArray2D_NPerm5");
    regina::python::GlobalArray3D<regina::NPerm5>::wrapClass("GlobalArray3D_NPerm5");
}

