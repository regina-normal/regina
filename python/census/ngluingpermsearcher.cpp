
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
#include "census/ngluingpermsearcher.h"
#include "triangulation/ntriangulation.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NGluingPermSearcher;

void addNGluingPermSearcher() {
    {
        scope s = class_<NGluingPermSearcher,
                std::auto_ptr<NGluingPermSearcher>,
                boost::noncopyable>("NGluingPermSearcher", no_init)
            .def(regina::python::add_eq_operators())
        ;

        enum_<regina::NGluingPermSearcher::PurgeFlags>("PurgeFlags")
            .value("PURGE_NONE",
                regina::NGluingPermSearcher::PURGE_NONE)
            .value("PURGE_NON_MINIMAL",
                regina::NGluingPermSearcher::PURGE_NON_MINIMAL)
            .value("PURGE_NON_PRIME",
                regina::NGluingPermSearcher::PURGE_NON_PRIME)
            .value("PURGE_NON_MINIMAL_PRIME",
                regina::NGluingPermSearcher::PURGE_NON_MINIMAL_PRIME)
            .value("PURGE_NON_MINIMAL_HYP",
                regina::NGluingPermSearcher::PURGE_NON_MINIMAL_HYP)
            .value("PURGE_P2_REDUCIBLE",
                regina::NGluingPermSearcher::PURGE_P2_REDUCIBLE)
            ;

        s.attr("PURGE_NONE") =
            NGluingPermSearcher::PURGE_NONE;
        s.attr("PURGE_NON_MINIMAL") =
            NGluingPermSearcher::PURGE_NON_MINIMAL;
        s.attr("PURGE_NON_PRIME") =
            NGluingPermSearcher::PURGE_NON_PRIME;
        s.attr("PURGE_NON_MINIMAL_PRIME") =
            NGluingPermSearcher::PURGE_NON_MINIMAL_PRIME;
        s.attr("PURGE_NON_MINIMAL_HYP") =
            NGluingPermSearcher::PURGE_NON_MINIMAL_HYP;
        s.attr("PURGE_P2_REDUCIBLE") =
            NGluingPermSearcher::PURGE_P2_REDUCIBLE;
    }

    scope global;

    global.attr("PURGE_NONE") =
        NGluingPermSearcher::PURGE_NONE;
    global.attr("PURGE_NON_MINIMAL") =
        NGluingPermSearcher::PURGE_NON_MINIMAL;
    global.attr("PURGE_NON_PRIME") =
        NGluingPermSearcher::PURGE_NON_PRIME;
    global.attr("PURGE_NON_MINIMAL_PRIME") =
        NGluingPermSearcher::PURGE_NON_MINIMAL_PRIME;
    global.attr("PURGE_NON_MINIMAL_HYP") =
        NGluingPermSearcher::PURGE_NON_MINIMAL_HYP;
    global.attr("PURGE_P2_REDUCIBLE") =
        NGluingPermSearcher::PURGE_P2_REDUCIBLE;
}

