
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <ctype.h>

#include "triangulation/ntriangulation.h"

/**
 * Determine the integer value represented by the given letter.
 */
#define VAL(x) ((x) - 'a')

/**
 * Determine the letter that represents the given integer value.
 */
#define LETTER(x) (char((x) + 'a'))

bool NTriangulation::insertRehydration(const NString& dehydration) {
    unsigned len = dehydration.length();

    // Ensure the string is non-empty.
    if (len == 0)
        return false;

    // Rewrite the string in lower case and verify that it contains only
    // letters.
    NString proper(dehydration.toLower());
    unsigned i, j;
    for (i = 0; i < len; i++)
        if (! isalpha(proper[i]))
            return false;

    // Determine the number of tetrahedra.
    unsigned nTet = VAL(proper[0]);

    // Determine the expected length of each piece of the dehydrated string.
    unsigned lenNewTet = 2 * ((nTet + 3) / 4);
    unsigned lenGluings = nTet + 1;

    // Ensure the string has the expected length.
    if (len != 1 + lenNewTet + lenGluings + lenGluings)
        return false;

    // Determine which face gluings should involve new tetrahedra.
    bool* newTetGluings = new bool[2 * nTet];

    unsigned val;
    for (i = 0; i < lenNewTet; i++) {
        val = VAL(proper[i + 1]);
        if (val > 15) {
            delete[] newTetGluings;
            return false;
        }

        if (i % 2 == 0) {
            // This letter stores values 4i+4 -> 4i+7.
            for (j = 0; (j < 4) && (4*i + 4 + j < 2 * nTet); j++)
                newTetGluings[4*i + 4 + j] = ((val & (1 << j)) != 0);
        } else {
            // This letter stores values 4i-4 -> 4i-1.
            for (j = 0; (j < 4) && (4*i - 4 + j < 2 * nTet); j++)
                newTetGluings[4*i - 4 + j] = ((val & (1 << j)) != 0);
        }
    }

    // Create the tetrahedra and start gluing.
    NTetrahedron** tet = new (NTetrahedron*)[nTet];
    for (i = 0; i < nTet; i++)
        tet[i] = new NTetrahedron();

    unsigned currTet = 0;       // Tetrahedron of the next face to glue.
    int currFace = 0;           // Face number of the next face to glue.
    unsigned gluingsMade = 0;   // How many face pairs have we already glued?
    unsigned specsUsed = 0;     // How many gluing specs have we already used?
    unsigned tetsUsed = 0;      // How many tetrahedra have we already used?
    bool broken = false;        // Have we come across an inconsistency?
    unsigned adjTet;            // The tetrahedron to glue this to.
    unsigned permIndex;         // The index of the gluing permutation to use.
    NPerm adjPerm;              // The gluing permutation to use.
    NPerm identity;             // The identity permutation.
    NPerm reverse(3,2,1,0);     // The reverse permutation.

    while (currTet < nTet) {
        // Is this face already glued?
        if (tet[currTet]->getAdjacentTetrahedron(currFace)) {
            if (currFace < 3)
                currFace++;
            else {
                currFace = 0;
                currTet++;
            }
            continue;
        }

        // If this is a new tetrahedron, be aware of this fact.
        if (tetsUsed <= currTet)
            tetsUsed = currTet + 1;

        // Do we simply glue to a new tetrahedron?
        if (newTetGluings[gluingsMade]) {
            // Glue to tetrahedron tetsUsed.
            if (tetsUsed < nTet) {
                tet[currTet]->joinTo(currFace, tet[tetsUsed], identity);
                tetsUsed++;
            } else {
                broken = true;
                break;
            }
        } else {
            // Glue according to the next gluing spec.
            if (specsUsed >= lenGluings) {
                broken = true;
                break;
            }

            adjTet = VAL(proper[1 + lenNewTet + specsUsed]);
            permIndex = VAL(proper[1 + lenNewTet + lenGluings + specsUsed]);

            if (adjTet >= nTet || permIndex >= 24) {
                broken = true;
                break;
            }

            adjPerm = orderedPermsS4[permIndex] * reverse;

            if (tet[adjTet]->getAdjacentTetrahedron(adjPerm[currFace]) ||
                    (adjTet == currTet && adjPerm[currFace] == currFace)) {
                broken = true;
                break;
            }

            tet[currTet]->joinTo(currFace, tet[adjTet], adjPerm);

            specsUsed++;
        }

        // Increment everything for the next gluing.
        gluingsMade++;

        if (currFace < 3)
            currFace++;
        else {
            currFace = 0;
            currTet++;
        }
    }

    // Insert the tetrahedra into the triangulation and we're done!
    if (broken)
        for (i = 0; i < nTet; i++)
            delete tet[i];
    else
        for (i = 0; i < nTet; i++)
            addTetrahedron(tet[i]);

    delete[] newTetGluings;
    delete[] tet;

    return (! broken);
}

