
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

/*! \file nngsfspair.h
 *  \brief Deals with non-geometric pairs of Seifert fibred spaces.
 */

#ifndef __NNGSFSPAIR_H
#ifndef __DOXYGEN
#define __NNGSFSPAIR_H
#endif

#include "manifold/nmanifold.h"
#include "utilities/nmatrix2.h"

namespace regina {

class NSFSpace;

/**
 * \weakgroup manifold
 * @{
 */

/**
 * TODO: Document NNGSFSPair!
 *
 * <pre>
 *     [ f1 ]       [ f0 ]
 *     [    ] = M * [    ]
 *     [ o1 ]       [ o0 ]
 * </pre>
 *
 * Note that getHomologyH1() is not implemented.
 */
class NNGSFSPair : public NManifold {
    private:
        NSFSpace* sfs_[2];
        NMatrix2 matchingReln_;
            /**< Second (f, -o) in terms of the first (f, o). */

    public:
        /**
         * Creates a new trivial pair.  Each space will be a trivial
         * solid torus, and the matching relation will be the identity matrix.
         *
         * \pre Each SFS is non-null and has a single puncture.
         */
        NNGSFSPair(NSFSpace* sfs0, NSFSpace* sfs1, long mat00, long mat01,
            long mat10, long mat11);
        ~NNGSFSPair();

        const NSFSpace& sfs(unsigned which) const;
        NSFSpace& sfs(unsigned which);

        const NMatrix2& matchingReln() const;
        NMatrix2& matchingReln();

        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
};

/*@}*/

// Inline functions for NNGSFSPair

inline NNGSFSPair::NNGSFSPair(NSFSpace* sfs0, NSFSpace* sfs1,
        long mat00, long mat01, long mat10, long mat11) :
        matchingReln_(mat00, mat01, mat10, mat11) {
    sfs_[0] = sfs0;
    sfs_[1] = sfs1;
}

inline const NSFSpace& NNGSFSPair::sfs(unsigned which) const {
    return *sfs_[which];
}

inline NSFSpace& NNGSFSPair::sfs(unsigned which) {
    return *sfs_[which];
}

inline const NMatrix2& NNGSFSPair::matchingReln() const {
    return matchingReln_;
}

inline NMatrix2& NNGSFSPair::matchingReln() {
    return matchingReln_;
}

} // namespace regina

#endif

