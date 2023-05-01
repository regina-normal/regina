
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2023, Ben Burton                                   *
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

/**
 * The situation for comparing std::type_info / std::type_index objects:
 *
 * - If hash_code() is different, then the types are different.
 * - If the type_info pointers are the same, then the types are the same.
 *   On "good" platforms, this is an if-and-only-if.
 * - If neither is true, we need to use type_info's == operator, which on
 *   "bad" platforms may trigger a deep string comparison.
 *
 * I think "good" includes GNU/Linux, "bad" includes Windows, and Apple
 * platforms (macOS / iOS) are "it depends".  I could be wrong about this.
 *
 * So:
 *
 * - When looking up type information from a std::type_info pointer,
 *   we do the lookup by hash_code() (that is, hash_code() forms the key
 *   in the underlying map).  This means that if there is no match, we will
 *   probably discover this on all platforms without a deep comparison.
 *
 * - If we do find a matching hash_code(), then we double-check by comparing
 *   type_info pointers.  This means that if there *is* a match, we still
 *   have the opportunity to certify this without a deep comparison even
 *   on bad platforms.
 *
 * - If we have a matching hash but the type_info pointers do not match,
 *   we do the deep comparison (type_info operator ==).  At this point
 *   this is unavoidable on bad platforms, and will still be fast on
 *   good platforms in the (probably never-occuring) scenario where the
 *   hashes match but the types are not actually the same.
 */

using TypeData = std::pair<const std::type_info*, std::string>;
using TypeMap = std::unordered_map<size_t, TypeData>;

#ifdef REGINA_HIGHDIM
  #define REGINA_MAXDIM 15
#else
  #define REGINA_MAXDIM 8
#endif

namespace {
    TypeMap* pythonNames = nullptr;

    inline void registerType(const std::type_info& t, std::string name) {
        pythonNames->emplace(t.hash_code(),
            TypeData(std::addressof(t), std::move(name)));
    }

