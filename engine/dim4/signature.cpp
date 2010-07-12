
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include <algorithm>
#include "dim4/dim4triangulation.h"

namespace regina {

namespace {
    /**
     * Determine the integer value represented by the given character in
     * a signature string.
     */
    inline unsigned SVAL(char c) {
        if (c >= 'a' && c <= 'z')
            return (c - 'a');
        if (c >= 'A' && c <= 'Z')
            return (c - 'A' + 26);
        if (c >= '0' && c <= '9')
            return (c - '0' + 52);
        if (c == '+')
            return 62;
        return 63;
    }

    /**
     * Determine the character that represents the given integer value
     * in a signature string.
     */
    inline char SCHAR(unsigned c) {
        if (c < 26)
            return (char(c) + 'a');
        if (c < 52)
            return (char(c - 26) + 'A');
        if (c < 62)
            return (char(c - 52) + '0');
        if (c == 62)
            return '+';
        return '-';
    }

    /**
     * Is the given character a valid character in a signature string?
     */
    inline bool SVALID(char c) {
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '+' || c == '-');
    }

    /**
     * Does the given string contain at least nChars characters?
     */
    inline bool SHASCHARS(const char* s, unsigned nChars) {
        for ( ; nChars > 0; --nChars)
            if (! *s)
                return false;
        return true;
    }

    /**
     * Append an encoding of the given integer to the given string.
     * The integer is broken into nChars distinct 6-bit blocks, and the
     * lowest-significance blocks are written first.
     */
    void SAPPEND(std::string& s, unsigned val, unsigned nChars) {
        for ( ; nChars > 0; --nChars) {
            s += SCHAR(val & 0x3F);
            val >>= 6;
        }
    }

    /**
     * Read the integer at the beginning of the given string.
     * Assumes the string has length >= nChars.
     */
    unsigned SREAD(const char* s, unsigned nChars) {
        unsigned ans = 0;
        for (unsigned i = 0; i < nChars; ++i)
            ans += (SVAL(s[i]) << (6 * i));
        return ans;
    }

    /**
     * Append up to three trits (0, 1 or 2) to the given string.
     * These are packed into a single character, with the first trit
     * representing the lowest-significance bits and so on.
     */
    void SAPPENDTRITS(std::string& s, const char* trits, unsigned nTrits) {
        char ans = 0;
        if (nTrits >= 1)
            ans |= trits[0];
        if (nTrits >= 2)
            ans |= (trits[1] << 2);
        if (nTrits >= 3)
            ans |= (trits[2] << 4);
        s += SCHAR(ans);
    }

    /**
     * Reads three trits (0, 1 or 2) from the given character.
     */
    void SREADTRITS(char c, char* result) {
        unsigned val = SVAL(c);
        result[0] = val & 3;
        result[1] = (val >> 2) & 3;
        result[2] = (val >> 4) & 3;
    }
}

std::string Dim4Triangulation::isoSig() const {
    if (pentachora_.empty()) {
        char c[2];
        c[0] = SCHAR(0);
        c[1] = 0;
        return c;
    }

    // The triangulation is non-empty.  Get a signature string for each
    // connected component.
    unsigned i, j;
    ComponentIterator it;
    unsigned cPent;
    unsigned pent, perm;
    std::string curr;

    std::string* comp = new std::string[getNumberOfComponents()];
    for (it = components_.begin(), i = 0; it != components_.end(); ++it, ++i) {
        cPent = (*it)->getNumberOfPentachora();

        for (pent = 0; pent < (*it)->getNumberOfPentachora(); ++pent)
            for (perm = 0; perm < 120; ++perm) {
                curr = isoSig((*it)->getPentachoron(pent)->markedIndex(),
                    NPerm5::orderedS5[perm]);
                if ((pent == 0 && perm == 0) || (curr < comp[i]))
                    comp[i].swap(curr);
            }
    }

    // Pack the components together.
    std::sort(comp, comp + getNumberOfComponents());

    std::string ans;
    for (i = 0; i < getNumberOfComponents(); ++i)
        ans += comp[i];

    delete[] comp;
    return ans;
}

