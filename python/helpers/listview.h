
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

namespace pybind11::detail {

/**
 * Tell pybind11 how to convert a C++ ListView into a Python list.
 * This allows pybind11 to automagically convert the return values for
 * functions such as Triangulation<dim>::faces<subdim>().
 */
template <class List>
struct type_caster<regina::ListView<List>> {
    private:
        typedef regina::ListView<List> ReginaType;

    public:
        PYBIND11_TYPE_CASTER(ReginaType, _("ListView"));

        bool load(handle, bool) {
            // Never allow conversion from Python to a C++ ListView.
            return false;
        }

        static handle cast(ReginaType src, return_value_policy policy,
                handle parent) {
            // Conversion from C++ to Python:
            pybind11::list ans;
            for (auto f : src)
                ans.append(pybind11::cast(f, policy, parent));
            return ans.release();
        }
};

} // namespace pybind11::detail

