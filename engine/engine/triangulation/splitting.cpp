
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

#include <cctype>
#include <algorithm>
#include "triangulation/ntriangulation.h"
#include "utilities/nmiscutils.h"

static NPerm exitFace(bool firstOccurrence, bool lowerCase) {
    if (firstOccurrence) {
        if (lowerCase)
            return NPerm(2,3,1,0);
        else
            return NPerm(2,3,0,1);
    } else {
        if (lowerCase)
            return NPerm(0,1,3,2);
        else
            return NPerm(0,1,2,3);
    }
}

bool NTriangulation::insertSplittingSurface(const NString& sig) {
    // See if the string looks correctly formed.
    // Note that we're not yet counting the individual frequency of each
    // letter, just the overall number of letters.
    // Cycles are assumed to be separated by any non-space
    // non-alphabetic characters.
    unsigned nAlpha = 0;
    int largestLetter = -1;

    unsigned len = sig.length();
    unsigned pos;
    char letter;
    for (pos = 0; pos < len; pos++)
        if (isalpha(sig[pos])) {
            nAlpha++;
            letter = tolower(sig[pos]);
            if (largestLetter < letter - 'a')
                largestLetter = letter - 'a';
        }

    if ((int)nAlpha != 2 * (largestLetter + 1))
        return false;

    // Looks fine so far.
    if (nAlpha == 0)
        return true;

    unsigned* letterPos = new unsigned[nAlpha];
        /* Position of a1, a2, b1, b2, etc. */
    unsigned* symbol = new unsigned[nAlpha];
        /* Specifies which letter is in this position. */
    bool* lowerCase = new bool[nAlpha];
        /* Case of the letter in this position. */
    unsigned* nextPos = new unsigned[nAlpha];
        /* Next position after this in the current cycle. */
    unsigned* occurrence = new unsigned[nAlpha];
        /* Contains 0 or 1 in each place denoting whether we've seen
           that position's letter before. */

    std::fill(letterPos, letterPos + nAlpha, nAlpha);
    unsigned whichPos = 0;
        /* Which letter position are we about to encounter? */
    unsigned cycleStart = 0;
    unsigned letterIndex;
    for (pos = 0; pos < len; pos++) {
        if (isspace(sig[pos]))
            continue;
        if (! isalpha(sig[pos])) {
            if (whichPos > 0)
                if (nextPos[whichPos - 1] == whichPos)
                    nextPos[whichPos - 1] = cycleStart;
            cycleStart = whichPos;
        } else {
            letterIndex = tolower(sig[pos]) - 'a';
            if (letterPos[2 * letterIndex] == nAlpha) {
                letterPos[2 * letterIndex] = whichPos;
                occurrence[whichPos] = 0;
            } else if (letterPos[2 * letterIndex + 1] == nAlpha) {
                letterPos[2 * letterIndex + 1] = whichPos;
                occurrence[whichPos] = 1;
            } else {
                // We've seen this letter a third time!
                delete[] letterPos;
                delete[] symbol;
                delete[] lowerCase;
                delete[] nextPos;
                delete[] occurrence;
                return false;
            }
            symbol[whichPos] = letterIndex;
            lowerCase[whichPos] = islower(sig[pos]);
            nextPos[whichPos] = whichPos + 1;
            whichPos++;
        }
    }
    if (nextPos[nAlpha - 1] == nAlpha)
        nextPos[nAlpha - 1] = cycleStart;

    // Now we absolutely know it's a valid signature.
    // Build the triangulation.
    // Tetrahedron vertices will be:
    //   bottom left -> top right: 0 -> 1
    //   bottom right -> top left: 2 -> 3
    unsigned nTet = largestLetter + 1;
    NTetrahedron** tet = new (NTetrahedron*)[nTet];
    std::generate(tet, tet + nTet, FuncNew<NTetrahedron>());

    unsigned next;
    NPerm myFacePerm, yourFacePerm;
    for (pos = 0; pos < nAlpha; pos++) {
        next = nextPos[pos];
        myFacePerm = exitFace(occurrence[pos], lowerCase[pos]);
        yourFacePerm = exitFace(occurrence[next], ! lowerCase[next]);
        tet[symbol[pos]]->joinTo(myFacePerm[3], tet[symbol[next]],
            yourFacePerm * myFacePerm.inverse());
        /*
        cerr << "Glued " << symbol[pos] << " (" << myFacePerm.toString()
            << ") to " << symbol[next] << " (" << yourFacePerm.toString()
            << ")\n";
        */
    }

    for (pos = 0; pos < nTet; pos++)
        addTetrahedron(tet[pos]);

    delete[] tet;
    delete[] symbol;
    delete[] letterPos;
    delete[] lowerCase;
    delete[] nextPos;
    delete[] occurrence;
    return true;
}

