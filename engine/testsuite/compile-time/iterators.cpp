
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "hypersurface/normalhypersurfaces.h"
#include "maths/permgroup.h"
#include "surface/disc.h"
#include "surface/normalsurfaces.h"
#include "utilities/tableview.h"

// Check that our hand-rolled iterator classes adhere to the requirements for
// their relevant iterator categories.

static_assert(std::forward_iterator<regina::ChildIterator<true>>);
static_assert(std::forward_iterator<regina::ChildIterator<false>>);
static_assert(std::forward_iterator<regina::SubtreeIterator<true>>);
static_assert(std::forward_iterator<regina::SubtreeIterator<false>>);

static_assert(std::bidirectional_iterator<
    regina::NormalSurfaces::VectorIterator>);
static_assert(std::bidirectional_iterator<
    regina::NormalHypersurfaces::VectorIterator>);

static_assert(std::random_access_iterator<regina::PermSn<2>::iterator>);
static_assert(std::random_access_iterator<regina::PermSn<3>::iterator>);
static_assert(std::random_access_iterator<regina::PermSn<4>::iterator>);
static_assert(std::random_access_iterator<regina::PermSn<5>::iterator>);
static_assert(std::random_access_iterator<regina::PermSn<6>::iterator>);
static_assert(std::random_access_iterator<regina::PermSn<7>::iterator>);
static_assert(std::forward_iterator<regina::PermSn<8>::iterator>);
static_assert(std::forward_iterator<regina::PermSn<16>::iterator>);

static_assert(std::input_iterator<regina::PermGroup<5>::iterator>);

static_assert(std::forward_iterator<
    regina::DiscSpecIterator<regina::DiscSetTet>>);

static_assert(std::input_iterator<regina::TableView<int, 4>::iterator>);
static_assert(std::input_iterator<regina::TableView<char, 2, 3>::iterator>);

// Check that iterator traits appear to be working as expected.

static_assert(std::is_same_v<
    typename std::iterator_traits<regina::ChildIterator<true>>::value_type,
    const regina::Packet>);
static_assert(std::is_same_v<
    std::iter_value_t<regina::ChildIterator<true>>,
    const regina::Packet>);

static_assert(std::is_same_v<
    typename std::iterator_traits<regina::ChildIterator<false>>::value_type,
    regina::Packet>);
static_assert(std::is_same_v<
    std::iter_value_t<regina::ChildIterator<false>>,
    regina::Packet>);

