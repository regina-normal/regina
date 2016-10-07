
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "utilities/stringutils.h"

using namespace boost::python;

namespace {
    std::string (*stringToToken_chars)(const char*) = &regina::stringToToken;
    std::string (*stringToToken_string)(const std::string&) =
        &regina::stringToToken;
    std::string (*subscript_long)(long) = &regina::subscript;
    std::string (*subscript_largeF)(const regina::Integer&) =
        &regina::subscript;
    std::string (*subscript_largeT)(const regina::LargeInteger&) =
        &regina::subscript;
    std::string (*superscript_long)(long) = &regina::superscript;
    std::string (*superscript_largeF)(const regina::Integer&) =
        &regina::superscript;
    std::string (*superscript_largeT)(const regina::LargeInteger&) =
        &regina::superscript;
}

void addStringUtils() {
    def("stringToToken", stringToToken_chars);
    def("stringToToken", stringToToken_string);
    def("subscript", subscript_long);
    def("subscript", subscript_largeF);
    def("subscript", subscript_largeT);
    def("superscript", superscript_long);
    def("superscript", superscript_largeF);
    def("superscript", superscript_largeT);
}

