
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

#include "triangulation/nperm.h"

const NPerm allPermsS4[24] = {
    NPerm(0,1,2,3), NPerm(0,1,3,2), NPerm(0,2,3,1), NPerm(0,2,1,3),
    NPerm(0,3,1,2), NPerm(0,3,2,1), NPerm(1,0,3,2), NPerm(1,0,2,3),
    NPerm(1,2,0,3), NPerm(1,2,3,0), NPerm(1,3,2,0), NPerm(1,3,0,2),
    NPerm(2,0,1,3), NPerm(2,0,3,1), NPerm(2,1,3,0), NPerm(2,1,0,3),
    NPerm(2,3,0,1), NPerm(2,3,1,0), NPerm(3,0,2,1), NPerm(3,0,1,2),
    NPerm(3,1,0,2), NPerm(3,1,2,0), NPerm(3,2,1,0), NPerm(3,2,0,1)
};

const NPerm allPermsS3[6] = {
    NPerm(0,1,2,3), NPerm(0,2,1,3),
    NPerm(1,2,0,3), NPerm(1,0,2,3),
    NPerm(2,0,1,3), NPerm(2,1,0,3)
};

const NPerm allPermsS2[2] = {
    NPerm(0,1,2,3), NPerm(1,0,2,3)
};

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

