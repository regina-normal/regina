
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

/*! \file python/helpers.h
 *  \brief Various tools to assist with Python bindings for Regina.
 */

#ifndef __HELPERS_H
#ifndef __DOXYGEN
#define __HELPERS_H
#endif

/**
 * On some systems we get warnings about Regina's helper classes having
 * greater visibility than the pybind11 code that they use.  The macro
 * MATCH_PYBIND11_VISIBILITY allows us to fix this by setting the same
 * visibility attributes on our classes also.
 */
#ifdef __GNUG__
  #define MATCH_PYBIND11_VISIBILITY __attribute__((visibility("hidden")))
#else
  #define MATCH_PYBIND11_VISIBILITY
#endif

#include "pybind11/pybind11.h"
#include "helpers/equality.h"
#include "helpers/gil.h"
#include "helpers/output.h"
#include "helpers/listview.h"
#include "helpers/packet.h"
#include "helpers/tightencoding.h"

#endif
