
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
        .def("size", overload_cast<>(&Cut::size, pybind11::const_))
        .def("size", overload_cast<int>(&Cut::size, pybind11::const_))
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
        .def("__call__", overload_cast<const Triangulation<2>&>(
            &Cut::operator()<2>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<3>&>(
            &Cut::operator()<3>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<4>&>(
            &Cut::operator()<4>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<5>&>(
            &Cut::operator()<5>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<6>&>(
            &Cut::operator()<6>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<7>&>(
            &Cut::operator()<7>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<8>&>(
            &Cut::operator()<8>, pybind11::const_))
#ifdef REGINA_HIGHDIM
        .def("__call__", overload_cast<const Triangulation<9>&>(
            &Cut::operator()<9>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<10>&>(
            &Cut::operator()<10>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<11>&>(
            &Cut::operator()<11>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<12>&>(
            &Cut::operator()<12>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<13>&>(
            &Cut::operator()<13>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<14>&>(
            &Cut::operator()<14>, pybind11::const_))
        .def("__call__", overload_cast<const Triangulation<15>&>(
            &Cut::operator()<15>, pybind11::const_))
#endif /* REGINA_HIGHDIM */
        .def("__call__", overload_cast<const FacetPairing<2>&>(
            &Cut::operator()<2>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<3>&>(
            &Cut::operator()<3>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<4>&>(
            &Cut::operator()<4>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<5>&>(
            &Cut::operator()<5>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<6>&>(
            &Cut::operator()<6>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<7>&>(
            &Cut::operator()<7>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<8>&>(
            &Cut::operator()<8>, pybind11::const_))
#ifdef REGINA_HIGHDIM
        .def("__call__", overload_cast<const FacetPairing<9>&>(
            &Cut::operator()<9>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<10>&>(
            &Cut::operator()<10>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<11>&>(
            &Cut::operator()<11>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<12>&>(
            &Cut::operator()<12>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<13>&>(
            &Cut::operator()<13>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<14>&>(
            &Cut::operator()<14>, pybind11::const_))
        .def("__call__", overload_cast<const FacetPairing<15>&>(
            &Cut::operator()<15>, pybind11::const_))
#endif /* REGINA_HIGHDIM */
        .def("inclusion", [](const Cut& c, int dim) {
            // We use pybind11::cast() here so that the return type is
            // consistent; otherwise the C++ compiler will complain.
            switch (dim) {
                case 2: return pybind11::cast(c.inclusion<2>());
                case 3: return pybind11::cast(c.inclusion<3>());
                case 4: return pybind11::cast(c.inclusion<4>());
                case 5: return pybind11::cast(c.inclusion<5>());
                case 6: return pybind11::cast(c.inclusion<6>());
                case 7: return pybind11::cast(c.inclusion<7>());
                case 8: return pybind11::cast(c.inclusion<8>());
#ifdef REGINA_HIGHDIM
                case 9: return pybind11::cast(c.inclusion<9>());
                case 10: return pybind11::cast(c.inclusion<10>());
                case 11: return pybind11::cast(c.inclusion<11>());
                case 12: return pybind11::cast(c.inclusion<12>());
                case 13: return pybind11::cast(c.inclusion<13>());
                case 14: return pybind11::cast(c.inclusion<14>());
                case 15: return pybind11::cast(c.inclusion<15>());
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

