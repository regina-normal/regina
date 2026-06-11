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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <algorithm>
#include <array>
#include "subcomplex/conical.h"
#include "triangulation/facepair.h"
#include "triangulation/dim3.h"

namespace regina {
namespace {

struct VirtualLinkTet {
    Pentachoron<4>* pent;
    int apex;
};

class VirtualLink {
    private:
        Vertex<4>* apex_;
        std::vector<VirtualLinkTet> tet_;
        std::vector<int> map_;
        std::vector<std::array<int, 4>> adjacent_;
        std::vector<std::array<int, 4>> adjacentFace_;
        std::vector<std::array<Perm<4>, 4>> gluing_;

    public:
        explicit VirtualLink(Vertex<4>* apex);

        bool valid() const;
        size_t size() const;
        Vertex<4>* apex() const;
        Pentachoron<4>* pentachoron(size_t tet) const;

        int adjacentTetrahedron(size_t tet, int face) const;
        int adjacentFace(size_t tet, int face) const;
        Perm<4> adjacentGluing(size_t tet, int face) const;
};

VirtualLink::VirtualLink(Vertex<4>* apex) :
        apex_(apex), map_(5 * apex->triangulation().size(), -1) {
    tet_.reserve(apex->degree());

    size_t i = 0;
    for (const auto& emb : apex->embeddings()) {
        size_t idx = 5 * emb.simplex()->index() + emb.face();
        if (map_[idx] >= 0) {
            // The same global vertex appears more than once in the same
            // pentachoron.  This is not the simple cone model handled here.
            tet_.clear();
            return;
        }
        map_[idx] = static_cast<int>(i++);
        tet_.push_back({ emb.simplex(), emb.face() });
    }

    adjacent_.resize(tet_.size());
    adjacentFace_.resize(tet_.size());
    gluing_.resize(tet_.size());
    for (i = 0; i < tet_.size(); ++i) {
        adjacent_[i].fill(-1);
        adjacentFace_[i].fill(-1);
    }

    for (i = 0; i < tet_.size(); ++i) {
        auto* pent = tet_[i].pent;
        int apex = tet_[i].apex;
        Perm<5> tetVertices = pent->tetrahedronMapping(apex);

        for (int f = 0; f < 5; ++f) {
            if (f == apex)
                continue;

            int linkFace = tetVertices.pre(f);
            auto* adjPent = pent->adjacentPentachoron(f);
            if (! adjPent)
                continue;

            Perm<5> pentGluing = pent->adjacentGluing(f);
            int adjApex = pentGluing[apex];
            if (adjApex < 0 || adjApex > 4)
                continue;

            int adjTet = map_[5 * adjPent->index() + adjApex];
            if (adjTet < 0)
                continue;

            Perm<5> adjTetVertices = adjPent->tetrahedronMapping(adjApex);
            Perm<4> linkGluing = Perm<4>::contract(
                adjTetVertices.inverse() * pentGluing * tetVertices);

            adjacent_[i][linkFace] = adjTet;
            adjacentFace_[i][linkFace] = linkGluing[linkFace];
            gluing_[i][linkFace] = linkGluing;
        }
    }
}

bool VirtualLink::valid() const {
    return apex_ && ! tet_.empty();
}

size_t VirtualLink::size() const {
    return tet_.size();
}

Vertex<4>* VirtualLink::apex() const {
    return apex_;
}

Pentachoron<4>* VirtualLink::pentachoron(size_t tet) const {
    return tet_[tet].pent;
}

int VirtualLink::adjacentTetrahedron(size_t tet, int face) const {
    return adjacent_[tet][face];
}

int VirtualLink::adjacentFace(size_t tet, int face) const {
    return adjacentFace_[tet][face];
}

Perm<4> VirtualLink::adjacentGluing(size_t tet, int face) const {
    return gluing_[tet][face];
}

struct VirtualLST {
    std::vector<bool> used;
    size_t size;
    size_t base;
    size_t top;
    FacePair baseFace;
    FacePair topFace;
    Perm<6> baseEdge;
    Perm<6> topEdge;
    long cuts[3];
};

std::unique_ptr<VirtualLST> recogniseLSTFromBase(
        const VirtualLink& link, size_t baseTet) {
    int baseFace1;
    int baseFace2 = -1;
    Perm<4> basePerm;
    bool okay;
    int i;
    for (baseFace1 = 0; baseFace1 < 3; ++baseFace1) {
        if (link.adjacentTetrahedron(baseTet, baseFace1) ==
                static_cast<int>(baseTet)) {
            baseFace2 = link.adjacentFace(baseTet, baseFace1);
            basePerm = link.adjacentGluing(baseTet, baseFace1);
            okay = true;
            for (i = 0; i < 4; ++i) {
                if (i == baseFace1 || i == baseFace2)
                    continue;
                if (basePerm[i] == i) {
                    okay = false;
                    break;
                }
            }
            if (okay && basePerm[baseFace2] != baseFace1)
                break;
            else
                baseFace2 = -1;
        }
    }

    if (baseFace2 == -1)
        return nullptr;

    auto ans = std::make_unique<VirtualLST>();
    ans->used.assign(link.size(), false);
    ans->used[baseTet] = true;
    ans->size = 1;
    ans->base = ans->top = baseTet;

    ans->baseFace = FacePair(baseFace1, baseFace2);
    if (baseFace1 > baseFace2)
        basePerm = basePerm.inverse();

    ans->topFace = ans->baseFace.complement();

    if (basePerm[ans->baseFace.upper()] == ans->topFace.lower()) {
        ans->baseEdge = Perm<6>(
            ans->baseFace.oppositeEdge(),
            Edge<3>::edgeNumber[ans->topFace.upper()][ans->baseFace.upper()],
            Edge<3>::edgeNumber[ans->topFace.lower()][ans->baseFace.lower()],
            Edge<3>::edgeNumber[ans->topFace.lower()][ans->baseFace.upper()],
            ans->baseFace.commonEdge(),
            Edge<3>::edgeNumber[ans->topFace.upper()][ans->baseFace.lower()]);

        ans->topEdge = ans->baseEdge * Perm<6>(5, 3, 1, 2, 0, 4);
    } else {
        ans->baseEdge = Perm<6>(
            ans->baseFace.oppositeEdge(),
            Edge<3>::edgeNumber[ans->topFace.lower()][ans->baseFace.upper()],
            Edge<3>::edgeNumber[ans->topFace.upper()][ans->baseFace.lower()],
            Edge<3>::edgeNumber[ans->topFace.upper()][ans->baseFace.upper()],
            ans->baseFace.commonEdge(),
            Edge<3>::edgeNumber[ans->topFace.lower()][ans->baseFace.lower()]);

        ans->topEdge = ans->baseEdge * Perm<6>(3, 5, 2, 1, 0, 4);
    }

    ans->cuts[0] = 1;
    ans->cuts[1] = 2;
    ans->cuts[2] = 3;

    int adjFace[2];
    int adjEdge;
    int layerOnEdge[2];
    int newTopEdge;
    Perm<4> adjPerm[2];
    int layerOnGroup;
    while (true) {
        int next = link.adjacentTetrahedron(ans->top, ans->topFace.lower());
        if (next < 0 || static_cast<size_t>(next) == ans->top ||
                next != link.adjacentTetrahedron(
                    ans->top, ans->topFace.upper()))
            break;

        if (ans->used[next])
            break;

        adjPerm[0] = link.adjacentGluing(ans->top, ans->topFace.lower());
        adjPerm[1] = link.adjacentGluing(ans->top, ans->topFace.upper());
        if (adjPerm[0].sign() != adjPerm[1].sign())
            break;

        adjFace[0] = link.adjacentFace(ans->top, ans->topFace.lower());
        adjFace[1] = link.adjacentFace(ans->top, ans->topFace.upper());
        newTopEdge = Edge<3>::edgeNumber[adjFace[0]][adjFace[1]];
        adjEdge = 5 - newTopEdge;

        layerOnEdge[0] = adjPerm[0].inverse().pairs()[adjEdge];
        layerOnEdge[1] = adjPerm[1].inverse().pairs()[adjEdge];
        if (layerOnEdge[0] != layerOnEdge[1] &&
                layerOnEdge[0] + layerOnEdge[1] != 5)
            break;

        layerOnGroup = ans->topEdge.pre(layerOnEdge[0]) / 2;
        Perm<6> edgePerm = adjPerm[0].pairs();
        switch (layerOnGroup) {
            case 0:
                ans->cuts[0] = ans->cuts[1];
                ans->cuts[1] = ans->cuts[2];
                ans->cuts[2] = ans->cuts[0] + ans->cuts[1];
                ans->topEdge = edgePerm * ans->topEdge *
                    Perm<6>(2, 3, 4, 5, 1, 0);
                break;
            case 1:
                ans->cuts[1] = ans->cuts[2];
                ans->cuts[2] = ans->cuts[0] + ans->cuts[1];
                ans->topEdge = edgePerm * ans->topEdge *
                    Perm<6>(0, 1, 4, 5, 3, 2);
                break;
            case 2:
                if (ans->cuts[1] - ans->cuts[0] < ans->cuts[0]) {
                    ans->cuts[2] = ans->cuts[1];
                    ans->cuts[1] = ans->cuts[0];
                    ans->cuts[0] = ans->cuts[2] - ans->cuts[1];
                    ans->topEdge = edgePerm * ans->topEdge *
                        Perm<6>(5, 4, 0, 1, 2, 3);
                } else {
                    ans->cuts[2] = ans->cuts[1];
                    ans->cuts[1] = ans->cuts[2] - ans->cuts[0];
                    ans->topEdge = edgePerm * ans->topEdge *
                        Perm<6>(0, 1, 5, 4, 2, 3);
                }
                break;
        }

        ans->topFace = FacePair(
            Edge<3>::edgeVertex[adjEdge][0],
            Edge<3>::edgeVertex[adjEdge][1]);

        for (i = 0; i < 6; i += 2) {
            if (ans->topFace.lower() ==
                        Edge<3>::edgeVertex[ans->topEdge[i]][0] ||
                    ans->topFace.lower() ==
                        Edge<3>::edgeVertex[ans->topEdge[i]][1]) {
                ans->topEdge = ans->topEdge * Perm<6>(i, i + 1);
            }
        }

        ans->top = static_cast<size_t>(next);
        ans->used[next] = true;
        ++ans->size;
    }

    return ans;
}

std::vector<Pentachoron<4>*> collectPentachora(const VirtualLink& link,
        const std::vector<bool>& used) {
    std::vector<Pentachoron<4>*> ans;
    for (size_t i = 0; i < used.size(); ++i)
        if (used[i])
            ans.push_back(link.pentachoron(i));
    std::sort(ans.begin(), ans.end(),
        [](const Pentachoron<4>* a, const Pentachoron<4>* b) {
            return a->index() < b->index();
        });
    ans.erase(std::unique(ans.begin(), ans.end()), ans.end());
    return ans;
}

} // anonymous namespace

std::unique_ptr<Conical> Conical::recognise(Vertex<4>* apex) {
    VirtualLink link(apex);
    if (! link.valid())
        return nullptr;

    std::vector<Pentachoron<4>*> pents;
    pents.reserve(link.size());
    for (size_t i = 0; i < link.size(); ++i)
        pents.push_back(link.pentachoron(i));

    std::sort(pents.begin(), pents.end(),
        [](const Pentachoron<4>* a, const Pentachoron<4>* b) {
            return a->index() < b->index();
        });
    if (std::unique(pents.begin(), pents.end()) != pents.end())
        return nullptr;

    return std::unique_ptr<Conical>(new Conical(apex, std::move(pents),
        BaseType::Unknown, 0, nullptr, nullptr, 0, 0, 0));
}

std::unique_ptr<Conical> Conical::recogniseLayeredSolidTorus(
        Vertex<4>* apex) {
    VirtualLink link(apex);
    if (! link.valid())
        return nullptr;

    for (size_t i = 0; i < link.size(); ++i) {
        if (auto ans = recogniseLayeredSolidTorus(apex, i))
            return ans;
    }

    return nullptr;
}

std::unique_ptr<Conical> Conical::recogniseLayeredSolidTorus(
        Vertex<4>* apex, size_t baseTet) {
    VirtualLink link(apex);
    if (! link.valid() || baseTet >= link.size())
        return nullptr;

    auto lst = recogniseLSTFromBase(link, baseTet);
    if (! lst)
        return nullptr;

    auto pents = collectPentachora(link, lst->used);
    if (pents.size() != lst->size)
        return nullptr;

    return std::unique_ptr<Conical>(new Conical(apex, std::move(pents),
        BaseType::LayeredSolidTorus, lst->size, link.pentachoron(lst->base),
        link.pentachoron(lst->top), lst->cuts[0], lst->cuts[1],
        lst->cuts[2]));
}

} // namespace regina
