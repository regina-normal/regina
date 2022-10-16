
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

#include "utilities/tableview.h"

namespace regina::python {

/**
 * Adds Python bindings for one of Regina's TableView classes, if this
 * has not been done already.
 *
 * The class to be wrapped will be regina::TableView<Element, dim1, dim...>.
 * The element type and the table dimensions must be explicitly specified
 * in the template arguments for addTableView().
 *
 * The corresponding Python class will not be given a unique name;
 * instead all such types will be called \c TableView, and will be put into
 * their own unique namespaces to avoid clashes.
 *
 * If this TableView class has already been wrapped in Python, then this
 * routine will do nothing (i.e., it is safe to call this routine
 * multiple times for the same TableView class).  This is to support
 * wrapTableView(), which might be used to wrap several global constant
 * arrays of the same type and dimensions.  Note that this behaviour is
 * different from addListView, which will throw an exception if called for
 * the same type more than once.
 *
 * Return value policies work as follows:
 *
 * - When accessing a table element (e.g., via iteration or indexing), the
 *   relationship between the table element and the TableView will be as
 *   follows.  Table elements that are pointers will use a \c reference_internal
 *   policy, and thus keep the TableView alive; table elements that are values
 *   will use a \c copy policy, and thus will have independent lifespans.
 *   At present, it is not possible to override these policies.
 *
 * - If you need the table elements to keep the underlying container alive,
 *   you will need to indicate this when wrapping whatever function returns a
 *   TableView.  This is generally not necessary, since TableView objects
 *   in Regina typically refer to global constant arrays; however, for further
 *   discussion on this issue you can see the addListView() documentation.
 */
template <typename Element, int dim1, int... dim>
void addTableView(pybind11::module_& m) {
    static constexpr pybind11::return_value_policy Policy =
        (std::is_pointer<Element>::value ?
            pybind11::return_value_policy::reference_internal :
            pybind11::return_value_policy::copy);

    using T = regina::TableView<Element, dim1, dim...>;

    // Do not wrap T if this has been done already.
    if (pybind11::detail::get_local_type_info(typeid(T)))
        return;

    // Wrap any subtables as necessary.
    if constexpr (T::dimension > 1)
        addTableView<Element, dim...>(m);

    // Instead of naming these classes uniquely, just call them all TableView
    // and make them all local to their own unique Python namespaces.
    // End users should not be constructing them anyway.
    auto c = pybind11::class_<T>(pybind11::handle(), "TableView",
            pybind11::module_local())
        .def(pybind11::init<const T&>())
        .def("size", &T::size)
        .def("__len__", [](const T&) {
            return T::size().front();
        })
        .def("__getitem__", [](const T& view, size_t index) {
            if (index >= view.size().front())
                throw pybind11::index_error("TableView index out of range");
            return view[index];
        }, Policy)
        ;
    c.attr("dimension") = T::dimension;
    regina::python::add_output_custom(c, [](const T& view, std::ostream& out) {
        out << "[ ";
        // For very small arrays, output the entire array.
        // For larger arrays, do not output everything.
        if (view.size().front() == 0) {
            out << "[ ]";
        } else if (view.size().front() <= 5) {
            bool started = false;
            for (size_t i = 0; i < view.size().front(); ++i) {
                if (started)
                    out << ", ";
                else
                    started = true;
                regina::python::writeStr(out, view[i]);
            }
            out << ' ';
        } else {
            for (int i = 0; i < 3; ++i) {
                regina::python::writeStr(out, view[i]);
                out << ", ";
            }
            out << "..., ";
            regina::python::writeStr(out, view[view.size().front() - 1]);
            out << ' ';
        }
        out << ']';
    });
    regina::python::add_eq_operators(c);
}

/**
 * Adds Python bindings for one of Regina's 1-D TableView classes, and wraps
 * the given fixed-size C-style array in such a TableView.  Typically the
 * given array would be a global constant array.
 *
 * The given array should be a C-style array of compile-time-constant
 * dimension \a dim, whose elements are of type \a Element.
 * Typically these template arguments \a Element and \a dim would be deduced
 * automatically, and would not need to be supplied with this function call.
 *
 * This routine has the effect of (i) creating a TableView of the appropriate
 * type to wrap \a array; (ii) wrapping this TableView class in Python, if
 * it has not been wrapped already; and then (iii) returning this TableView
 * object so that it can be set as a class attribute or global constant.
 *
 * The default return value policies supplied by addTableView() will be used,
 * and it is not possible to override them here.  See addTableView() for
 * further details.
 */
template <typename Element, int dim1>
regina::TableView<Element, dim1> wrapTableView(pybind11::module_& m,
        const Element (&array)[dim1]) {
    addTableView<Element, dim1>(m);
    // Remember: TableView is lightweight and cheap to pass by value.
    return regina::TableView(array);
}

/**
 * Adds Python bindings for one of Regina's 2-D TableView classes, and wraps
 * the given fixed-size C-style array in such a TableView.  Typically the
 * given array would be a global constant array.
 *
 * The given array should be a C-style 3-D array of compile-time-constant
 * dimensions \a dim1 and \a dim2, whose elements are of type \a Element.
 * Typically these template arguments \a Element, \a dim1 and \a dim2 would
 * be deduced automatically, and would not need to be supplied with this
 * function call.
 *
 * This routine has the effect of (i) creating a TableView of the appropriate
 * type to wrap \a array; (ii) wrapping this TableView class and all necessary
 * sub-table classes in Python, if they have not been wrapped already; and
 * then (iii) returning this TableView object so that it can be set as a
 * class attribute or global constant.
 *
 * The default return value policies supplied by addTableView() will be used,
 * and it is not possible to override them here.  See addTableView() for
 * further details.
 */
template <typename Element, int dim1, int dim2>
regina::TableView<Element, dim1, dim2> wrapTableView(pybind11::module_& m,
        const Element (&array)[dim1][dim2]) {
    addTableView<Element, dim1, dim2>(m);
    // Remember: TableView is lightweight and cheap to pass by value.
    return regina::TableView(array);
}

/**
 * Adds Python bindings for one of Regina's 3-D TableView classes, and wraps
 * the given fixed-size C-style array in such a TableView.  Typically the
 * given array would be a global constant array.
 *
 * The given array should be a C-style 3-D array of compile-time-constant
 * dimensions \a dim1, \a dim2 and \a dim3, whose elements are of type
 * \a Element.  Typically these template arguments \a Element, \a dim1,
 * \a dim2 and \a dim3 would be deduced automatically, and would not need
 * to be supplied with this function call.
 *
 * This routine has the effect of (i) creating a TableView of the appropriate
 * type to wrap \a array; (ii) wrapping this TableView class and all necessary
 * sub-table classes in Python, if they have not been wrapped already; and
 * then (iii) returning this TableView object so that it can be set as a
 * class attribute or global constant.
 *
 * The default return value policies supplied by addTableView() will be used,
 * and it is not possible to override them here.  See addTableView() for
 * further details.
 */
template <typename Element, int dim1, int dim2, int dim3>
regina::TableView<Element, dim1, dim2, dim3> wrapTableView(pybind11::module_& m,
        const Element (&array)[dim1][dim2][dim3]) {
    addTableView<Element, dim1, dim2, dim3>(m);
    // Remember: TableView is lightweight and cheap to pass by value.
    return regina::TableView(array);
}

} // namespace regina::python

