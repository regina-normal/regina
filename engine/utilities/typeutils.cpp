
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2022, Ben Burton                                   *
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

#include "regina-config.h" // for REGINA_HIGHDIM
#include "link/link.h"
#include "angle/anglestructures.h"
#include "hypersurface/normalhypersurfaces.h"
#include "snappea/snappeatriangulation.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/facetpairing.h"
#include "triangulation/facetpairing3.h"
#include "triangulation/generic.h"
#include "utilities/typeutils.h"
#include <unordered_map>

using TypeMap = std::unordered_map<size_t, std::string>;

#ifdef REGINA_HIGHDIM
  #define REGINA_MAXDIM 15
#else
  #define REGINA_MAXDIM 8
#endif

namespace {
    TypeMap* pythonNames = nullptr;

    void initPythonNames() {
        pythonNames = new TypeMap;

        using namespace regina;

        pythonNames->emplace(typeid(IntegerBase<true>).hash_code(),
            "regina.LargeInteger");
        pythonNames->emplace(typeid(IntegerBase<false>).hash_code(),
            "regina.Integer");

        // We use the hard-coded dimension strings to build names for
        // Perm2..Perm15.
        for_constexpr<2, 16>([](auto i) {
            pythonNames->emplace(typeid(Perm<i.value>).hash_code(),
                std::string("regina.Perm") +
                    regina::detail::Strings<i.value>::dim);
        });
        // Perm16 does not have a hard-coded dimension string, so do
        // this separately.
        pythonNames->emplace(typeid(Perm<16>).hash_code(), "regina.Perm16");

        // TODO: Use Vertex etc instead of Face/Simplex

        for_constexpr<2, REGINA_MAXDIM + 1>([](auto i) {
            pythonNames->emplace(typeid(Triangulation<i.value>).hash_code(),
                std::string("regina.Triangulation") +
                    regina::detail::Strings<i.value>::dim);
            pythonNames->emplace(typeid(Component<i.value>).hash_code(),
                std::string("regina.Component") +
                    regina::detail::Strings<i.value>::dim);
            pythonNames->emplace(typeid(BoundaryComponent<i.value>).hash_code(),
                std::string("regina.BoundaryComponent") +
                    regina::detail::Strings<i.value>::dim);
            pythonNames->emplace(typeid(Face<i.value, i.value>).hash_code(),
                std::string("regina.Simplex") +
                    regina::detail::Strings<i.value>::dim);
            pythonNames->emplace(typeid(Isomorphism<i.value>).hash_code(),
                std::string("regina.Isomorphism") +
                    regina::detail::Strings<i.value>::dim);
            pythonNames->emplace(typeid(FacetSpec<i.value>).hash_code(),
                std::string("regina.FacetSpec") +
                    regina::detail::Strings<i.value>::dim);
            pythonNames->emplace(typeid(FacetPairing<i.value>).hash_code(),
                std::string("regina.FacetPairing") +
                    regina::detail::Strings<i.value>::dim);
            // We see FacetPairingBase show up in some swap() functions.
            pythonNames->emplace(
                typeid(regina::detail::FacetPairingBase<i.value>).hash_code(),
                std::string("regina.FacetPairing") +
                    regina::detail::Strings<i.value>::dim);

            pythonNames->emplace(
                typeid(decltype(Triangulation<i.value>().components())).
                    hash_code(),
                std::string("<internal>.ListView[regina.Component") +
                    regina::detail::Strings<i.value>::dim + ']');
            pythonNames->emplace(
                typeid(decltype(Triangulation<i.value>().boundaryComponents())).
                    hash_code(),
                std::string("<internal>.ListView[regina.BoundaryComponent") +
                    regina::detail::Strings<i.value>::dim + ']');
            pythonNames->emplace(
                typeid(decltype(Triangulation<i.value>().simplices())).
                    hash_code(),
                std::string("<internal>.ListView[regina.Simplex") +
                    regina::detail::Strings<i.value>::dim + ']');
            pythonNames->emplace(
                typeid(decltype(Triangulation<i.value>().component(0)->
                    simplices())).hash_code(),
                std::string("<internal>.ListView[regina.Simplex") +
                    regina::detail::Strings<i.value>::dim + ']');
            pythonNames->emplace(
                typeid(decltype(Triangulation<i.value>().component(0)->
                    boundaryComponents())).hash_code(),
                std::string("<internal>.ListView[regina.BoundaryComponent") +
                    regina::detail::Strings<i.value>::dim + ']');
            pythonNames->emplace(
                typeid(decltype(Triangulation<i.value>().boundaryComponent(0)->
                    facets())).hash_code(),
                std::string("<internal>.ListView[regina.Face") +
                    regina::detail::Strings<i.value>::dim + '_' +
                    regina::detail::Strings<i.value - 1>::dim + ']');

            for_constexpr<0, i.value>([i](auto j) {
                pythonNames->emplace(typeid(Face<i.value, j.value>).hash_code(),
                    std::string("regina.Face") +
                        regina::detail::Strings<i.value>::dim + '_' +
                        regina::detail::Strings<j.value>::dim);
                pythonNames->emplace(
                    typeid(decltype(Triangulation<i.value>().
                        template faces<j.value>())).hash_code(),
                    std::string("<internal>.ListView[regina.Face") +
                        regina::detail::Strings<i.value>::dim + '_' +
                        regina::detail::Strings<j.value>::dim + ']');
                pythonNames->emplace(
                    typeid(decltype(Triangulation<i.value>().
                        template face<j.value>(0)->embeddings())).hash_code(),
                    std::string("<internal>.ListView[regina.FaceEmbedding") +
                        regina::detail::Strings<i.value>::dim + '_' +
                        regina::detail::Strings<j.value>::dim + ']');
            });

            pythonNames->emplace(
                typeid(PacketOf<Triangulation<i.value>>).hash_code(),
                std::string("regina.PacketOfTriangulation") +
                    regina::detail::Strings<i.value>::dim);
        });

        pythonNames->emplace(typeid(Vertex<3>::LinkType).hash_code(),
            "regina.Vertex3.LinkType");
        pythonNames->emplace(typeid(Triangle<3>::Type).hash_code(),
            "regina.Triangle3.Type");

        pythonNames->emplace(typeid(PacketOf<Link>).hash_code(),
            "regina.PacketOfLink");
        pythonNames->emplace(typeid(PacketOf<AngleStructures>).hash_code(),
            "regina.PacketOfAngleStructures");
        pythonNames->emplace(typeid(PacketOf<NormalSurfaces>).hash_code(),
            "regina.PacketOfNormalSurfaces");
        pythonNames->emplace(typeid(PacketOf<NormalHypersurfaces>).hash_code(),
            "regina.PacketOfNormalHypersurfaces");
        pythonNames->emplace(typeid(PacketOf<SnapPeaTriangulation>).hash_code(),
            "regina.PacketOfSnapPeaTriangulation");
    }
}

namespace regina {

const char* pythonTypename(const std::type_info* t) {
    if (! pythonNames)
        initPythonNames();

    auto r = pythonNames->equal_range(t->hash_code());

    if (r.first == r.second) {
        // No match.
        return nullptr;
    }

    // We have a possible match.
    // However, these are hash matches, not necessarily exact type matches.
    //
    // TODO: For the moment, we will treat these as exact type matches.
    // On some systems this is actually guaranteed to be true, and I
    // need to unravel on which systems it is not.
    return r.first->second.c_str();
}

} // namespace regina

