
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include "hypersurface/normalhypersurface.h"
#include "triangulation/dim4.h"

// When indicating that a hypersurface is *not* a link of a k-face,
// we use |= to set the "known" bit but ignore the "is a link" bit.
// This is fine, since if the hypersurface is not the link of a k-face then
// there is no way in which the "is a link" bit could have been already set.
#define NO_4D_VERTEX_LINK        0x01 // bits 00000001
#define NO_4D_EDGE_LINK          0x04 // bits 00000100
#define NO_4D_TRIANGLE_LINK      0x10 // bits 00010000
#define NO_4D_TETRAHEDRON_LINK   0x40 // bits 01000000
#define NO_4D_POSITIVE_FACE_LINK 0x54 // bits 01010100
#define NO_4D_FACE_LINK          0x55 // bits 01010101

#define IS_4D_VERTEX_LINK        0x03 // bits 00000011
#define IS_4D_EDGE_LINK          0x0c // bits 00001100
#define IS_4D_TRIANGLE_LINK      0x30 // bits 00110000
#define IS_4D_TETRAHEDRON_LINK   0xc0 // bits 11000000

namespace regina {

bool NormalHypersurface::isVertexLinking() const {
    // The relevant bits of linkOf_ could be any of 00, 01 or 11.

    if (! enc_.couldBeVertexLink()) {
        linkOf_ |= NO_4D_VERTEX_LINK;
        return false;
    }

    size_t nPents = triangulation_->size();
    for (size_t pent = 0; pent < nPents; pent++) {
        for (int type = 0; type < 10; type++)
            if (prisms(pent, type) != 0) {
                linkOf_ |= NO_4D_VERTEX_LINK;
                return false;
            }
    }

    // Might or might not be a *single* vertex link, so leave linkOf_ untouched.
    return true;
}

const Vertex<4>* NormalHypersurface::isVertexLink() const {
    if ((linkOf_ & IS_4D_VERTEX_LINK) == NO_4D_VERTEX_LINK)
        return nullptr; // already known this is not a vertex link

    // At this point, the relevant bits of linkOf_ are 00 (not computed),
    // or 11 (it's a vertex link, but we don't know which).

    if (! enc_.couldBeVertexLink()) {
        linkOf_ |= NO_4D_VERTEX_LINK;
        return nullptr;
    }

    // Get a local reference to the triangulation so we do not have to
    // repeatedly bounce through the snapshot.
    const Triangulation<4>& tri(*triangulation_);
    size_t nPents = tri.size();

    // Check that there are no prism pieces.
    for (size_t pent = 0; pent < nPents; pent++) {
        for (int type = 0; type < 10; type++)
            if (prisms(pent, type) != 0) {
                linkOf_ |= NO_4D_VERTEX_LINK;
                return nullptr;
            }
    }

    // It follows from the matching equations that what we have is a
    // union of vertex links.  Make sure we are linking just the one vertex.

    Vertex<4>* ans = nullptr;

    for (size_t pent = 0; pent < nPents; pent++) {
        const Pentachoron<4>* p = tri.pentachoron(pent);
        for (int type = 0; type < 5; type++) {
            LargeInteger coord = tetrahedra(pent, type);

            if (coord != 0) {
                // Some tetrahedron discs of this type.
                if (! ans) {
                    // We've found our first and only possible candidate.
                    ans = p->vertex(type);
                } else if (ans != p->vertex(type)) {
                    // We seem to be linking more than one vertex.
                    linkOf_ |= NO_4D_VERTEX_LINK;
                    return nullptr;
                }
            }
        }
    }

    // Either we are linking exactly one vertex (ans != null), or we
    // have the empty vector (ans == null).
    linkOf_ |= (ans ? IS_4D_VERTEX_LINK : NO_4D_VERTEX_LINK);
    return ans;
}

const Edge<4>* NormalHypersurface::isThinEdgeLink() const {
    if ((linkOf_ & IS_4D_EDGE_LINK) == NO_4D_EDGE_LINK)
        return nullptr; // already known this is not an edge link

    // Get a local reference to the triangulation so we do not have to
    // repeatedly bounce through the snapshot.
    const Triangulation<4>& tri(*triangulation_);
    size_t nPents = tri.size();

    // Search through prism pieces for one and only one candidate edge.
    Edge<4>* ans = nullptr;
    LargeInteger ansMult;

    for (size_t pent = 0; pent < nPents; pent++) {
        const Pentachoron<4>* p = tri.pentachoron(pent);
        for (int type = 0; type < 10; type++) {
            LargeInteger coord = prisms(pent, type);

            if (coord != 0) {
                // Some prism discs of this type.
                if (! ans) {
                    // We've found our first and only possible candidate.
                    ans = p->edge(type);
                    ansMult = coord;
                } else if (ans != p->edge(type)) {
                    // We seem to be linking more than one edge.
                    // This might still be a *normalised* edge link, so do not
                    // touch linkOf_.
                    return nullptr;
                }
            }
        }
    }

    // Did we find any candidate edges at all?
    if (! ans) {
        // This might still be a *normalised* edge link, so do not
        // touch linkOf_.
        return nullptr;
    }

    // There are no unwanted prism piece types.  However, we must still
    // run through the prism types that do appear to make sure that they
    // each appear with the same multiple.
    for (size_t e = 0; e < ans->degree(); ++e)
        if (ansMult != prisms(
                ans->embedding(e).pentachoron()->index(),
                ans->embedding(e).edge())) {
            // This might still be a *normalised* edge link, so do not
            // touch linkOf_.
            return nullptr;
        }

    // Finally, run through the tetrahedron piece types and make sure
    // that everything checks out.
    for (size_t pent = 0; pent < nPents; pent++) {
        const Pentachoron<4>* p = tri.pentachoron(pent);
        for (int type = 0; type < 5; type++) {
            Vertex<4>* v = p->vertex(type);

            if (ans->vertex(0) == v || ans->vertex(1) == v) {
                // We should see tetrahedra here, but only if none of
                // the four pentachoron edges touching this vertex are
                // the same edge as ans.
                bool crosses = false;
                for (int i = 0; i < 5; ++i) {
                    if (i == type)
                        continue;
                    if (p->edge(Edge<4>::edgeNumber[type][i]) == ans) {
                        crosses = true;
                        break;
                    }
                }

                if (crosses) {
                    if (tetrahedra(pent, type) != 0) {
                        // This might still be a *normalised* edge link, so
                        // do not touch linkOf_.
                        return nullptr;
                    }
                } else {
                    if (tetrahedra(pent, type) != ansMult) {
                        // This might still be a *normalised* edge link, so
                        // do not touch linkOf_.
                        return nullptr;
                    }
                }
            } else {
                if (tetrahedra(pent, type) != 0) {
                    // This might still be a *normalised* edge link, so
                    // do not touch linkOf_.
                    return nullptr;
                }
            }
        }
    }

    // All good!
    linkOf_ |= IS_4D_EDGE_LINK;
    return ans;
}

std::pair<std::vector<const Edge<4>*>, unsigned>
        NormalHypersurface::isNormalEdgeLink() const {
    std::pair<std::vector<const Edge<4>*>, unsigned> ans;
    ans.second = 0;

    if ((linkOf_ & IS_4D_EDGE_LINK) == NO_4D_EDGE_LINK)
        return ans; // already known it's not an edge link

    if (isEmpty()) {
        // Treat the empty hypersurface separately.
        // Note: none of these edge links will be thin.
        for (auto e : triangulation_->edges())
            if (e->linkingSurface().first.isEmpty())
                ans.first.push_back(e);
        linkOf_ |= (ans.first.empty() ? NO_4D_EDGE_LINK : IS_4D_EDGE_LINK);
        return ans;
    }

    std::optional<NormalHypersurface> mult = couldLinkFace();
    if (! mult) {
        // This could still be a vertex link, but cannot be the thin or
        // normalised link of any other type of face.
        linkOf_ |= NO_4D_POSITIVE_FACE_LINK;
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
                // at most once.
                ans.first.insert(ans.first.begin(), e);
                ++ans.second;
            } else {
                // Not a thin link.
                ans.first.push_back(e);
            }
        }
    }

    linkOf_ |= (ans.first.empty() ? NO_4D_EDGE_LINK : IS_4D_EDGE_LINK);
    return ans;
}

