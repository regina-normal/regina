
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

#include "surface/normalsurface.h"
#include "triangulation/dim3.h"
#include <set>

// When indicating that a surface is *not* a link of a k-face,
// we use |= to set the "known" bit but ignore the "is a link" bit.
// This is fine, since if the surface is not the link of a k-face then
// there is no way in which the "is a link" bit could have been already set.
#define NO_3D_VERTEX_LINK        0x01 // bits 00000001
#define NO_3D_EDGE_LINK          0x04 // bits 00000100
#define NO_3D_TRIANGLE_LINK      0x10 // bits 00010000
#define NO_3D_POSITIVE_FACE_LINK 0x14 // bits 00010100
#define NO_3D_FACE_LINK          0x15 // bits 00010101

#define IS_3D_VERTEX_LINK        0x03 // bits 00000011
#define IS_3D_EDGE_LINK          0x0c // bits 00001100
#define IS_3D_TRIANGLE_LINK      0x30 // bits 00110000

namespace regina {

bool NormalSurface::isVertexLinking() const {
    // The relevant bits of linkOf_ could be any of 00, 01 or 11.

    if (! enc_.couldBeVertexLink()) {
        linkOf_ |= NO_3D_VERTEX_LINK;
        return false;
    }

    size_t nTets = triangulation_->size();
    for (size_t tet = 0; tet < nTets; tet++) {
        for (int type = 0; type < 3; type++)
            if (quads(tet, type) != 0) {
                linkOf_ |= NO_3D_VERTEX_LINK;
                return false;
            }
    }
    if (enc_.storesOctagons())
        for (size_t tet = 0; tet < nTets; tet++)
            for (int type = 0; type < 3; type++)
                if (octs(tet, type) != 0) {
                    linkOf_ = NO_3D_FACE_LINK;
                    return false;
                }

    // Might or might not be a *single* vertex link, so leave linkOf_ untouched.
    return true;
}

const Vertex<3>* NormalSurface::isVertexLink() const {
    if ((linkOf_ & IS_3D_VERTEX_LINK) == NO_3D_VERTEX_LINK)
        return nullptr; // already known this is not a vertex link

    // At this point, the relevant bits of linkOf_ are 00 (not computed),
    // or 11 (it's a vertex link, but we don't know which).

    if (! enc_.couldBeVertexLink()) {
        linkOf_ |= NO_3D_VERTEX_LINK;
        return nullptr;
    }

    // Get a local reference to the triangulation so we do not have to
    // repeatedly bounce through the snapshot.
    const Triangulation<3>& tri(*triangulation_);
    size_t nTets = tri.size();

    // Check that there are no quad/oct discs.
    for (size_t tet = 0; tet < nTets; tet++) {
        for (int type = 0; type < 3; type++)
            if (quads(tet, type) != 0) {
                linkOf_ |= NO_3D_VERTEX_LINK;
                return nullptr;
            }
    }
    if (enc_.storesOctagons())
        for (size_t tet = 0; tet < nTets; tet++)
            for (int type = 0; type < 3; type++)
                if (octs(tet, type) != 0) {
                    linkOf_ = NO_3D_FACE_LINK;
                    return nullptr;
                }

    // It follows from the matching equations that what we have is a
    // union of vertex links.  Make sure we are linking just the one vertex.

    Vertex<3>* ans = nullptr;

    for (size_t tet = 0; tet < nTets; tet++) {
        const Tetrahedron<3>* t = tri.tetrahedron(tet);
        for (int type = 0; type < 4; type++) {
            LargeInteger coord = triangles(tet, type);

            if (coord != 0) {
                // Some triangle discs of this type.
                if (! ans) {
                    // We've found our first and only possible candidate.
                    ans = t->vertex(type);
                } else if (ans != t->vertex(type)) {
                    // We seem to be linking more than one vertex.
                    linkOf_ |= NO_3D_VERTEX_LINK;
                    return nullptr;
                }
            }
        }
    }

    // Either we are linking exactly one vertex (ans != null), or we
    // have the empty vector (ans == null).
    linkOf_ |= (ans ? IS_3D_VERTEX_LINK : NO_3D_VERTEX_LINK);
    return ans;
}

std::pair<const Edge<3>*, const Edge<3>*> NormalSurface::isThinEdgeLink() const {
    if ((linkOf_ & IS_3D_EDGE_LINK) == NO_3D_EDGE_LINK)
        return { nullptr, nullptr }; // already known this is not an edge link

    // Get a local reference to the triangulation so we do not have to
    // repeatedly bounce through the snapshot.
    const Triangulation<3>& tri(*triangulation_);
    size_t nTets = tri.size();

    // Check that there are no octagonal discs.
    if (enc_.storesOctagons())
        for (size_t tet = 0; tet < nTets; tet++)
            for (int type = 0; type < 3; type++)
                if (octs(tet, type) != 0) {
                    linkOf_ = NO_3D_FACE_LINK;
                    return { nullptr, nullptr };
                }

    // Run through the quadrilateral discs and work out if there are any
    // valid candidates.
    std::set<Edge<3>*> notAns;
        /**< Edges that we know the answer *isn't*.
             We will stop updating this set once foundQuads is true. */
    bool foundQuads = false;
    const Edge<3>* ans[2];
    LargeInteger ansMultDouble;

    const Tetrahedron<3>* t;
    Edge<3>* e[6]; // { 2*link, 4*intersect }
    LargeInteger coord;
    int i;

    for (size_t tet = 0; tet < nTets; tet++) {
        t = tri.tetrahedron(tet);
        for (int type = 0; type < 3; type++) {
            coord = quads(tet, type);
            e[0] = t->edge(Edge<3>::edgeNumber[quadDefn[type][0]]
                [quadDefn[type][1]]);
            e[1] = t->edge(Edge<3>::edgeNumber[quadDefn[type][2]]
                [quadDefn[type][3]]);
            e[2] = t->edge(Edge<3>::edgeNumber[quadDefn[type][0]]
                [quadDefn[type][2]]);
            e[3] = t->edge(Edge<3>::edgeNumber[quadDefn[type][0]]
                [quadDefn[type][3]]);
            e[4] = t->edge(Edge<3>::edgeNumber[quadDefn[type][1]]
                [quadDefn[type][2]]);
            e[5] = t->edge(Edge<3>::edgeNumber[quadDefn[type][1]]
                [quadDefn[type][3]]);

            if (coord == 0) {
                // No discs in this coordinate.
                // Do we have any candidate edges yet?
                if (foundQuads) {
                    // Rule out any candidates that should have discs here.
                    for (i = 0; i < 2; i++)
                        if (ans[i] == e[0] || ans[i] == e[1])
                            ans[i] = nullptr;
                } else {
                    // Still haven't found any candidates.
                    notAns.insert(e[0]);
                    notAns.insert(e[1]);
                }
            } else {
                // Some discs in this coordinate.
                // Do we have any candidate edges yet?
                if (foundQuads) {
                    // Check consistency with our candidates.
                    if (e[0] == e[1]) {
                        // Same edge on both sides of the quad.
                        // Note that there can only be one candidate now.
                        if (e[0] == ans[0])
                            ans[1] = nullptr;
                        else if (e[0] == ans[1]) {
                            ans[0] = ans[1];
                            ans[1] = nullptr;
                        } else {
                            // This might still be a *normalised* edge link,
                            // so do not touch linkOf_.
                            return { nullptr, nullptr };
                        }

                        // The only possible candidate is ans[0].
                        if (ans[0] == nullptr || ansMultDouble != coord) {
                            // This might still be a *normalised* edge link,
                            // so do not touch linkOf_.
                            return { nullptr, nullptr };
                        }
                    } else {
                        // Different edges on both sides of the quad.
                        // Check each candidate in turn.
                        for (i = 0; i < 2; i++)
                            if (ans[i] != e[0] && ans[i] != e[1])
                                ans[i] = nullptr;
                        if (ansMultDouble != coord * 2) {
                            // This might still be a *normalised* edge link,
                            // so do not touch linkOf_.
                            return { nullptr, nullptr };
                        }
                    }
                } else {
                    // We've found our first and only possible candidates.
                    if (e[0] == e[1]) {
                        // Same edge on both sides of the quad.
                        if (notAns.find(e[0]) != notAns.end()) {
                            // This might still be a *normalised* edge link,
                            // so do not touch linkOf_.
                            return { nullptr, nullptr };
                        }
                        ans[0] = e[0];
                        ans[1] = nullptr;
                        ansMultDouble = coord;
                    } else {
                        // Different edges on both sides of the quad.
                        for (i = 0; i < 2; i++) {
                            if (notAns.find(e[i]) != notAns.end())
                                ans[i] = nullptr;
                            else {
                                ans[i] = e[i];
                                ansMultDouble = coord;
                                ansMultDouble *= 2;
                            }
                        }
                    }
                    foundQuads = true;
                }

                // We now absolutely have candidates (or have exhausted
                // them all).  Check that these candidates don't
                // intersect the new quads.
                for (i = 2; i < 6; i++) {
                    if (ans[0] == e[i])
                        ans[0] = nullptr;
                    if (ans[1] == e[i])
                        ans[1] = nullptr;
                }
            }

            // Have we ruled out all the candidates we ever had?
            if (foundQuads && (! ans[0]) && (! ans[1])) {
                // This might still be a *normalised* edge link,
                // so do not touch linkOf_.
                return { nullptr, nullptr };
            }
        }
    }

    // So did we actually find anything?
    if (! foundQuads) {
        // This might still be a *normalised* edge link,
        // so do not touch linkOf_.
        return { nullptr, nullptr };
    }
    if ((! ans[0]) && (! ans[1])) {
        // This might still be a *normalised* edge link,
        // so do not touch linkOf_.
        return { nullptr, nullptr };
    }

    // Finally check the triangular discs.
    Vertex<3>* v;
    bool expectZero[2];
    int j;
    for (size_t tet = 0; tet < nTets; tet++) {
        t = tri.tetrahedron(tet);
        for (int type = 0; type < 4; type++) {
            v = t->vertex(type);
            coord = triangles(tet, type);

            // Should we actually see any discs?
            for (i = 0; i < 2; i++) {
                if (! ans[i])
                    continue;

                // If the candidate edge does not touch this vertex, the
                // triangular coordinate should be 0.
                expectZero[i] = (v != ans[i]->vertex(0) &&
                    v != ans[i]->vertex(1));

                // If this triangular disc type intersects the candidate
                // edge, the coordinate should also be 0.
                if (! expectZero[i])
                    for (j = 0; j < 3; j++)
                        if (t->edge(Edge<3>::edgeNumber[type][(type+j+1) % 4])
                                == ans[i]) {
                            expectZero[i] = true;
                            break;
                        }

                // So did we get the right triangular coordinate?
                if (expectZero[i]) {
                    if (coord != 0)
                        ans[i] = nullptr;
                } else {
                    if (ansMultDouble != coord * 2)
                        ans[i] = nullptr;
                }
            }

            // Have we ruled out all possibilities?
            if ((! ans[0]) && (! ans[1])) {
                // This might still be a *normalised* edge link,
                // so do not touch linkOf_.
                return { nullptr, nullptr };
            }
        }
    }

    // One or more candidates have survived: return them.
    linkOf_ |= IS_3D_EDGE_LINK;
    if (ans[0])
        return { ans[0], ans[1] };
    else
        return { ans[1], ans[0] };
}

std::pair<std::vector<const Edge<3>*>, unsigned>
        NormalSurface::isNormalEdgeLink() const {
    std::pair<std::vector<const Edge<3>*>, unsigned> ans;
    ans.second = 0;

    if ((linkOf_ & IS_3D_EDGE_LINK) == NO_3D_EDGE_LINK)
        return ans; // already known this is not an edge link

    if (isEmpty()) {
        // Treat the empty surface separately.
        // Note: none of these edge links will be thin.
        for (auto e : triangulation_->edges())
            if (e->linkingSurface().first.isEmpty())
                ans.first.push_back(e);
        linkOf_ |= (ans.first.empty() ? NO_3D_EDGE_LINK : IS_3D_EDGE_LINK);
        return ans;
    }

    std::optional<NormalSurface> mult = couldLinkFace();
    if (! mult) {
        // This could still be a vertex link, but cannot be the thin or
        // normalised link of any other type of face.
        linkOf_ |= NO_3D_POSITIVE_FACE_LINK;
        return ans; // empty
    }

    for (auto e : triangulation_->edges()) {
        if (edgeWeight(e->index()) != 0)
            continue;

        auto link = e->linkingSurface();
        if (link.first == mult) {
            if (link.second) {
                // Thin link.
                // Note: this vector insertion is costly, but it only happens
                // at most twice.
                if (ans.second == 0) {
                    ans.first.insert(ans.first.begin(), e);
                } else {
                    // We only have at most two thin edge links, so we
                    // must be inserting at position 1.
                    auto pos = ans.first.begin();
                    ++pos;
                    ans.first.insert(pos, e);
                }
                ++ans.second;
            } else {
                // Not a thin link.
                ans.first.push_back(e);
            }
        }
    }
    linkOf_ |= (ans.first.empty() ? NO_3D_EDGE_LINK : IS_3D_EDGE_LINK);
    return ans;
}

std::pair<const Triangle<3>*, const Triangle<3>*>
        NormalSurface::isThinTriangleLink() const {
    if ((linkOf_ & IS_3D_TRIANGLE_LINK) == NO_3D_TRIANGLE_LINK)
        return { nullptr, nullptr }; // already known it's not a triangle link

    // This is essentially the same implementation as isNormalTriangleLink(),
    // just slimmed down slightly to account for some extra facts that
    // we know about thin links.

    std::pair<const Triangle<3>*, const Triangle<3>*> ans { nullptr, nullptr };

    // Thin links are never empty.
    if (isEmpty()) {
        // This might still be a *normalised* triangle link,
        // so do not touch linkOf_.
        return ans;
    }

    std::optional<NormalSurface> mult = couldLinkFace();
    if (! mult) {
        // This could still be a vertex link, but cannot be the thin or
        // normalised link of any other type of face.
        linkOf_ |= NO_3D_POSITIVE_FACE_LINK;
        return ans;
    }

    for (auto t : triangulation_->triangles()) {
        for (int i = 0; i < 3; ++i)
            if (edgeWeight(t->edge(i)->index()) != 0)
                continue;

        auto link = t->linkingSurface();
        if (link.second /* thin */ && link.first == mult) {
            if (! ans.first)
                ans.first = t;
            else {
                // There can be at most two thin triangle links, and we
                // have found them both.
                ans.second = t;
                linkOf_ |= IS_3D_TRIANGLE_LINK;
                return ans;
            }
        }
    }

    if (ans.first)
        linkOf_ |= IS_3D_TRIANGLE_LINK;
    return ans;
}

std::pair<std::vector<const Triangle<3>*>, unsigned>
        NormalSurface::isNormalTriangleLink() const {
    std::pair<std::vector<const Triangle<3>*>, unsigned> ans;
    ans.second = 0;

    if ((linkOf_ & IS_3D_TRIANGLE_LINK) == NO_3D_TRIANGLE_LINK)
        return ans; // already known this is not a triangle link

    if (isEmpty()) {
        // Treat the empty surface separately.
        // Note: none of these triangle links will be thin.
        for (auto t : triangulation_->triangles())
            if (t->linkingSurface().first.isEmpty())
                ans.first.push_back(t);
        linkOf_ |= (ans.first.empty() ?
            NO_3D_TRIANGLE_LINK : IS_3D_TRIANGLE_LINK);
        return ans;
    }

    std::optional<NormalSurface> mult = couldLinkFace();
    if (! mult) {
        // This could still be a vertex link, but cannot be the thin or
        // normalised link of any other type of face.
        linkOf_ |= NO_3D_POSITIVE_FACE_LINK;
        return ans; // empty
    }

    for (auto t : triangulation_->triangles()) {
        for (int i = 0; i < 3; ++i)
            if (edgeWeight(t->edge(i)->index()) != 0)
                continue;

        auto link = t->linkingSurface();
        if (link.first == mult) {
            if (link.second) {
                // Thin link.
                // Note: this vector insertion is costly, but it only happens
                // at most twice.
                if (ans.second == 0) {
                    ans.first.insert(ans.first.begin(), t);
                } else {
                    // We only have at most two thin triangle links, so we
                    // must be inserting at position 1.
                    auto pos = ans.first.begin();
                    ++pos;
                    ans.first.insert(pos, t);
                }
                ++ans.second;
            } else {
                // Not a thin link.
                ans.first.push_back(t);
            }
        }
    }

    linkOf_ |= (ans.first.empty() ? NO_3D_TRIANGLE_LINK : IS_3D_TRIANGLE_LINK);
    return ans;
}

std::optional<NormalSurface> NormalSurface::couldLinkFace() const {
    if (! normal()) {
        return std::nullopt;
    }

    // All edge weights should be in { 0, k, 2k } for some k.

    // We store the values k and 2k as we find them; these are initialised to
    // zero.  If only one value has been seen so far, we store it as k.
    LargeInteger k, kk;

    bool foundWeightZero = false;
    for (auto e : triangulation_->edges()) {
        LargeInteger w = edgeWeight(e->index());

        if (w == 0) {
            foundWeightZero = true;
            continue;
        } else if (w.isInfinite()) {
            return std::nullopt;
        } else if (k == 0) {
            // First non-zero weight we've seen.
            k = w;
        } else if (kk == 0) {
            // We've only seen one value so far; this is stored in k.
            if (w != k) {
                if (w == 2 * k) {
                    kk = w;
                } else if (2 * w == k) {
                    // What we thought was k was really 2k.
                    kk = k;
                    k = w;
                } else {
                    // This cannot be a subcomplex link.
                    return std::nullopt;
                }
            }
        } else {
            // Both k and 2k have already been seen.
            if (w != k && w != kk) {
                return std::nullopt;
            }
        }
    }

    if (! foundWeightZero) {
        // This cannot link a face of positive dimension.
        return std::nullopt;
    }

    // The edge weights are consistent with a multiple of a normalised
    // non-vertex face link.
    //
    // Now we construct the exact multiple of this surface that should
    // be such a link.
    //
    // In any normalised face link, all disc coordinates are 0, 1 or 2, and
    // all edge weights are 0, 1 or 2.  It follows that the multiple we are
    // looking for is either the scaled-down surface (i.e., divide the
    // underlying vector by its gcd), or the double of the scaled-down surface.
    //
    // We will therefore call scaleDown(), and then double the surface
    // if necessary.  To identify when doubling is necessary, we observe:
    //
    // - Any (non-empty) normalised face link must be 2-sided and separating,
    //   and even though the surface could be disconnected, the portion
    //   of the 3-manifold on the side of the surface containing the original
    //   face must still be connected (call this portion X).
    //
    // - Doubling is only required for normalised face links where all
    //   non-zero normal coordinates are 2 (and therefore, using what else we
    //   know, all edge weights are 2 also).  In such a scenario where doubling
    //   is required, the scaled-down surface (where all non-zero coordinates
    //   are 1) will be non-separating, since cutting the 3-manifold along
    //   this scaled-down surface will result in X (and no other components).
    //
    // So, to summarise: doubling is required if and only if the
    // scaled-down surface is non-separating.  (However, there are other
    // necessary conditions for doubling that are cheaper to test, such
    // as all normal coordinates and edge weights being 0 or 1, and so
    // we will make use of these cheaper tests also.)

    NormalSurface mult = *this;
    LargeInteger scale = mult.scaleDown();
    if (kk != 0) {
        if (scale != k) {
            // The edge weights were {0,k,2k}, but the normal coordinates were
            // not.
            return std::nullopt;
        }
    } else {
        // All non-zero edge weights were equal to k.
        if (scale == k) {
            // All non-zero edge weights have been scaled down to 1, and so
            // this *could* be a scenario where we need to double again,
            // as described above.
            //
            // For this we test whether the surface is separating.  However,
            // in any scenario where doubling is necessary, all edge weights
            // are 0 or 1, and so the separating test becomes just a test to see
            // if we can find a path from some vertex back to itself through
            // the 1-skeleton that traverses an *odd* number of weight-one edges
            // (and any number of weight-zero edges).
            //
            // Here we use a fairly naive test based on Floyd-Warshall.  This
            // could be sped up by using union-find.  However, our naive test
            // is cubic in the number of *vertices*, which in typical scenarios
            // is very small.  So let's not fuss too much about this for now.

            size_t v = triangulation_->countVertices();
            int* join = new int[v * v]; // 0,1,-1: no path, even path, odd path
            std::fill(join, join + v * v, 0);

            for (auto e : triangulation_->edges()) {
                LargeInteger w = mult.edgeWeight(e->index());
                size_t a = e->vertex(0)->index();
                size_t b = e->vertex(1)->index();
                if (w == 0) {
                    if (a != b) {
                        if (join[a * v + b] == -1) {
                            // We have an odd cycle (a-b-a).
                            mult *= 2;
                            goto noMoreScaling;
                        } else {
                            join[a * v + b] = join[b * v + a] = 1;
                        }
                    }
                } else if (w == 1) {
                    if (a == b) {
                        // We have an odd cycle (a-a).
                        mult *= 2;
                        goto noMoreScaling;
                    } else {
                        if (join[a * v + b] == 1) {
                            // We have an odd cycle (a-b-a).
                            mult *= 2;
                            goto noMoreScaling;
                        } else {
                            join[a * v + b] = join[b * v + a] = -1;
                        }
                    }
                } else if (w == 2) {
                    // This could be a face link, but it is not a case
                    // where we need to double.
                    goto noMoreScaling;
                } else {
                    // This can never be a face link.
                    return std::nullopt;
                }
            }

            for (size_t via = 0; via < v; ++via) {
                for (size_t a = 0; a < v; ++a) {
                    if (join[a * v + via] != 0) {
                        for (size_t b = 0; b < v; ++b) {
                            if (a != b && join[b * v + via] != 0) {
                                // Examine the path a-via-b.
                                if (join[a * v + b] == 0)
                                    join[a * v + b] = join[b * v + a] =
                                        join[a * v + via] * join[b * v + via];
                                else if (join[a * v + b] !=
                                        join[a * v + via] * join[b * v + via]) {
                                    // We have an odd cycle (a-via-b-a).
                                    mult *= 2;
                                    goto noMoreScaling;
                                }
                            }
                        }
                    }
                }
            }

    noMoreScaling:
            delete[] join;
        } else {
            // All non-zero edge weights were k, but the scaling factor
            // was not k.  In this case the edge weights should have been
            // scaled down to 2; otherwise we cannot have a normalised
            // edge link at all.
            if (scale + scale != k) {
                return std::nullopt;
            }
        }
    }

    return mult;
}

} // namespace regina

