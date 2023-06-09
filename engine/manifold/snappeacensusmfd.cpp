
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "file/globaldirs.h"
#include "manifold/snappeacensusmfd.h"
#include "subcomplex/snappeacensustri.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

#include <cstdio>
#include <cstring>

namespace regina {

namespace {
    int homDecode(char c) {
        if (c >= 'a' && c <= 'z')
            return c - 'a';
        if (c >= 'A' && c <= 'Z')
            return c - 'A' + 26;
        return -1;
    }
}

Triangulation<3> SnapPeaCensusManifold::construct() const {
    // Hard-code a few special cases so that the numbering of tetrahedra
    // and vertices is compatible with earlier versions of Regina.
    if (section_ == SEC_5) {
        if (index_ == 0) {
            return Example<3>::gieseking();
        } else if (index_ == 1) {
            Triangulation<3> ans;
            auto [r, s] = ans.newTetrahedra<2>();
            r->join(0, s, Perm<4>(0, 1, 3, 2));
            r->join(1, s, Perm<4>(2, 3, 1, 0));
            r->join(2, s, Perm<4>(3, 2, 1, 0));
            r->join(3, s, Perm<4>(1, 0, 3, 2));
            return ans;
        } else if (index_ == 2) {
            Triangulation<3> ans;
            auto [r, s] = ans.newTetrahedra<2>();
            r->join(0, s, Perm<4>(0, 1, 3, 2));
            r->join(1, s, Perm<4>(3, 1, 2, 0));
            r->join(2, s, Perm<4>(2, 1, 3, 0));
            r->join(3, s, Perm<4>(3, 1, 0, 2));
            return ans;
        } else if (index_ == 3) {
            Triangulation<3> ans;
            auto [r, s] = ans.newTetrahedra<2>();
            r->join(0, s, Perm<4>(0, 1, 3, 2));
            r->join(1, s, Perm<4>(2, 1, 0, 3));
            r->join(2, s, Perm<4>(0, 3, 2, 1));
            r->join(3, s, Perm<4>(1, 0, 2, 3));
            return ans;
        } else if (index_ == 4) {
            return Example<3>::figureEight();
        } else if (index_ == 129) {
            return Example<3>::whitehead();
        }
    }

    // Fetch the relevant data from the census dehydration files.
    std::string file = GlobalDirs::data() + "/snappea";
    switch (section_) {
        case SEC_5:
            file += "/snappea-census-sec5.dat"; break;
        case SEC_6_OR:
            file += "/snappea-census-sec6o.dat"; break;
        case SEC_6_NOR:
            file += "/snappea-census-sec6n.dat"; break;
        case SEC_7_OR:
            file += "/snappea-census-sec7o.dat"; break;
        case SEC_7_NOR:
            file += "/snappea-census-sec7n.dat"; break;
        default:
            throw regina::FileError("The requested section of the "
                "SnapPea census database does not correspond to "
                "any installed database file");
    }

    FILE* dat = fopen(file.c_str(), "r");
    if (! dat)
        throw regina::FileError("Cannot open SnapPea census database");
    char tri[30], hom[30]; /* Long enough to deal with the snappea census
                              files for <= 7 tetrahedra. */
    for (size_t i = 0; i <= index_; ++i) {
        if (fscanf(dat, "%s%s", tri, hom) != 2) {
            if (feof(dat))
                throw regina::FileError("Attempted to read beyond the "
                    "end of the SnapPea census database");
            else
                throw regina::FileError("Could not read the contents of the "
                    "SnapPea census database");
        }
    }
    fclose(dat);

    try {
        return Triangulation<3>::rehydrate(tri);
    } catch (const InvalidArgument&) {
        throw regina::FileError("The dehydration string read from the "
            "SnapPea census database was invalid");
    }
}

AbelianGroup SnapPeaCensusManifold::homology() const {
    // Fetch the relevant data from the census dehydration files.
    std::string file = GlobalDirs::data() + "/snappea";
    switch (section_) {
        case SEC_5:
            file += "/snappea-census-sec5.dat"; break;
        case SEC_6_OR:
            file += "/snappea-census-sec6o.dat"; break;
        case SEC_6_NOR:
            file += "/snappea-census-sec6n.dat"; break;
        case SEC_7_OR:
            file += "/snappea-census-sec7o.dat"; break;
        case SEC_7_NOR:
            file += "/snappea-census-sec7n.dat"; break;
        default:
            throw regina::FileError("The requested section of the "
                "SnapPea census database does not correspond to "
                "any installed database file");
    }

    FILE* dat = fopen(file.c_str(), "r");
    if (! dat)
        throw regina::FileError("Cannot open SnapPea census database");
    char tri[30], hom[30]; /* Long enough to deal with the snappea census
                              files for <= 7 tetrahedra. */
    for (size_t i = 0; i <= index_; ++i) {
        if (fscanf(dat, "%s%s", tri, hom) != 2) {
            if (feof(dat))
                throw regina::FileError("Attempted to read beyond the "
                    "end of the SnapPea census database");
            else
                throw regina::FileError("Could not read the contents of the "
                    "SnapPea census database");
        }
    }
    fclose(dat);

    AbelianGroup ans;
    char* c;
    int val;

    // First character of the homology string represents rank.
    val = homDecode(hom[0]); // Empty string is picked up and dealt with here.
    if (val < 0)
        throw regina::FileError("Read invalid rank from the "
            "SnapPea census database");
    ans.addRank(val);

    // The remaining characters represent torsion.
    for (c = hom + 1; *c; ++c) {
        val = homDecode(*c);
        if (val < 0)
            throw regina::FileError("Read invalid torsion from the "
                "SnapPea census database");
        ans.addTorsion(val);
    }
    return ans;
}

std::ostream& SnapPeaCensusManifold::writeName(std::ostream& out) const {
    // Some manifolds will get special names, and will have their usual
    // SnapPea names written in writeStructure() instead.
    if (section_ == SEC_5) {
        if (index_ == 0)
            return out << "Gieseking manifold";
        if (index_ == 4)
            return out << "Figure eight knot complement";
        if (index_ == 129)
            return out << "Whitehead link complement";
    }

    // No special names, just the usual SnapPea notation.
    return SnapPeaCensusTri(section_, index_).writeName(out);
}

std::ostream& SnapPeaCensusManifold::writeTeXName(std::ostream& out) const {
    return SnapPeaCensusTri(section_, index_).writeTeXName(out);
}

std::ostream& SnapPeaCensusManifold::writeStructure(std::ostream& out) const {
    // If we didn't give the usual SnapPea name in writeName(), give it here.
    if (section_ == SEC_5) {
        if (index_ == 0 || index_ == 4 || index_ == 129)
            return SnapPeaCensusTri(section_, index_).writeName(out);
    }

    return out;
}

} // namespace regina

