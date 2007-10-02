
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include "algebra/nmarkedabeliangroup.h"
#include "maths/nmatrixint.h"
#include <boost/python.hpp>
#include <boost/python/detail/api_placeholder.hpp> // For len().

using namespace boost::python;
using regina::NMarkedAbelianGroup;
using regina::NMatrixInt;

namespace {
    unsigned (NMarkedAbelianGroup::*getTorsionRank_large)(
        const regina::NLargeInteger&) const = &NMarkedAbelianGroup::getTorsionRank;
    unsigned (NMarkedAbelianGroup::*getTorsionRank_long)(unsigned long)
        const = &NMarkedAbelianGroup::getTorsionRank;
}

void addNMarkedAbelianGroup() {
    class_<NMarkedAbelianGroup, bases<regina::ShareableObject>,
            std::auto_ptr<NMarkedAbelianGroup>, boost::noncopyable>(
            "NMarkedAbelianGroup", init<const NMatrixInt&, const NMatrixInt&>())
        .def(init<const NMarkedAbelianGroup&>())
        .def("getRank", &NMarkedAbelianGroup::getRank)
        .def("getTorsionRank", getTorsionRank_large)
        .def("getTorsionRank", getTorsionRank_long)
        .def("getNumberOfInvariantFactors",
            &NMarkedAbelianGroup::getNumberOfInvariantFactors)
        .def("getInvariantFactor", &NMarkedAbelianGroup::getInvariantFactor,
            return_value_policy<return_by_value>())
        .def("isTrivial", &NMarkedAbelianGroup::isTrivial)
        .def("getMRB", &NMarkedAbelianGroup::getMRB,
            return_internal_reference<>())
        .def("getMRBi", &NMarkedAbelianGroup::getMRBi,
            return_internal_reference<>())
        .def("getMCB", &NMarkedAbelianGroup::getMCB,
            return_internal_reference<>())
        .def("getMCBi", &NMarkedAbelianGroup::getMCBi,
            return_internal_reference<>())
        .def("getNRB", &NMarkedAbelianGroup::getNRB,
            return_internal_reference<>())
        .def("getNRBi", &NMarkedAbelianGroup::getNRBi,
            return_internal_reference<>())
        .def("getNCB", &NMarkedAbelianGroup::getNCB,
            return_internal_reference<>())
        .def("getNCBi", &NMarkedAbelianGroup::getNCBi,
            return_internal_reference<>())
        .def("getRankOM", &NMarkedAbelianGroup::getRankOM)
        .def("getFreeLoc", &NMarkedAbelianGroup::getFreeLoc)
        .def("getTorLoc", &NMarkedAbelianGroup::getTorLoc)
        .def("getTorNum", &NMarkedAbelianGroup::getTorNum)
        .def("getOM", &NMarkedAbelianGroup::getOM,
            return_internal_reference<>())
        .def("getON", &NMarkedAbelianGroup::getON,
            return_internal_reference<>())
        .def(self == self)
    ;
}

