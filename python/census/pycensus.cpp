
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

namespace pybind11 { class module_; }

void addPurgeFlags(pybind11::module_& m);
void addGluingPerms2(pybind11::module_& m);
void addGluingPerms3(pybind11::module_& m);
void addGluingPerms4(pybind11::module_& m);
void addGluingPerms5(pybind11::module_& m);
void addGluingPerms6(pybind11::module_& m);
void addGluingPerms7(pybind11::module_& m);
void addGluingPerms8(pybind11::module_& m);
void addGluingPerms9(pybind11::module_& m);
void addGluingPerms10(pybind11::module_& m);
void addGluingPerms11(pybind11::module_& m);
void addGluingPerms12(pybind11::module_& m);
void addGluingPerms13(pybind11::module_& m);
void addGluingPerms14(pybind11::module_& m);
void addGluingPerms15(pybind11::module_& m);
void addGluingPermSearcher2(pybind11::module_& m);
void addGluingPermSearcher3(pybind11::module_& m);
void addGluingPermSearcher4(pybind11::module_& m);
void addCensus(pybind11::module_& m);

void addCensusClasses(pybind11::module_& m) {
    addPurgeFlags(m);
    addGluingPerms2(m);
    addGluingPerms3(m);
    addGluingPerms4(m);
    addGluingPerms5(m);
    addGluingPerms6(m);
    addGluingPerms7(m);
    addGluingPerms8(m);
#ifdef REGINA_HIGHDIM
    addGluingPerms9(m);
    addGluingPerms10(m);
    addGluingPerms11(m);
    addGluingPerms12(m);
    addGluingPerms13(m);
    addGluingPerms14(m);
    addGluingPerms15(m);
#endif /* REGINA_HIGHDIM */
    addGluingPermSearcher2(m);
    addGluingPermSearcher3(m);
    addGluingPermSearcher4(m);
    addCensus(m);
}

