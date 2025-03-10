
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "snappea/snappeatriangulation.h"
#include "snappea/kernel/SnapPea.h"

namespace regina {

const GroupPresentation& SnapPeaTriangulation::fundamentalGroupFilled(
            bool simplifyPresentation,
            bool fillingsMayAffectGenerators,
            bool minimiseNumberOfGenerators,
            bool tryHardToShortenRelators) const {
    if (fundGroupFilled_.has_value())
        return *fundGroupFilled_;
    if (! data_)
        throw SnapPeaIsNull("SnapPeaTriangulation::fundamentalGroupFilled");

    // Note: TRUE and FALSE are #defines in SnapPea, and so don't live in any
    // namespace.  We avoid them here, and directly use 0 and 1 instead.

    // Pass all the work to SnapPea.
    regina::snappea::GroupPresentation* pres =
        regina::snappea::fundamental_group(data_,
            (simplifyPresentation ? 1 : 0),
            (fillingsMayAffectGenerators ? 1 : 0),
            (minimiseNumberOfGenerators ? 1 : 0),
            (tryHardToShortenRelators ? 1 : 0));

    // Convert the results into Regina's GroupPresentation class.
    // Note that SnapPea gives a sequence of generators, whereas Regina
    // gives a sequence of (generator, exponent) pairs.  Therefore we
    // "compress" relations below to group consecutive occurrences of
    // the same generator, even if simplifyPresentation is false.
    GroupPresentation ans;
    ans.addGenerator(regina::snappea::fg_get_num_generators(pres));
    for (int i = 0; i < regina::snappea::fg_get_num_relations(pres); ++i) {
        int* sReln = regina::snappea::fg_get_relation(pres, i);

        GroupExpression rReln;
        int currGen = 0;
        long currExp = 0;
        for (int* sPos = sReln; *sPos; ++sPos) {
            int gen = (*sPos > 0 ? *sPos : -*sPos);
            if (gen != currGen) {
                if (currExp)
                    rReln.addTermLast(currGen - 1, currExp);
                currGen = gen;
                currExp = 0;
            }
            if (*sPos > 0)
                ++currExp;
            else
                --currExp;
        }
        if (currExp)
            rReln.addTermLast(currGen - 1, currExp);
        ans.addRelation(std::move(rReln));
        regina::snappea::fg_free_relation(sReln);
    }

    regina::snappea::free_group_presentation(pres);
    return *(fundGroupFilled_ = std::move(ans));
}

} // namespace regina

