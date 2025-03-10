
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include <type_traits>
#include "utilities/tableview.h"

using regina::TableView;

static_assert(TableView<int, 3, 5>::dimension == 2);

static_assert(std::is_same_v<
    TableView<int, 3, 5>::Subview, TableView<int, 5>>);

static_assert(std::is_same_v<
    TableView<int, 3, 5>::Subarray, const int[5]>);

static_assert(std::is_same_v<
    TableView<int, 3, 5>::Array, const int[3][5]>);

static_assert(TableView<int, 3>::dimension == 1);

static_assert(std::is_same_v<
    TableView<int, 3>::Subview, const int&>);

static_assert(std::is_same_v<
    TableView<int, 3>::Subarray, const int>);

static_assert(std::is_same_v<
    TableView<int, 3>::Array, const int[3]>);

