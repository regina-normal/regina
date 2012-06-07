
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

#include <algorithm>
#include "dim2/dim2triangulation.h"

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

std::string Dim2Triangulation::isoSig() const {
    if (triangles_.empty()) {
        char c[2];
        c[0] = SCHAR(0);
        c[1] = 0;
        return c;
    }

    // The triangulation is non-empty.  Get a signature string for each
    // connected component.
    unsigned i, j;
    ComponentIterator it;
    unsigned cTri;
    unsigned tri, perm;
    std::string curr;

    std::string* comp = new std::string[getNumberOfComponents()];
    for (it = components_.begin(), i = 0; it != components_.end(); ++it, ++i) {
        cTri = (*it)->getNumberOfTriangles();

        for (tri = 0; tri < (*it)->getNumberOfTriangles(); ++tri)
            for (perm = 0; perm < 6; ++perm) {
                curr = isoSig((*it)->getTriangle(tri)->markedIndex(),
                    NPerm3::orderedS3[perm]);
                if ((tri == 0 && perm == 0) || (curr < comp[i]))
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

std::string Dim2Triangulation::isoSig(unsigned tri, const NPerm3& vertices)
        const {
    // Only process the component that tri belongs to.

    // ---------------------------------------------------------------------
    // Data for reconstructing a triangulation from an isomorphism signature
    // ---------------------------------------------------------------------

    // The number of triangles.
    unsigned nTriangles = triangles_.size();

    // What happens to each new edge that we encounter?
    // Options are:
    //   0 -> boundary
    //   1 -> joined to a triangle not yet seen [gluing perm = identity]
    //   2 -> joined to a triangle already seen
    // These actions are stored in lexicographical order by (triangle, edge),
    // but only once for each edge (so we "skip" gluings that we've
    // already seen from the other direction).
    char* edgeAction = new char[getNumberOfEdges()];

    // What are the destination triangles and gluing permutations for
    // each edge under case #2 above?
    // For gluing permutations, we store the index of the permutation in
    // NPerm3::orderedS3.
    unsigned* joinDest = new unsigned[getNumberOfEdges()];
    unsigned* joinGluing = new unsigned[getNumberOfEdges()];

    // ---------------------------------------------------------------------
    // Data for finding the unique canonical isomorphism from this
    // connected component that maps (triangle, vertices) -> (0, 012)
    // ---------------------------------------------------------------------

    // The image for each triangle and its vertices:
    int* image = new int[nTriangles];
    NPerm3* vertexMap = new NPerm3[nTriangles];

    // The preimage for each triangle:
    int* preImage = new int[nTriangles];

    // ---------------------------------------------------------------------
    // Looping variables
    // ---------------------------------------------------------------------
    unsigned edgePos, joinPos, nextUnusedTri;
    unsigned triImg, edgeImg;
    unsigned triSrc, edgeSrc, dest;
    Dim2Triangle* p;

    // ---------------------------------------------------------------------
    // The code!
    // ---------------------------------------------------------------------

    std::fill(image, image + nTriangles, -1);
    std::fill(preImage, preImage + nTriangles, -1);

    image[tri] = 0;
    vertexMap[tri] = vertices.inverse();
    preImage[0] = tri;

    edgePos = 0;
    joinPos = 0;
    nextUnusedTri = 1;

    // To obtain a canonical isomorphism, we must run through the triangles
    // and their edges in image order, not preimage order.
    //
    // This main loop is guaranteed to exit when (and only when) we have
    // exhausted a single connected component of the triangulation.
    for (triImg = 0; triImg < nTriangles && preImage[triImg] >= 0;
            ++triImg) {
        triSrc = preImage[triImg];
        p = triangles_[triSrc];

        for (edgeImg = 0; edgeImg < 3; ++edgeImg) {
            edgeSrc = vertexMap[triSrc].preImageOf(edgeImg);

            // INVARIANTS (held while we stay within a single component):
            // - nextUnusedTri > triImg
            // - image[triSrc], preImage[image[triSrc]] and vertexMap[triSrc]
            //   are already filled in.

            // Work out what happens to our source edge.
            if (! p->adjacentTriangle(edgeSrc)) {
                // A boundary edge.
                edgeAction[edgePos++] = 0;
                continue;
            }

            // We have a real gluing.  Is it a gluing we've already seen
            // from the other side?
            dest = triangleIndex(p->adjacentTriangle(edgeSrc));

            if (image[dest] >= 0)
                if (image[dest] < image[triSrc] ||
                        (dest == triSrc &&
                         vertexMap[triSrc][p->adjacentEdge(edgeSrc)]
                         < vertexMap[triSrc][edgeSrc])) {
                    // Yes.  Just skip this gluing entirely.
                    continue;
                }

            // Is it a completely new triangle?
            if (image[dest] < 0) {
                // Yes.  The new triangle takes the next available
                // index, and the canonical gluing becomes the identity.
                image[dest] = nextUnusedTri++;
                preImage[image[dest]] = dest;
                vertexMap[dest] = vertexMap[triSrc] *
                    p->adjacentGluing(edgeSrc).inverse();

                edgeAction[edgePos++] = 1;
                continue;
            }

            // It's a triangle we've seen before.  Record the gluing.
            joinDest[joinPos] = image[dest];
            joinGluing[joinPos] = (vertexMap[dest] *
                p->adjacentGluing(edgeSrc) * vertexMap[triSrc].inverse()).
                orderedS3Index();
            ++joinPos;

            edgeAction[edgePos++] = 2;
        }
    }

    // We have all we need.  Pack it all together into a string.
    // We need to encode:
    // - the number of triangles in this component;
    // - edgeAction[i], 0 <= i < edgePos;
    // - joinDest[i], 0 <= i < joinPos;
    // - joinGluing[i], 0 <= i < joinPos.
    std::string ans;

    // Keep it simple for small triangulations (1 character per integer).
    // For large triangulations, start with a special marker followed by
    // the number of chars per integer.
    unsigned nCompTri = triImg;
    unsigned nChars;
    if (nCompTri < 63)
        nChars = 1;
    else {
        nChars = 0;
        unsigned tmp = nCompTri;
        while (tmp > 0) {
            tmp >>= 6;
            ++nChars;
        }

        ans = SCHAR(63);
        ans += SCHAR(nChars);
    }

    // Off we go.
    unsigned i;
    SAPPEND(ans, nCompTri, nChars);
    for (i = 0; i < edgePos; i += 3)
        SAPPENDTRITS(ans, edgeAction + i, (edgePos >= i + 3 ? 3 : edgePos - i));
    for (i = 0; i < joinPos; ++i)
        SAPPEND(ans, joinDest[i], nChars);
    for (i = 0; i < joinPos; ++i)
        SAPPEND(ans, joinGluing[i], 1); // One char is enough since 3! < 64.

    // Done!
    delete[] image;
    delete[] vertexMap;
    delete[] preImage;
    delete[] edgeAction;
    delete[] joinDest;
    delete[] joinGluing;

    return ans;
}

Dim2Triangulation* Dim2Triangulation::fromIsoSig(const std::string& sig) {
    std::auto_ptr<Dim2Triangulation> ans(new Dim2Triangulation());

    ChangeEventSpan span(ans.get());

    const char* c = sig.c_str();

    // Initial check for invalid characters.
    const char* d;
    for (d = c; *d; ++d)
        if (! SVALID(*d))
            return 0;

    unsigned i, j;
    unsigned nTri, nChars;
    while (*c) {
        // Read one component at a time.
        nTri = SVAL(*c++);
        if (nTri < 63)
            nChars = 1;
        else {
            if (! *c)
                return 0;
            nChars = SVAL(*c++);
            if (! SHASCHARS(c, nChars))
                return 0;
            nTri = SREAD(c, nChars);
            c += nChars;
        }

        if (nTri == 0) {
            // Empty component.
            continue;
        }

        // Non-empty component; keep going.
        char* edgeAction = new char[3 * nTri + 2];
        unsigned nEdges = 0;
        unsigned edgePos = 0;
        unsigned nJoins = 0;

        for ( ; nEdges < 3 * nTri; edgePos += 3) {
            if (! *c) {
                delete[] edgeAction;
                return 0;
            }
            SREADTRITS(*c++, edgeAction + edgePos);
            for (i = 0; i < 3; ++i) {
                // If we're already finished, make sure the leftover trits
                // are zero.
                if (nEdges == 3 * nTri) {
                    if (edgeAction[edgePos + i] != 0) {
                        delete[] edgeAction;
                        return 0;
                    }
                    continue;
                }

                if (edgeAction[edgePos + i] == 0)
                    ++nEdges;
                else if (edgeAction[edgePos + i] == 1)
                    nEdges += 2;
                else if (edgeAction[edgePos + i] == 2) {
                    nEdges += 2;
                    ++nJoins;
                } else {
                    delete[] edgeAction;
                    return 0;
                }
                if (nEdges > 3 * nTri) {
                    delete[] edgeAction;
                    return 0;
                }
            }
        }

        unsigned* joinDest = new unsigned[nJoins + 1];
        for (i = 0; i < nJoins; ++i) {
            if (! SHASCHARS(c, nChars)) {
                delete[] edgeAction;
                delete[] joinDest;
                return 0;
            }

            joinDest[i] = SREAD(c, nChars);
            c += nChars;
        }

        unsigned* joinGluing = new unsigned[nJoins + 1];
        for (i = 0; i < nJoins; ++i) {
            if (! SHASCHARS(c, 1)) {
                delete[] edgeAction;
                delete[] joinDest;
                delete[] joinGluing;
                return 0;
            }

            joinGluing[i] = SREAD(c, 1);
            ++c;

            if (joinGluing[i] >= 6) {
                delete[] edgeAction;
                delete[] joinDest;
                delete[] joinGluing;
                return 0;
            }
        }

        // End of component!
        Dim2Triangle** tri = new Dim2Triangle*[nTri];
        for (i = 0; i < nTri; ++i)
            tri[i] = ans->newTriangle();

        edgePos = 0;
        unsigned nextUnused = 1;
        unsigned joinPos = 0;
        for (i = 0; i < nTri; ++i)
            for (j = 0; j < 3; ++j) {
                // Already glued from the other side:
                if (tri[i]->adjacentTriangle(j))
                    continue;

                if (edgeAction[edgePos] == 0) {
                    // Boundary edge.
                } else if (edgeAction[edgePos] == 1) {
                    // Join to new triangle.
                    tri[i]->joinTo(j, tri[nextUnused++], NPerm3());
                } else {
                    // Join to existing triangle.
                    if (joinDest[joinPos] >= nextUnused ||
                            tri[joinDest[joinPos]]->adjacentTriangle(
                            NPerm3::orderedS3[joinGluing[joinPos]][j])) {
                        delete[] edgeAction;
                        delete[] joinDest;
                        delete[] joinGluing;
                        for (int k = 0; k < nTri; ++k)
                            delete tri[k];
                        delete[] tri;
                        return 0;
                    }
                    tri[i]->joinTo(j, tri[joinDest[joinPos]],
                        NPerm3::orderedS3[joinGluing[joinPos]]);
                    ++joinPos;
                }

                ++edgePos;
            }

        delete[] edgeAction;
        delete[] joinDest;
        delete[] joinGluing;
        delete[] tri;
    }

    return ans.release();
}

} // namespace regina