std::pair<const Triangle<4>*, const Triangle<4>*>
        NormalHypersurface::isThinTriangleLink() const {
    if ((linkOf_ & IS_4D_TRIANGLE_LINK) == NO_4D_TRIANGLE_LINK)
        return { nullptr, nullptr }; // already known it's not a triangle link

    // This is essentially the same implementation as isNormalTriangleLink(),
    // just slimmed down slightly to account for some extra facts that
    // we know about thin links.

    std::pair<const Triangle<4>*, const Triangle<4>*> ans { nullptr, nullptr };

    // Thin links are never empty.
    if (isEmpty()) {
        // This might still be a *normalised* triangle link, so
        // do not touch linkOf_.
        return ans;
    }

    std::optional<NormalHypersurface> mult = couldLinkFace();
    if (! mult) {
        // This could still be a vertex link, but cannot be the thin or
        // normalised link of any other type of face.
        linkOf_ |= NO_4D_POSITIVE_FACE_LINK;
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
                linkOf_ |= IS_4D_TRIANGLE_LINK;
                return ans;
            }
        }
    }

    linkOf_ |= (ans.first ? IS_4D_TRIANGLE_LINK : NO_4D_TRIANGLE_LINK);
    return ans;
}

std::pair<std::vector<const Triangle<4>*>, unsigned>
        NormalHypersurface::isNormalTriangleLink() const {
    std::pair<std::vector<const Triangle<4>*>, unsigned> ans;
    ans.second = 0;

    if ((linkOf_ & IS_4D_TRIANGLE_LINK) == NO_4D_TRIANGLE_LINK)
        return ans; // already known it's not a triangle link

    if (isEmpty()) {
        // Treat the empty hypersurface separately.
        // Note: none of these triangle links will be thin.
        for (auto t : triangulation_->triangles())
            if (t->linkingSurface().first.isEmpty())
                ans.first.push_back(t);
        linkOf_ |= (ans.first.empty() ?
            NO_4D_TRIANGLE_LINK : IS_4D_TRIANGLE_LINK);
        return ans;
    }

    std::optional<NormalHypersurface> mult = couldLinkFace();
    if (! mult) {
        // This could still be a vertex link, but cannot be the thin or
        // normalised link of any other type of face.
        linkOf_ |= NO_4D_POSITIVE_FACE_LINK;
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

    linkOf_ |= (ans.first.empty() ? NO_4D_TRIANGLE_LINK : IS_4D_TRIANGLE_LINK);
    return ans;
}

std::pair<const Tetrahedron<4>*, const Tetrahedron<4>*>
        NormalHypersurface::isThinTetrahedronLink() const {
    if ((linkOf_ & IS_4D_TETRAHEDRON_LINK) == NO_4D_TETRAHEDRON_LINK)
        return { nullptr, nullptr }; // already known it's not a tet. link

    // This is essentially the same implementation as isNormalTetrahedron(),
    // just slimmed down slightly to account for some extra facts that
    // we know about thin links.

    std::pair<const Tetrahedron<4>*, const Tetrahedron<4>*> ans
        { nullptr, nullptr };

    // Thin links are never empty.
    if (isEmpty()) {
        // This might still be a *normalised* tetrahedron link,
        // so do not touch linkOf_.
        return ans;
    }

    std::optional<NormalHypersurface> mult = couldLinkFace();
    if (! mult) {
        // This could still be a vertex link, but cannot be the thin or
        // normalised link of any other type of face.
        linkOf_ |= NO_4D_POSITIVE_FACE_LINK;
        return ans;
    }

    for (auto t : triangulation_->tetrahedra()) {
        for (int i = 0; i < 6; ++i)
            if (edgeWeight(t->edge(i)->index()) != 0)
                continue;

        auto link = t->linkingSurface();
        if (link.second /* thin */ && link.first == mult) {
            if (! ans.first)
                ans.first = t;
            else {
                // There can be at most two thin tetrahedron links, and we
                // have found them both.
                ans.second = t;
                linkOf_ |= IS_4D_TETRAHEDRON_LINK;
                return ans;
            }
        }
    }

    linkOf_ |= (ans.first ? IS_4D_TETRAHEDRON_LINK : NO_4D_TETRAHEDRON_LINK);
    return ans;
}

std::pair<std::vector<const Tetrahedron<4>*>, unsigned>
        NormalHypersurface::isNormalTetrahedronLink() const {
    std::pair<std::vector<const Tetrahedron<4>*>, unsigned> ans;
    ans.second = 0;

    if ((linkOf_ & IS_4D_TETRAHEDRON_LINK) == NO_4D_TETRAHEDRON_LINK)
        return ans; // already known it's not a tetrahedron link

    if (isEmpty()) {
        // Treat the empty hypersurface separately.
        // Note: none of these triangle links will be thin.
        for (auto t : triangulation_->tetrahedra())
            if (t->linkingSurface().first.isEmpty())
                ans.first.push_back(t);
        linkOf_ |= (ans.first.empty() ?
            NO_4D_TETRAHEDRON_LINK : IS_4D_TETRAHEDRON_LINK);
        return ans;
    }

    std::optional<NormalHypersurface> mult = couldLinkFace();
    if (! mult) {
        // This could still be a vertex link, but cannot be the thin or
        // normalised link of any other type of face.
        linkOf_ |= NO_4D_POSITIVE_FACE_LINK;
        return ans; // empty
    }

    for (auto t : triangulation_->tetrahedra()) {
        for (int i = 0; i < 6; ++i)
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

    linkOf_ |= (ans.first.empty() ?
        NO_4D_TETRAHEDRON_LINK : IS_4D_TETRAHEDRON_LINK);
    return ans;
}

std::optional<NormalHypersurface> NormalHypersurface::couldLinkFace() const {
    // The implementation of this is almost (but not quite) identical to the
    // 3-D analogue NormalHypersurface::couldLinkFace().  Ideally these two
    // routines should have a unified implementation.  Not today however.

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
    // In any normalised face link, all standard coordinates are 0, 1 or 2, and
    // all edge weights are 0, 1 or 2.  It follows that the multiple we are
    // looking for is either the scaled-down hypersurface (i.e., divide the
    // underlying vector by its gcd), or its double.
    //
    // We will therefore call scaleDown(), and then double the hypersurface
    // if necessary.  To identify when doubling is necessary, we observe:
    //
    // - Any (non-empty) normalised face link must be 2-sided and separating,
    //   and even though the hypersurface could be disconnected, the portion
    //   of the 4-manifold on the side of the hypersurface containing the
    //   original face must still be connected (call this portion X).
    //
    // - Doubling is only required for normalised face links where all
    //   non-zero normal coordinates are 2 (and therefore, using what else we
    //   know, all edge weights are 2 also).  In such a scenario where doubling
    //   is required, the scaled-down hypersurface (where all non-zero
    //   coordinates are 1) will be non-separating, since cutting the
    //   4-manifold along this scaled-down hypersurface will result in X
    //   (and no other components).
    //
    // So, to summarise: doubling is required if and only if the
    // scaled-down hypersurface is non-separating.  (However, there are other
    // necessary conditions for doubling that are cheaper to test, such
    // as all normal coordinates and edge weights being 0 or 1, and so
    // we will make use of these cheaper tests also.)

    NormalHypersurface mult = *this;
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
            // For this we test whether the hypersurface is separating.
            // However, in any scenario where doubling is necessary, all edge
            // weights are 0 or 1, and so the separating test becomes just a
            // test to see if we can find a path from some vertex back to
            // itself through the 1-skeleton that traverses an *odd* number
            // of weight-one edges (and any number of weight-zero edges).
            //
            // Here we use a fairly naive test based on Floyd-Warshall.  This
            // could be sped up by using union-find.  However, our naive test
            // is cubic in the number of *vertices*, which in typical scenarios
            // is small.  So let's not fuss too much about this for now.

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

