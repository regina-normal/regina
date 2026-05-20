
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file python/helpers/globals.h
 *  \brief Assists with safely wrapping heavily overloaded functions.
 */

/**
 * Adds a Python binding for the global function `regina::swap(type&, type&)`,
 * assuming the docstring can be accessed via the "usual" syntax.
 * Specifically, the docstring will be accessed via the syntax
 * `regina::python::doc::global_swap_SUFFIX`, where \a SUFFIX is replaced with
 * the argument \a type.
 *
 * This macro adds safety by using a `static_cast` (not a C-style cast) to
 * resolve the overloaded `regina::swap`, so that there is no risk of
 * accidentally binding the wrong swap function.
 */
#define ADD_GLOBAL_SWAP(module, type) \
    module.def("swap", static_cast<void(&)(type&, type&)>(regina::swap), \
    regina::python::doc::global_swap_ ## type)

/**
 * Adds a Python binding for the global function `regina::swap(type&, type&)`,
 * with explicit instructions on how to locate the docstring.
 * Specifically, the docstring will be accessed via the syntax
 * `regina::python::doc::global_swap_SUFFIX`, where \a SUFFIX is replaced with
 * the extra argument \a suffix.
 *
 * Like ADD_GLOBAL_SWAP(), this macro adds safety by using a `static_cast`
 * (not a C-style cast) to resolve the overloaded `regina::swap`, so that
 * there is no risk of accidentally binding the wrong swap function.
 */
#define ADD_GLOBAL_SWAP_SUFFIX(module, type, suffix) \
    module.def("swap", static_cast<void(&)(type&, type&)>(regina::swap), \
    regina::python::doc::global_swap_ ## suffix)

