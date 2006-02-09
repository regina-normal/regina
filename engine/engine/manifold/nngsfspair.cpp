
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

#include "manifold/nngsfspair.h"
#include "manifold/nsfs.h"

namespace regina {

NNGSFSPair::~NNGSFSPair() {
    delete sfs_[0];
    delete sfs_[1];
}

std::ostream& NNGSFSPair::writeName(std::ostream& out) const {
    sfs_[0]->writeName(out);
    out << " U/m ";
    sfs_[1]->writeName(out);
    return out << ", m = [ " <<
        matchingReln_[0][0] << ',' << matchingReln_[0][1] << " | " <<
        matchingReln_[1][0] << ',' << matchingReln_[1][1] << " ]";
}

std::ostream& NNGSFSPair::writeTeXName(std::ostream& out) const {
    sfs_[0]->writeTeXName(out);
    out << " \\bigcup_{\\homtwo{" <<
        matchingReln_[0][0] << "}{" << matchingReln_[0][1] << "}{" <<
        matchingReln_[1][0] << "}{" << matchingReln_[1][1] << "}} ";
    return sfs_[1]->writeTeXName(out);
}

} // namespace regina

