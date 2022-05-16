
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

/*! \file triangulation/detail/facetpairing-canonical-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  FacetPairing class template.
 *
 *  This file is \e not included from facetpairing.h, and it is not
 *  shipped with Regina's development headers.  The routines it contains are
 *  explicitly instantiated in Regina's calculation engine for all dimensions.
 *
 *  The reason for "quarantining" this file is simply to avoid putting
 *  excessive implementation details in the headers where this is not needed.
 */

#ifndef __REGINA_FACETPAIRING_CANONICAL_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_FACETPAIRING_CANONICAL_IMPL_H_DETAIL
#endif

#include "triangulation/generic/facetpairing.h"
#include "triangulation/generic/isomorphism.h"

namespace regina::detail {

/**
 * Gives a unified way to initialise and update the isomorphisms(s) that
 * are returned by FacetPairingBase::canonicalInternal().
 */
template <int dim, bool allIsos>
struct CanonicalInternalReturn;

template <int dim>
struct CanonicalInternalReturn<dim, true> {
    typename FacetPairingBase<dim>::IsoList result;

    CanonicalInternalReturn(size_t size) {
    }

    void append(const Isomorphism<dim>& iso) {
        result.push_back(iso);
    }

    void reset(const Isomorphism<dim>& iso) {
        result.clear();
        result.push_back(iso);
    }
};

template <int dim>
struct CanonicalInternalReturn<dim, false> {
    Isomorphism<dim> result;

    CanonicalInternalReturn(size_t size) : result(size) {}

    void append(const Isomorphism<dim>&) {
        // We only need to return one isomorphism, so just ignore any others.
    }

