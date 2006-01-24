
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include <sstream>
#include "triangulation/nperm.h"

namespace regina {

const NPerm allPermsS4[24] = {
    NPerm(0,1,2,3), NPerm(0,1,3,2), NPerm(0,2,3,1), NPerm(0,2,1,3),
    NPerm(0,3,1,2), NPerm(0,3,2,1), NPerm(1,0,3,2), NPerm(1,0,2,3),
    NPerm(1,2,0,3), NPerm(1,2,3,0), NPerm(1,3,2,0), NPerm(1,3,0,2),
    NPerm(2,0,1,3), NPerm(2,0,3,1), NPerm(2,1,3,0), NPerm(2,1,0,3),
    NPerm(2,3,0,1), NPerm(2,3,1,0), NPerm(3,0,2,1), NPerm(3,0,1,2),
    NPerm(3,1,0,2), NPerm(3,1,2,0), NPerm(3,2,1,0), NPerm(3,2,0,1)
};

const unsigned allPermsS4Inv[24] = {
    0, 1, 4, 3,
    2, 5, 6, 7,
    12, 19, 18, 13,
    8, 11, 20, 15,
    16, 23, 10, 9,
    14, 21, 22, 17
};

const NPerm orderedPermsS4[24] = {
    NPerm(0,1,2,3), NPerm(0,1,3,2), NPerm(0,2,1,3), NPerm(0,2,3,1),
    NPerm(0,3,1,2), NPerm(0,3,2,1), NPerm(1,0,2,3), NPerm(1,0,3,2),
    NPerm(1,2,0,3), NPerm(1,2,3,0), NPerm(1,3,0,2), NPerm(1,3,2,0),
    NPerm(2,0,1,3), NPerm(2,0,3,1), NPerm(2,1,0,3), NPerm(2,1,3,0),
    NPerm(2,3,0,1), NPerm(2,3,1,0), NPerm(3,0,1,2), NPerm(3,0,2,1),
    NPerm(3,1,0,2), NPerm(3,1,2,0), NPerm(3,2,0,1), NPerm(3,2,1,0)
};

const NPerm allPermsS3[6] = {
    NPerm(0,1,2,3), NPerm(0,2,1,3),
    NPerm(1,2,0,3), NPerm(1,0,2,3),
    NPerm(2,0,1,3), NPerm(2,1,0,3)
};

const unsigned allPermsS3Inv[6] = {
    0, 1,
    4, 3,
    2, 5
};

const NPerm orderedPermsS3[6] = {
    NPerm(0,1,2,3), NPerm(0,2,1,3),
    NPerm(1,0,2,3), NPerm(1,2,0,3),
    NPerm(2,0,1,3), NPerm(2,1,0,3)
};

const NPerm allPermsS2[2] = {
    NPerm(0,1,2,3), NPerm(1,0,2,3)
};

const unsigned allPermsS2Inv[2] = {
    0, 1
};

bool NPerm::isPermCode(unsigned char code) {
    unsigned char mask = 0;
    for (int i = 0; i < 4; i++)
        mask |= (1 << ((code >> (2 * i)) & 3));
            // mask |= (1 << imageOf(i));
    return (mask == 15);
}

int NPerm::sign() const {
    int sign = 1;
    int i,j;
    for (i=0; i<4; i++)
        for (j=i+1; j<4; j++)
            if (imageOf(i) > imageOf(j))
                sign = -sign;
    return sign;
}

int NPerm::compareWith(const NPerm& other) const {
    for (int i = 0; i < 4; i++) {
        if (imageOf(i) < other.imageOf(i))
            return -1;
        if (imageOf(i) > other.imageOf(i))
            return 1;
    }
    return 0;
}

std::string NPerm::toString() const {
    char ans[5];
    for (int i = 0; i < 4; i++)
        ans[i] = '0' + imageOf(i);
    ans[4] = 0;

    return ans;
}

NPerm faceOrdering(int face) {
    switch(face) {
        case 0:
            return NPerm(1,2,3,0);
        case 1:
            return NPerm(0,2,3,1);
        case 2:
            return NPerm(0,1,3,2);
        case 3:
            return NPerm(0,1,2,3);
    }
    return NPerm();
}

NPerm edgeOrdering(int edge) {
    switch(edge) {
        case 0:
            return NPerm(0,1,2,3);
        case 1:
            return NPerm(0,2,3,1);
        case 2:
            return NPerm(0,3,1,2);
        case 3:
            return NPerm(1,2,0,3);
        case 4:
            return NPerm(1,3,2,0);
        case 5:
            return NPerm(2,3,0,1);
    }
    return NPerm();
}

std::string faceDescription(int face) {
    return faceDescription(faceOrdering(face));
}

std::string faceDescription(const NPerm& p) {
    std::string ans;
    ans += ('0' + p[0]);
    ans += ('0' + p[1]);
    ans += ('0' + p[2]);
    return ans;
}

std::string edgeDescription(int edge) {
    return edgeDescription(edgeOrdering(edge));
}

std::string edgeDescription(const NPerm& p) {
    std::string ans;
    ans += ('0' + p[0]);
    ans += ('0' + p[1]);
    return ans;
}

} // namespace regina

