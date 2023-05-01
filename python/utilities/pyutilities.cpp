
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

void addBase64(pybind11::module_& m);
void addBitManipulator(pybind11::module_& m);
void addBitmask(pybind11::module_& m);
void addBoolSet(pybind11::module_& m);
void addException(pybind11::module_& m);
void addIntUtils(pybind11::module_& m);
void addLocale(pybind11::module_& m);
void addOSUtils(pybind11::module_& m);
void addQitmask(pybind11::module_& m);
void addRandUtils(pybind11::module_& m);
void addSigUtils(pybind11::module_& m);
void addStringUtils(pybind11::module_& m);
void addTightEncoding(pybind11::module_& m);
void addTrieSet(pybind11::module_& m);
void addXMLUtils(pybind11::module_& m);

void addUtilitiesClasses(pybind11::module_& m) {
    addBase64(m);
    addBitManipulator(m);
    addBitmask(m);
    addBoolSet(m);
    addException(m);
    addIntUtils(m);
    addLocale(m);
    addOSUtils(m);
    addQitmask(m);
    addRandUtils(m);
    addSigUtils(m);
    addStringUtils(m);
    addTightEncoding(m);
    addTrieSet(m);
    addXMLUtils(m);
}

