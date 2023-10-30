
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

/*! \file triangulation/detail/algebra-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Triangulation class template.
 *
 *  This file is _not_ included from triangulation.h, but the routines
 *  it contains are explicitly instantiated in Regina's calculation engine.
 *  Therefore end users should never need to include this header.
 */

#ifndef __REGINA_ALGEBRA_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_ALGEBRA_IMPL_H_DETAIL
#endif

#include "triangulation/generic/triangulation.h"

namespace regina::detail {

template <int dim>
template <int k>
AbelianGroup TriangulationBase<dim>::homology() const {
    if constexpr (standardDim(dim))
        static_assert(1 <= k && k <= dim - 1);
    else
        static_assert(1 <= k && k <= dim - 2);

    if (isEmpty())
        return AbelianGroup();

    if constexpr (k == 1) {
        if (H1_.has_value())
            return *H1_;

        // Calculate a maximal forest in the dual 1-skeleton.
        ensureSkeleton();

        // Build a presentation matrix.
        // Each non-boundary not-in-forest (dim-1)-face is a generator.
        // Each non-boundary (dim-2)-face is a relation.
        long nBdryRidges = 0;
        for (auto bc : boundaryComponents())
            nBdryRidges += bc->countRidges();

        // Cast away all unsignedness in case we run into problems subtracting.
        long nGens = static_cast<long>(countFaces<dim-1>())
            - static_cast<long>(countBoundaryFacets())
            + static_cast<long>(countComponents())
            - static_cast<long>(size());
        long nRels = static_cast<long>(countFaces<dim-2>()) - nBdryRidges;

        MatrixInt pres(nRels, nGens);

        // Find out which (dim-1)-face corresponds to which generator.
        long* genIndex = new long[countFaces<dim-1>()];
        long i = 0;
        for (Face<dim, dim-1>* f : faces<dim-1>())
            if (! (f->isBoundary() || f->inMaximalForest()))
                genIndex[f->index()] = i++;

        // Run through each (dim-2)-face and put the relations into the matrix.
        Simplex<dim>* simp;
        int facet;
        Face<dim, dim-1>* gen;
        i = 0;
        for (Face<dim, dim-2>* f : faces<dim-2>()) {
            if (! f->isBoundary()) {
                // Put in the relation corresponding to this (dim-2)-face.
                for (auto& emb : *f) {
                    simp = emb.simplex();
                    facet = emb.vertices()[dim-1];
                    gen = simp->template face<dim-1>(facet);
                    if (! gen->inMaximalForest()) {
                        // We define the "direction" for this dual edge to point
                        // from embedding gen->front() to embedding gen->back().
                        //
                        // Test if we are traversing this dual edge forwards
                        // or backwards as we walk around the (dim-2)-face f.
                        if ((gen->front().simplex() == simp) &&
                                (gen->front().face() == facet))
                            pres.entry(i, genIndex[gen->index()]) += 1;
                        else
                            pres.entry(i, genIndex[gen->index()]) -= 1;
                    }
                }
                ++i;
            }
        }

        delete[] genIndex;

        // Build the group from the presentation matrix and tidy up.
        return H1_.emplace(std::move(pres));
    } else if constexpr (k == dim - 1 && dim == 3) {
        // In dimension 3 we have both H1 and H1Rel, and so we can compute
        // H2 from those.
        if (! isValid())
            throw FailedPrecondition("Computing kth homology for k >= 2 "
                "requires a valid triangulation");

        const auto* tri = static_cast<const Triangulation<dim>*>(this);
        if (tri->prop_.H2_.has_value())
            return *tri->prop_.H2_;

        if (isEmpty())
            return *(tri->prop_.H2_ = AbelianGroup());

        const AbelianGroup& h1Rel = tri->homologyRel();

        // We know the only summands of H2 will be Z and Z_2.
        AbelianGroup ans;
        if (isOrientable()) {
            // Same as H1Rel without the torsion elements.
            ans.addRank(h1Rel.rank());
        } else {
            // Non-orientable!
            // Z_2 rank = # closed cmpts - # closed orientable cmpts
            for (auto c : components())
                if (c->isClosed() && ! c->isOrientable())
                    ans.addTorsion(2);

            if (H1_.has_value())
                ans.addRank(h1Rel.rank() + h1Rel.torsionRank(2)
                    - H1_->torsionRank(2) - ans.countInvariantFactors());
            else
                ans.addRank(h1Rel.rank() + h1Rel.torsionRank(2)
                    - homology().torsionRank(2) - ans.countInvariantFactors());
        }
        return *(tri->prop_.H2_ = std::move(ans));
    } else {
        // Here we handle the remaining cases:
        //   2 <= k <= 3 in dimension 4;
        //   2 <= k <= (dim-2) in higher dimensions.

        if (! isValid())
            throw FailedPrecondition("Computing kth homology for k >= 2 "
                "requires a valid triangulation");

        // At this point we know that the triangulation is valid and non-empty.
        // Compute the homology using the dual chain complex.
        return AbelianGroup(dualBoundaryMap<k>(), dualBoundaryMap<k + 1>());
    }
}

template <int dim>
const GroupPresentation& TriangulationBase<dim>::group(bool simplify) const {
    if (simplify) {
        if (simplifiedFundGroup_)
            return *simplifiedFundGroup_;
        if (isEmpty())
            return *(simplifiedFundGroup_ = GroupPresentation());
        GroupPresentation ans =
            fundGroup_ ? *fundGroup_ : unsimplifiedGroup();
        ans.intelligentSimplify();
        return *(simplifiedFundGroup_ = std::move(ans));
    } else {
        if (isEmpty())
            return *(fundGroup_ = GroupPresentation());
        return *(fundGroup_ = unsimplifiedGroup());
    }
}
    
template <int dim>
GroupPresentation TriangulationBase<dim>::unsimplifiedGroup() const {
    GroupPresentation ans;
    
    // Calculate a maximal forest in the dual 1-skeleton.
    ensureSkeleton();

    // Each non-boundary not-in-forest (dim-1)-face is a generator.
    // Each non-boundary (dim-2)-face is a relation.

    // Cast away all unsignedness in case we run into problems subtracting.
    long nGens = static_cast<long>(countFaces<dim-1>())
        - static_cast<long>(countBoundaryFacets())
        + static_cast<long>(countComponents())
        - static_cast<long>(size());

    // Insert the generators.
    ans.addGenerator(nGens);

    // Find out which (dim-1)-face corresponds to which generator.
    std::unique_ptr<long[]> genIndex =
        std::make_unique<long[]>(countFaces<dim-1>());
    long i = 0;
    for (Face<dim, dim-1>* f : faces<dim-1>())
        if (! (f->isBoundary() || f->inMaximalForest()))
            genIndex[f->index()] = i++;

    // Run through each (dim-2)-face and insert the corresponding relations.
    Simplex<dim>* simp;
    int facet;
    Face<dim, dim-1>* gen;
    for (Face<dim, dim-2>* f : faces<dim-2>()) {
        if (! f->isBoundary()) {
            // Put in the relation corresponding to this triangle.
            GroupExpression rel;
            for (auto& emb : *f) {
                simp = emb.simplex();
                facet = emb.vertices()[dim-1];
                gen = simp->template face<dim-1>(facet);
                if (! gen->inMaximalForest()) {
                    // We define the "direction" for this dual edge to point
                    // from embedding gen->front() to embedding gen->back().
                    //
                    // Test whether we are traversing this dual edge forwards or
                    // backwards as we walk around the (dim-2)-face f.
                    if ((gen->front().simplex() == simp) &&
                            (gen->front().face() == facet))
                        rel.addTermLast(genIndex[gen->index()], 1);
                    else
                        rel.addTermLast(genIndex[gen->index()], -1);
                }
            }
            ans.addRelation(std::move(rel));
        }
    }

    return ans;
}

template <int dim>
template <int subdim>
MatrixInt TriangulationBase<dim>::boundaryMap() const {
    static_assert(subdim > 0 && subdim <= dim);
    MatrixInt ans(countFaces<subdim - 1>(), countFaces<subdim>());

    if constexpr (subdim == dim) {
        size_t row = 0;
        for (auto s : simplices_) {
            Perm<dim + 1> subface;
            int i = subdim;
            while (true) {
                auto* facet = s->template face<dim - 1>(i);
                if (Perm<dim>::contract(
                        s->template faceMapping<dim - 1>(i).inverse()
                            * subface).sign() > 0) {
                    // The boundary face was not reflected.
                    if (i % 2 == 0)
                        ++ans.entry(facet->index(), row);
                    else
                        --ans.entry(facet->index(), row);
                } else {
                    // The boundary face was reflected.
                    if (i % 2 == 0)
                        --ans.entry(facet->index(), row);
                    else
                        ++ans.entry(facet->index(), row);
                }

                if (i > 0) {
                    --i;
                    subface = subface * Perm<dim + 1>(i, subdim);
                } else
                    break;
            }
            ++row;
        }
    } else {
        size_t row = 0;
        for (Face<dim, subdim>* f : faces<subdim>()) {
            Simplex<dim>* s = f->front().simplex();
            Perm<dim + 1> subface = f->front().vertices();
            int i = subdim;
            while (true) {
                int facetNum = Face<dim, subdim - 1>::faceNumber(subface);
                auto* facet = s->template face<subdim - 1>(facetNum);
                if constexpr (subdim == 1) {
                    // We do not need to worry about orientation of
                    // (subdim-1)-faces, since these are vertices
                    // (and there is no Perm<1> class).
                    if (i % 2 == 0)
                        ++ans.entry(facet->index(), row);
                    else
                        --ans.entry(facet->index(), row);
                } else if (Perm<subdim>::contract(
                        s->template faceMapping<subdim - 1>(facetNum).inverse()
                            * subface).sign() > 0) {
                    // The boundary face was not reflected.
                    if (i % 2 == 0)
                        ++ans.entry(facet->index(), row);
                    else
                        --ans.entry(facet->index(), row);
                } else {
                    // The boundary face was reflected.
                    if (i % 2 == 0)
                        --ans.entry(facet->index(), row);
                    else
                        ++ans.entry(facet->index(), row);
                }

                if (i > 0) {
                    --i;
                    subface = subface * Perm<dim + 1>(i, subdim);
                } else
                    break;
            }
            ++row;
        }
    }

    return ans;
}

template <int dim>
template <int subdim>
MatrixInt TriangulationBase<dim>::dualBoundaryMap() const {
    static_assert(subdim >= 1 && subdim <= dim);
    static_assert(standardDim(dim) || subdim < dim);

    ensureSkeleton();

    if constexpr (subdim == 1) {
        MatrixInt bdry(size(), countFaces<dim-1>() - nBoundaryFaces_[dim-1]);

        // Just walk through each dual edge and record its boundary.
        size_t col = 0;
        for (auto f : faces<dim-1>()) {
            if (f->isBoundary())
                continue;

            // The endpoints of the dual edge are labelled (0, 1) according to
            // (f->back(), f->front()).
            ++bdry.entry(f->front().simplex()->index(), col);
            --bdry.entry(f->back().simplex()->index(), col);

            ++col;
        }

        return bdry;
    } else {
        MatrixInt bdry(
            countFaces<dim-subdim+1>() - nBoundaryFaces_[dim-subdim+1],
            countFaces<dim-subdim>() - nBoundaryFaces_[dim-subdim]);

        // For dual subdim-faces, build a map from (primal) face index in the
        // triangulation to coordinate position in the chain complex.
        size_t row = 0;
        std::vector<size_t> lookup(std::get<dim-subdim>(faces_).size());
        for (auto f : faces<dim-subdim>())
            if (! f->isBoundary())
                lookup[f->index()] = row++;

        row = 0;
        for (auto f : faces<dim-subdim+1>()) {
            if (f->isBoundary())
                continue;

            // The dual to f sits on the boundary of which other dual faces?
            Simplex<dim>* s = f->front().simplex();
            Perm<dim + 1> subface = f->front().vertices();
            int i = dim-subdim+1;
            while (true) {
                // Examine the facet that excludes vertex number i of the
                // (dim-subdim+1)-face f.
                int facetNum = Face<dim, dim-subdim>::faceNumber(subface);
                auto* facet = s->template face<dim-subdim>(facetNum);
                if (! facet->isBoundary()) {
                    // The following permutation should map
                    // {0,...,dim-subdim} to {0,...,dim-subdim}, and should map
                    // {dim-subdim+1,...,dim} to {dim-subdim+1,...,dim}.
                    Perm p = s->template faceMapping<dim-subdim>(facetNum).
                        inverse() * subface;

                    // We need the sign of the induced permutation of
                    // {dim-subdim+1,...,dim}.
                    if constexpr (subdim == dim) {
                        if (p.sign() > 0)
                            ++bdry.entry(row, lookup[facet->index()]);
                        else
                            --bdry.entry(row, lookup[facet->index()]);
                    } else {
                        if (Perm<dim-subdim+1>::contract(p).sign() == p.sign())
                            ++bdry.entry(row, lookup[facet->index()]);
                        else
                            --bdry.entry(row, lookup[facet->index()]);
                    }
                }

                if (i > 0) {
                    --i;
                    subface = subface * Perm<dim+1>(i, dim-subdim+1);
                } else
                    break;
            }
            ++row;
        }

        return bdry;
    }
}

template <int dim>
template <int subdim>
MatrixInt TriangulationBase<dim>::dualToPrimal() const {
    static_assert(subdim >= 0 && subdim < dim);

    ensureSkeleton();

    if constexpr (subdim == 0) {
        MatrixInt ans(std::get<0>(faces_).size(), size());

        size_t col = 0;
        for (auto dual : simplices_)
            ans.entry(dual->vertex(0)->index(), col++) = 1;

        return ans;
    } else {
        MatrixInt ans(
            std::get<subdim>(faces_).size(),
            std::get<dim-subdim>(faces_).size() - nBoundaryFaces_[dim-subdim]);

        size_t col = 0;
        for (auto dual : faces<dim-subdim>()) {
            if (dual->isBoundary())
                continue;

            if constexpr (subdim == 1) {
                // The endpoints of the dual edge are labelled (0, 1)
                // according to (dual->back(), dual->front()).
                {
                    auto emb = dual->front();
                    // Edge: centre of dual -> centre of simplex
                    int v0 = emb.vertices()[0];
                    if (v0 != 0) {
                        // Add edge (v0, 0) of emb.simplex().
                        int e = (dim == 2 ? 3 - v0 : v0 - 1);
                        Edge<dim>* edge = emb.simplex()->edge(e);
                        if (emb.simplex()->edgeMapping(e)[0] == 0)
                            --ans.entry(edge->index(), col);
                        else
                            ++ans.entry(edge->index(), col);
                    }
                }
                {
                    auto emb = dual->back();
                    // Edge: centre of simplex -> centre of dual
                    int v0 = emb.vertices()[0];
                    if (v0 != 0) {
                        // Add edge (0, v0) of emb.simplex().
                        int e = (dim == 2 ? 3 - v0 : v0 - 1);
                        Edge<dim>* edge = emb.simplex()->edge(e);
                        if (emb.simplex()->edgeMapping(e)[0] == 0)
                            ++ans.entry(edge->index(), col);
                        else
                            --ans.entry(edge->index(), col);
                    }
                }
            } else {
                int v[subdim + 1];
                v[subdim] = 0;

                for (const auto& emb : *dual) {
                    v[0] = emb.vertices()[0];
                    if (v[0] == 0)
                        continue;

                    if constexpr (subdim == 2) {
                        // This follows the generic code for higher face
                        // dimensions below; however, it is streamlined here
                        // since this is a common case (in particular, it is
                        // used to compute 4-manifold intersection forms).
                        v[1] = emb.simplex()->template faceMapping<dim-1>(
                            Face<dim, dim-1>::faceNumber(emb.vertices()))[0];
                        if (v[1] != 0 && v[1] != v[0]) {
                            // Build a permutation (v0, v1, 0, junk...).
                            Perm<dim+1> primal(0, v[0]);
                            primal = Perm<dim+1>(primal[1], v[1]) * primal;
                            primal = Perm<dim+1>(primal[2], 0) * primal;

                            int n = Face<dim, 2>::faceNumber(primal);
                            size_t row = emb.simplex()->template face<2>(n)->
                                index();

                            // Get the inherent ordering of vertices {v0, v1, 0}
                            // for the corresponding triangle.
                            auto map = emb.simplex()->
                                template faceMapping<2>(n);

                            // Now we can find out how we have reordered
                            // the inherent vertices {0,1,2}.
                            if (Perm<3>::contract(map.inverse() * primal).
                                    sign() > 0) {
                                ++ans.entry(row, col);
                            } else {
                                --ans.entry(row, col);
                            }
                        }

                        // This second piece of the dual triangle is reflected.
                        v[1] = emb.simplex()->template faceMapping<dim-1>(
                            Face<dim, dim-1>::faceNumber(emb.vertices() *
                                Perm<dim+1>(dim-1, dim)))[0];
                        if (v[1] != 0 && v[1] != v[0]) {
                            // Build a permutation (v0, v1, 0, junk...).
                            Perm<dim+1> primal(0, v[0]);
                            primal = Perm<dim+1>(primal[1], v[1]) * primal;
                            primal = Perm<dim+1>(primal[2], 0) * primal;

                            int n = Face<dim, 2>::faceNumber(primal);
                            size_t row = emb.simplex()->template face<2>(n)->
                                index();

                            // Get the inherent ordering of vertices {v0, v1, 0}
                            // for the corresponding triangle.
                            auto map = emb.simplex()->
                                template faceMapping<2>(n);

                            // Now we can find out how we have reordered
                            // the inherent vertices {0,1,2}.
                            if (Perm<3>::contract(map.inverse() * primal).
                                    sign() > 0) {
                                --ans.entry(row, col);
                            } else {
                                ++ans.entry(row, col);
                            }
                        }
                    } else {
                        auto rot1 = Perm<dim + 1>::rot(subdim);
                        auto rot2 = rot1.inverse();
                        for (typename Perm<subdim>::Index p = 0;
                                p < Perm<subdim>::nPerms; ++p) {
                            // We need to apply Perm<subdim>::Sn[p] to the
                            // last subdim elements of emb.vertices().
                            Perm<dim + 1> vertices = emb.vertices() * rot2 *
                                Perm<dim+1>::extend(Perm<subdim>::Sn[p]) * rot1;
                            bool distinct = true;
                            for_constexpr<1, subdim>([&](auto k) {
                                if (distinct) {
                                    v[k] = emb.simplex()->
                                        template faceMapping<dim - subdim + k>(
                                        Face<dim, dim - subdim + k>::faceNumber(
                                        vertices))[0];
                                    if (v[k] == 0)
                                        distinct = false;
                                    else {
                                        for (int j = 0; j < k; ++j)
                                            if (v[k] == v[j]) {
                                                distinct = false;
                                                break;
                                            }
                                    }
                                }
                            });
                            if (! distinct)
                                continue;

                            // Build a permutation (v0, v1, ..., 0, junk...).
                            Perm<dim+1> primal(subdim, 0);
                            for (int k = 0; k < subdim; ++k)
                                primal = Perm<dim+1>(primal[k], v[k]) * primal;

                            int n = Face<dim, subdim>::faceNumber(primal);
                            size_t row = emb.simplex()->
                                template face<subdim>(n)->index();

                            // Get the inherent ordering of vertices
                            // {v0, v1, ..., 0} for the corresponding face.
                            auto map = emb.simplex()->
                                template faceMapping<subdim>(n);

                            // Now we can find out how we have reordered
                            // the inherent vertices {0,1,...,subdim}.
                            if (Perm<subdim+1>::contract(
                                    map.inverse() * primal).sign() > 0) {
                                if (p % 2 == 0)
                                    ++ans.entry(row, col);
                                else
                                    --ans.entry(row, col);
                            } else {
                                if (p % 2 == 0)
                                    --ans.entry(row, col);
                                else
                                    ++ans.entry(row, col);
                            }
                        }
                    }
                }
            }

            ++col;
        }

        return ans;
    }
}

} // namespace regina::detail

#endif
