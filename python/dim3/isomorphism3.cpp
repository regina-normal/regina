
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
#include "triangulation/dim3.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Isomorphism;
using regina::Perm;
using regina::Triangulation;

void addIsomorphism3(pybind11::module_& m) {
    auto c = pybind11::class_<Isomorphism<3>>(m, "Isomorphism3")
        .def(pybind11::init<const Isomorphism<3>&>())
        .def(pybind11::init<unsigned>())
        .def("swap", &Isomorphism<3>::swap)
        .def("size", &Isomorphism<3>::size)
        .def("simpImage", overload_cast<unsigned>(
            &Isomorphism<3>::simpImage, pybind11::const_))
        .def("setSimpImage", [](Isomorphism<3>& iso, unsigned s, int image) {
            iso.simpImage(s) = image;
        })
        .def("tetImage", overload_cast<unsigned>(
            &Isomorphism<3>::tetImage, pybind11::const_))
        .def("setTetImage", [](Isomorphism<3>& iso, unsigned s, int image) {
            iso.tetImage(s) = image;
        })
        .def("facetPerm", overload_cast<unsigned>(
            &Isomorphism<3>::facetPerm, pybind11::const_))
        .def("setFacetPerm", [](Isomorphism<3>& iso, unsigned s, Perm<4> p) {
            iso.facetPerm(s) = p;
        })
        .def("facePerm", overload_cast<unsigned>(
            &Isomorphism<3>::facePerm, pybind11::const_))
        .def("setFacePerm", [](Isomorphism<3>& iso, unsigned s, Perm<4> p) {
            iso.facePerm(s) = p;
        })
        .def("__getitem__", &Isomorphism<3>::operator[])
        .def("isIdentity", &Isomorphism<3>::isIdentity)
        .def("__call__", &Isomorphism<3>::operator())
        .def("apply", // deprecated
                [](const Isomorphism<3>& iso, const Triangulation<3>& tri) {
            return iso(tri);
        })
        .def("applyInPlace", // deprecated
                [](const Isomorphism<3>& iso, Triangulation<3>& tri) {
            tri = iso(tri);
        })
        .def(pybind11::self * pybind11::self)
        .def("inverse", &Isomorphism<3>::inverse)
        .def_static("random", &Isomorphism<3>::random,
            pybind11::arg(), pybind11::arg("even") = false)
        .def_static("identity", &Isomorphism<3>::identity)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.def("swap", (void(*)(Isomorphism<3>&, Isomorphism<3>&))(regina::swap));
}

