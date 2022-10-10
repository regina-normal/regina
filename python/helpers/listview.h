
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
 * Adds Python bindings for one of Regina's ListView classes.
 *
 * The type \a T must be of the form regina::ListView<T>.
 *
 * Typically you would not write out the full type explicitly (since this is
 * an implementation detail that may change in future versions of Regina);
 * instead you would access the type using an appropriate decltype() statement.
 *
 * The Python class corresponding to \a T will not be given a unique name;
 * instead all such types will be called \c ListView, and will be put into
 * their own unique namespaces to avoid clashes.
 *
 * Return value policies work as follows:
 *
 * - When accessing a list element (e.g., through front() or back(), or
 *   via iteration or indexing), the relationship between the list element
 *   and the ListView will be defined by the template argument \a Policy.
 *   By default, list elements that are pointers (e.g., when calling
 *   Triangulation::vertices()) will use a \c reference_internal policy,
 *   and thus keep the ListView alive; list elements that are values
 *   (e.g., when calling Vertex::embeddings()) will use a \c copy policy,
 *   and thus will have independent lifespans.
 *
 * - If you need the list elements to keep the underlying container alive,
 *   you will need to indicate this when wrapping whatever function returns a
 *   ListView.  Since ListView objects are passed by value, you cannot
 *   use \c reference_internal; instead you should pass an appropriate
 *   \c keep_alive argument.  For example, Triangulation::vertices()
 *   would use pybind11::keep_alive<0,1>().  Note that this assumes that the
 *   list elements are pointers (or if not, the \a Policy argument has been set
 *   appropriately): thus the list elements keep the ListView alive, and
 *   in turn the ListView keeps the container alive.
 */
template <class T, pybind11::return_value_policy Policy =
    (std::is_pointer<typename T::value_type>::value ?
        pybind11::return_value_policy::reference_internal :
        pybind11::return_value_policy::copy)>
void addListView(pybind11::module_& m) {
    // Instead of naming these classes uniquely, just call them all ListView
    // and make them all local to their own unique Python namespaces.
    // End users should not be constructing them anyway.
    auto c = pybind11::class_<T>(pybind11::handle(), "ListView",
            pybind11::module_local(), doc::common::ListView)
        .def(pybind11::init<const T&>(), doc::common::ListView_ListView)
        .def("__iter__", [](const T& view) {
            return pybind11::make_iterator<Policy>(view.begin(), view.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps ListView alive
            doc::common::iter)
        .def("__getitem__", [](const T& view, size_t index) {
            return view[index];
        }, Policy, doc::common::ListView_array)
        .def("empty", &T::empty, doc::common::ListView_empty)
        .def("size", &T::size, doc::common::ListView_size)
        .def("front", &T::front, Policy, doc::common::ListView_front)
        .def("back", &T::back, Policy, doc::common::ListView_back)
        ;
    regina::python::add_eq_operators(c,
        doc::common::ListView_eq, doc::common::ListView_neq);
}

} // namespace regina::python

