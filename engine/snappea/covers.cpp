
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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

#include "algebra/abeliangroup.h"
#include "snappea/snappeatriangulation.h"
#include "snappea/kernel/kernel_prototypes.h"
#include <sstream>
#include <vector>

namespace regina {

std::string SnapPeaTriangulation::coverHash(int degree) const {
    if (! data_)
        return std::string(); // Error: Null triangulation.

    regina::snappea::RepresentationList* reps =
        regina::snappea::find_representations(data_, degree,
        regina::snappea::permutation_subgroup_Sn);

    std::vector<std::pair<int, AbelianGroup*>> covers;

    regina::snappea::RepresentationIntoSn* rep = reps->list;
    regina::snappea::Triangulation* cover;
    int i;
    while (rep) {
        cover = regina::snappea::construct_cover(data_, rep, reps->num_sheets);

        Triangulation<3> regCover;
        fillRegina(cover, regCover);
        regina::snappea::free_triangulation(cover);

        covers.push_back(std::make_pair(rep->covering_type,
            new AbelianGroup(regCover.homology())));

        rep = rep->next;
    }

    std::sort(covers.begin(), covers.end(),
        [](const std::pair<int, AbelianGroup*>& a,
           const std::pair<int, AbelianGroup*>& b) {
            // Compare cover types.
            if (a.first < b.first)
                return true;
            if (a.first > b.first)
                return false;

            // Compare ranks.
            if (a.second->rank() < b.second->rank())
                return true;
            if (a.second->rank() > b.second->rank())
                return false;

            // Compare number of torsion coefficients.
            if (a.second->countInvariantFactors() <
                    b.second->countInvariantFactors())
                return true;
            if (a.second->countInvariantFactors() >
                    b.second->countInvariantFactors())
                return false;

            // Compare the coefficients themselves.
            for (int i = 0; i < a.second->countInvariantFactors(); ++i) {
                if (a.second->invariantFactor(i) <
                        b.second->invariantFactor(i))
                    return true;
                if (a.second->invariantFactor(i) >
                        b.second->invariantFactor(i))
                    return false;
            }

            // Equal.
            return false;
        });

    std::ostringstream ans;
    if (covers.empty())
        ans << '_';
    else for (auto c : covers) {
        switch (c.first) {
            case regina::snappea::unknown_cover:
                ans << 'u'; break;
            case regina::snappea::irregular_cover:
                ans << 'i'; break;
            case regina::snappea::regular_cover:
                ans << 'r'; break;
            case regina::snappea::cyclic_cover:
                ans << 'c'; break;
        }
        ans << c.second->rank() << ':';
        for (int i = 0; i < c.second->countInvariantFactors(); ++i) {
            if (i > 0)
                ans << ',';
            ans << c.second->invariantFactor(i);
        }
        delete c.second;
    }

    regina::snappea::free_representation_list(reps);
    return ans.str();
}

} // namespace regina

