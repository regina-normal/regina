
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

#include "triangulation/nvertex.h"

void NVertex::writeTextShort(ostream& out) const {
    switch(link) {
        case SPHERE: out << "Internal "; break;
        case DISC: out << "Boundary "; break;
        case TORUS: out << "Torus cusp "; break;
        case KLEIN_BOTTLE: out << "Klein bottle cusp "; break;
        case NON_STANDARD_CUSP: out << "Non-standard cusp "; break;
        case NON_STANDARD_BDRY: out << "Non-standard boundary "; break;
    }
    out << "vertex of degree " << getNumberOfEmbeddings();
}
