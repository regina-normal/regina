
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

/*! \file python/helpers/listview.h
 *  \brief Assists with wrapping instances of Regina's lightweight ListView
 *  template class.
 */

#include "utilities/listview.h"

namespace regina::python {

/**
 * TODO: Document
 */
template <class T, pybind11::return_value_policy Policy =
    pybind11::return_value_policy::reference_internal>
void addListView(pybind11::module_& m) {
    // Instead of naming these classes uniquely, just call them all ListView
    // and make them all local to their own unique Python namespaces.
    // End users should not be constructing them anyway.
    auto c = pybind11::class_<T>(pybind11::handle(), "ListView",
            pybind11::module_local())
        .def(pybind11::init<const T&>())
        .def("__iter__", [](const T& view) {
            return pybind11::make_iterator<Policy>(view.begin(), view.end());
        }, pybind11::keep_alive<0, 1>()) // iterator keeps ListView alive
        .def("__getitem__", [](const T& view, size_t index) {
            return view[index];
        }, Policy)
        .def("empty", &T::empty)
        .def("size", &T::size)
        .def("front", &T::front, Policy)
        .def("back", &T::back, Policy)
        ;
    regina::python::add_eq_operators(c);
}

// TODO: patient/nurse with list
// TODO: Change the way we manage Policy
// TODO: Get rid of BeginEndIterator

} // namespace regina::python

