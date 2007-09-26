
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

#include "algebra/marked_abeliangroup.h"
#include "maths/nmatrixint.h"
#include <boost/python.hpp>
#include <boost/python/detail/api_placeholder.hpp> // For len().

using namespace boost::python;
using regina::MarkedAbelianGroup;
using regina::NMatrixInt;

namespace {
    unsigned (MarkedAbelianGroup::*getTorsionRank_large)(
        const regina::NLargeInteger&) const = &MarkedAbelianGroup::getTorsionRank;
    unsigned (MarkedAbelianGroup::*getTorsionRank_long)(unsigned long)
        const = &MarkedAbelianGroup::getTorsionRank;
}

void addNMarkedAbelianGroup() {
    class_<MarkedAbelianGroup, bases<regina::ShareableObject>,
            std::auto_ptr<MarkedAbelianGroup>, boost::noncopyable>(
            "MarkedAbelianGroup", init<const NMatrixInt&, const NMatrixInt&>())
        .def(init<const MarkedAbelianGroup&>())
        .def("getRank", &MarkedAbelianGroup::getRank)
        .def("getTorsionRank", getTorsionRank_large)
        .def("getTorsionRank", getTorsionRank_long)
        .def("getNumberOfInvariantFactors",
            &MarkedAbelianGroup::getNumberOfInvariantFactors)
        .def("getInvariantFactor", &MarkedAbelianGroup::getInvariantFactor,
            return_value_policy<return_by_value>())
        .def("isTrivial", &MarkedAbelianGroup::isTrivial)
        .def("getMRB", &MarkedAbelianGroup::getMRB)
        .def("getMRBi", &MarkedAbelianGroup::getMRBi)
        .def("getMCB", &MarkedAbelianGroup::getMCB)
        .def("getMCBi", &MarkedAbelianGroup::getMCBi)
        .def("getNRB", &MarkedAbelianGroup::getNRB)
        .def("getNRBi", &MarkedAbelianGroup::getNRBi)
        .def("getNCB", &MarkedAbelianGroup::getNCB)
        .def("getNCBi", &MarkedAbelianGroup::getNCBi)
        .def("getRankOM", &MarkedAbelianGroup::getRankOM)
        .def("getFreeLoc", &MarkedAbelianGroup::getFreeLoc)
        .def("getTorLoc", &MarkedAbelianGroup::getTorLoc)
        .def("getTorNum", &MarkedAbelianGroup::getTorNum)
        .def("getOM", &MarkedAbelianGroup::getOM)
        .def("getON", &MarkedAbelianGroup::getON)
        .def(self == self)
    ;
}