std::string Dim4Triangulation::isoSig(unsigned pent, const NPerm5& vertices)
        const {
    // Only process the component that pent belongs to.

    // ---------------------------------------------------------------------
    // Data for reconstructing a triangulation from an isomorphism signature
    // ---------------------------------------------------------------------

    // The number of pentachora.
    unsigned nPents = pentachora_.size();

    // What happens to each new facet that we encounter?
    // Options are:
    //   0 -> boundary
    //   1 -> joined to a pentachoron not yet seen [gluing perm = identity]
    //   2 -> joined to a pentachoron already seen
    // These actions are stored in lexicographical order by (pent, facet),
    // but only once for each facet (so we "skip" gluings that we've
    // already seen from the other direction).
    char* facetAction = new char[getNumberOfTetrahedra()];

    // What are the destination pentachora and gluing permutations for
    // each facet under case #2 above?
    // For gluing permutations, we store the index of the permutation in
    // NPerm5::orderedS5.
    unsigned* joinDest = new unsigned[getNumberOfTetrahedra()];
    unsigned* joinGluing = new unsigned[getNumberOfTetrahedra()];

    // ---------------------------------------------------------------------
    // Data for finding the unique canonical isomorphism from this
    // connected component that maps (pent, vertices) -> (0, 01234)
    // ---------------------------------------------------------------------

    // The image for each pentachoron and its vertices:
    int* image = new int[nPents];
    NPerm5* vertexMap = new NPerm5[nPents];

    // The preimage for each pentachoron:
    int* preImage = new int[nPents];

    // ---------------------------------------------------------------------
    // Looping variables
    // ---------------------------------------------------------------------
    unsigned facetPos, joinPos, nextUnusedPent;
    unsigned pentImg, facetImg;
    unsigned pentSrc, facetSrc, dest;
    Dim4Pentachoron* p;

    // ---------------------------------------------------------------------
    // The code!
    // ---------------------------------------------------------------------

    std::fill(image, image + nPents, -1);
    std::fill(preImage, preImage + nPents, -1);

    image[pent] = 0;
    vertexMap[pent] = vertices.inverse();
    preImage[0] = pent;

    facetPos = 0;
    joinPos = 0;
    nextUnusedPent = 1;

    // To obtain a canonical isomorphism, we must run through the pentachora
    // and their facets in image order, not preimage order.
    //
    // This main loop is guaranteed to exit when (and only when) we have
    // exhausted a single connected component of the triangulation.
    for (pentImg = 0; pentImg < nPents && preImage[pentImg] >= 0; ++pentImg) {
        pentSrc = preImage[pentImg];
        p = pentachora_[pentSrc];

        for (facetImg = 0; facetImg < 5; ++facetImg) {
            facetSrc = vertexMap[pentSrc].preImageOf(facetImg);

            // INVARIANTS (held while we stay within a single component):
            // - nextUnusedPent > pentImg
            // - image[pentSrc], preImage[image[pentSrc]] and vertexMap[pentSrc]
            //   are already filled in.

            // Work out what happens to our source facet.
            if (! p->adjacentPentachoron(facetSrc)) {
                // A boundary facet.
                facetAction[facetPos++] = 0;
                continue;
            }

            // We have a real gluing.  Is it a gluing we've already seen
            // from the other side?
            dest = pentachoronIndex(p->adjacentPentachoron(facetSrc));

            if (image[dest] >= 0)
                if (image[dest] < image[pentSrc] ||
                        (dest == pentSrc &&
                         vertexMap[pentSrc][p->adjacentFacet(facetSrc)]
                         < vertexMap[pentSrc][facetSrc])) {
                    // Yes.  Just skip this gluing entirely.
                    continue;
                }

            // Is it a completely new pentachoron?
            if (image[dest] < 0) {
                // Yes.  The new pentachoron takes the next available
                // index, and the canonical gluing becomes the identity.
                image[dest] = nextUnusedPent++;
                preImage[image[dest]] = dest;
                vertexMap[dest] = vertexMap[pentSrc] *
                    p->adjacentGluing(facetSrc).inverse();

                facetAction[facetPos++] = 1;
                continue;
            }

            // It's a pentachoron we've seen before.  Record the gluing.
            joinDest[joinPos] = image[dest];
            joinGluing[joinPos] = (vertexMap[dest] *
                p->adjacentGluing(facetSrc) * vertexMap[pentSrc].inverse()).
                orderedS5Index();
            ++joinPos;

            facetAction[facetPos++] = 2;
        }
    }

    // We have all we need.  Pack it all together into a string.
    // We need to encode:
    // - the number of pentachora in this component;
    // - facetAction[i], 0 <= i < facetPos;
    // - joinDest[i], 0 <= i < joinPos;
    // - joinGluing[i], 0 <= i < joinPos.
    std::string ans;

    // Keep it simple for small triangulations (1 character per integer).
    // For large triangulations, start with a special marker followed by
    // the number of chars per integer.
    unsigned nCompPent = pentImg;
    unsigned nChars;
    if (nCompPent < 63)
        nChars = 1;
    else {
        nChars = 0;
        unsigned tmp = nCompPent;
        while (tmp > 0) {
            tmp >>= 6;
            ++nChars;
        }

        ans = SCHAR(63);
        ans += SCHAR(nChars);
    }

    // Off we go.
    unsigned i;
    SAPPEND(ans, nCompPent, nChars);
    for (i = 0; i < facetPos; i += 3)
        SAPPENDTRITS(ans, facetAction + i,
            (facetPos >= i + 3 ? 3 : facetPos - i));
    for (i = 0; i < joinPos; ++i)
        SAPPEND(ans, joinDest[i], nChars);
    for (i = 0; i < joinPos; ++i)
        SAPPEND(ans, joinGluing[i], 2); // Two characters required for 5!=120.

    // Done!
    delete[] image;
    delete[] vertexMap;
    delete[] preImage;
    delete[] facetAction;
    delete[] joinDest;
    delete[] joinGluing;

    return ans;
}

