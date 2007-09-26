
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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
        .def("getMRB", &NMarkedAbelianGroup::getMRB)
        .def("getMRBi", &NMarkedAbelianGroup::getMRBi)
        .def("getMCB", &NMarkedAbelianGroup::getMCB)
        .def("getMCBi", &NMarkedAbelianGroup::getMCBi)
        .def("getNRB", &NMarkedAbelianGroup::getNRB)
        .def("getNRBi", &NMarkedAbelianGroup::getNRBi)
        .def("getNCB", &NMarkedAbelianGroup::getNCB)
        .def("getNCBi", &NMarkedAbelianGroup::getNCBi)
        .def("getRankOM", &NMarkedAbelianGroup::getRankOM)
        .def("getFreeLoc", &NMarkedAbelianGroup::getFreeLoc)
        .def("getTorLoc", &NMarkedAbelianGroup::getTorLoc)
        .def("getTorNum", &NMarkedAbelianGroup::getTorNum)
        .def("getOM", &NMarkedAbelianGroup::getOM)
        .def("getON", &NMarkedAbelianGroup::getON)
        .def(self == self)
    ;
}

