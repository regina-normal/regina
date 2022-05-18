
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

#include "../pybind11/pybind11.h"
#include "../pybind11/operators.h"
#include "../pybind11/stl.h"
#include "regina-config.h" // for REGINA_HIGHDIM
#include "triangulation/cut.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/facetpairing.h"
#include "triangulation/facetpairing3.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Cut;
using regina::FacetPairing;
using regina::Triangulation;

void addCut(pybind11::module_& m) {
    auto c = pybind11::class_<Cut>(m, "Cut")
        .def(pybind11::init<size_t>())
        .def(pybind11::init<size_t, size_t>())
        .def(pybind11::init<const Cut&>())
        .def(pybind11::init([](const std::vector<int> sides) {
            return new Cut(sides.begin(), sides.end());
        }))
        .def("side", &Cut::side)
        .def("set", &Cut::set)
        .def("size", &Cut::size)
        .def("weight", overload_cast<const Triangulation<2>&>(
            &Cut::weight<2>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<3>&>(
            &Cut::weight<3>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<4>&>(
            &Cut::weight<4>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<5>&>(
            &Cut::weight<5>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<6>&>(
            &Cut::weight<6>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<7>&>(
            &Cut::weight<7>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<8>&>(
            &Cut::weight<8>, pybind11::const_))
#ifdef REGINA_HIGHDIM
        .def("weight", overload_cast<const Triangulation<9>&>(
            &Cut::weight<9>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<10>&>(
            &Cut::weight<10>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<11>&>(
            &Cut::weight<11>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<12>&>(
            &Cut::weight<12>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<13>&>(
            &Cut::weight<13>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<14>&>(
            &Cut::weight<14>, pybind11::const_))
        .def("weight", overload_cast<const Triangulation<15>&>(
            &Cut::weight<15>, pybind11::const_))
#endif /* REGINA_HIGHDIM */
        .def("weight", overload_cast<const FacetPairing<2>&>(
            &Cut::weight<2>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<3>&>(
            &Cut::weight<3>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<4>&>(
            &Cut::weight<4>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<5>&>(
            &Cut::weight<5>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<6>&>(
            &Cut::weight<6>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<7>&>(
            &Cut::weight<7>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<8>&>(
            &Cut::weight<8>, pybind11::const_))
#ifdef REGINA_HIGHDIM
        .def("weight", overload_cast<const FacetPairing<9>&>(
            &Cut::weight<9>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<10>&>(
            &Cut::weight<10>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<11>&>(
            &Cut::weight<11>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<12>&>(
            &Cut::weight<12>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<13>&>(
            &Cut::weight<13>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<14>&>(
            &Cut::weight<14>, pybind11::const_))
        .def("weight", overload_cast<const FacetPairing<15>&>(
            &Cut::weight<15>, pybind11::const_))
#endif /* REGINA_HIGHDIM */
        // TODO: ()
        .def("inclusion", [](const Cut& c, int dim) {
            switch (dim) {
                case 2: return nullptr; // TODO
                case 3: return nullptr; // TODO
                case 4: return nullptr; // TODO
                case 5: return nullptr; // TODO
                case 6: return nullptr; // TODO
                case 7: return nullptr; // TODO
                case 8: return nullptr; // TODO
#ifdef REGINA_HIGHDIM
                case 9: return nullptr; // TODO
                case 10: return nullptr; // TODO
                case 11: return nullptr; // TODO
                case 12: return nullptr; // TODO
                case 13: return nullptr; // TODO
                case 14: return nullptr; // TODO
                case 15: return nullptr; // TODO
                default:
                    throw regina::InvalidArgument("The argument to "
                        "Cut::inclusion() must be one of the supported "
                        "dimensions 2..15.");
#else /* REGINA_HIGHDIM */
                default:
                    throw regina::InvalidArgument("The argument to "
                        "Cut::inclusion() must be one of the supported "
                        "dimensions 2..8.");
#endif /* REGINA_HIGHDIM */
            }
        })
        .def("swap", &Cut::swap)
        .def("inc", &Cut::inc)
        .def("incFixedSizes", &Cut::incFixedSizes)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.def("swap", (void(*)(Cut&, Cut&))(regina::swap));
}

