
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* Template definitions for ngeneralfacetpairing.h. */

#include <algorithm>
#include <sstream>
#include <vector>
#include "census/ngeneralfacetpairing.h"
#include "utilities/memutils.h"
#include "utilities/stringutils.h"

namespace regina {

template <int dim>
NGeneralFacetPairing<dim>::NGeneralFacetPairing(
        const NGeneralFacetPairing<dim>& cloneMe) :
        /* TODO NThread(), */
        size_(cloneMe.size_),
        pairs_(new NFacetSpec<dim>[cloneMe.size_ * (dim + 1)]) {
    std::copy(cloneMe.pairs_, cloneMe.pairs_ + (size_ * (dim + 1)), pairs_);
}

template <int dim>
bool NGeneralFacetPairing<dim>::isClosed() const {
    for (NFacetSpec<dim> f(0, 0); ! f.isPastEnd(size_, true); ++f)
        if (isUnmatched(f))
            return false;
    return true;
}

template <int dim>
std::string NGeneralFacetPairing<dim>::toString() const {
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
std::string NGeneralFacetPairing<dim>::dotHeader(const char* graphName) {
    std::ostringstream ans;
    writeDotHeader(ans, graphName);
    return ans.str();
}

template <int dim>
void NGeneralFacetPairing<dim>::writeDotHeader(
        std::ostream& out, const char* graphName) {
    static const char defaultGraphName[] = "G";

    if ((! graphName) || (! *graphName))
        graphName = defaultGraphName;

    out << "graph " << graphName << " {" << std::endl;
    out << "graph [bgcolor=white];" << std::endl;
    out << "edge [color=black];" << std::endl;
    out << "node [shape=circle,style=filled,height=0.15,fixedsize=true,label=\"\",fontsize=9,fontcolor=\"#751010\"];" << std::endl;
}

template <int dim>
std::string NGeneralFacetPairing<dim>::dot(
        const char* prefix, bool subgraph, bool labels) const {
    std::ostringstream ans;
    writeDot(ans, prefix, subgraph, labels);
    return ans.str();
}

template <int dim>
void NGeneralFacetPairing<dim>::writeDot(std::ostream& out,
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
std::string NGeneralFacetPairing<dim>::toTextRep() const {
    std::ostringstream ans;

    for (NFacetSpec<dim> f(0, 0); ! f.isPastEnd(size_, true); ++f) {
        if (f.simp || f.facet)
            ans << ' ';
        ans << dest(f).simp << ' ' << dest(f).facet;
    }

    return ans.str();
}

/*
template <int dim>
NGeneralFacetPairing* NGeneralFacetPairing::fromTextRep(const std::string& rep) {
    std::vector<std::string> tokens;
    unsigned nTokens = basicTokenise(back_inserter(tokens), rep);

    if (nTokens == 0 || nTokens % 10 != 0)
        return 0;

    long nPent = nTokens / 10;
    NGeneralFacetPairing* ans = new NGeneralFacetPairing(nPent);

    // Read the raw values.
    // Check the range of each value while we're at it.
    long val;
    for (long i = 0; i < nPent * (dim + 1); ++i) {
        if (! valueOf(tokens[2 * i], val)) {
            delete ans;
            return 0;
        }
        if (val < 0 || val > nPent) {
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
    for (NFacetSpec<dim> f(0, 0); ! f.isPastEnd(nPent, true); ++f) {
        destFacet = ans->dest(f);
        if (destFacet.simp == nPent && destFacet.facet != 0)
            broken = true;
        else if (destFacet.simp < nPent && ! (ans->dest(destFacet) == f))
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
*/

template <int dim>
bool NGeneralFacetPairing<dim>::isCanonical() const {
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
bool NGeneralFacetPairing<dim>::isCanonicalInternal(
        typename NGeneralFacetPairing<dim>::IsoList& list) const {
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

} // namespace regina

