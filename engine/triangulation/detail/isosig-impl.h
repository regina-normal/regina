
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

/*! \file triangulation/detail/isosig-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Triangulation class template.
 *
 *  This file is _not_ included from triangulation.h, but the routines
 *  it contains are explicitly instantiated in Regina's calculation engine.
 *  Therefore end users should never need to include this header.
 */

#ifndef __REGINA_ISOSIG_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_ISOSIG_IMPL_H_DETAIL
#endif

#include <algorithm>
#include "triangulation/generic/triangulation.h"
#include "triangulation/isosigtype.h"
#include "utilities/fixedarray.h"
#include "utilities/sigutils.h"

namespace regina::detail {

template <int dim>
template <class Encoding>
typename Encoding::Signature TriangulationBase<dim>::isoSigFrom(
        size_t simp, const Perm<dim+1>& vertices,
        Isomorphism<dim>* relabelling) const {
    // Only process the component that simp belongs to.

    // ---------------------------------------------------------------------
    // Data for reconstructing a triangulation from an isomorphism signature
    // ---------------------------------------------------------------------

    // The number of simplices.
    size_t nSimp = size();

    // What happens to each new facet that we encounter?
    // Options are:
    //   0 -> boundary
    //   1 -> joined to a simplex not yet seen [gluing perm = identity]
    //   2 -> joined to a simplex already seen
    // These actions are stored in lexicographical order by (simplex, facet),
    // but only once for each facet (so we "skip" gluings that we've
    // already seen from the other direction).
    size_t nFacets = ((dim + 1) * size() + countBoundaryFacets()) / 2;
    FixedArray<uint8_t> facetAction(nFacets);

    // What are the destination simplices and gluing permutations for
    // each facet under case #2 above?
    // For gluing permutations, we store the index of the permutation in
    // Perm<dim+1>::orderedSn.
    FixedArray<size_t> joinDest(nFacets);
    FixedArray<typename Perm<dim+1>::Index> joinGluing(nFacets);

    // ---------------------------------------------------------------------
    // Data for finding the unique canonical isomorphism from this
    // connected component that maps (simplex, vertices) -> (0, 0..dim)
    // ---------------------------------------------------------------------

    // The image for each simplex and its vertices:
    FixedArray<ssize_t> image(nSimp);
    FixedArray<Perm<dim+1>> vertexMap(nSimp);

    // The preimage for each simplex:
    FixedArray<ssize_t> preImage(nSimp);

    // ---------------------------------------------------------------------
    // Looping variables
    // ---------------------------------------------------------------------
    size_t facetPos, joinPos, nextUnusedSimp;
    size_t simpImg, simpSrc, dest;
    unsigned facetImg, facetSrc;
    const Simplex<dim>* s;

    // ---------------------------------------------------------------------
    // The code!
    // ---------------------------------------------------------------------

    std::fill(image.begin(), image.end(), -1);
    std::fill(preImage.begin(), preImage.end(), -1);

    image[simp] = 0;
    vertexMap[simp] = vertices.inverse();
    preImage[0] = simp;

    facetPos = 0;
    joinPos = 0;
    nextUnusedSimp = 1;

    // To obtain a canonical isomorphism, we must run through the simplices
    // and their facets in image order, not preimage order.
    //
    // This main loop is guaranteed to exit when (and only when) we have
    // exhausted a single connected component of the triangulation.
    for (simpImg = 0; simpImg < nSimp && preImage[simpImg] >= 0; ++simpImg) {
        simpSrc = preImage[simpImg];
        s = simplex(simpSrc);

        for (facetImg = 0; facetImg <= dim; ++facetImg) {
            facetSrc = vertexMap[simpSrc].pre(facetImg);

            // INVARIANTS (held while we stay within a single component):
            // - nextUnusedSimp > simpImg
            // - image[simpSrc], preImage[image[simpSrc]] and vertexMap[simpSrc]
            //   are already filled in.

            // Work out what happens to our source facet.
            if (! s->adjacentSimplex(facetSrc)) {
                // A boundary facet.
                facetAction[facetPos++] = 0;
                continue;
            }

            // We have a real gluing.  Is it a gluing we've already seen
            // from the other side?
            dest = s->adjacentSimplex(facetSrc)->index();

            if (image[dest] >= 0)
                if (image[dest] < image[simpSrc] ||
                        (dest == simpSrc &&
                         vertexMap[simpSrc][s->adjacentFacet(facetSrc)]
                         < vertexMap[simpSrc][facetSrc])) {
                    // Yes.  Just skip this gluing entirely.
                    continue;
                }

            // Is it a completely new simplex?
            if (image[dest] < 0) {
                // Yes.  The new simplex takes the next available
                // index, and the canonical gluing becomes the identity.
                image[dest] = nextUnusedSimp++;
                preImage[image[dest]] = dest;
                vertexMap[dest] = vertexMap[simpSrc] *
                    s->adjacentGluing(facetSrc).inverse();

                facetAction[facetPos++] = 1;
                continue;
            }

            // It's a simplex we've seen before.  Record the gluing.
            joinDest[joinPos] = image[dest];
            joinGluing[joinPos] = (vertexMap[dest] *
                s->adjacentGluing(facetSrc) * vertexMap[simpSrc].inverse()).
                orderedSnIndex();
            ++joinPos;

            facetAction[facetPos++] = 2;
        }
    }

    // We have all we need.  Pack it all together into a string.
    //
    // Note: Encoding::encode takes C-style arrays, whereas we have FixedArrays.
    // For now we use the fact that FixedArray iterators are just pointers
    // into C-style arrays; however, in the long term we should change the
    // Encoding interface to be templated and take iterators instead.
    typename Encoding::Signature ans = Encoding::encode(simpImg,
        facetPos, facetAction.begin(),
        joinPos, joinDest.begin(), joinGluing.begin());

    // Record the canonical isomorphism if required.
    if (relabelling)
        for (size_t i = 0; i < simpImg; ++i) {
            relabelling->simpImage(i) = image[i];
            relabelling->facetPerm(i) = vertexMap[i];
        }

    // Done!
    return ans;
}

template <int dim>
template <class Type, class Encoding>
typename Encoding::Signature TriangulationBase<dim>::isoSig() const {
    if (isEmpty())
        return Encoding::emptySig();

    // The triangulation is non-empty.  Get a signature string for each
    // connected component.
    size_t i;
    typename Encoding::Signature curr;

    FixedArray<typename Encoding::Signature> comp(countComponents());
    auto it = components().begin();
    for (i = 0; it != components().end(); ++it, ++i) {
        Type type(**it);
        bool first = true;
        do {
            curr = isoSigFrom<Encoding>((*it)->simplex(type.simplex())->index(),
                type.perm(), nullptr);
            if (first || curr < comp[i]) {
                comp[i].swap(curr);
                first = false;
            }
        } while (type.next());
    }

    // Pack the components together.
    std::sort(comp.begin(), comp.end());

    typename Encoding::Signature ans;
    for (i = 0; i < countComponents(); ++i)
        ans += comp[i];

    return ans;
}

template <int dim>
template <class Type, class Encoding>
std::pair<typename Encoding::Signature, Isomorphism<dim>>
        TriangulationBase<dim>::isoSigDetail() const {
    // Make sure the user is not trying to do something illegal.
    if (isEmpty())
        throw FailedPrecondition(
            "isoSigDetail() requires a non-empty triangulation");
    if (countComponents() > 1)
        throw FailedPrecondition(
            "isoSigDetail() requires a connected triangulation");

    std::pair<typename Encoding::Signature, Isomorphism<dim>> ans(
        std::piecewise_construct, std::forward_as_tuple(),
        std::forward_as_tuple(size()));
    Isomorphism<dim> currRelabelling(size());

    // The triangulation is non-empty.  Get a signature string for each
    // connected component.
    size_t i;
    typename Encoding::Signature curr;

    FixedArray<typename Encoding::Signature> comp(countComponents());
    auto it = components().begin();
    for (i = 0; it != components().end(); ++it, ++i) {
        Type type(**it);
        bool first = true;
        do {
            curr = isoSigFrom<Encoding>((*it)->simplex(type.simplex())->index(),
                type.perm(), &currRelabelling);
            if (first || curr < comp[i]) {
                comp[i].swap(curr);
                ans.second.swap(currRelabelling);
                first = false;
            }
        } while (type.next());
    }

    // Pack the components together.
    std::sort(comp.begin(), comp.end());

    for (i = 0; i < countComponents(); ++i)
        ans.first += comp[i];

    return ans;
}

template <int dim>
Triangulation<dim> TriangulationBase<dim>::fromIsoSig(const std::string& sig) {
    Triangulation<dim> ans;

    const char* c = sig.c_str();

    // Skip any leading whitespace.
    while (*c && ::isspace(*c))
        ++c;

    // Find the end of the string.
    const char* end = c;
    while (*end && ! ::isspace(*end))
        ++end;

    // Initial check for invalid characters.
    const char* d;
    for (d = c; d != end; ++d)
        if (! Base64SigEncoding::isValid(*d))
            throw InvalidArgument("fromIsoSig(): invalid base64 character");
    for (d = end; *d; ++d)
        if (! ::isspace(*d))
            throw InvalidArgument(
                "fromIsoSig(): unexpected internal whitespace");

    while (c != end) {
        // Read one component at a time.
        size_t nSimp = Base64SigEncoding::decodeSingle(*c++);
        int nChars;
        if (nSimp < 63)
            nChars = 1;
        else {
            if (c == end)
                throw InvalidArgument(
                    "fromIsoSig(): incomplete signature");
            nChars = Base64SigEncoding::decodeSingle(*c++);
            if (c + nChars > end)
                throw InvalidArgument(
                    "fromIsoSig(): incomplete signature");
            nSimp = Base64SigEncoding::decodeInt<size_t>(c, nChars);
            c += nChars;
        }

        if (nSimp == 0) {
            // Empty component.
            continue;
        }

        // Non-empty component; keep going.
        FixedArray<uint8_t> facetAction((dim+1) * nSimp + 2);
        size_t nFacets = 0;
        auto facetPos = facetAction.begin();
        size_t nJoins = 0;

        for ( ; nFacets < (dim+1) * nSimp; facetPos += 3) {
            if (c == end) {
                throw InvalidArgument("fromIsoSig(): incomplete signature");
            }
            Base64SigEncoding::decodeTrits(*c++, facetPos);
            for (int j = 0; j < 3; ++j) {
                // If we're already finished, make sure the leftover trits
                // are zero.
                if (nFacets == (dim+1) * nSimp) {
                    if (*(facetPos + j) != 0) {
                        throw InvalidArgument(
                            "fromIsoSig(): extraneous facet actions");
                    }
                    continue;
                }

                switch (*(facetPos + j)) {
                    case 0:
                        ++nFacets;
                        break;
                    case 1:
                        nFacets += 2;
                        break;
                    case 2:
                        nFacets += 2;
                        ++nJoins;
                        break;
                    default:
                        throw InvalidArgument(
                            "fromIsoSig(): invalid facet action");
                }
                if (nFacets > (dim+1) * nSimp) {
                    throw InvalidArgument("fromIsoSig(): facet actions "
                        "do not match triangulation size");
                }
            }
        }

        FixedArray<size_t> joinDest(nJoins + 1);
        for (size_t pos = 0; pos < nJoins; ++pos) {
            if (c + nChars > end) {
                throw InvalidArgument("fromIsoSig(): incomplete signature");
            }

            joinDest[pos] = Base64SigEncoding::decodeInt<size_t>(c, nChars);
            c += nChars;
        }

        FixedArray<typename Perm<dim+1>::Index> joinGluing(nJoins + 1);
        for (size_t pos = 0; pos < nJoins; ++pos) {
            if (c + IsoSigPrintable<dim>::charsPerPerm > end) {
                throw InvalidArgument("fromIsoSig(): incomplete signature");
            }

            joinGluing[pos] =
                Base64SigEncoding::decodeInt<typename Perm<dim+1>::Index>(c,
                    IsoSigPrintable<dim>::charsPerPerm);
            c += IsoSigPrintable<dim>::charsPerPerm;

            if (joinGluing[pos] >= Perm<dim+1>::nPerms ||
                    joinGluing[pos] < 0) {
                throw InvalidArgument(
                    "fromIsoSig(): invalid gluing permutation");
            }
        }

        // End of component!
        FixedArray<Simplex<dim>*> simp(nSimp);
        for (size_t pos = 0; pos < nSimp; ++pos)
            simp[pos] = ans.newSimplex();

        facetPos = facetAction.begin();
        size_t nextUnused = 1;
        size_t joinPos = 0;
        Perm<dim+1> gluing;
        for (size_t pos = 0; pos < nSimp; ++pos)
            for (int j = 0; j <= dim; ++j) {
                // Already glued from the other side:
                if (simp[pos]->adjacentSimplex(j))
                    continue;

                if (*facetPos == 0) {
                    // Boundary facet.
                } else if (*facetPos == 1) {
                    // Join to new simplex.
                    if (nextUnused >= nSimp) {
                        throw InvalidArgument(
                            "fromIsoSig(): gluing to non-existent simplex");
                    }
                    simp[pos]->join(j, simp[nextUnused++], Perm<dim+1>());
                } else {
                    // Join to existing simplex.
                    gluing = Perm<dim+1>::orderedSn[joinGluing[joinPos]];
                    if (joinDest[joinPos] >= nextUnused ||
                            simp[joinDest[joinPos]]->adjacentSimplex(
                            gluing[j])) {
                        throw InvalidArgument(
                            "fromIsoSig(): invalid gluing destination");
                    }
                    simp[pos]->join(j, simp[joinDest[joinPos]], gluing);
                    ++joinPos;
                }

                ++facetPos;
            }
    }

    return ans;
}

template <int dim>
size_t TriangulationBase<dim>::isoSigComponentSize(const std::string& sig) {
    const char* c = sig.c_str();

    // Examine the first character.
    // Note that isValid also ensures that *c is non-null (i.e., it
    // detects premature end of string).
    if (! Base64SigEncoding::isValid(*c))
        return 0;
    size_t nSimp = Base64SigEncoding::decodeSingle(*c);
    if (nSimp < 63)
        return nSimp;

    // The number of simplices is so large that it requires several
    // characters to store.
    ++c;
    if (! *c)
        return 0;
    int nChars = Base64SigEncoding::decodeSingle(*c++);

    for (const char* d = c; d < c + nChars; ++d)
        if (! Base64SigEncoding::isValid(*d))
            return 0;
    return Base64SigEncoding::decodeInt<size_t>(c, nChars);
}

} // namespace regina::detail

#endif
