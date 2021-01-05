
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include "maths/perm.h"
#include "globalarray.h"

void addGlobalArray(pybind11::module_& m) {
    regina::python::GlobalArray<int>::wrapClass(m, "GlobalArray_int");
    regina::python::GlobalArray2D<int>::wrapClass(m, "GlobalArray2D_int");
    regina::python::GlobalArray3D<int>::wrapClass(m, "GlobalArray3D_int");
    regina::python::GlobalArray<unsigned>::wrapClass(m, "GlobalArray_unsigned");
    regina::python::GlobalArray<const char*>::wrapClass(m, "GlobalArray_char_string");
    regina::python::GlobalArray<regina::Perm<2>>::wrapClass(m, "GlobalArray_Perm2");
    regina::python::GlobalArray<regina::Perm<3>>::wrapClass(m, "GlobalArray_Perm3");
    regina::python::GlobalArray<regina::Perm<4>>::wrapClass(m, "GlobalArray_Perm4");
    regina::python::GlobalArray2D<regina::Perm<4>>::wrapClass(m, "GlobalArray2D_Perm4");
    regina::python::GlobalArray<regina::Perm<5>>::wrapClass(m, "GlobalArray_Perm5");
    regina::python::GlobalArray2D<regina::Perm<5>>::wrapClass(m, "GlobalArray2D_Perm5");
    regina::python::GlobalArray3D<regina::Perm<5>>::wrapClass(m, "GlobalArray3D_Perm5");
}

