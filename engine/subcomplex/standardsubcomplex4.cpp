
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "subcomplex/snappedball4.h"
#include "subcomplex/standardsubcomplex.h"

namespace regina {

template <>
std::unique_ptr<StandardSubcomplex<4>> StandardSubcomplex<4>::recognise(
        Component<4>* comp) {
    if (comp->size() == 1) {
        if (auto ans = SnappedBall4::recognise(comp->pentachoron(0)))
            return ans;
    }
    // Fill in the rest...

    return nullptr;
}

template <>
std::unique_ptr<StandardSubcomplex<4>> StandardSubcomplex<4>::recognise(
        const Triangulation<4>& tri) {
    if (tri.countComponents() != 1)
        return nullptr;

    // Do what we can through components.
    if (auto ans = recognise(tri.component(0)))
        return ans;

    // If there are any tests that require full triangluations (i.e., that
    // cannot work with component), include them here.

    return nullptr;
}

} // namespace regina

