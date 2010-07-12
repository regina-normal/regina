
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
#include "triangulation/ntriangulation.h"

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
            c == '+' || c == '-');
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
     * Read the integer at the given position in the string.
     * Assumes there are at least nChars characters available from
     * position index onwards.
     */
    unsigned SREAD(const std::string& s, unsigned index, unsigned nChars) {
        unsigned ans = 0;
        for (unsigned i = 0; i < nChars; ++i)
            ans += (SVAL(s[index + i]) << (6 * i));
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
}

std::string NTriangulation::isomorphismSignature() const {
    if (tetrahedra.empty()) {
        char c[2];
        c[0] = SCHAR(0);
        c[1] = 0;
        return c;
    }

    // The triangulation is non-empty.  Get a signature string for each
    // connected component.
    unsigned i, j;
    ComponentIterator it;
    unsigned cTet;
    unsigned tet, perm;
    std::string curr;

    std::string* comp = new std::string[getNumberOfComponents()];
    for (it = components.begin(), i = 0; it != components.end(); ++it, ++i) {
        cTet = (*it)->getNumberOfTetrahedra();

        for (tet = 0; tet < (*it)->getNumberOfTetrahedra(); ++tet)
            for (perm = 0; perm < 24; ++perm) {
                curr = isomorphismSignature(
                    (*it)->getTetrahedron(tet)->markedIndex(),
                    NPerm4::orderedS4[perm]);
                if ((tet == 0 && perm == 0) || (curr < comp[i]))
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

std::string NTriangulation::isomorphismSignature(
        unsigned tet, const NPerm4& vertices) const {
    // Only process the component that tet belongs to.

    // ---------------------------------------------------------------------
    // Data for reconstructing a triangulation from an isomorphism signature
    // ---------------------------------------------------------------------

    // The number of tetrahedra.
    unsigned nTets = tetrahedra.size();

    // What happens to each new face that we encounter?
    // Options are:
    //   0 -> boundary
    //   1 -> joined to a tetrahedron not yet seen [gluing perm = identity]
    //   2 -> joined to a tetrahedron already seen
    // These actions are stored in lexicographical order by (tet, face),
    // but only once for each face (so we "skip" gluings that we've
    // already seen from the other direction).
    char* faceAction = new char[getNumberOfFaces()];

    // What are the destination tetrahedra and gluing permutations for
    // each face under case #2 above?
    // For gluing permutations, we store the index of the permutation in
    // NPerm4::orderedS4.
    unsigned* joinDest = new unsigned[getNumberOfFaces()];
    unsigned* joinGluing = new unsigned[getNumberOfFaces()];

    // ---------------------------------------------------------------------
    // Data for finding the unique canonical isomorphism from this
    // connected component that maps (tet, vertices) -> (0, 0123)
    // ---------------------------------------------------------------------

    // The image for each tetrahedron and its vertices:
    int* image = new int[nTets];
    NPerm4* vertexMap = new NPerm4[nTets];

    // The preimage for each tetrahedron:
    int* preImage = new int[nTets];

    // ---------------------------------------------------------------------
    // Looping variables
    // ---------------------------------------------------------------------
    unsigned facePos, joinPos, nextUnusedTet;
    unsigned tetImg, faceImg;
    unsigned tetSrc, faceSrc, dest;
    NTetrahedron* t;

    // ---------------------------------------------------------------------
    // The code!
    // ---------------------------------------------------------------------

    std::fill(image, image + nTets, -1);
    std::fill(preImage, preImage + nTets, -1);

    image[tet] = 0;
    vertexMap[tet] = vertices.inverse();
    preImage[0] = tet;

    facePos = 0;
    joinPos = 0;
    nextUnusedTet = 1;

    // To obtain a canonical isomorphism, we must run through the tetrahedra
    // and their faces in image order, not preimage order.
    //
    // This main loop is guaranteed to exit when (and only when) we have
    // exhausted a single connected component of the triangulation.
    for (tetImg = 0; tetImg < nTets && preImage[tetImg] >= 0; ++tetImg) {
        tetSrc = preImage[tetImg];
        t = tetrahedra[tetSrc];

        for (faceImg = 0; faceImg < 4; ++faceImg) {
            faceSrc = vertexMap[tetSrc].preImageOf(faceImg);

            // INVARIANTS (held while we stay within a single component):
            // - nextUnusedTet > tetImg
            // - image[tetSrc], preImage[image[tetSrc]] and vertexMap[tetSrc]
            //   are already filled in.

            // Work out what happens to our source face.
            if (! t->adjacentTetrahedron(faceSrc)) {
                // A boundary face.
                faceAction[facePos++] = 0;
                continue;
            }

            // We have a real gluing.  Is it a gluing we've already seen
            // from the other side?
            dest = tetrahedronIndex(t->adjacentTetrahedron(faceSrc));

            if (image[dest] >= 0)
                if (image[dest] < image[tetSrc] ||
                        (dest == tetSrc &&
                         vertexMap[tetSrc][t->adjacentFace(faceSrc)]
                         < vertexMap[tetSrc][faceSrc])) {
                    // Yes.  Just skip this gluing entirely.
                    continue;
                }

            // Is it a completely new tetrahedron?
            if (image[dest] < 0) {
                // Yes.  The new tetrahedron takes the next available
                // index, and the canonical gluing becomes the identity.
                image[dest] = nextUnusedTet++;
                preImage[image[dest]] = dest;
                vertexMap[dest] = vertexMap[tetSrc] *
                    t->adjacentGluing(faceSrc).inverse();

                faceAction[facePos++] = 1;
                continue;
            }

            // It's a tetrahedron we've seen before.  Record the gluing.
            joinDest[joinPos] = image[dest];
            joinGluing[joinPos] = (vertexMap[dest] *
                t->adjacentGluing(faceSrc) * vertexMap[tetSrc].inverse()).
                orderedS4Index();
            ++joinPos;

            faceAction[facePos++] = 2;
        }
    }

    // We have all we need.  Pack it all together into a string.
    // We need to encode:
    // - the number of tetrahedra in this component;
    // - faceAction[i], 0 <= i < facePos;
    // - joinDest[i], 0 <= i < joinPos;
    // - joinGluing[i], 0 <= i < joinPos.
    std::string ans;

    // Keep it simple for small triangulations (1 character per integer).
    // For large triangulations, start with a special marker followed by
    // the number of chars per integer.
    unsigned nCompTet = tetImg;
    unsigned nChars;
    if (nCompTet < 63)
        nChars = 1;
    else {
        nChars = 0;
        unsigned tmp = nCompTet;
        while (tmp > 0) {
            tmp >>= 6;
            ++nChars;
        }

        ans = SCHAR(63);
        ans += SCHAR(nChars);
    }

    // Off we go.
    unsigned i;
    SAPPEND(ans, nCompTet, nChars);
    for (i = 0; i < facePos; i += 3)
        SAPPENDTRITS(ans, faceAction + i, (facePos >= i + 3 ? 3 : facePos - i));
    for (i = 0; i < joinPos; ++i)
        SAPPEND(ans, joinDest[i], nChars);
    for (i = 0; i < joinPos; ++i)
        SAPPEND(ans, joinGluing[i], 1);

    // Done!
    delete[] image;
    delete[] vertexMap;
    delete[] preImage;
    delete[] faceAction;
    delete[] joinDest;
    delete[] joinGluing;

    return ans;
}

} // namespace regina