    void reset(const Isomorphism<dim>& iso) {
        result = iso;
    }
};

template <int dim>
bool FacetPairingBase<dim>::isCanonical() const {
    // Check the preconditions for isCanonicalInternal().
    for (size_t simp = 0; simp < size_; ++simp) {
        for (int facet = 0; facet < dim; ++facet)
            if (dest(simp, facet + 1) < dest(simp, facet))
                if (dest(simp, facet + 1) != FacetSpec<dim>(simp, facet))
                    return false;
        // Note: any signed/unsigned comparisons between simplex numbers are
        // okay, since every destination simplex should be non-negative anyway.
        if (simp > 0)
            if (dest(simp, 0).simp >= simp)
                return false;
        if (simp > 1)
            if (dest(simp, 0) <= dest(simp - 1, 0))
                return false;
    }

    // We've met all the preconditions, so we can now run
    // isCanonicalInternal().
    return isCanonicalInternal();
}

template <int dim>
template <bool allIsos>
std::pair<FacetPairing<dim>,
        typename FacetPairingBase<dim>::template CanonicalIsos<allIsos>>
        FacetPairingBase<dim>::canonicalInternal() const {
    const FacetPairing<dim>& me = static_cast<const FacetPairing<dim>&>(*this);

    if (size_ == 0) {
        if constexpr (allIsos) {
            IsoList autos;
            autos.push_back(Isomorphism<dim>::identity(0));
            return { me, std::move(autos) };
        } else {
            return { me, Isomorphism<dim>::identity(0) };
        }
    }

    FacetPairing<dim> best(me);
    CanonicalInternalReturn<dim, allIsos> bestIso(size_);

    // Create the isomorphism (this -> canonical) one simplex at a time,
    // selecting the preimage of 0 first, then the preimage of 1 and so on.
    Isomorphism<dim> to(size_); // this -> canonical
    Isomorphism<dim> from(size_); // canonical -> this

    for (size_t i = 0; i < size_; ++i)
        to.simpImage(i) = from.simpImage(i) = -1;

    auto* perm = new typename Perm<dim+1>::Index[size_];
    auto* usedSimp = new ssize_t[size_ + 1];
    usedSimp[0] = 1;

    ssize_t currSimp = 0;

    // Note the decision point at which the current selection moved
    // from being lexicographically equal to the previous best solution
    // to being strictly lexicographically smaller.
    // If the current solution (as far as it has been determined) is still
    // lexicographically equal, then this will be the same as currSimp.
    //
    // We begin by setting lexSmallerFrom to -1, since there is no
    // previous best solution.
    ssize_t lexSmallerFrom = -1;

    // Run through all possible preimages of simplex 0.
    for (size_t pre0 = 0; pre0 < size_; ++pre0) {
        from.simpImage(0) = pre0;
        to.simpImage(pre0) = 0;

        perm[0] = 0;

        while (true) {
            if (currSimp == size_) {
                // We have a complete pair of isomorphisms!
                if (lexSmallerFrom == size_) {
                    // We have found an automorphism.
                    bestIso.append(to);

                    // Since we are about to decrement currSimp:
                    --lexSmallerFrom;
                } else {
                    // This solution is strictly better.
                    bestIso.reset(to);
                    best = to(me);

                    // We were strictly smaller before, but now we are
                    // equal to the best known solution.
                    lexSmallerFrom = size_ - 1;
                }

                --currSimp;
                ++perm[currSimp];

                // There is no need to roll back changes to usedSimp here,
                // since the last simplex cannot have been connected to
                // anything *beyond* the last simplex.

                continue;
            }

            // Due to connectedness and lexicographical minimality, the
            // preimage of this simplex should already be fixed.
            // However, the facet permutations is only partially
            // determined, in that we know the preimages of some initial
            // set of facets 0..k but not the remaining facets k+1..dim.

            while (true) {
                if (perm[currSimp] == Perm<dim+1>::nPerms) {
                    // Out of options for this permutation.
                    if (currSimp == 0)
                        goto endPermSearch;

                    // Roll back.
                    if (lexSmallerFrom == currSimp)
                        --lexSmallerFrom;
                    --currSimp;

                    for (ssize_t i = usedSimp[currSimp];
                            i < usedSimp[currSimp + 1]; ++i)
                        if (from.simpImage(i) >= 0) {
                            to.simpImage(from.simpImage(i)) = -1;
                            from.simpImage(i) = -1;
                        }

                    ++perm[currSimp];
                    break;
                }

                // We are sitting on the next permutation to try.
                Perm<dim+1> p = Perm<dim+1>::Sn[perm[currSimp]];

                usedSimp[currSimp + 1] = usedSimp[currSimp];
                bool smaller = (lexSmallerFrom < currSimp);
                bool unusable = false;
                FacetSpec<dim> prevDest(-1, dim);
                for (int i = 0; i <= dim; ++i) {
                    // Examine the candiate canonical permutations's
                    // dest(currSimp, i).
                    FacetSpec<dim> nextMe =
                        dest(from.simpImage(currSimp), p[i]);
                    FacetSpec<dim> nextCanon;
                    if (nextMe.simp == size_) {
                        // This is a boundary facet.
                        nextCanon = nextMe; // also boundary
                    } else {
                        ssize_t nextSimp = to.simpImage(nextMe.simp);
                        if (nextSimp < 0) {
                            // This gluing goes beyond the range of
                            // simplices that have been decided already.
                            // Make sure it goes to the next free simplex.
                            if (prevDest.simp == size_) {
                                // Non-boundary cannot come *after* boundary in
                                // a lexicographically minimal representation.
                                unusable = true;
                                break;
                            }
                            nextCanon = FacetSpec<dim>(
                                usedSimp[currSimp + 1]++, 0);
                            from.simpImage(nextCanon.simp) = nextMe.simp;
                            to.simpImage(nextMe.simp) = nextCanon.simp;
                        } else if (nextSimp == currSimp) {
                            // This is glued to another facet of this simplex.
                            nextCanon = { nextSimp, p.pre(nextMe.facet) };
                            if (currSimp < prevDest.simp) {
                                // This cannot lead to something that is
                                // lexicographically minimal.
                                unusable = true;
                                break;
                            } else if (currSimp == prevDest.simp &&
                                    nextCanon < prevDest) {
                                // The previous facet is also glued to this
                                // simplex.  If nextCanon < prevDest, then
                                // nextCanon must refer exactly to that
                                // previous facet (i.e., we are seeing the
                                // second side of a gluing (s,f) <-> (s,f+1)).
                                if (nextCanon.facet != i - 1) {
                                    // Again, this cannot lead to something that
                                    // is lexicographically minimal.
                                    unusable = true;
                                    break;
                                }
                            }
                        } else if (nextSimp < currSimp) {
                            // This is glued to a facet of an earlier simplex,
                            // whose permutation has already been decided.
                            nextCanon = to(nextMe);
                            if (nextCanon < prevDest) {
                                // This cannot lead to something that is
                                // lexicographically minimal.
                                unusable = true;
                                break;
                            }

                            // Since this is the other side of a gluing that has
                            // already been decided, check that the gluing is
                            // consistent with the other direction.
                            auto other = dest(from(nextCanon));
                            if (from.simpImage(currSimp) != other.simp ||
                                    p[i] != other.facet) {
                                unusable = true;
                                break;
                            }
                        } else {
                            // This destination is a simplex whose number
                            // has been fixed but whose permutation has not.
                            //
                            // In this case, we do not know (easily) how
                            // many facets of nextSimp have already been
                            // accounted for, and so we do not know what
                            // the canonical destination facet *should* be.
                            // We just call it 1 here (since this is not the
                            // first time we have visited this destination
                            // simplex).  This weakens but does not break
                            // the lexicographical comparison with prevDest;
                            // if we do end up putting things in the wrong
                            // order as a result then this will be noticed
                            // when we process nextSimp.
                            //
                            // Note that we also need to tweak the
                            // comparison with best.dest(...) accordingly.
                            nextCanon = { nextSimp, 1 };
                            if (nextCanon < prevDest) {
                                // This cannot lead to something that is
                                // lexicographically minimal.
                                unusable = true;
                                break;
                            }
                        }
                    }
                    prevDest = nextCanon;

                    if (! smaller) {
                        auto nextBest = best.dest(currSimp, i);
                        if (nextCanon.simp > currSimp) {
                            // Account for the fact that nextCanon might
                            // be using the wrong facet number; see the
                            // more detailed discussion above where we
                            // set nextCanon = { nextSimp, 1 }.
                            // Note that the boundary is unaffected by this,
                            // since the boundary is represented as size_:0.
                            if (nextBest.facet > 1)
                                nextBest.facet = 1;

                            // Now we can safely do our lexicographical
                            // comparison.
                            if (nextCanon < nextBest)
                                smaller = true;
                            else if (nextBest < nextCanon) {
                                unusable = true;
                                break;
                            }
                        } else {
                            if (nextCanon < nextBest)
                                smaller = true;
                            else if (nextBest < nextCanon) {
                                unusable = true;
                                break;
                            }
                        }
                    }
                }
                if (unusable) {
                    for (ssize_t i = usedSimp[currSimp];
                            i < usedSimp[currSimp + 1]; ++i)
                        if (from.simpImage(i) >= 0) {
                            to.simpImage(from.simpImage(i)) = -1;
                            from.simpImage(i) = -1;
                        }
                } else {
                    // We are committing to this permutation.
                    from.facetPerm(currSimp) = p;
                    to.facetPerm(from.simpImage(currSimp)) = p.inverse();

                    // Go deeper.
                    ++currSimp;
                    if (! smaller)
                        ++lexSmallerFrom;
                    if (currSimp < size_)
                        perm[currSimp] = 0;
                    break;
                }

                // This permutation is not usable; move directly
                // to the next one.
                ++perm[currSimp];
            }
        }

endPermSearch:
        from.simpImage(0) = to.simpImage(pre0) = -1;
    }

    delete[] perm;
    delete[] usedSimp;
    return { std::move(best), std::move(bestIso.result) };
}

template <int dim>
bool FacetPairingBase<dim>::isCanonicalInternal(
        typename FacetPairingBase<dim>::IsoList* list) const {
    // Create the automorphisms one simplex at a time, selecting the
    // preimage of 0 first, then the preimage of 1 and so on.

    // We want to cycle through all possible first facet gluings, so we'll
    // special-case the situation in which there are no facet gluings at all.
    if (isUnmatched(0, 0)) {
        // We must have just one simplex with no facet gluings at all.
        if (list) {
            for (int i = 0; i < Perm<dim+1>::nPerms; ++i) {
                Isomorphism<dim> ans(1);
                ans.simpImage(0) = 0;
                ans.facetPerm(0) = Perm<dim+1>::orderedSn[i];
                list->push_back(std::move(ans));
            }
        }
        return true;
    }

    // Now we know that facet 0 of simplex 0 is glued to something.

    auto* image = new FacetSpec<dim>[size_ * (dim + 1)];
        /**< The automorphism currently under construction. */
    auto* preImage = new FacetSpec<dim>[size_ * (dim + 1)];
        /**< The inverse of this automorphism. */

    size_t i, j;
    for (i = 0; i < size_ * (dim + 1); ++i) {
        image[i].setBeforeStart();
        preImage[i].setBeforeStart();
    }

    // Note that we know size_ >= 1.
    // For the preimage of facet 0 of simplex 0 we simply cycle
    // through all possibilities.
    const FacetSpec<dim> firstFace(0, 0);
    const FacetSpec<dim> firstFaceDest(dest(firstFace));
    FacetSpec<dim> firstDestPre;
    FacetSpec<dim> trying;
    FacetSpec<dim> fImg, fPre;
    bool stepDown;
    ssize_t simp;
    int facet;
    for (preImage[0] = firstFace ; ! preImage[0].isPastEnd(size_, true);
            ++preImage[0]) {
        // Note that we know firstFace is not unmatched.
        if (isUnmatched(preImage[0]))
            continue;

        // If firstFace glues to the same simplex and this facet
        // doesn't, we can ignore this permutation.
        firstDestPre = dest(preImage[0]);
        if (firstFaceDest.simp == 0 && firstDestPre.simp != preImage[0].simp)
            continue;

        // If firstFace doesn't glue to the same simplex but this
        // facet does, we're not in canonical form.
        if (firstFaceDest.simp != 0 && firstDestPre.simp == preImage[0].simp) {
            if (list)
                list->clear();
            delete[] image;
            delete[] preImage;
            return false;
        }

        // We can use this facet.  Set the corresponding reverse mapping
        // and off we go.
        image[preImage[0].simp * (dim + 1) + preImage[0].facet] = firstFace;
        preImage[firstFaceDest.simp * (dim + 1) + firstFaceDest.facet] =
            firstDestPre;
        image[firstDestPre.simp * (dim + 1) + firstDestPre.facet] =
            firstFaceDest;

        // Step forwards to the next facet whose preimage is undetermined.
        trying = firstFace;
        ++trying;
        if (trying == firstFaceDest)
            ++trying;
        while (! (trying == firstFace)) {
            // INV: We've successfully selected preimages for all facets
            // before trying.  We're currently looking at the last
            // attempted candidate for the preimage of trying.

            // Note that if preimage facet A is glued to preimage facet B
            // and the image of A is earlier than the image of B, then
            // the image of A will be selected whereas the image of B
            // will be automatically derived.

            stepDown = false;
            FacetSpec<dim>& pre =
                preImage[trying.simp * (dim + 1) + trying.facet];

            if (trying.isPastEnd(size_, true)) {
                // We have a complete automorphism!
                Isomorphism<dim> ans(size_);
                for (i = 0; i < size_; i++) {
                    ans.simpImage(i) = image[i * (dim + 1)].simp;
                    std::array<int, dim+1> permImg;
                    for (j = 0; j <= dim; ++j)
                        permImg[j] = image[i * (dim + 1) + j].facet;
                    ans.facetPerm(i) = Perm<dim+1>(permImg);
                }
                if (list)
                    list->push_back(std::move(ans));
                stepDown = true;
            } else {
                // Move to the next candidate.
                if (pre.simp >= 0 && pre.facet == dim) {
                    // We're all out of candidates.
                    pre.setBeforeStart();
                    stepDown = true;
                } else {
                    if (pre.isBeforeStart()) {
                        // Which simplex must we look in?
                        // Note that this simplex will already have been
                        // determined.
                        pre.simp = preImage[trying.simp * (dim + 1)].simp;
                        pre.facet = 0;
                    } else
                        ++pre.facet;

                    // Step forwards until we have a preimage whose image
                    // has not already been set.
                    // If the preimage is unmatched and trying isn't,
                    // we'll also skip it.
                    // If trying is unmatched and the preimage isn't,
                    // we're not in canonical form.
                    for ( ; pre.facet <= dim; ++pre.facet) {
                        if (! image[pre.simp * (dim + 1) + pre.facet].
                                isBeforeStart())
                            continue;
                        if ((! isUnmatched(trying)) && isUnmatched(pre))
                            continue;
                        if (isUnmatched(trying) && (! isUnmatched(pre))) {
                            // We're not in canonical form.
                            if (list)
                                list->clear();
                            delete[] image;
                            delete[] preImage;
                            return false;
                        }
                        break;
                    }
                    while (pre.facet <= dim &&
                            ! image[pre.simp * (dim + 1) + pre.facet].
                            isBeforeStart())
                        ++pre.facet;
                    if (pre.facet == (dim + 1)) {
                        pre.setBeforeStart();
                        stepDown = true;
                    }
                }
            }

            if (! stepDown) {
                // We found a candidate.
                // We also know that trying is unmatched iff the preimage
                // is unmatched.
                image[pre.simp * (dim + 1) + pre.facet] = trying;
                if (! isUnmatched(pre)) {
                    fPre = dest(pre);
                    if (image[fPre.simp * (dim + 1) + fPre.facet].
                            isBeforeStart()) {
                        // The image of fPre (the partner of the preimage
                        // facet) can be determined at this point.
                        // Specifically, it should go into the next
                        // available slot.

                        // Do we already know which simplex we should
                        // be looking into?
                        for (i = 0; i <= dim; i++)
                            if (! image[fPre.simp * (dim + 1) + i].
                                    isBeforeStart()) {
                                // Here's the simplex!
                                // Find the first available facet.
                                simp = image[fPre.simp * (dim + 1) + i].simp;
                                for (facet = 0;
                                        ! preImage[simp * (dim + 1) + facet].
                                        isBeforeStart(); ++facet)
                                    ;
                                image[fPre.simp * (dim + 1) +
                                    fPre.facet].simp = simp;
                                image[fPre.simp * (dim + 1) +
                                    fPre.facet].facet = facet;
                                break;
                            }
                        if (i == (dim + 1)) {
                            // We need to map to a new simplex.
                            // Find the first available simplex.
                            for (simp = trying.simp + 1;
                                    ! preImage[simp * (dim + 1)].
                                    isBeforeStart();
                                    ++simp)
                                ;
                            image[fPre.simp * (dim + 1) + fPre.facet].simp =
                                simp;
                            image[fPre.simp * (dim + 1) + fPre.facet].facet = 0;
                        }

                        // Set the corresponding preimage.
                        fImg = image[fPre.simp * (dim + 1) + fPre.facet];
                        preImage[fImg.simp * (dim + 1) + fImg.facet] = fPre;
                    }
                }

                // Do a lexicographical comparison and shunt trying up
                // if need be.
                do {
                    fImg = dest(trying);
                    fPre = dest(preImage[trying.simp * (dim + 1) +
                        trying.facet]);
                    if (! fPre.isBoundary(size_))
                        fPre = image[fPre.simp * (dim + 1) + fPre.facet];

                    // Currently trying is glued to fImg.
                    // After applying our isomorphism, trying will be
                    // glued to fPre.

                    if (fImg < fPre) {
                        // This isomorphism will lead to a
                        // lexicographically greater representation.
                        // Ignore it.
                        stepDown = true;
                    } else if (fPre < fImg) {
                        // Whapow, we're not in canonical form.
                        if (list)
                            list->clear();
                        delete[] image;
                        delete[] preImage;
                        return false;
                    }

                    // What we have so far is consistent with an automorphism.
                    ++trying;
                } while (! (stepDown || trying.isPastEnd(size_, true) ||
                        preImage[trying.simp * (dim + 1) + trying.facet].
                        isBeforeStart()));
            }

            if (stepDown) {
                // We're shunting trying back down.
                --trying;
                while (true) {
                    fPre = preImage[trying.simp * (dim + 1) + trying.facet];
                    if (! isUnmatched(fPre)) {
                        fPre = dest(fPre);
                        if (image[fPre.simp * (dim + 1) + fPre.facet] <
                                trying) {
                            // This preimage/image was automatically derived.
                            --trying;
                            continue;
                        }
                    }
                    break;
                }

                // Note that this resetting of facets that follows will
                // also take place when trying makes it all the way back
                // down to firstFace.
                fPre = preImage[trying.simp * (dim + 1) + trying.facet];
                image[fPre.simp * (dim + 1) + fPre.facet].setBeforeStart();
                if (! isUnmatched(fPre)) {
                    fPre = dest(fPre);
                    fImg = image[fPre.simp * (dim + 1) + fPre.facet];
                    preImage[fImg.simp * (dim + 1) + fImg.facet].
                        setBeforeStart();
                    image[fPre.simp * (dim + 1) + fPre.facet].setBeforeStart();
                }
            }
        }
    }

    // The pairing is in canonical form and we have all our automorphisms.
    // Tidy up and return.
    delete[] image;
    delete[] preImage;
    return true;
}

} // namespace regina::detail

#endif