Dim4Triangulation* Dim4Triangulation::fromIsoSig(const std::string& sig) {
    std::auto_ptr<Dim4Triangulation> ans(new Dim4Triangulation());

    const char* c = sig.c_str();

    // Initial check for invalid characters.
    const char* d;
    for (d = c; *d; ++d)
        if (! SVALID(*d))
            return 0;

    unsigned i, j;
    unsigned nPent, nChars;
    while (*c) {
        // Read one component at a time.
        nPent = SVAL(*c++);
        if (nPent < 63)
            nChars = 1;
        else {
            if (! *c)
                return 0;
            nChars = SVAL(*c++);
            if (! SHASCHARS(c, nChars))
                return 0;
            nPent = SREAD(c, nChars);
            c += nChars;
        }

        if (nPent == 0) {
            // Empty component.
            continue;
        }

        // Non-empty component; keep going.
        char* facetAction = new char[5 * nPent + 2];
        unsigned nFacets = 0;
        unsigned facetPos = 0;
        unsigned nJoins = 0;

        for ( ; nFacets < 5 * nPent; facetPos += 3) {
            if (! *c) {
                delete[] facetAction;
                return 0;
            }
            SREADTRITS(*c++, facetAction + facetPos);
            for (i = 0; i < 3; ++i) {
                // If we're already finished, make sure the leftover trits
                // are zero.
                if (nFacets == 5 * nPent) {
                    if (facetAction[facetPos + i] != 0) {
                        delete[] facetAction;
                        return 0;
                    }
                    continue;
                }

                if (facetAction[facetPos + i] == 0)
                    ++nFacets;
                else if (facetAction[facetPos + i] == 1)
                    nFacets += 2;
                else if (facetAction[facetPos + i] == 2) {
                    nFacets += 2;
                    ++nJoins;
                } else {
                    delete[] facetAction;
                    return 0;
                }
                if (nFacets > 5 * nPent) {
                    delete[] facetAction;
                    return 0;
                }
            }
        }

        unsigned* joinDest = new unsigned[nJoins + 1];
        for (i = 0; i < nJoins; ++i) {
            if (! SHASCHARS(c, nChars)) {
                delete[] facetAction;
                delete[] joinDest;
                return 0;
            }

            joinDest[i] = SREAD(c, nChars);
            c += nChars;
        }

        unsigned* joinGluing = new unsigned[nJoins + 1];
        for (i = 0; i < nJoins; ++i) {
            if (! SHASCHARS(c, 1)) {
                delete[] facetAction;
                delete[] joinDest;
                delete[] joinGluing;
                return 0;
            }

            joinGluing[i] = SREAD(c, 2);
            c += 2;

            if (joinGluing[i] >= 120) {
                delete[] facetAction;
                delete[] joinDest;
                delete[] joinGluing;
                return 0;
            }
        }

        // End of component!
        Dim4Pentachoron** pent = new Dim4Pentachoron*[nPent];
        for (i = 0; i < nPent; ++i)
            pent[i] = new Dim4Pentachoron();

        facetPos = 0;
        unsigned nextUnused = 1;
        unsigned joinPos = 0;
        for (i = 0; i < nPent; ++i)
            for (j = 0; j < 5; ++j) {
                // Already glued from the other side:
                if (pent[i]->adjacentPentachoron(j))
                    continue;

                if (facetAction[facetPos] == 0) {
                    // Boundary facet.
                } else if (facetAction[facetPos] == 1) {
                    // Join to new pentachoron.
                    pent[i]->joinTo(j, pent[nextUnused++], NPerm5());
                } else {
                    // Join to existing pentachoron.
                    if (joinDest[joinPos] >= nextUnused ||
                            pent[joinDest[joinPos]]->adjacentPentachoron(
                            NPerm5::orderedS5[joinGluing[joinPos]][j])) {
                        delete[] facetAction;
                        delete[] joinDest;
                        delete[] joinGluing;
                        for (int k = 0; k < nPent; ++k)
                            delete pent[k];
                        delete[] pent;
                        return 0;
                    }
                    pent[i]->joinTo(j, pent[joinDest[joinPos]],
                        NPerm5::orderedS5[joinGluing[joinPos]]);
                    ++joinPos;
                }

                ++facetPos;
            }

        for (i = 0; i < nPent; ++i)
            ans->addPentachoron(pent[i]);

        delete[] facetAction;
        delete[] joinDest;
        delete[] joinGluing;
        delete[] pent;
    }

    return ans.release();
}

} // namespace regina

