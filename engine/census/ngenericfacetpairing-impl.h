
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file census/ngenericfacetpairing-impl.h
 *  \brief Contains implementation details for the NGenericFacetPairing
 *  class template.
 *
 *  This file is \e not included automatically by ngenericfacetpairing.h.
 *  However, typical end users should never need to include it, since
 *  Regina's calculation engine provides full explicit instantiations
 *  of NGenericFacetPairing for \ref stddim "standard dimensions".
 */

#include <algorithm>
#include <sstream>
#include <vector>
#include "census/ngenericfacetpairing.h"
#include "utilities/memutils.h"
#include "utilities/stringutils.h"

namespace regina {

template <int dim>
NGenericFacetPairing<dim>::NGenericFacetPairing(
        const NGenericFacetPairing<dim>& cloneMe) : size_(cloneMe.size_),
        pairs_(new NFacetSpec<dim>[cloneMe.size_ * (dim + 1)]) {
    std::copy(cloneMe.pairs_, cloneMe.pairs_ + (size_ * (dim + 1)), pairs_);
}

template <int dim>
NGenericFacetPairing<dim>::NGenericFacetPairing(
        const typename NGenericFacetPairing<dim>::Triangulation& tri) :
        size_(tri.getNumberOfSimplices()),
        pairs_(new NFacetSpec<dim>[tri.getNumberOfSimplices() * (dim + 1)]) {
    unsigned p, f, index;
    const Simplex *simp, *adj;
    for (index = 0, p = 0; p < size_; ++p) {
        simp = tri.getSimplex(p);
        for (f = 0; f <= dim; ++f) {
            adj = simp->adjacentSimplex(f);
            if (adj) {
                pairs_[index].simp = tri.simplexIndex(adj);
                pairs_[index].facet = simp->adjacentFacet(f);
            } else
                pairs_[index].setBoundary(size_);

            ++index;
        }
    }
}

template <int dim>
bool NGenericFacetPairing<dim>::isClosed() const {
    for (NFacetSpec<dim> f(0, 0); ! f.isPastEnd(size_, true); ++f)
        if (isUnmatched(f))
            return false;
    return true;
}

template <int dim>
std::string NGenericFacetPairing<dim>::str() const {
    std::ostringstream ans;

    for (NFacetSpec<dim> f(0, 0); ! f.isPastEnd(size_, true); ++f) {
        if (f.facet == 0 && f.simp > 0)
            ans << " | ";
        else if (f.simp || f.facet)
            ans << ' ';

        if (dest(f).isBoundary(size_))
            ans << "bdry";
        else
            ans << dest(f).simp << ':' << dest(f).facet;
    }
    return ans.str();
}

template <int dim>
std::string NGenericFacetPairing<dim>::dotHeader(const char* graphName) {
    std::ostringstream ans;
    writeDotHeader(ans, graphName);
    return ans.str();
}

template <int dim>
void NGenericFacetPairing<dim>::writeDotHeader(
        std::ostream& out, const char* graphName) {
    static const char defaultGraphName[] = "G";

    if ((! graphName) || (! *graphName))
        graphName = defaultGraphName;

    out << "graph " << graphName << " {" << std::endl;
    out << "edge [color=black];" << std::endl;
    out << "node [shape=circle,style=filled,height=0.15,fixedsize=true,label=\"\",fontsize=9,fontcolor=\"#751010\"];" << std::endl;
}

template <int dim>
std::string NGenericFacetPairing<dim>::dot(
        const char* prefix, bool subgraph, bool labels) const {
    std::ostringstream ans;
    writeDot(ans, prefix, subgraph, labels);
    return ans.str();
}

template <int dim>
void NGenericFacetPairing<dim>::writeDot(std::ostream& out,
        const char* prefix, bool subgraph, bool labels) const {
    static const char defaultPrefix[] = "g";

    if ((! prefix) || (! *prefix))
        prefix = defaultPrefix;

    // We are guaranteed that prefix is a non-empty string.

    if (subgraph)
        out << "subgraph pairing_" << prefix << " {" << std::endl;
    else
        writeDotHeader(out, (prefix + std::string("_graph")).c_str());

    // Ancient versions of graphviz seem to ignore the default label="".
    // Make this explicit for each node.
    unsigned p;
    for (p = 0; p < size_; ++p) {
        out << prefix << '_' << p << " [label=\"";
        if (labels)
            out << p;
        out << "\"]" << std::endl;
    }

    int f;
    NFacetSpec<dim> adj;
    for (p = 0; p < size_; ++p)
        for (f = 0; f < (dim + 1); ++f) {
            adj = dest(p, f);
            if (adj.isBoundary(size_) ||
                    adj.simp < static_cast<int>(p) ||
                    (adj.simp == static_cast<int>(p) && adj.facet < f))
                continue;
            out << prefix << '_' << p << " -- " << prefix << '_'
                << adj.simp << ';' << std::endl;
        }

    out << '}' << std::endl;
}

template <int dim>
std::string NGenericFacetPairing<dim>::toTextRep() const {
    std::ostringstream ans;

    for (NFacetSpec<dim> f(0, 0); ! f.isPastEnd(size_, true); ++f) {
        if (f.simp || f.facet)
            ans << ' ';
        ans << dest(f).simp << ' ' << dest(f).facet;
    }

    return ans.str();
}

template <int dim>
typename NGenericFacetPairing<dim>::FacetPairing*
        NGenericFacetPairing<dim>::fromTextRep(const std::string& rep) {
    std::vector<std::string> tokens;
    unsigned nTokens = basicTokenise(back_inserter(tokens), rep);

    if (nTokens == 0 || nTokens % (2 * (dim + 1)) != 0)
        return 0;

    long nSimp = nTokens / (2 * (dim + 1));
    FacetPairing* ans = new FacetPairing(nSimp);

    // Read the raw values.
    // Check the range of each value while we're at it.
    long val;
    for (long i = 0; i < nSimp * (dim + 1); ++i) {
        if (! valueOf(tokens[2 * i], val)) {
            delete ans;
            return 0;
        }
        if (val < 0 || val > nSimp) {
            delete ans;
            return 0;
        }
        ans->pairs_[i].simp = val;

        if (! valueOf(tokens[2 * i + 1], val)) {
            delete ans;
            return 0;
        }
        if (val < 0 || val >= (dim + 1)) {
            delete ans;
            return 0;
        }
        ans->pairs_[i].facet = val;
    }

    // Run a sanity check.
    NFacetSpec<dim> destFacet;
    bool broken = false;
    for (NFacetSpec<dim> f(0, 0); ! f.isPastEnd(nSimp, true); ++f) {
        destFacet = ans->dest(f);
        if (destFacet.simp == nSimp && destFacet.facet != 0)
            broken = true;
        else if (destFacet.simp < nSimp && ! (ans->dest(destFacet) == f))
            broken = true;
        else
            continue;
        break;
    }

    if (broken) {
        delete ans;
        return 0;
    }

    // All is well.
    return ans;
}

template <int dim>
bool NGenericFacetPairing<dim>::isCanonical() const {
    // Check the preconditions for isCanonicalInternal().
    unsigned simp, facet;
    for (simp = 0; simp < size_; ++simp) {
        for (facet = 0; facet < dim; ++facet)
            if (dest(simp, facet + 1) < dest(simp, facet))
                if (! (dest(simp, facet + 1) == NFacetSpec<dim>(simp, facet)))
                    return false;
        if (simp > 0)
            if (dest(simp, 0).simp >= static_cast<int>(simp))
                return false;
        if (simp > 1)
            if (dest(simp, 0) <= dest(simp - 1, 0))
                return false;
    }

    // We've met all the preconditions, so we can now run
    // isCanonicalInternal().
    IsoList list;
    return isCanonicalInternal(list);
}

template <int dim>
bool NGenericFacetPairing<dim>::isCanonicalInternal(
        typename NGenericFacetPairing<dim>::IsoList& list) const {
    // Create the automorphisms one simplex at a time, selecting the
    // preimage of 0 first, then the preimage of 1 and so on.

    // We want to cycle through all possible first facet gluings, so we'll
    // special-case the situation in which there are no facet gluings at all.
    if (isUnmatched(0, 0)) {
        // We must have just one simplex with no facet gluings at all.
        Isomorphism* ans;
        for (int i = 0; i < Perm::nPerms; ++i) {
            ans = new Isomorphism(1);
            ans->simpImage(0) = 0;
            ans->facetPerm(0) = Perm::Sn[i];
            list.push_back(ans);
        }
        return true;
    }

    // Now we know that facet 0 of simplex 0 is glued to something.

    NFacetSpec<dim>* image = new NFacetSpec<dim>[size_ * (dim + 1)];
        /**< The automorphism currently under construction. */
    NFacetSpec<dim>* preImage = new NFacetSpec<dim>[size_ * (dim + 1)];
        /**< The inverse of this automorphism. */

    unsigned i, j;
    for (i = 0; i < size_ * (dim + 1); ++i) {
        image[i].setBeforeStart();
        preImage[i].setBeforeStart();
    }

    // Note that we know size_ >= 1.
    // For the preimage of facet 0 of simplex 0 we simply cycle
    // through all possibilities.
    const NFacetSpec<dim> firstFace(0, 0);
    const NFacetSpec<dim> firstFaceDest(dest(firstFace));
    NFacetSpec<dim> firstDestPre;
    NFacetSpec<dim> trying;
    NFacetSpec<dim> fImg, fPre;
    bool stepDown;
    int simp, facet;
    int permImg[dim + 1];
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
            for_each(list.begin(), list.end(),
                FuncDelete<Isomorphism>());
            list.clear();
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
            NFacetSpec<dim>& pre =
                preImage[trying.simp * (dim + 1) + trying.facet];

            if (trying.isPastEnd(size_, true)) {
                // We have a complete automorphism!
                Isomorphism* ans = new Isomorphism(size_);
                for (i = 0; i < size_; i++) {
                    ans->simpImage(i) = image[i * (dim + 1)].simp;
                    for (j = 0; j <= dim; ++j)
                        permImg[j] = image[i * (dim + 1) + j].facet;
                    ans->facetPerm(i) = Perm(permImg);
                }
                list.push_back(ans);
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
                            for_each(list.begin(), list.end(),
                                FuncDelete<Isomorphism>());
                            list.clear();
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
                        for_each(list.begin(), list.end(),
                            FuncDelete<Isomorphism>());
                        list.clear();
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

template <int dim>
typename NGenericFacetPairing<dim>::Isomorphism* NGenericFacetPairing<dim>::makeCanonical() {
    // Create the automorphisms one simplex at a time, selecting the
    // preimage of 0 first, then the preimage of 1 and so on.

    // We're going to skip over any completely isolated simplex, and put all
    // such simplices at the end of any canonical representation. Thus, we want
    // to find the first simplex that is connected to something.
#define toInt(fs) ((fs.simp) * (dim + 1) + (fs.facet))
    int numIsolated = 0;

    // First count how many isolated simplices we have.
    for ( int i = 0; i < size_; i++ ) {
        bool isolated = true;
        for (int j = 0; j < (dim + 1); j++) {
            if (! isUnmatched(i, j)) {
                isolated = false;
                break;
            }
        }
        if (isolated)
            numIsolated += 1;
    }
    // Now find a suitable place to start.
    int startSimplex = 0;
    bool isolated = true;
    while (isolated && startSimplex < size_) {
        for (int facet = 0 ; facet < (dim + 1); facet++) {
            if (! isUnmatched(startSimplex, facet)) {
                isolated = false;
                break;
            }
        }
        if ( isolated )
            startSimplex += 1;
    }

    if ( startSimplex == size_ ) {
        Isomorphism* ans;
        // Only isolated simplices, so already canonical.
        ans = new Isomorphism(size_);
        for ( int i = 0; i < size_ ; i++ ) {
            ans->simpImage(i) = i;
            ans->facetPerm(i) = Perm::Sn[0];
        }
        return ans;
    }

    // Now we know that at least one gluing exists.

    NFacetSpec<dim>* best = new NFacetSpec<dim>[size_ * (dim + 1)];
        /**< The best representation found so far. */
    NFacetSpec<dim>* image = new NFacetSpec<dim>[size_ * (dim + 1)];
        /**< The automorphism currently under construction. */
    NFacetSpec<dim>* preImage = new NFacetSpec<dim>[size_ * (dim + 1)];
        /**< The inverse of this automorphism. */

    for (unsigned i = 0; i < size_ ; ++i) {
        for (unsigned j = 0; j < (dim + 1); ++j) {
            image[i * (dim + 1) + j].setBeforeStart();
            preImage[i * (dim + 1) + j].setBeforeStart();
            NFacetSpec<dim> &b = dest(i,j);
            best[i * (dim + 1) + j].simp = b.simp;
            best[i * (dim + 1) + j].facet = b.facet;
        }
    }

    // Note that we know size_ >= 1.
    // For the preimage of facet 0 of simplex 0 we simply cycle
    // through all possibilities.
    NFacetSpec<dim> firstFace(0, 0);
    NFacetSpec<dim> firstFaceDest(dest(firstFace));
    NFacetSpec<dim> trying;
    NFacetSpec<dim> fImg, fPre;
    bool stepDown;
    int simp, facet;
    int permImg[dim + 1];

    // We need to track whether the automorphism we are working on is better
    // than anything we have found so far (in which case we no longer care
    // about comparisons but instead just want to fill out the automorphism).
    // isBetterAt is used to track this. -1 means "currently equal" whereas a
    // positive value means "up until this point we were equal but at this
    // point we were better".
    // That is, if isBetterAt is positive and we are trying to fill out the
    // automorphism after this point, stop doing comparisons.
    int isBetterAt = -1;

    Isomorphism* ans = NULL;
    for (preImage[0] = firstFace ; ! preImage[0].isPastEnd(size_, true);
            ++preImage[0]) {
        // Note that we know firstFace is not unmatched.
        if (isUnmatched(preImage[0]))
            continue;

        // We can use this facet.
        // Set the corresponding reverse mapping and off we go.
        image[toInt(preImage[0])] = firstFace;


        // We will step forwards to the next facet whose preimage is undetermined.
        trying = firstFace;
        ++trying;

        // But first set up mappings for the first destination.
        firstFaceDest = dest(preImage[0]);
        if (firstFaceDest.simp == preImage[0].simp) {
            // Different facet on same simplex
            image[toInt(firstFaceDest)].simp = 0;
            image[toInt(firstFaceDest)].facet = 1;
            preImage[1] = firstFaceDest;

            // If the new first facet is identified with the second facet, we
            // need to skip one further ahead before searching.
            ++trying;
        } else {
            image[toInt(firstFaceDest)].simp = 1;
            image[toInt(firstFaceDest)].facet = 0;
            preImage[dim + 1] = firstFaceDest;
        }
        if (image[toInt(firstFaceDest)] < best[0]) {
            isBetterAt = 0;
        } else if (image[toInt(firstFaceDest)] > best[0]) {
            image[toInt(preImage[0])].setBeforeStart();
            image[toInt(firstFaceDest)].setBeforeStart();
            // Technically only one of these two should be needed. I wonder which
            // is faster though, the if statement or the extra two variable
            // assignments.
            preImage[1].setBeforeStart();
            preImage[dim + 1].setBeforeStart();
            continue;
        }

        while (! (trying == firstFace)) {
            // INV: We've successfully selected preimages for all facets
            // before trying.  We're currently looking at the last
            // attempted candidate for the preimage of trying.

            // Note that if preimage facet A is glued to preimage facet B
            // and the image of A is earlier than the image of B, then
            // the image of A will be selected whereas the image of B
            // will be automatically derived.

            stepDown = false;
            NFacetSpec<dim>& pre =
                preImage[trying.simp * (dim + 1) + trying.facet];

            // Note that we ignore the last numIsolated simplices. We know that
            // the facet pairing contains at most one connected component which
            // does not represent an isolated simplex. The algorithm below
            // follows identifications in this connected component, which
            // ensures that this whole connected component will appear first in
            // any representation.
            if (trying.isPastEnd(size_ - numIsolated, true)) {
                // If isBetterAt == -1, then we have an isomorphism that is "as
                // good" as the best we've found. Since we iterate through
                // isomorphisms lexicographically, we just keep this first one.
                if ((ans == NULL) || (isBetterAt != -1)) {
                    // We almost have a complete automorphism!
                    if (ans != NULL)
                        delete ans;
                    ans = new Isomorphism(size_);
                    int k = 0; // How many isolated simps have we encountered.
                    for (unsigned i = 0; i < size_ ; i++) {
                        if (image[i * (dim + 1)].isBeforeStart()) {
                            // Isolated.
                            ans->simpImage(i) = size_ - numIsolated + k;
                            k+=1;
                            ans->facetPerm(i) = Perm::Sn[0];
                        } else {
                            ans->simpImage(i) = image[i * (dim + 1)].simp;
                            for (unsigned j = 0; j <= dim; ++j)
                                permImg[j] = image[i * (dim + 1) + j].facet;
                            ans->facetPerm(i) = Perm(permImg);
                        }
                        if (preImage[i * (dim + 1)].isBeforeStart()) {
                            // Isolated.
                            for (unsigned j = 0; j <= dim; j++)
                                best[i * (dim  + 1) + j].setBoundary(size_);
                        } else {
                            for (unsigned j = 0; j <= dim; ++j) {
                                NFacetSpec<dim> &other = dest(preImage[i * (dim + 1) + j]);
                                if (! other.isBoundary(size_))
                                    best[i * (dim + 1) + j] = image[toInt(other)];
                                else
                                    best[i * (dim + 1) + j].setBoundary(size_);
                            }
                        }
                    }
                    isBetterAt = -1;
                    // We've got a new "best", so when stepping through any
                    // other isomorphisms we have to assume they start of "as
                    // good". Note that any worse candidates will thus be
                    // examined and we will step down, while any better
                    // candidates will again mark isBetterAt.
                }
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
                    for ( ; pre.facet <= dim; ++pre.facet) {
                        if (! image[pre.simp * (dim + 1) + pre.facet].
                                isBeforeStart())
                            continue;
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
                        unsigned i;
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
                    // Do we need to check?
                    if ( isBetterAt == -1 ) {
                        // In the best known isomorphism, trying is glued to fImg.
                        fImg = best[trying.simp * (dim + 1) + trying.facet];
                        fPre = dest(preImage[trying.simp * (dim + 1) +
                            trying.facet]);
                        if (! fPre.isBoundary(size_))
                            fPre = image[fPre.simp * (dim + 1) + fPre.facet];

                        // After applying our isomorphism, trying will be
                        // glued to fPre.

                        if (fImg < fPre) {
                            // This isomorphism will lead to a
                            // lexicographically greater representation.
                            // Ignore it.
                            stepDown = true;
                        } else if (fImg > fPre) {
                            // This isomorphism will lead to a
                            // lexicographically lesser representation.
                            // Always use it.
                            isBetterAt = trying.simp * (dim + 1) +
                                trying.facet;
                        } // else leave isBetterAt = -1 and check at next step.
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
                if ( isBetterAt >= trying.simp * (dim + 1) + trying.facet )
                    isBetterAt = -1;

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
#undef toInt

    // Apply the isomorphism we found.

    // First "remove" extra isolated tetrahedra
    size_ -= numIsolated;
    // Recreate storage room for pairs_.
    delete[] pairs_;
    pairs_ = new NFacetSpec<dim>[size_ * (dim + 1)];
    // Then use our "best" representation.
    for ( unsigned i = 0; i < size_ * (dim + 1); ++i ) {
        pairs_[i].simp = best[i].simp;
        if (pairs_[i].simp == (size_ + numIsolated)) // Boundary changed
            pairs_[i].simp = size_;
        pairs_[i].facet = best[i].facet;
    }
    // Note that in the above, size_ has been changed since allocating best, so
    // just using best results in slight memory loss.

    // Memory cleanup
    delete[] image;
    delete[] preImage;
    delete[] best;
    return ans;
}


template <int dim>
bool NGenericFacetPairing<dim>::findAllPairings(unsigned nSimplices,
        NBoolSet boundary, int nBdryFacets,
        typename NGenericFacetPairing<dim>::Use use,
        void* useArgs, bool newThread) {
    // Create a set of arguments.
    Args* args = new Args;
    args->boundary = boundary;
    args->nBdryFacets = nBdryFacets;
    args->use = use;
    args->useArgs = useArgs;

    // Start the facet pairing generation.
    FacetPairing* pairing = new FacetPairing(nSimplices);
    if (newThread)
        return pairing->start(args, true);
    else {
        pairing->run(args);
        delete pairing;
        return true;
    }
}

void NGenericFacetPairing<dim>::enumerateInternal(NBoolSet boundary,
        int nBdryFacets, Use use, void* useArgs) {
    // Bail if it's obvious that nothing will happen.
    if (boundary == NBoolSet::sNone || size_ == 0) {
        use(0, 0, useArgs);
        return;
    }
    if (boundary.hasTrue() && nBdryFacets >= 0 &&
            (nBdryFacets % 2 != ((dim+1) * static_cast<int>(size_)) % 2 ||
            nBdryFacets > (dim - 1) * static_cast<int>(size_) + 2
            || (nBdryFacets == 0 && ! boundary.hasFalse()))) {
        use(0, 0, useArgs);
        return;
    }

    // Initialise the pairings to unspecified (i.e., facet -> itself).
    for (NFacetSpec<dim> f(0,0); f.simp < static_cast<int>(size_); ++f)
        dest(f) = f;

    // Note that we have at least one simplex.
    NFacetSpec<dim> trying(0, 0);
        /**< The facet we're currently trying to match. */
    int boundaryFacets = 0;
        /**< How many (deliberately) unmatched facets do we currently have? */
    int usedFacets = 0;
        /**< How many facets have we already determined matchings for? */
    IsoList allAutomorphisms;
        /**< The set of all automorphisms of the current facet pairing. */

    // Run through and find all possible matchings.
    NFacetSpec<dim> oldTrying, tmpFacet;
    while (true) {
        // TODO: Check for cancellation,

        // INVARIANT: Facet trying needs to be joined to something.
        // dest(trying) represents the last tried destination for the
        // join, and there is no reciprocal join from dest(trying) back
        // to trying.
        // The current value of dest(trying) is >= trying.

        // Move to the next destination.
        ++dest(trying);

        // If we're about to close off the current set of simplices
        // and it's not all the simplices, we will have something
        // disconnected!
        // We will now avoid tying the last two facets in a set together,
        // and later we will avoid sending the last facet of a set to the
        // boundary.
        if (usedFacets % (dim + 1) == (dim - 1) &&
                usedFacets < (dim + 1) * static_cast<int>(size_) - 2 &&
                noDest((usedFacets / (dim + 1)) + 1, 0) &&
                dest(trying).simp <= (usedFacets / (dim + 1))) {
            // Move to the first unused simplex.
            dest(trying).simp = (usedFacets / (dim + 1)) + 1;
            dest(trying).facet = 0;
        }

        // We'd better make sure we're not going to glue together so
        // many facets that there is no room for the required number of
        // boundary facets.
        if (boundary.hasTrue()) {
            // We're interested in triangulations with boundary.
            if (nBdryFacets < 0) {
                // We don't care how many boundary facets.
                if (! boundary.hasFalse()) {
                    // We must have some boundary though.
                    if (boundaryFacets == 0 &&
                            usedFacets ==
                                (dim + 1) * static_cast<int>(size_) - 2 &&
                            dest(trying).simp <
                                static_cast<int>(size_))
                        dest(trying).setBoundary(size_);
                }
            } else {
                // We're specific about the number of boundary facets.
                if (usedFacets - boundaryFacets + nBdryFacets ==
                        (dim + 1) * static_cast<int>(size_) &&
                        dest(trying).simp < static_cast<int>(size_))
                    // We've used our entire quota of non-boundary facets.
                    dest(trying).setBoundary(size_);
            }
        }

        // dest(trying) is now the first remaining candidate destination.
        // We still don't know whether this destination is valid however.
        while(true) {
            // Move onwards to the next free destination.
            while (dest(trying).simp < static_cast<int>(size_) &&
                    ! noDest(dest(trying)))
                ++dest(trying);

            // If we are past facet 0 of a simplex and the previous facet
            // was not used, we can't do anything with this simplex.
            // Move to the next simplex.
            if (dest(trying).simp < static_cast<int>(size_) &&
                    dest(trying).facet > 0 &&
                    noDest(dest(trying).simp, dest(trying).facet - 1)) {
                ++dest(trying).simp;
                dest(trying).facet = 0;
                continue;
            }

            break;
        }

        // If we're still at an illegitimate destination, it must be
        // facet 0 of a simplex where the previous simplex is
        // unused.  Note that facet == 0 implies simp > 0.
        // In this case, we've passed the last sane choice; head
        // straight to the boundary.
        if (dest(trying).simp < static_cast<int>(size_) &&
                dest(trying).facet == 0 &&
                noDest(dest(trying).simp - 1, 0))
            dest(trying).setBoundary(size_);

        // Finally, return to the issue of prematurely closing off a
        // set of simplices.  This time we will avoid sending the last
        // facet of a set of simplices to the boundary.
        if (usedFacets % (dim + 1) == dim &&
                usedFacets < (dim + 1) * static_cast<int>(size_) - 1 &&
                noDest((usedFacets / (dim + 1)) + 1, 0) &&
                isUnmatched(trying)) {
            // Can't use the boundary; all we can do is push past the
            // end.
            ++dest(trying);
        }

        // And so we're finally looking at the next real candidate for
        // dest(trying) that we know we're actually allowed to use.

        // Check if after all that we've been pushed past the end.
        if (dest(trying).isPastEnd(size_, (! boundary.hasTrue()) ||
                boundaryFacets == nBdryFacets)) {
            // We can't join trying to anything else.  Step back.
            dest(trying) = trying;
            --trying;

            // Keep heading back until we find a facet that joins
            // forwards or to the boundary.
            while (! trying.isBeforeStart()) {
                if (dest(trying) < trying)
                    --trying;
                else
                    break;
            }

            // Is the search over?
            if (trying.isBeforeStart())
                break;

            // Otherwise undo the previous gluing and prepare to loop
            // again trying the next option.
            if (isUnmatched(trying)) {
                --usedFacets;
                --boundaryFacets;
            } else {
                usedFacets -= 2;
                dest(dest(trying)) = dest(trying);
            }

            continue;
        }

        // Let's match it up and head to the next free facet!
        if (isUnmatched(trying)) {
            ++usedFacets;
            ++boundaryFacets;
        } else {
            usedFacets += 2;
            dest(dest(trying)) = trying;
        }

        // Now we increment trying to move to the next unmatched facet.
        oldTrying = trying;
        ++trying;
        while (trying.simp < static_cast<int>(size_) && ! noDest(trying))
            ++trying;

        // Have we got a solution?
        if (trying.simp == static_cast<int>(size_)) {
            // Deal with the solution!
            if (isCanonicalInternal(allAutomorphisms)) {
                use(static_cast<FacetPairing*>(this),
                    &allAutomorphisms, useArgs);
                for_each(allAutomorphisms.begin(), allAutomorphisms.end(),
                    FuncDelete<Isomorphism>());
                allAutomorphisms.clear();
            }

            // Head back down to the previous gluing and undo it, ready
            // for the next loop.
            trying = oldTrying;
            if (isUnmatched(trying)) {
                --usedFacets;
                --boundaryFacets;
            } else {
                usedFacets -= 2;
                dest(dest(trying)) = dest(trying);
            }
        } else {
            // We're about to start working on a new unmatched facet.
            // Set dest(trying) to one step *before* the first feasible
            // destination.

            // Note that currently the destination is set to trying.

            // Ensure the destination is at least the
            // previous forward destination from an earlier facet of this
            // simplex.
            if (trying.facet > 0) {
                tmpFacet = trying;
                for (--tmpFacet; tmpFacet.simp == trying.simp; --tmpFacet)
                    if (tmpFacet < dest(tmpFacet)) {
                        // Here is the previous forward destination in
                        // this simplex.
                        if (dest(trying) < dest(tmpFacet)) {
                            dest(trying) = dest(tmpFacet);

                            // Remember that dest(trying) will be
                            // incremented before it is used.  This
                            // should not happen if we're already on the
                            // boundary, so we need to move back one
                            // step so we will be pushed back onto the
                            // boundary.
                            if (isUnmatched(trying))
                                --dest(trying);
                        }
                        break;
                    }
            }

            // If the first simplex doesn't glue to itself and this
            // is not the first simplex, it can't glue to itself either.
            // (Note that we already know there is at least 1 simplex.)
            if (dest(trying).simp == trying.simp && dest(trying).facet < dim &&
                    trying.simp > 0)
                if (dest(0, 0).simp != 0)
                    dest(trying).facet = dim;
        }
    }

    use(0, 0, useArgs);
    return;
}

} // namespace regina