    void initPythonNames() {
        pythonNames = new TypeMap;

        using namespace regina;

        registerType(typeid(IntegerBase<true>), "regina.LargeInteger");
        registerType(typeid(IntegerBase<false>), "regina.Integer");

        // We use the hard-coded dimension strings to build names for
        // Perm2..Perm15.
        for_constexpr<2, 16>([](auto i) {
            registerType(typeid(Perm<i.value>),
                std::string("regina.Perm") +
                    regina::detail::Strings<i.value>::dim);
        });
        // Perm16 does not have a hard-coded dimension string, so do
        // this separately.
        registerType(typeid(Perm<16>), "regina.Perm16");

        for_constexpr<2, REGINA_MAXDIM + 1>([](auto i) {
            registerType(typeid(Triangulation<i.value>),
                std::string("regina.Triangulation") +
                    regina::detail::Strings<i.value>::dim);

            registerType(typeid(Component<i.value>),
                std::string("regina.Component") +
                    regina::detail::Strings<i.value>::dim);
            registerType(
                typeid(decltype(Triangulation<i.value>().components())),
                std::string("<internal>.ListView[regina.Component") +
                    regina::detail::Strings<i.value>::dim + ']');

            registerType(typeid(BoundaryComponent<i.value>),
                std::string("regina.BoundaryComponent") +
                    regina::detail::Strings<i.value>::dim);
            registerType(
                typeid(decltype(Triangulation<i.value>().boundaryComponents())),
                std::string("<internal>.ListView[regina.BoundaryComponent") +
                    regina::detail::Strings<i.value>::dim + ']');
            registerType(
                typeid(decltype(Triangulation<i.value>().component(0)->
                    boundaryComponents())),
                std::string("<internal>.ListView[regina.BoundaryComponent") +
                    regina::detail::Strings<i.value>::dim + ']');

            registerType(typeid(Isomorphism<i.value>),
                std::string("regina.Isomorphism") +
                    regina::detail::Strings<i.value>::dim);
            registerType(typeid(FacetSpec<i.value>),
                std::string("regina.FacetSpec") +
                    regina::detail::Strings<i.value>::dim);
            registerType(typeid(FacetPairing<i.value>),
                std::string("regina.FacetPairing") +
                    regina::detail::Strings<i.value>::dim);
            // We see FacetPairingBase show up in some swap() functions.
            registerType(
                typeid(regina::detail::FacetPairingBase<i.value>),
                std::string("regina.FacetPairing") +
                    regina::detail::Strings<i.value>::dim);

            if constexpr (i.value == 2) {
                registerType(typeid(Triangle<2>), "regina.Triangle2");
                registerType(
                    typeid(decltype(Triangulation<2>().simplices())),
                    "<internal>.ListView[regina.Triangle2]");
                registerType(
                    typeid(decltype(Triangulation<2>().component(0)->
                        simplices())),
                    "<internal>.ListView[regina.Triangle2]");
                registerType(
                    typeid(decltype(Triangulation<2>().boundaryComponent(0)->
                        facets())),
                    "<internal>.ListView[regina.Edge2]");
                registerType(
                    typeid(decltype(Triangulation<2>().boundaryComponent(0)->
                        vertices())),
                    "<internal>.ListView[regina.Vertex2]");
            } else if constexpr (i.value == 3) {
                registerType(typeid(Tetrahedron<3>), "regina.Tetrahedron3");
                registerType(
                    typeid(decltype(Triangulation<3>().simplices())),
                    "<internal>.ListView[regina.Tetrahedron3]");
                registerType(
                    typeid(decltype(Triangulation<3>().component(0)->
                        simplices())),
                    "<internal>.ListView[regina.Tetrahedron3]");
                registerType(
                    typeid(decltype(Triangulation<3>().boundaryComponent(0)->
                        facets())),
                    "<internal>.ListView[regina.Triangle3]");
                registerType(
                    typeid(decltype(Triangulation<3>().boundaryComponent(0)->
                        edges())),
                    "<internal>.ListView[regina.Edge3]");
                registerType(
                    typeid(decltype(Triangulation<3>().boundaryComponent(0)->
                        vertices())),
                    "<internal>.ListView[regina.Vertex3]");
            } else if constexpr (i.value == 4) {
                registerType(typeid(Pentachoron<4>), "regina.Pentachoron4");
                registerType(
                    typeid(decltype(Triangulation<4>().simplices())),
                    "<internal>.ListView[regina.Pentachoron4]");
                registerType(
                    typeid(decltype(Triangulation<4>().component(0)->
                        simplices())),
                    "<internal>.ListView[regina.Pentachoron4]");
                registerType(
                    typeid(decltype(Triangulation<4>().boundaryComponent(0)->
                        facets())),
                    "<internal>.ListView[regina.Tetrahedron4]");
                registerType(
                    typeid(decltype(Triangulation<4>().boundaryComponent(0)->
                        triangles())),
                    "<internal>.ListView[regina.Triangle4]");
                registerType(
                    typeid(decltype(Triangulation<4>().boundaryComponent(0)->
                        edges())),
                    "<internal>.ListView[regina.Edge4]");
                registerType(
                    typeid(decltype(Triangulation<4>().boundaryComponent(0)->
                        vertices())),
                    "<internal>.ListView[regina.Vertex4]");
            } else {
                registerType(typeid(Face<i.value, i.value>),
                    std::string("regina.Simplex") +
                        regina::detail::Strings<i.value>::dim);
                registerType(
                    typeid(decltype(Triangulation<i.value>().simplices())),
                    std::string("<internal>.ListView[regina.Simplex") +
                        regina::detail::Strings<i.value>::dim + ']');
                registerType(
                    typeid(decltype(Triangulation<i.value>().component(0)->
                        simplices())),
                    std::string("<internal>.ListView[regina.Simplex") +
                        regina::detail::Strings<i.value>::dim + ']');
                if constexpr (i.value == 5) {
                    registerType(
                        typeid(decltype(Triangulation<5>().
                            boundaryComponent(0)->facets())),
                        "<internal>.ListView[regina.Pentachoron5]");
                } else {
                    registerType(
                        typeid(decltype(Triangulation<i.value>().
                            boundaryComponent(0)->facets())),
                        std::string("<internal>.ListView[regina.Face") +
                            regina::detail::Strings<i.value>::dim + '_' +
                            regina::detail::Strings<i.value - 1>::dim + ']');
                }
            }

            for_constexpr<0, i.value>([i](auto j) {
                if constexpr (j.value == 0) {
                    registerType(
                        typeid(Vertex<i.value>),
                        std::string("regina.Vertex") +
                            regina::detail::Strings<i.value>::dim);
                    registerType(
                        typeid(decltype(Triangulation<i.value>().vertices())),
                        std::string("<internal>.ListView[regina.Vertex") +
                            regina::detail::Strings<i.value>::dim + ']');
                    registerType(
                        typeid(decltype(Triangulation<i.value>().
                            vertex(0)->embeddings())),
                        std::string(
                            "<internal>.ListView[regina.VertexEmbedding") +
                            regina::detail::Strings<i.value>::dim + ']');
                } else if constexpr (j.value == 1) {
                    registerType(
                        typeid(Edge<i.value>),
                        std::string("regina.Edge") +
                            regina::detail::Strings<i.value>::dim);
                    registerType(
                        typeid(decltype(Triangulation<i.value>().edges())),
                        std::string("<internal>.ListView[regina.Edge") +
                            regina::detail::Strings<i.value>::dim + ']');
                    registerType(
                        typeid(decltype(Triangulation<i.value>().
                            edge(0)->embeddings())),
                        std::string(
                            "<internal>.ListView[regina.EdgeEmbedding") +
                            regina::detail::Strings<i.value>::dim + ']');
                } else if constexpr (j.value == 2) {
                    // Despite the fact that this code is only generated
                    // for 0 <= j < i, it is still being *parsed* for i == 2,
                    // thus generating compile errors (because
                    // Triangle<2>::embeddings() does not exist).
                    // Putting another constexpr-if check on i seems to fix it.
                    // Strangely, combining this with the constexpr test on j
                    // above does *not* fix it.  Is this a flaw in my compiler
                    // (clang / xcode 14), or a flaw in my understanding of
                    // how constexpr-if works?
                    if constexpr (i.value > 2) {
                        registerType(
                            typeid(Triangle<i.value>),
                            std::string("regina.Triangle") +
                                regina::detail::Strings<i.value>::dim);
                        registerType(
                            typeid(decltype(Triangulation<i.value>().
                                triangles())),
                            std::string("<internal>.ListView[regina.Triangle") +
                                regina::detail::Strings<i.value>::dim + ']');
                        registerType(
                            typeid(decltype(Triangulation<i.value>().
                                triangle(0)->embeddings())),
                            std::string("<internal>.ListView["
                                "regina.TriangleEmbedding") +
                                regina::detail::Strings<i.value>::dim + ']');
                    }
                } else if constexpr (j.value == 3) {
                    // See the j == 2 case for why we need this extra test on i.
                    if constexpr (i.value > 3) {
                        registerType(
                            typeid(Tetrahedron<i.value>),
                            std::string("regina.Tetrahedron") +
                                regina::detail::Strings<i.value>::dim);
                        registerType(
                            typeid(decltype(Triangulation<i.value>().
                                tetrahedra())),
                            std::string(
                                "<internal>.ListView[regina.Tetrahedron") +
                                regina::detail::Strings<i.value>::dim + ']');
                        registerType(
                            typeid(decltype(Triangulation<i.value>().
                                tetrahedron(0)->embeddings())),
                            std::string(
                                "<internal>.ListView["
                                "regina.TetrahedronEmbedding") +
                                regina::detail::Strings<i.value>::dim + ']');
                    }
                } else if constexpr (j.value == 4) {
                    // See the j == 2 case for why we need this extra test on i.
                    if constexpr (i.value > 4) {
                        registerType(
                            typeid(Pentachoron<i.value>),
                            std::string("regina.Pentachoron") +
                                regina::detail::Strings<i.value>::dim);
                        registerType(
                            typeid(decltype(Triangulation<i.value>().
                                pentachora())),
                            std::string(
                                "<internal>.ListView[regina.Pentachoron") +
                                regina::detail::Strings<i.value>::dim + ']');
                        registerType(
                            typeid(decltype(Triangulation<i.value>().
                                pentachoron(0)->embeddings())),
                            std::string(
                                "<internal>.ListView["
                                "regina.PentachoronEmbedding") +
                                regina::detail::Strings<i.value>::dim + ']');
                    }
                } else {
                    registerType(
                        typeid(Face<i.value, j.value>),
                        std::string("regina.Face") +
                            regina::detail::Strings<i.value>::dim + '_' +
                            regina::detail::Strings<j.value>::dim);
                    registerType(
                        typeid(decltype(Triangulation<i.value>().
                            template faces<j.value>())),
                        std::string("<internal>.ListView[regina.Face") +
                            regina::detail::Strings<i.value>::dim + '_' +
                            regina::detail::Strings<j.value>::dim + ']');
                    registerType(
                        typeid(decltype(Triangulation<i.value>().
                            template face<j.value>(0)->embeddings())),
                        std::string(
                            "<internal>.ListView[regina.FaceEmbedding") +
                            regina::detail::Strings<i.value>::dim + '_' +
                            regina::detail::Strings<j.value>::dim + ']');
                }
            });

            registerType(
                typeid(PacketOf<Triangulation<i.value>>),
                std::string("regina.PacketOfTriangulation") +
                    regina::detail::Strings<i.value>::dim);
        });

        registerType(typeid(decltype(SnapPeaTriangulation().cusps())),
            "<internal>.ListView[regina.Cusp]");
        registerType(typeid(decltype(Link().crossings())),
            "<internal>.ListView[regina.Crossing]");
        registerType(typeid(decltype(Link().components())),
            "<internal>.ListView[regina.StrandRef]");
        registerType(typeid(decltype(ModelLinkGraph().nodes())),
            "<internal>.ListView[regina.ModelLinkGraphNode]");
        registerType(typeid(decltype(ModelLinkGraph().cells().arcs(0))),
            "<internal>.ListView[regina.ModelLinkGraphArc]");

        // Enums that live within individual face classes:
        registerType(typeid(Vertex<3>::LinkType), "regina.Vertex3.LinkType");
        registerType(typeid(Triangle<3>::Type), "regina.Triangle3.Type");

        registerType(typeid(PacketOf<Link>),
            "regina.PacketOfLink");
        registerType(typeid(PacketOf<AngleStructures>),
            "regina.PacketOfAngleStructures");
        registerType(typeid(PacketOf<NormalSurfaces>),
            "regina.PacketOfNormalSurfaces");
        registerType(typeid(PacketOf<NormalHypersurfaces>),
            "regina.PacketOfNormalHypersurfaces");
        registerType(typeid(PacketOf<SnapPeaTriangulation>),
            "regina.PacketOfSnapPeaTriangulation");
    }
}

namespace regina {

const char* pythonTypename(const std::type_info* t) {
    if (! pythonNames)
        initPythonNames();

    auto r = pythonNames->equal_range(t->hash_code());

    // We have a set of possible matches.
    // See the discussion at the top of this file as to how we organise
    // the following tests.
    for ( ; r.first != r.second; ++r.first) {
        // Compare pointers first, then do a deep type_info comparison.
        const auto& value = r.first->second;
        if (value.first == t || (*value.first == *t))
            return value.second.c_str();
    }

    // We have no match.
    return nullptr;
}

} // namespace regina

