
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
 *  This file is \e not included from triangulation.h, but the routines
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

        // Our aim is to build a chain complex:
        // A (dual (k+1)-faces) -> B (dual k-faces) -> C (dual (k-1)-faces).
        //
        // Begin by computing the dimensions of A, B and C, and also for
        // spaces A and B building a map from face index to coordinate position.

        size_t dimA = 0, dimB = 0, dimC = 0;
        std::vector<size_t> indexA(countFaces<dim-k-1>());
        std::vector<size_t> indexB(countFaces<dim-k>());

        for (auto a : faces<dim-k-1>())
            if (! a->isBoundary())
                indexA[a->index()] = dimA++;
        for (auto b : faces<dim-k>())
            if (! b->isBoundary())
                indexB[b->index()] = dimB++;
        for (auto c : faces<dim-k+1>())
            if (! c->isBoundary())
                ++dimC;

        return AbelianGroup(dualBoundaryMap<k>(indexB, dimB, dimC),
            dualBoundaryMap<k + 1>(indexA, dimA, dimB));
    }
}

template <int dim>
const GroupPresentation& TriangulationBase<dim>::fundamentalGroup() const {
    if (fundGroup_.has_value())
        return *fundGroup_;

    GroupPresentation ans;

    if (isEmpty())
        return *(fundGroup_ = std::move(ans));

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
    long* genIndex = new long[countFaces<dim-1>()];
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

    // Tidy up.
    delete[] genIndex;
    ans.intelligentSimplify();

    return *(fundGroup_ = std::move(ans));
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
                unsigned facetNum = Face<dim, subdim - 1>::faceNumber(subface);
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
MatrixInt TriangulationBase<dim>::dualBoundaryMap(
        const std::vector<size_t>& lookup, size_t domain, size_t codomain)
        const {
    static_assert(subdim >= 2 && subdim <= dim);
    static_assert(standardDim(dim) || subdim < dim);

    MatrixInt bdry(codomain, domain);
    size_t row = 0;
    for (auto f : faces<dim-subdim+1>()) {
        if (f->isBoundary())
            continue;

        // The dual face to f sits on the boundary of which other dual faces?
        Simplex<dim>* s = f->front().simplex();
        Perm<dim + 1> subface = f->front().vertices();
        int i = dim-subdim+1;
        while (true) {
            // Examine the facet that excludes vertex number i of the
            // (dim-subdim+1)-face f.
            unsigned facetNum = Face<dim, dim-subdim>::faceNumber(subface);
            auto* facet = s->template face<dim-subdim>(facetNum);
            if (! facet->isBoundary()) {
                // The following permutation should map
                // {0,...,dim-subdim} to {0,...,dim-subdim}, and should map
                // {dim-subdim+1,...,dim} to {dim-subdim+1,...,dim}.
                Perm p = s->template faceMapping<dim-subdim>(facetNum).inverse()
                    * subface;

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

} // namespace regina::detail

#endif
